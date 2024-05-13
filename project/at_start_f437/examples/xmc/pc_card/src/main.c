/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"
#include "cf.h"


/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_XMC_pc_card
  * @{
  */

typedef enum
{
  FAILED = 0,
  PASSED = 1
} test_status;

#define Start_Sector  0
pccard_handle_type pccardHandle;
xmc_nand_pccard_timinginit_type PCCARD_PMEM4_Timing;
xmc_nand_pccard_timinginit_type PCCARD_PATT4_Timing;
xmc_nand_pccard_timinginit_type PCCARD_PIO4_Timing;
verification_result_struct verification_result;
/* 0 -> Memory/PIO mode, 1 -> IO/PIO mode, 2 -> Memory/DMA mode, 3 -> IO/DMA mode  */
uint8_t Test_Mode = 0;  
uint8_t pccard_txbuffer[PCCARD_SECTOR_SIZE * sizeof(uint8_t)] __attribute__ ((aligned (4)));
uint8_t pccard_rxbuffer[PCCARD_SECTOR_SIZE * sizeof(uint8_t)] __attribute__ ((aligned (4)));
uint8_t pccard_25_sectors_txbuffer[PCCARD_25_SECTORS_SIZES * sizeof(uint8_t)] __attribute__ ((aligned (4)));
uint8_t pccard_25_sectors_rxbuffer[PCCARD_25_SECTORS_SIZES * sizeof(uint8_t)] __attribute__ ((aligned (4)));
uint8_t orig_default_sector;
uint8_t orig_default_head;
uint32_t u32i;

static void fill_buffer(uint8_t *pbuffer, uint32_t BufferLenght, uint32_t Offset);
static test_status buffer_cmp(uint8_t* pbuffer, uint8_t* pBuffer1, uint32_t BufferLength);

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  uart_print_init(115200);
 
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);
 
  pccardHandle.instance  = XMC_BANK4;
  PCCARD_PMEM4_Timing.mem_setup_time     =  12;
  PCCARD_PMEM4_Timing.mem_waite_time = 61;
  PCCARD_PMEM4_Timing.mem_hold_time = 14 ;
  PCCARD_PMEM4_Timing.mem_hiz_time  = 0;

  PCCARD_PATT4_Timing.mem_setup_time     = 12;
  PCCARD_PATT4_Timing.mem_waite_time = 99;
  PCCARD_PATT4_Timing.mem_hold_time =  12;
  PCCARD_PATT4_Timing.mem_hiz_time  = 1;

  PCCARD_PIO4_Timing.mem_setup_time     = 24;
  PCCARD_PIO4_Timing.mem_waite_time = 61;
  PCCARD_PIO4_Timing.mem_hold_time = 8;
  PCCARD_PIO4_Timing.mem_hiz_time  = 1;


  pccardHandle.init.enable_wait   = (xmc_nand_pccard_wait_type)1;
  pccardHandle.init.delay_time_cr = 0;
  pccardHandle.init.delay_time_ar  = 0;
  memset(&verification_result, 0, sizeof(verification_result));


  while(Test_Mode < 4)
  {
    /* 0 -> 16-bit Memory/PIO mode
     * 2 -> 16-bit IO/PIO mode    
     * 4 -> 16-bit Memory/DMA mode
     * 6 -> 16-bit IO/DMA mode    
     */
    switch(Test_Mode)
    {
      case 0:
        pccardHandle.cf.protocol = CF_MEM_MAP_MODE;
        pccardHandle.cf.io_addr = PCCARD_COMMON_SPACE_ADDRESS;
        pccardHandle.cf.dma_enable = FALSE;
        pccardHandle.cf.enable_8_bit_mode = FALSE;
        printf("\r\n16-bit Memory/PIO mode Start... ");
        break;

      case 1:
        pccardHandle.cf.protocol = CF_IO_16_MODE;
        pccardHandle.cf.io_addr = PCCARD_IO_SPACE_ADDRESS;
        pccardHandle.cf.dma_enable = FALSE;
        pccardHandle.cf.enable_8_bit_mode = FALSE;
        printf("\r\n16-bit IO/PIO mode Start... ");
        break;

      case 2:
        pccardHandle.cf.protocol = CF_MEM_MAP_MODE;
        pccardHandle.cf.io_addr = PCCARD_COMMON_SPACE_ADDRESS;
        pccardHandle.cf.dma_enable = TRUE;
        pccardHandle.cf.enable_8_bit_mode = FALSE;
        printf("\r\n16-bit Memory/DMA mode Start... ");
        break;

      case 3:
        pccardHandle.cf.protocol = CF_IO_16_MODE;
        pccardHandle.cf.io_addr = PCCARD_IO_SPACE_ADDRESS;
        pccardHandle.cf.dma_enable = TRUE;
        pccardHandle.cf.enable_8_bit_mode = FALSE;
        printf("\r\n16-bit IO/DMA mode Start... ");
        break;


      default:
        printf("\r\nSelect Test Mode Failed");
        break;
    }

    pccardHandle.cf.drive = 0;

    /* init compact flash controller */
    CF_print(PRT_INIT_INFO, ("\r\nInit Compact Flash Card..."));

    if(pccard_init(&pccardHandle, &PCCARD_PMEM4_Timing, &PCCARD_PATT4_Timing, &PCCARD_PIO4_Timing) != status_ok)
    {
      printf("\r\nCompact Flash card init Failed");
      break;
    }

    /* reset compact flash card */
    CF_print(PRT_INIT_INFO, ("\r\nReset Compact Flash Card..."));

    if(pccard_reset(&pccardHandle))
    {
      verification_result.attribute_rw_pass = TRUE;
    }

    CF_print(PRT_INIT_INFO, ("\r\nSet Wait Feature..."));

    if(enable_wait_feature(&pccardHandle) != status_ok)
    {
      printf("\r\nCompact Flash card Set Wait Feature Failed");
      break;
    }

    /* read cf card cis from cf card's attribute memory */
    if(read_cfcard_cis_information(&pccardHandle))
    {
      verification_result.cis_read_pass = TRUE;
    }

    /* cf-ata command set test started */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_IDENTIFY_DRIVE..."));

    /* read compact flash card id (cf_ata_identify_drive) */
    if(pccard_read_id(&pccardHandle))
    {
      verification_result.ata_identify_drive_pass = TRUE;
    }

    /* execute drive diagnostic */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_EXECUTE_DRIVE_DIAG..."));

    if(pccard_diagnostic(&pccardHandle))
    {
      verification_result.ata_execute_drive_diag_pass = TRUE;
    }

    /* write compact flash card sector */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_WRITE_SECTOR..."));
    memset(pccard_txbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));
     /* fill the buffer to write */
    fill_buffer(pccard_txbuffer, PCCARD_SECTOR_SIZE, 0x00);
    if(!pccard_write_sector(&pccardHandle, pccard_txbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Write Secor Failed");
      break;
    }

    /* read compact flash card sector */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_READ_SECTOR..."));
    memset(pccard_rxbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    if(!pccard_read_sector(&pccardHandle, pccard_rxbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Read Secor Failed");
      break;
    }
    /* checking data integrity */
    if(buffer_cmp(pccard_txbuffer, pccard_rxbuffer, PCCARD_SECTOR_SIZE) == PASSED)  
    {
      verification_result.ata_read_sector_pass = TRUE;
      verification_result.ata_write_sector_pass = TRUE;
    }
    else
    {
      verification_result.ata_read_sector_pass = FALSE;
      verification_result.ata_write_sector_pass = FALSE;
    }


    /* write compact flash card buffer */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_WRITE_BUFFER..."));
    memset(pccard_txbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));
    /* fill the buffer to write */
    fill_buffer(pccard_txbuffer, PCCARD_SECTOR_SIZE, 0x00);  

    if(!pccard_write_buffer(&pccardHandle, pccard_txbuffer))
    {
      printf("\r\nCompact Flash card Write Buffer Failed");
      break;
    }

    /* read compact flash card buffer */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_READ_BUFFER..."));
    memset(pccard_rxbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    if(!pccard_read_buffer(&pccardHandle, pccard_rxbuffer))
    {
      printf("\r\nCompact Flash card Read Buffer Failed");
      break;
    }
     /* checking data integrity */
    if(buffer_cmp(pccard_txbuffer, pccard_rxbuffer, PCCARD_SECTOR_SIZE) == PASSED)
    {
      verification_result.ata_read_buffer_pass = TRUE;
      verification_result.ata_write_buffer_pass = TRUE;
    }
    else
    {
      verification_result.ata_read_buffer_pass = FALSE;
      verification_result.ata_write_buffer_pass = FALSE;
    }

    /* recalibrate */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_RECALIBRATE..."));

    if(pccard_recalibrate(&pccardHandle))
    {
      verification_result.ata_recalibrate_pass = TRUE;
    }

    /* initialize drive parameters */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_Init_Drive_Para..."));
    orig_default_sector = pccardHandle.cf.cfcard_info.current_sector;
    orig_default_head = pccardHandle.cf.cfcard_info.current_head;


    pccardHandle.cf.cfcard_info.current_sector -= 1;
    pccardHandle.cf.cfcard_info.current_head -= 1;

    if(!pccard_init_drive_para(&pccardHandle, pccardHandle.cf.cfcard_info.current_sector, pccardHandle.cf.cfcard_info.current_head - 1))
    {
      printf("\r\nInit drive Para fail");
      break;
    }
    /*  check the command is vailded */
    if(vaild_init_drive_para(&pccardHandle, pccardHandle.cf.cfcard_info.current_sector, pccardHandle.cf.cfcard_info.current_head)) 
    {
      verification_result.ata_init_drive_para_pass = TRUE;
    }

    pccardHandle.cf.cfcard_info.current_sector = orig_default_sector;
    pccardHandle.cf.cfcard_info.current_head = orig_default_head;

    if(!pccard_init_drive_para(&pccardHandle, orig_default_sector, orig_default_head - 1))
    {
      printf("\r\nInit drive Para fail");
      break;
    }
    /*  check restore is vailded */
    if(!vaild_init_drive_para(&pccardHandle, pccardHandle.cf.cfcard_info.current_sector, pccardHandle.cf.cfcard_info.current_head))  
    {
      verification_result.ata_init_drive_para_pass = FALSE;
    }

    /* seek */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_SEEK..."));

    if(pccard_seek(&pccardHandle, pccardHandle.cf.cfcard_info.default_head - 1, pccardHandle.cf.cfcard_info.default_cylinder + 1))
    {
      verification_result.ata_seek_pass = TRUE;
    }

    /* erase compact flash card sector */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_ERASE_SECTOR..."));
    memset(pccard_txbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));
    /* fill the buffer to write */
    fill_buffer(pccard_txbuffer, PCCARD_SECTOR_SIZE, 0x00);  

    if(!pccard_write_sector(&pccardHandle, pccard_txbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Write Secor Failed");
      break;
    }

    if(!pccard_erase_sector(&pccardHandle, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Erase Secor Failed");
      break;
    }

    memset(pccard_rxbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    if(!pccard_read_sector(&pccardHandle, pccard_rxbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Read Secor Failed");
      break;
    }
    else
    {
      verification_result.ata_erase_sector_pass = TRUE;

      for(u32i = 0; u32i < PCCARD_SECTOR_SIZE; u32i++)
      {
        if(*(pccard_rxbuffer + u32i) != 0x0)
        {
          verification_result.ata_erase_sector_pass = FALSE;
          CF_print(PRT_ERR_INFO, ("\r\nErase sector error at address offset 0x%0.2x:(0x%0.2x)", u32i, *(pccard_rxbuffer + u32i)));
          break;
        }
      }
    }

    /* write compact flash card write sector without erase */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_WRITE_SECTOR_WO_ERASE..."));

    if(!pccard_erase_sector(&pccardHandle, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Erase Secor Failed");
      break;
    }

    memset(pccard_txbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));
    /* fill the buffer to write */
    fill_buffer(pccard_txbuffer, PCCARD_SECTOR_SIZE, 0x00);  

    if(!pccard_write_sector_wo_erase(&pccardHandle, pccard_txbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Write Secor Without Erase Failed");
      break;
    }

    memset(pccard_rxbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    if(!pccard_read_sector(&pccardHandle, pccard_rxbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Read Secor Failed");
      break;
    }
    /* checking data integrity */
    if(buffer_cmp(pccard_txbuffer, pccard_rxbuffer, PCCARD_SECTOR_SIZE) == PASSED)  
    {
      verification_result.ata_write_sector_wo_erase_pass = TRUE;
    }
    else
    {
      verification_result.ata_write_sector_wo_erase_pass = FALSE;
    }

    /* format track */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_FORMAT_TRACK..."));
    memset(pccard_txbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));
    /* fill the buffer to write */
    fill_buffer(pccard_txbuffer, PCCARD_SECTOR_SIZE, 0x00);  

    if(!pccard_write_sector(&pccardHandle, pccard_txbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Write Secor Failed");
      break;
    }


    memset(pccard_rxbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    if(!pccard_read_sector(&pccardHandle, pccard_rxbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Read Secor Failed");
      break;
    }
    else
    {
      verification_result.ata_format_track_pass = TRUE;

      for(u32i = 0; u32i < PCCARD_SECTOR_SIZE; u32i++)
      {
        if(!(*(pccard_rxbuffer + u32i) == 0xff || *(pccard_rxbuffer + u32i) == 0x0))
        {
          verification_result.ata_format_track_pass = FALSE;
          CF_print(PRT_ERR_INFO, ("\r\nFormat track error at address offset 0x%0.2x:(0x%0.2x)", u32i, *(pccard_rxbuffer + u32i)));
          break;
        }
      }
    }

    /* read verify sector */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_READ_VERIFY_SECTOR..."));

    if(pccard_read_verify_sector(&pccardHandle, Start_Sector, 1))
    {
      verification_result.ata_read_verify_sector_pass = TRUE;
    }

    /* write verify */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_WRITE_VERIFY..."));
    memset(pccard_txbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    fill_buffer(pccard_txbuffer, PCCARD_SECTOR_SIZE, 0x00);  // Fill the buffer to write

    if(!pccard_write_verify(&pccardHandle, pccard_txbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Write Verify Failed");
      break;
    }

    /* read compact flash card sector */
    memset(pccard_rxbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    if(!pccard_read_sector(&pccardHandle, pccard_rxbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Read Secor Failed");
      break;
    }
    /* checking data integrity */
    if(buffer_cmp(pccard_txbuffer, pccard_rxbuffer, PCCARD_SECTOR_SIZE) == PASSED)  
    {
      verification_result.ata_write_verify_pass = TRUE;
    }
    else
    {
      verification_result.ata_write_verify_pass = FALSE;
    }

    /* translate sector */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_TRANSLATE_SECTOR..."));

    if(!pccard_translate_sector(&pccardHandle, Start_Sector))
    {
      printf("\r\nCompact Flash card Translate sector Failed");
      break;
    }

    memset(pccard_rxbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    if(!pccard_read_buffer(&pccardHandle, pccard_rxbuffer))
    {
      printf("\r\nCompact Flash card Read Buffer Failed");
      break;
    }

    if((pccard_rxbuffer[1]) == pccardHandle.cf.cf_addr.cylinder &&
        (pccard_rxbuffer[2]) == pccardHandle.cf.cf_addr.head &&
        (pccard_rxbuffer[3]) == pccardHandle.cf.cf_addr.sector)
    {
      verification_result.ata_translate_sector_pass = TRUE;
    }

    /* set multiple mode */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_SET_MULTIPLE_MODE..."));
    pccard_set_multiple_mode(&pccardHandle, pccardHandle.cf.cfcard_info.max_mutiple_sector + 1);

    if(pccard_read_error_reg(&pccardHandle) & ABRT_BIT)
    {
      verification_result.ata_set_multiple_mode_pass = TRUE;
    }

    pccard_set_multiple_mode(&pccardHandle, pccardHandle.cf.cfcard_info.max_mutiple_sector);

    if(pccard_read_error_reg(&pccardHandle))
    {
      verification_result.ata_set_multiple_mode_pass = FALSE;
    }

    /* set feature */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_SET_FEATURE..."));

    pccard_set_features(&pccardHandle, 0, NOP);

    if(pccard_read_error_reg(&pccardHandle))
    {
      verification_result.ata_set_feature_pass = FALSE;
    }
    else
    {
      verification_result.ata_set_feature_pass = TRUE;
    }

    /* write multiple */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_WRITE_MULTIPLE..."));
    /* note that the maximun number of sectors on read/write multiple command in our cf test card is 1,
       so it will work same as read/write sector command. */
    memset(pccard_txbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    fill_buffer(pccard_txbuffer, PCCARD_SECTOR_SIZE, 0x00);  // Fill the buffer to write

    if(!pccard_write_multiple(&pccardHandle, pccard_txbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Write Multiple Secors Failed");
      break;
    }

    /* read multiple */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_READ_MULTIPLE..."));
    /* note that the maximun number of sectors on read/write multiple command in our cf test card is 1,
       so it will work same as read/write sector command. */
    memset(pccard_rxbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    if(!pccard_read_multiple(&pccardHandle, pccard_rxbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Read Multiple Secors Failed");
      break;
    }
    /* checking data integrity */
    if(buffer_cmp(pccard_txbuffer, pccard_rxbuffer, PCCARD_SECTOR_SIZE) == PASSED)  
    {
      verification_result.ata_read_multiple_pass = TRUE;
      verification_result.ata_write_multiple_pass = TRUE;
    }
    else
    {
      verification_result.ata_read_multiple_pass = FALSE;
      verification_result.ata_write_multiple_pass = FALSE;
    }

    /* write multiple without erase */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying CF_ATA_WRITE_MULTIPLE_WO_ERASE..."));

    /* note that the maximun number of sectors on read/write multiple command in our cf test card is 1,
       so it will work same as read/write sector command. */
    if(!pccard_erase_sector(&pccardHandle, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Erase Secor Failed");
      break;
    }

    memset(pccard_txbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));
    /* fill the buffer to write */
    fill_buffer(pccard_txbuffer, PCCARD_SECTOR_SIZE, 0x00);  

    if(!pccard_write_multiple_wo_erase(&pccardHandle, pccard_txbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Write Multiple Secors Without Erase Failed");
      break;
    }

    memset(pccard_rxbuffer, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

    if(!pccard_read_multiple(&pccardHandle, pccard_rxbuffer, Start_Sector, 1))
    {
      printf("\r\nCompact Flash card Read Multiple Secors Failed");
      break;
    }

    if(buffer_cmp(pccard_txbuffer, pccard_rxbuffer, PCCARD_SECTOR_SIZE) == PASSED)  // Checking data integrity
    {
      verification_result.ata_write_multiple_wo_erase_pass = TRUE;
    }
    else
    {
      verification_result.ata_write_multiple_wo_erase_pass = FALSE;
    }


    /*  disable the read/write multiple command, we expect abort bit in error register will be assert. */
    if(!pccard_set_multiple_mode(&pccardHandle, 0))
    {
      printf("\r\nSet multiple mode to 0 Failed");
      break;
    }

    /* test read/write 50 sectors */
    CF_print(PRT_CMD_INFO, ("\r\nVerifying Test Read/Write 50 sectors..."));

    memset(pccard_25_sectors_txbuffer, 0, PCCARD_25_SECTORS_SIZES * sizeof(uint8_t));
    /* fill the buffer to write */
    fill_buffer(pccard_25_sectors_txbuffer, PCCARD_25_SECTORS_SIZES, 0x78);  



    if(!pccard_write_sector(&pccardHandle, pccard_25_sectors_txbuffer, Start_Sector, 25))
    {
      printf("\r\nCompact Flash card Write 50 Secors Failed");
      break;
    }

    memset(pccard_25_sectors_rxbuffer, 0, PCCARD_25_SECTORS_SIZES * sizeof(uint8_t));

    if(!pccard_read_sector(&pccardHandle, pccard_25_sectors_rxbuffer, Start_Sector, 25))
    {
      printf("\r\nCompact Flash card Read 50 Secors Failed");
      break;
    }
     /* checking data integrity */
    if(buffer_cmp(pccard_25_sectors_txbuffer, pccard_25_sectors_rxbuffer, PCCARD_25_SECTORS_SIZES) == PASSED)
    {
      verification_result.acess_25_sectors_pass = TRUE;
    }
    else
    {
      verification_result.acess_25_sectors_pass = FALSE;
    }

    /* show the compact flash storage card test results */
    show_verification_result(&verification_result);

    Test_Mode++;
  }

  printf("\r\nCompact Flash Storage Card Test End");

  while(1)
  {
  }


}


/**
  * @brief  fills buffer with user predefined data.
  * @param  pbuffer: pointer on the buffer to fill
  * @param  uwbufferlenght: size of the buffer to fill
  * @param  uwoffset: first value to fill on the buffer
  * @retval none
  */
static void fill_buffer(uint8_t *pbuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t index = 0;

  /* put in global buffer same values */
  for (index = 0; index < uwBufferLenght; index++ )
  {
    pbuffer[index] = index + uwOffset;
  }
}

/**
  * @brief  compares two buffers.
  * @param  pbuffer: pointer to the buffers.
  * @param  pbuffer1: pointer to the buffers1.
  * @param  uwbufferlenght: compared buffer's length
  * @retval 1: pbuffer identical to pbuffer1
  *         0: pbuffer differs from pbuffer1
  */
static test_status buffer_cmp(uint8_t* pbuffer, uint8_t* pBuffer1, uint32_t uwBufferLenght)
{
  uint32_t counter = 0;

  while(uwBufferLenght--)
  {
    if(*pbuffer != *pBuffer1)
    {
      printf("\r\nFormat track error at address offset 0x%0.2x:(0x%0.2x) <-> (0x%0.2x)", counter, *(pbuffer), *(pBuffer1));
      return FAILED;
    }

    pbuffer++;
    pBuffer1++;
    counter++;
  }
  return PASSED;
}

/**
  * @}
  */

/**
  * @}
  */

