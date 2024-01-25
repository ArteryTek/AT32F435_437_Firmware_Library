/**
  **************************************************************************
  * @file     cf.c
  * @brief    xmc pc card program
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

#include "cf.h"
#include "at32f435_437.h"
#include "at32f435_437_board.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup XMC_CF_CARD
  * @{
  */

u16 DMA1_MEM_LEN;


#define PCCARD_TIMEOUT_READ_ID      (uint32_t)0x0000FFFF
#define PCCARD_TIMEOUT_SECTOR       (uint32_t)0x0000FFFF
#define PCCARD_TIMEOUT_STATUS       (uint32_t)0x01000000

#define PCCARD_status_ok            (uint8_t)0x58
#define PCCARD_STATUS_WRITE_OK      (uint8_t)0x50

/**
  * @brief  dma configuration
  * @param  edma_chx: pointer to a edma_channel_type structure
  * @param  cpar_src: the memory 1 addr
  * @param  cmar_dst: the memory 2 addr
  * @param  cndtr: transfer size
  * @param  transfer_type: 
  *     @arg read_transfer: from memory 1 to memory 2
  *     @arg write_transfer: from memory 2 to memory 1
  * @param  transfer_width: byte or halfword
  * @retval none
  */
void MYDMA_Config(edma_stream_type* dmay_streamx, uint32_t cpar_src, uint32_t cmar_dst, uint32_t cndtr, uint8_t Transfer_Type, uint8_t Transfer_Width)
{
  edma_init_type  edma_init_struct;
  crm_periph_clock_enable(CRM_EDMA_PERIPH_CLOCK, TRUE);

  edma_reset(dmay_streamx);

  if(Transfer_Type == READ_TRANSFER)
  {
    edma_init_struct.peripheral_inc_enable = FALSE;
    edma_init_struct.memory_inc_enable = TRUE;
  }
  else
  {
    edma_init_struct.peripheral_inc_enable = TRUE;
    edma_init_struct.memory_inc_enable = FALSE;
  }

  if(Transfer_Width == ENABLE_8_BIT_TRANSFER)
  {
    edma_init_struct.peripheral_data_width = EDMA_PERIPHERAL_DATA_WIDTH_BYTE;
    edma_init_struct.memory_data_width = EDMA_MEMORY_DATA_WIDTH_BYTE;
  }
  else
  {
    edma_init_struct.peripheral_data_width = EDMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    edma_init_struct.memory_data_width = EDMA_MEMORY_DATA_WIDTH_HALFWORD;
  }

  edma_init_struct.direction = EDMA_DIR_MEMORY_TO_MEMORY;
  edma_init_struct.buffer_size = cndtr;
  edma_init_struct.peripheral_base_addr = cpar_src;

  edma_init_struct.peripheral_burst_mode = EDMA_PERIPHERAL_SINGLE;
  edma_init_struct.memory0_base_addr = cmar_dst;

  edma_init_struct.memory_burst_mode = EDMA_MEMORY_SINGLE;
  edma_init_struct.loop_mode_enable = FALSE;
  edma_init_struct.fifo_mode_enable = TRUE;
  edma_init_struct.fifo_threshold = EDMA_FIFO_THRESHOLD_FULL;
  edma_init_struct.priority = EDMA_PRIORITY_MEDIUM;
  edma_init(dmay_streamx,&edma_init_struct);
 
  
  edma_stream_enable(dmay_streamx, TRUE);
}

/**
  * @brief  xmc init ctrl
  * @param  device: pointer to a xmc_bank4_type structure
  * @param  init: pointer to a xmc_pccardinittype structure
  * @param  set_reg_8_bit: choose 8/16bit
  * @retval status_ok or status_error
  */
status_type xmc_pccard_init_ctrl(xmc_bank4_type *Device, xmc_pccard_init_type *init, uint8_t Set_reg_8_bit)
{
  if(Set_reg_8_bit == 1)
  {
   Device->bk4ctrl_bit.extmdbw = XMC_MEM_WIDTH_8;
  }
  else
  {
   Device->bk4ctrl_bit.extmdbw = XMC_MEM_WIDTH_16;
  }
   
  Device->bk4ctrl_bit.tar = init->delay_time_ar;
  Device->bk4ctrl_bit.tcr = init->delay_time_cr;

  return status_ok;

}

/**
  * @brief  xmc enable wait feature
  * @param  device: pointer to a xmc_bank4_type structure
  * @param  init: pointer to a xmc_pccardinittype structure
  * @retval status_ok or status_error
  */
status_type xmc_enable_wait_feature(xmc_bank4_type *Device, xmc_pccard_init_type *init)
{
 
 Device->bk4ctrl_bit.nwen = init->enable_wait;

  return status_ok;
}

/**
  * @brief  set 16bit_feature
  * @param  device: pointer to a xmc_bank4_type structure
  * @param  init: pointer to a xmc_pccardinittype structure
  * @param  set_reg_8_bit: choose 8/16bit
  * @retval status
  */
status_type xmc_enable_16bit_feature(xmc_bank4_type *Device, xmc_pccard_init_type *init, uint8_t Set_reg_8_bit)
{
  if(Set_reg_8_bit == 1)
  {
   Device->bk4ctrl_bit.extmdbw = XMC_MEM_WIDTH_8;
  }
  else
  {
   Device->bk4ctrl_bit.extmdbw = XMC_MEM_WIDTH_16;
  }

  return status_ok;
}

/**
  * @brief  init commonspace timing
  * @param  device: pointer to a xmc_bank4_type structure
  * @param  timing: pointer to a common space timing structure
  * @retval status
  */
status_type xmc_pccard_init_common_space_timing(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing)
{
 Device->bk4tmgmem_bit.cmdhizt = Timing->mem_hiz_time;
 Device->bk4tmgmem_bit.cmht = Timing->mem_hold_time;
 Device->bk4tmgmem_bit.cmst = Timing->mem_setup_time;
 Device->bk4tmgmem_bit.cmwt = Timing->mem_waite_time;

  return status_ok;
}

/**
  * @brief  init attributespace timing
  * @param  device: pointer to a xmc_bank4_type structure
  * @param  timing: pointer to a attribute space timing structure
  * @retval status
  */
status_type xmc_pccard_init_attribute_space_timing(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing)
{
  Device->bk4tmgatt_bit.amdhizt = Timing->mem_hiz_time;
  Device->bk4tmgatt_bit.amht = Timing->mem_hold_time;
  Device->bk4tmgatt_bit.amst = Timing->mem_setup_time;
  Device->bk4tmgatt_bit.amwt = Timing->mem_waite_time;
  return status_ok;
}


/**
  * @brief  init iospace timing
  * @param  device: pointer to a xmc_bank4_type structure
  * @param  timing: pointer to a io space timing structure
  * @retval status
  */
status_type xmc_pccard_init_iospace_timing(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing)
{
 
  Device->bk4tmgio_bit.iohizt = Timing->mem_hiz_time;
  Device->bk4tmgio_bit.ioht = Timing->mem_hold_time;
  Device->bk4tmgio_bit.iost = Timing->mem_setup_time;
  Device->bk4tmgio_bit.iowt = Timing->mem_waite_time;

  return status_ok;
}

/**
  * @brief  perform the pccard memory initialization sequence
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  comspacetiming: common space timing structure
  * @param  attspacetiming: attribute space timing structure
  * @param  iospacetiming: io space timing structure
  * @retval status
  */
status_type pccard_init(pccard_handle_type *hpccard, xmc_nand_pccard_timinginit_type *comspacetiming, xmc_nand_pccard_timinginit_type *attspacetiming, xmc_nand_pccard_timinginit_type *iospacetiming)
{
  gpio_init_type  GPIO_InitStructure;
  uint8_t Set_reg_8_bit;

  /* check the pccard controller state */
  if(hpccard == NULL)
  {
    return status_error;
  }

 if(hpccard->cf.enable_8_bit_mode == TRUE)
  Set_reg_8_bit = 1;
 else
  Set_reg_8_bit = 0;


  if(hpccard->state == pccard_state_reset)
  {
    /* allocate lock resource and initialize it */
    hpccard->lock = unlocked;

  }

  /* initialize the pccard state */
  hpccard->state = pccard_state_busy;


  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK,TRUE);
  /* address line:pf0 pf1 pf2 pf3 pf4 pf5 pf12 pf13 pf14 pf15 pd5 pg1 pg2 pg3 pg4 pg5 pd11 pd12 pd13 pe3 pe4 pe5 pe6 pe2 pg13 pg14 */
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE2, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE3, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE15, GPIO_MUX_12);
  
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_0|GPIO_PINS_1|GPIO_PINS_2|GPIO_PINS_3|GPIO_PINS_4|GPIO_PINS_5|GPIO_PINS_12|GPIO_PINS_13|GPIO_PINS_14|GPIO_PINS_15; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOF, &GPIO_InitStructure);
  
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE5, GPIO_MUX_10);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_11|GPIO_PINS_12|GPIO_PINS_13|GPIO_PINS_5; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &GPIO_InitStructure);

  /* data line:   pb14 pc6 pc11 pc12 pe7 pa3 pa4 pa5 pe11 pe12 pe13 pe14 pe15 pb12 pd9 pd10 */
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE14, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE12, GPIO_MUX_14);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_14|GPIO_PINS_12; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE6, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE11, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE12, GPIO_MUX_14);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_6|GPIO_PINS_11|GPIO_PINS_12; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE3, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE4, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE5, GPIO_MUX_14);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_2|GPIO_PINS_3|GPIO_PINS_4|GPIO_PINS_5; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE7, GPIO_MUX_12); 
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE15, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_11|GPIO_PINS_12|GPIO_PINS_13|GPIO_PINS_14|GPIO_PINS_15|GPIO_PINS_7; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOE, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_9|GPIO_PINS_10; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &GPIO_InitStructure);
  /* xmc_nce4_1/xmc_nce4_2:pg10/pg11 xmc_noe:pd4 xmc_nwe:pc2 xmc_nbl0:pe0 xmc_nbl1:pe1 xmc_nadv:pb7  xmc_nwait:pd6 */
  /* xmc_niord:pf6  xmc_niowr:pf8   xmc_nreg:pf7   xmc_intr:pf10  xmc_cd:pf9  */
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE6, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_4|GPIO_PINS_6; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOD, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE2, GPIO_MUX_14);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_2; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOC, &GPIO_InitStructure);

  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_10|GPIO_PINS_11; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOG, &GPIO_InitStructure);
  
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE6, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE7, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_9|GPIO_PINS_6|GPIO_PINS_7|GPIO_PINS_8|GPIO_PINS_10; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOF, &GPIO_InitStructure);

  /* initialize pccard control interface */
  xmc_pccard_init_ctrl(hpccard->instance, &(hpccard->init), Set_reg_8_bit);

  /* init pccard common space timing interface */
  xmc_pccard_init_common_space_timing(hpccard->instance, comspacetiming);

  /* init pccard attribute space timing interface */
  xmc_pccard_init_attribute_space_timing(hpccard->instance, attspacetiming);

  /* init pccard io space timing interface */
  xmc_pccard_init_iospace_timing(hpccard->instance, iospacetiming);

  /* enable the pccard device */
  xmc_pccard_enable(TRUE);

  /* update the pccard state */
  hpccard->state = pccard_state_ready;

  return status_ok;

}

/**
  * @brief  enable wait feature
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval status_ok or status_error
  */
status_type enable_wait_feature(pccard_handle_type *hpccard)
{
  /* check the pccard controller state */
  if(hpccard == NULL)
  {
    return status_error;
  }

  if(hpccard->state == pccard_state_reset)
  {
    /* allocate lock resource and initialize it */
    hpccard->lock = unlocked;
  }

  /* initialize the pccard state */
  hpccard->state = pccard_state_busy;

  xmc_enable_wait_feature(hpccard->instance, &(hpccard->init));

  hpccard->state = pccard_state_ready;

  return status_ok;
}

/**
  * @brief  perform the pccard memory de-initialization sequence
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval status
  */
status_type  pccard_deinit(pccard_handle_type *hpccard)
{
  /* configure the pccard registers with their reset values */
  xmc_pccard_reset();

  /* update the pccard controller state */
  hpccard->state = pccard_state_reset;
  return status_ok;
}

/**
  * @brief  identify
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL pccard_identify(pccard_handle_type *hpccard)
{
  uint8_t Reg;

  Reg = hpccard->cf.cf_addr.drv;

  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD) = Reg;
  if(!cf_send_command(hpccard, ATA_IDENTIFY_DRIVE_CMD))
  {
    return FALSE;
  }

  return TRUE;
}


/**
  * @brief  read compact flash's id.
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  compactflash_id: compact flash id structure.
  * @retval status
  *
  */
BOOL pccard_read_id(pccard_handle_type *hpccard)
{
  uint8_t CardInfo[PCCARD_SECTOR_SIZE * sizeof(uint8_t)] = {0};

  memset(CardInfo, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));
  pccard_read_status_reg(hpccard);

  if(pccard_identify(hpccard))
  {
    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
        {
          /* read cf id bytes */

          read_sector(hpccard, CardInfo, PCCARD_SECTOR_SIZE);

          break;
        }
        else if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
        {
          printf("\r\nCommand Pass, but ERR bit is high");
          printf("\r\nCF Status Reg=%#x", pccard_read_status_reg(hpccard));
          printf("\r\nCF Error  Reg=%#x", pccard_read_error_reg(hpccard));
          return FALSE;
        }
      }
    }
  }

  while(1)
  {
    if(task_file_regis_valid(hpccard))
    {
      if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
      {
        break;
      }
      else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        printf("\r\nSend Command Failed");
        printf("\r\nCF Status Reg=%#x", pccard_read_status_reg(hpccard));
        printf("\r\nCF Error  Reg=%#x", pccard_read_error_reg(hpccard));
        return FALSE;
      }
    }
  }

#if 1

  printf("\r\n===============================================================\n");
  printf("\r\n\nIdentify drive Information : ");
  printf("\r\n%s%#x", "Signature : ", ((uint16_t)CardInfo[0] | (uint16_t)CardInfo[1] << 8));
  printf("\r\n%s%u", "Default number of cylinders : ", ((uint16_t)CardInfo[2] | (uint16_t)CardInfo[3] << 8));
  printf("\r\n%s%u", "Default number of heads : ", ((uint16_t)CardInfo[6] | (uint16_t)CardInfo[7] << 8));
  printf("\r\n%s%u", "Number of unformatted bytes per tract : ", ((uint16_t)CardInfo[8] | (uint16_t)CardInfo[9] << 8));
  printf("\r\n%s%u", "Number of unformatted bytes per sector : ", ((uint16_t)CardInfo[10] | (uint16_t)CardInfo[11] << 8));
  printf("\r\n%s%u", "Default number of sectors per track : ", ((uint16_t)CardInfo[12] | (uint16_t)CardInfo[13] << 8));
  printf("\r\n%s%u", "Number of sectors per card : ", (uint32_t)((uint16_t)CardInfo[16] | (uint16_t)CardInfo[17] << 8) | ((uint32_t)((uint16_t)CardInfo[14] | (uint16_t)CardInfo[15] << 8) << 16));
  printf("\r\n%s%u", "Vendor Unique : ", ((uint16_t)CardInfo[18] | (uint16_t)CardInfo[19] << 8));
  printf("\r\n%s%#x", "Buffer type : ", ((uint16_t)CardInfo[40] | (uint16_t)CardInfo[41] << 8));
  printf("\r\n%s%u", "Buffer size in 512 byte increments : ", ((uint16_t)CardInfo[42] | (uint16_t)CardInfo[43] << 8));
  printf("\r\n%s%u", "#of ECC bytes passed on Read/Write Long Commands : ", ((uint16_t)CardInfo[44] | (uint16_t)CardInfo[45] << 8));
  printf("\r\n%s%u", "Maximum number of sectors on Read/Write Multiple command : ", ((uint16_t)CardInfo[94] | (uint16_t)CardInfo[95] << 8));
  printf("\r\n%s%u", "Double Word not supported : ", ((uint16_t)CardInfo[96] | (uint16_t)CardInfo[97] << 8));
  printf("\r\n%s%u", "Capabilities : ", ((uint16_t)CardInfo[98] | (uint16_t)CardInfo[99] << 8));
  printf("\r\n%s%u", "PIO data transfer cycle timing mode : ", ((uint16_t)CardInfo[102] | (uint16_t)CardInfo[103] << 8));
  printf("\r\n%s%u", "DMA data transfer cycle timing mode: ", ((uint16_t)CardInfo[104] | (uint16_t)CardInfo[105] << 8));
  printf("\r\n%s%u", "Translation parameters are valid : ", ((uint16_t)CardInfo[106] | (uint16_t)CardInfo[107] << 8));
  printf("\r\n%s%u", "Current numbers of cylinders : ", ((uint16_t)CardInfo[108] | (uint16_t)CardInfo[109] << 8));
  printf("\r\n%s%u", "Current numbers of heads : ", ((uint16_t)CardInfo[110] | (uint16_t)CardInfo[111] << 8));
  printf("\r\n%s%u", "Current sectors per track : ", ((uint16_t)CardInfo[112] | (uint16_t)CardInfo[113] << 8));
  printf("\r\n%s%u", "Current capacity in sectors : ", (uint32_t)((uint16_t)CardInfo[116] | (uint16_t)CardInfo[117] << 8) | ((uint32_t)((uint16_t)CardInfo[114] | (uint16_t)CardInfo[115] << 8) << 16));
  printf("\r\n%s%#x", "Multiple sector setting : ", ((uint16_t)CardInfo[118] | (uint16_t)CardInfo[119] << 8));
  printf("\r\n%s%u", "Total number of sectors addressable in LBA Mode : ", (uint32_t)((uint16_t)CardInfo[120] | (uint16_t)CardInfo[121] << 8) | ((uint32_t)((uint16_t)CardInfo[122] | (uint16_t)CardInfo[123] << 8) << 16));
  printf("\r\n%s%#x", "Security status : ",  ((uint16_t)CardInfo[256] | (uint16_t)CardInfo[257] << 8));
  printf("\r\n%s%#x", "Power requirement description : ",  ((uint16_t)CardInfo[320] | (uint16_t)CardInfo[321] << 8));
  printf("\r\n===============================================================");

#endif

  /* signature of cf storage */
  if(((uint16_t)CardInfo[0] | (uint16_t)CardInfo[1] << 8) == 0x848A)
  {
    hpccard->cf.cfcard_info.default_cylinder = ((uint16_t)CardInfo[2] | (uint16_t)CardInfo[3] << 8);
    hpccard->cf.cfcard_info.default_head = ((uint16_t)CardInfo[6] | (uint16_t)CardInfo[7] << 8);
    hpccard->cf.cfcard_info.default_sector = ((uint16_t)CardInfo[12] | (uint16_t)CardInfo[13] << 8);
    hpccard->cf.cfcard_info.current_cylinder = ((uint16_t)CardInfo[108] | (uint16_t)CardInfo[109] << 8);
    hpccard->cf.cfcard_info.current_head = ((uint16_t)CardInfo[110] | (uint16_t)CardInfo[111] << 8);
    hpccard->cf.cfcard_info.current_sector = ((uint16_t)CardInfo[112] | (uint16_t)CardInfo[113] << 8);

    hpccard->cf.cfcard_info.total_sector = (uint32_t)((uint16_t)CardInfo[16] | (uint16_t)CardInfo[17] << 8) | ((uint32_t)((uint16_t)CardInfo[14] | (uint16_t)CardInfo[15] << 8) << 16);
    hpccard->cf.cfcard_info.total_lba_sector = (uint32_t)((uint16_t)CardInfo[120] | (uint16_t)CardInfo[121] << 8) | ((uint32_t)((uint16_t)CardInfo[122] | (uint16_t)CardInfo[123] << 8) << 16);
    hpccard->cf.cfcard_info.max_mutiple_sector = ((uint16_t)CardInfo[94] | (uint16_t)CardInfo[95] << 8);
    hpccard->cf.cfcard_info.mutiple_sector_setting = ((uint16_t)CardInfo[118] | (uint16_t)CardInfo[119] << 8);
  }
  else
  {

    return FALSE;
  }

  return TRUE;
}

/**
  * @brief  read sector from pccard memory
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to destination read buffer
  * @param  sector_address: sector address to read
  * @retval status
  */
BOOL pccard_read_sector(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(sector_count == 0)
  {
    sector_count = 256;
  }

  while(Count < sector_count)
  {

    translate_chsaddr(hpccard, sector_address, Count, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = (uint8_t)hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_READ_SECTOR_CMD))
    {
      while(1)
      {
        if(task_file_regis_valid(hpccard))
        {
          if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            pbuffer = read_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

            sector_address += 1;
            Count += 1;

            if (Count == sector_count)
            {
              break;
            }

            if (Count <= (sector_count / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector)
              if (Count % hpccard->cf.cf_addr.sector_count == 0)
              {
                break;
              }
          }
          else if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
                    printf("Error Code=%X\n", pccard_read_status_reg(hpccard));
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
  * @brief  write sector to pccard memory
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to source write buffer
  * @param  sector_address: sector address to write
  * @retval status
  */
BOOL pccard_write_sector(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(sector_count == 0)
  {
    sector_count = 256;
  }

  while(Count < sector_count)
  {
    // Set the parameters to write a sector //
    translate_chsaddr(hpccard, sector_address, Count, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    //*(__IO uint16_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = hpccard->cf.cf_addr.cylinder;
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = (uint8_t)hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_WRITE_SECTOR_CMD))
    {
      while(1)
      {
        if(task_file_regis_valid(hpccard))
        {
          if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            pbuffer = write_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

            sector_address += 1;
            Count += 1;

            if (Count == sector_count)
            {
              break;
            }

            if (Count <= (sector_count / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector)
              if (Count % hpccard->cf.cf_addr.sector_count == 0)
              {
                break;
              }
          }
          else if (pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
                    printf("Error Code=%X\n", pccard_read_status_reg(hpccard));
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}


/**
  * @brief  erase sector from pccard memory
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_address: sector address to erase
  * @retval status
  */
BOOL pccard_erase_sector(pccard_handle_type *hpccard, uint32_t sector_address, uint32_t sector_count)
{
  uint8_t Reg;
  uint16_t Count = 0;

  while (Count < sector_count)
  {

    translate_chsaddr(hpccard, sector_address, 0, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_ERASE_SECTOR_CMD))
    {
      Count += hpccard->cf.cf_addr.sector_count;
      sector_address += hpccard->cf.cf_addr.sector_count;
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
  * @brief  diagnostic
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL pccard_diagnostic(pccard_handle_type *hpccard)
{
  uint8_t ERROR_REG;

  /* Set the parameters */
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(!cf_send_command(hpccard, ATA_EXECUTE_DRIVE_DIAG_CMD))
  {
    return FALSE;
  }

  ERROR_REG = pccard_read_error_reg(hpccard);

  if((ERROR_REG == 0x01) || (ERROR_REG == 0x02) || (ERROR_REG == 0x03) || (ERROR_REG == 0x04) || (ERROR_REG == 0x05) || (ERROR_REG == 0x80))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  check power mode
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL pccard_check_power_mode(pccard_handle_type *hpccard)
{
  uint8_t sector_count_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = 0x80;    
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(!cf_send_command(hpccard, ATA_CHECK_POWER_MODE_CMD))
  {
    return FALSE;
  }

  sector_count_REG =  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count);

  if(sector_count_REG == 0xFF || sector_count_REG == 0x00)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  set idle 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_count: the sector count 
  * @retval true or false.
  */
BOOL pccard_idle(pccard_handle_type *hpccard, uint32_t sector_count)
{
  uint8_t sector_count_REG;

  hpccard->cf.cf_addr.sector_count = sector_count;

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(!cf_send_command(hpccard, ATA_IDLE_CMD))
  {
    return FALSE;
  }

  /* check power mode */
  if(!pccard_check_power_mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  sector_count_REG =  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count);

  if(sector_count_REG == 0xFF)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  idle immediate 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL pccard_idle_immediate(pccard_handle_type *hpccard)
{
  uint8_t sector_count_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = 0x80;    
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(!cf_send_command(hpccard, ATA_IDLE_IMMEDIATE_CMD))
  {
    return FALSE;
  }

  /* Check Power Mode */
  if(!pccard_check_power_mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  sector_count_REG =  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count);

  if(sector_count_REG == 0xFF)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  set sleep mode 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL pccard_set_sleep_mode(pccard_handle_type *hpccard)
{
  uint8_t sector_count_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = 0x80;    
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(!cf_send_command(hpccard, ATA_SET_SLEEP_MODE_CMD))
  {
    return FALSE;
  }

  /* Check Power Mode */
  if(!pccard_check_power_mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  sector_count_REG =  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count);

  if(sector_count_REG == 0x00)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  set standby 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL pccard_standby(pccard_handle_type *hpccard)
{
  uint8_t sector_count_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = 0x80;    
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(!cf_send_command(hpccard, ATA_STANDBY_CMD))
  {
    return FALSE;
  }

  /* check power mode */
  if(!pccard_check_power_mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  sector_count_REG =  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count);

  if(sector_count_REG == 0x00)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  standby immediate 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL pccard_standby_immediate(pccard_handle_type *hpccard)
{
  uint8_t sector_count_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = 0x80;    
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(!cf_send_command(hpccard, ATA_STANDBY_IMMEDIATE_CMD))
  {
    return FALSE;
  }

  /* check power mode */
  if(!pccard_check_power_mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  sector_count_REG =  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count);

  if(sector_count_REG == 0x00)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  reuqest sense the pccard 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL pccard_reuqest_sense(pccard_handle_type *hpccard)
{
  uint8_t Reg, ERROR_REG;

  Reg = hpccard->cf.cf_addr.drv | 0xA0;

  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

  if(!cf_send_command(hpccard, ATA_REQUEST_SENSE_CMD))
  {
    return FALSE;
  }

  ERROR_REG = pccard_read_error_reg(hpccard);

  if((ERROR_REG == 0x2F) || (ERROR_REG == 0x21))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  recalibrate the pccard 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL pccard_recalibrate(pccard_handle_type *hpccard)
{
  uint8_t Reg, ERROR_REG;

  Reg = hpccard->cf.cf_addr.drv | 0xA0;

  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

  if(!cf_send_command(hpccard, ATA_RECALIBRATE_CMD))
  {
    return FALSE;
  }

  ERROR_REG = pccard_read_error_reg(hpccard);

  if((0x00 == ERROR_REG))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  init drive para 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_count: the sector count 
  * @param  head_count: the head count.
  * @retval true or false.
  */
BOOL pccard_init_drive_para(pccard_handle_type *hpccard, uint16_t sector_count, uint8_t head_count)
{
  uint8_t Reg;

  hpccard->cf.cf_addr.head = (uint8_t)head_count;
  hpccard->cf.cf_addr.sector_count = (uint8_t)sector_count;

  Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv;

  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

  if(!cf_send_command(hpccard, ATA_INIT_DRIVE_PARA_CMD))
  {
    return FALSE;
  }

  return TRUE;
}

/**
  * @brief  seek pccard 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  head_count: the head count.
  * @param  cylinder_count: the cylinder count 
  * @retval true or false.
  */
BOOL pccard_seek(pccard_handle_type *hpccard, uint8_t head_count, uint16_t Cylinder_Count)
{
  uint8_t Reg, ERROR_REG;

  hpccard->cf.cf_addr.head = (uint8_t)head_count;
  hpccard->cf.cf_addr.cylinder = (uint16_t)Cylinder_Count;

  Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);

  /* This command should return false */
  if(cf_send_command(hpccard, ATA_SEEK_CMD)) 
  {
    return FALSE;
  }

  ERROR_REG = pccard_read_error_reg(hpccard);

  if((ERROR_REG != 0x00))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  format track 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @param  sector_address: the sector addr 
  * @param  sector_count: the sector count 
  * @retval true or false.
  */
BOOL pccard_format_track(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count)
{
  uint16_t Count = 0;
  uint8_t  Reg;

  if(sector_count == 0)
  {
    sector_count = 256;
  }

  while(Count < sector_count)
  {
    /* set the parameters to write a sector */
    translate_chsaddr(hpccard, sector_address, Count, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_FORMAT_TRACK_CMD))
    {
      while(1)
      {
        if(task_file_regis_valid(hpccard))
        {
          if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
           // write_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

            sector_address += 1;
            Count += 1;

            if (Count == sector_count)
            {
              break;
            }

            if (Count <= (sector_count / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector)
              if (Count % hpccard->cf.cf_addr.sector_count == 0)
              {
                break;
              }
          }
          else if (pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
  * @brief  read verify sector
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_address: the sector addr 
  * @param  sector_count: the sector count 
  * @retval true or false.
  */
BOOL pccard_read_verify_sector(pccard_handle_type *hpccard, uint32_t sector_address, uint16_t sector_count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(sector_count == 0)
  {
    sector_count = 256;
  }

  while(Count < sector_count)
  {

    translate_chsaddr(hpccard, sector_address, Count, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_READ_VERIFY_SECTOR_CMD))
    {
      while(1)
      {
        if(task_file_regis_valid(hpccard))
        {
          if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT))
          {
            if(pccard_read_status_reg(hpccard) == DRQ_BIT)
            {
              return FALSE;
            }
            else
            {
              sector_address += 1;
              Count += 1;

              if (Count == sector_count)
              {
                break;
              }

              if (Count <= (sector_count / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector)
                if (Count % hpccard->cf.cf_addr.sector_count == 0)
                {
                  break;
                }
            }
          }
          else if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
  * @brief  write verify 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @param  sector_address: the sector addr 
  * @param  sector_count: the sector count 
  * @retval true or false.
  */
BOOL pccard_write_verify(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count)
{
  uint16_t Count = 0;
  uint8_t  Reg;

  if(sector_count == 0)
  {
    sector_count = 256;
  }

  while(Count < sector_count)
  {
    // Set the parameters to write a sector //
    translate_chsaddr(hpccard, sector_address, Count, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_WRITE_VERIFY_CMD))
    {
      while(1)
      {
        if(task_file_regis_valid(hpccard))
        {
          if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            write_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

            sector_address += 1;
            Count += 1;

            if (Count == sector_count)
            {
              break;
            }

            if (Count <= (sector_count / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector)
              if (Count % hpccard->cf.cf_addr.sector_count == 0)
              {
                break;
              }
          }
          else if (pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
  * @brief  set multiple mode 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_count: the sector count 
  * @retval true or false.
  */
BOOL pccard_set_multiple_mode(pccard_handle_type *hpccard, uint32_t sector_count)
{
  hpccard->cf.cf_addr.sector_count = sector_count;

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(!cf_send_command(hpccard, ATA_SET_MULTIPLE_MODE_CMD))
  {
    return FALSE;
  }

  return TRUE;
}

/**
  * @brief  set features 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_count: the sector count 
  * @param  feature: feature val
  * @retval true or false.
  */
BOOL pccard_set_features(pccard_handle_type *hpccard, uint32_t sector_count, uint8_t Feature)
{
  hpccard->cf.cf_addr.sector_count = sector_count;

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_FEATURE_REG)   = Feature;

  if(!cf_send_command(hpccard, ATA_SET_FEATURE_CMD))
  {
    return FALSE;
  }

  return TRUE;
}

/**
  * @brief  read buffer 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @retval true or false.
  */
BOOL pccard_read_buffer(pccard_handle_type *hpccard, uint8_t *pbuffer)
{
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(cf_send_command(hpccard, ATA_READ_BUFFER_CMD))
  {
    if (task_file_regis_valid(hpccard))
    {
      if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
      {
        read_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);
      }
      else if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
      {
        return FALSE;
      }
    }
  }

  while(1)
  {
    if(task_file_regis_valid(hpccard))
    {
      if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
      {
        break;
      }
      else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
  * @brief  write buffer 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @retval true or false.
  */
BOOL pccard_write_buffer(pccard_handle_type *hpccard, uint8_t *pbuffer)
{
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = hpccard->cf.cf_addr.drv;

  if(cf_send_command(hpccard, ATA_WRITE_BUFFER_CMD))
  {
    if (task_file_regis_valid(hpccard))
    {
      if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
      {
        write_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);
      }
      else if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
      {
        return FALSE;
      }
    }
  }

  while(1)
  {
    if(task_file_regis_valid(hpccard))
    {
      if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT))
      {
        break;
      }
      else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
  * @brief  translate sector 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_address: the sector addr 
  * @retval true or false.
  */
BOOL pccard_translate_sector(pccard_handle_type *hpccard, uint32_t sector_address)
{
  uint32_t Total_Cylinder;
  uint8_t Reg;

  Total_Cylinder = (uint32_t)(hpccard->cf.cfcard_info.default_head * hpccard->cf.cfcard_info.default_sector); // The total sectors in one cylinder

  hpccard->cf.cf_addr.cylinder  = (uint16_t)(sector_address / Total_Cylinder);
  hpccard->cf.cf_addr.head      = (uint8_t)((sector_address % Total_Cylinder) / hpccard->cf.cfcard_info.default_sector);
  hpccard->cf.cf_addr.sector    = (uint8_t)((sector_address % Total_Cylinder) % hpccard->cf.cfcard_info.default_sector) + 1;

  /* set the parameters to write a sector */
  Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

  if(!cf_send_command(hpccard, ATA_TRANSLATE_SECTOR_CMD))
  {
    return FALSE;
  }

  return TRUE;
}

/**
  * @brief  write sector wo_erase
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @param  sector_address: the sector addr 
  * @param  sector_count: the sector count 
  * @retval true or false.
  */
BOOL pccard_write_sector_wo_erase(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count)
{
  uint16_t Count = 0;
  uint8_t  Reg;

  if(sector_count == 0)
  {
    sector_count = 256;
  }

  while(Count < sector_count)
  {
    /* set the parameters to write a sector */
    translate_chsaddr(hpccard, sector_address, Count, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_WRITE_SECTOR_WO_ERASE_CMD))
    {
      while(1)
      {
        if(task_file_regis_valid(hpccard))
        {
          if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            write_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

            sector_address += 1;
            Count += 1;

            if (Count == sector_count)
            {
              break;
            }

            if (Count <= (sector_count / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector)
              if (Count % hpccard->cf.cf_addr.sector_count == 0)
              {
                break;
              }
          }
          else if (pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
  * @brief  write multiple
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @param  sector_address: the sector addr 
  * @param  sector_count: the sector count 
  * @retval true or false.
  */
BOOL pccard_write_multiple(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count)
{
  uint16_t Count = 0;
  uint8_t  Reg;

  if(sector_count == 0)
  {
    sector_count = 256;
  }

  while(Count < sector_count)
  {
    /* set the parameters to write a sector */
    translate_chsaddr(hpccard, sector_address, Count, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_WRITE_MULTIPLE_CMD))
    {
      while(1)
      {
        if(task_file_regis_valid(hpccard))
        {
          if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            write_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

            sector_address += 1;
            Count += 1;

            if (Count == sector_count)
            {
              break;
            }

            if (Count <= (sector_count / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector)
              if (Count % hpccard->cf.cf_addr.sector_count == 0)
              {
                break;
              }
          }
          else if (pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
  * @brief  read multiple 
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @param  sector_address: the sector addr 
  * @param  sector_count: the sector count 
  * @retval true or false.
  */
BOOL pccard_read_multiple(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(sector_count == 0)
  {
    sector_count = 256;
  }

  while(Count < sector_count)
  {

    translate_chsaddr(hpccard, sector_address, Count, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_READ_MULTIPLE_CMD))
    {
      while(1)
      {
        if(task_file_regis_valid(hpccard))
        {
          if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            read_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

            sector_address += 1;
            Count += 1;

            if (Count == sector_count)
            {
              break;
            }

            if (Count <= (sector_count / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector)
              if (Count % hpccard->cf.cf_addr.sector_count == 0)
              {
                break;
              }
          }
          else if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
  * @brief  write multiple wo_erase
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @param  sector_address: the sector addr 
  * @param  sector_count: the sector count 
  * @retval true or false.
  */
BOOL pccard_write_multiple_wo_erase(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(sector_count == 0)
  {
    sector_count = 256;
  }

  while(Count < sector_count)
  {
    /* set the parameters to write a sector */
    translate_chsaddr(hpccard, sector_address, Count, sector_count);
    Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
    *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

    if(cf_send_command(hpccard, ATA_WRITE_MULTIPLE_WO_ERASE_CMD))
    {
      while(1)
      {
        if(task_file_regis_valid(hpccard))
        {
          if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            write_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

            sector_address += 1;
            Count += 1;

            if (Count == sector_count)
            {
              break;
            }

            if (Count <= (sector_count / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector)
              if (Count % hpccard->cf.cf_addr.sector_count == 0)
              {
                break;
              }
          }
          else if (pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

/**
  * @brief  read long sector
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_address: the sector addr 
  * @param  pbuffer: pointer to the data buffer.
  * @retval true or false.
  */
BOOL pccard_read_long_sector(pccard_handle_type *hpccard, uint32_t sector_address, uint8_t *pbuffer)
{
  uint32_t index = 0, Total_Cylinder;
  uint8_t Reg;
  BOOL command_vaild = TRUE;

  Total_Cylinder = (uint32_t)(hpccard->cf.cfcard_info.default_head * hpccard->cf.cfcard_info.default_sector);

  hpccard->cf.cf_addr.cylinder  = (uint16_t)(sector_address / Total_Cylinder);
  hpccard->cf.cf_addr.head      = (uint8_t)((sector_address % Total_Cylinder) / hpccard->cf.cfcard_info.default_sector);
  hpccard->cf.cf_addr.sector    = (uint8_t)((sector_address % Total_Cylinder) % hpccard->cf.cfcard_info.default_sector) + 1;
  hpccard->cf.cf_addr.sector_count = 1;

  Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

  if(cf_send_command(hpccard, ATA_READ_LONG_SECTOR_CMD))
  {
    if(task_file_regis_valid(hpccard))
    {
      if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
      {
        read_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

        if(pccard_read_status_reg(hpccard) & DRQ_BIT)
        {
          for(index = 0; index < 4; index++)
          {
            *(uint8_t *)pbuffer++ = *(uint8_t *)(hpccard->cf.io_addr);
          }
        }
        else
        {
          command_vaild = FALSE;
        }

        if(pccard_read_status_reg(hpccard) & DRQ_BIT)
        {
          command_vaild = FALSE;
        }

        else if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
        {
          return FALSE;

        }
      }
    }
  }

  while(1)
  {
    if(task_file_regis_valid(hpccard))
    {
      if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
      {
        break;
      }
      else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        return FALSE;
      }
    }
  }

  if(command_vaild == TRUE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  write long sector
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_address: the sector addr 
  * @param  pbuffer: pointer to the data buffer.
  * @retval true or false.
  */
BOOL pccard_write_long_sector(pccard_handle_type *hpccard, uint32_t sector_address, uint8_t *pbuffer)
{
  uint32_t index = 0, Total_Cylinder;
  uint8_t Reg;
  BOOL command_vaild = TRUE;

  Total_Cylinder = (uint32_t)(hpccard->cf.cfcard_info.default_head * hpccard->cf.cfcard_info.default_sector);

  hpccard->cf.cf_addr.cylinder  = (uint16_t)(sector_address / Total_Cylinder);
  hpccard->cf.cf_addr.head      = (uint8_t)((sector_address % Total_Cylinder) / hpccard->cf.cfcard_info.default_sector);
  hpccard->cf.cf_addr.sector    = (uint8_t)((sector_address % Total_Cylinder) % hpccard->cf.cfcard_info.default_sector) + 1;
  hpccard->cf.cf_addr.sector_count = 1; 

  Reg = hpccard->cf.cf_addr.head | hpccard->cf.cf_addr.drv | 0xA0;

  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder >> 8);
  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->cf.cf_addr.cylinder);
  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_SECTOR_NUMBER) = hpccard->cf.cf_addr.sector;
  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_sector_count)  = hpccard->cf.cf_addr.sector_count;
  *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_CARD_HEAD)     = Reg;

  if(cf_send_command(hpccard, ATA_WRITE_LONG_SECTOR_CMD))
  {
    if(task_file_regis_valid(hpccard))
    {
      if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
      {
        write_sector(hpccard, pbuffer, PCCARD_SECTOR_SIZE);

        if(pccard_read_status_reg(hpccard) & DRQ_BIT)
        {
          for(index = 0; index < 4; index++)
          {
            *(uint8_t *)(hpccard->cf.io_addr) = *(uint8_t *)pbuffer++;
          }
        }
        else
        {
          command_vaild = FALSE;
        }

        if(pccard_read_status_reg(hpccard) & DRQ_BIT)
        {
          command_vaild = FALSE;
        }

        else if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
        {
          return FALSE;
        }
      }
    }
  }

  while(1)
  {
    if(task_file_regis_valid(hpccard))
    {
      if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT))
      {
        break;
      }
      else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        return FALSE;
      }
    }
  }

  if(command_vaild == TRUE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  read sector
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @param  transfer_size: the data length.
  * @retval pointer to the data buffer after read.
  */
uint8_t *read_sector(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t transfer_size)
{
  uint32_t index = 0;

  xmc_enable_16bit_feature(hpccard->instance, &(hpccard->init), hpccard->cf.enable_8_bit_mode);

  if(hpccard->cf.dma_enable == TRUE)
  {
    if(hpccard->cf.enable_8_bit_mode)
    {
      MYDMA_Config(EDMA_STREAM1, (uint32_t)hpccard->cf.io_addr, (uint32_t)pbuffer, transfer_size, READ_TRANSFER, ENABLE_8_BIT_TRANSFER);
    }
    else
    {
      MYDMA_Config(EDMA_STREAM1, (uint32_t)hpccard->cf.io_addr, (uint32_t)pbuffer, (transfer_size / 2), READ_TRANSFER, ENABLE_16_BIT_TRANSFER);
    }

    while(1)
    {
      if(edma_flag_get(EDMA_FDT1_FLAG) != RESET)
      {
        edma_flag_clear(EDMA_FDT1_FLAG);
        break;
      }
    }

    pbuffer += 512;
  }
  else
  {
    if(hpccard->cf.enable_8_bit_mode)
    {
      for(; index < (transfer_size) ; index++)
      {
        *(uint8_t *)pbuffer++ = *(uint8_t *)(hpccard->cf.io_addr);
      }
    }
    else
    {
      for(; index < (transfer_size / 2); index++)
      {
        *(uint16_t *)pbuffer = *(uint16_t *)(hpccard->cf.io_addr);
       /* offset to next 16 bit */
        pbuffer += 2;  
      }
    }
  }

  xmc_enable_16bit_feature(hpccard->instance, &(hpccard->init), 1);
  return pbuffer;
}

/**
  * @brief  write sector
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  pbuffer: pointer to the data buffer.
  * @param  transfer_size: the data length.
  * @retval pointer to the data buffer after write.
  */
uint8_t *write_sector(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t transfer_size)
{
  uint32_t index = 0;

  xmc_enable_16bit_feature(hpccard->instance, &(hpccard->init), hpccard->cf.enable_8_bit_mode);

  if(hpccard->cf.dma_enable == TRUE)
  {
    if(hpccard->cf.enable_8_bit_mode)
    {
      MYDMA_Config(EDMA_STREAM1, (uint32_t)pbuffer, (uint32_t)hpccard->cf.io_addr, transfer_size, WRITE_TRANSFER, ENABLE_8_BIT_TRANSFER);
    }
    else
    {
      MYDMA_Config(EDMA_STREAM1, (uint32_t)pbuffer, (uint32_t)hpccard->cf.io_addr, (transfer_size / 2), WRITE_TRANSFER, ENABLE_16_BIT_TRANSFER);
    }

    while(1)
    {
      if(edma_flag_get(EDMA_FDT1_FLAG) != RESET)  
      {
        edma_flag_clear(EDMA_FDT1_FLAG);
        break;
      }
    }

    pbuffer += 512;
  }
  else
  {
    if(hpccard->cf.enable_8_bit_mode)
    {
      for(; index < (transfer_size); index++)
      {
        *(uint8_t *)(hpccard->cf.io_addr) = *(uint8_t *)pbuffer++;
      }
    }
    else
    {
      for(; index < (transfer_size / 2); index++)
      {
        *(uint16_t *)(hpccard->cf.io_addr) = *(uint16_t *)pbuffer;
        /* offset to next 16 bit */
        pbuffer += 2;  // offset to next 16 bit
      }
    }
  }

  xmc_enable_16bit_feature(hpccard->instance, &(hpccard->init), 1);
  return pbuffer;
}

/**
  * @brief  reset the pccard memory
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval status
  */
BOOL pccard_reset(pccard_handle_type *hpccard)
{
  /* provide an sw reset and read and verify the:
   - cf configuration option register at address 0x98000200 --> 0x80
   - card configuration and status register at address 0x98000202 --> 0x00
   - pin replacement register  at address 0x98000204 --> 0x0c
   - socket and copy register at address 0x98000206 --> 0x00
  */

  uint8_t config_option_reg;

  /* set up the reset bit in config option register */
  *(__IO uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | CONFIG_OPTION_REG) = 0x80;
  delay_ms(100);
  /* clear the reset bit in config option register */
  *(__IO uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | CONFIG_OPTION_REG) = 0x00;
  delay_ms(100);
  /* set the pc card i/o configurations */
  *(__IO uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | CONFIG_OPTION_REG) = hpccard->cf.protocol;
  delay_ms(100);

  config_option_reg = *(__IO uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | CONFIG_OPTION_REG);

  if(config_option_reg == hpccard->cf.protocol)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * translate_chsaddr:
  *  translate disk sector number to c.h.s address.
  *  input  : cfcard
  *        start (start sector number)
  *        sector_count (sector count which read from cf card)
  *        sector_limit (maximum sector number)
  *  output  : none
  */
void translate_chsaddr(pccard_handle_type *hpccard, uint32_t Start, uint16_t sector_count, uint16_t Sector_Limit)
{
  uint32_t Total_Cylinder;
  uint16_t Prev_Count;

  Prev_Count = (Sector_Limit / hpccard->cf.cfcard_info.default_sector) * hpccard->cf.cfcard_info.default_sector;

  Total_Cylinder = (uint32_t)(hpccard->cf.cfcard_info.default_head * hpccard->cf.cfcard_info.default_sector); // The total sectors in one cylinder

  hpccard->cf.cf_addr.cylinder  = (uint16_t)(Start / Total_Cylinder);
  hpccard->cf.cf_addr.head      = (uint8_t)((Start % Total_Cylinder) / hpccard->cf.cfcard_info.default_sector);
  hpccard->cf.cf_addr.sector    = (uint8_t)((Start % Total_Cylinder) % hpccard->cf.cfcard_info.default_sector) + 1;

  if ((Prev_Count <= sector_count) && (sector_count <= Sector_Limit))
  {
    hpccard->cf.cf_addr.sector_count = (uint8_t)(Sector_Limit % hpccard->cf.cfcard_info.default_sector);
  }
  else
  {
    hpccard->cf.cf_addr.sector_count = (uint8_t)hpccard->cf.cfcard_info.default_sector;
  }
}

/**
  * @brief  read cis information.
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL read_cfcard_cis_information(pccard_handle_type *hpccard)
{
  uint8_t CIS_1, CIS_2, CIS_3;

  CIS_1 = *(uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | 0x0);
  CIS_2 = *(uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | 0x2);
  CIS_3 = *(uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | 0x4);

  if(CIS_1 == 0x1 && CIS_2 == 0x4 && CIS_3 == 0xdf)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  send command.
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  cmd: the command value
  * @retval true or false.
  */
BOOL cf_send_command(pccard_handle_type *hpccard, uint8_t Cmd)
{
  uint8_t Reg;

  do
  {
    *(__IO uint8_t  *)(hpccard->cf.io_addr | ATA_STATUS_CMD) = Cmd;

    while(!task_file_regis_valid(hpccard)) ;

    Reg = pccard_read_status_reg(hpccard);

    if ((Reg & ERR_BIT) != 0)
    {
      return FALSE;
    }
  }
  while((Reg & RDY_BIT) == 0);

  return TRUE;
}

/**
  * @brief  check the pccard taskfilereg valid.
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval true or false.
  */
BOOL task_file_regis_valid(pccard_handle_type *hpccard)
{
  uint8_t status = 0;

  status = pccard_read_status_reg(hpccard);

  if(status & BUSY_BIT)
  {
    return FALSE;
  }

  return TRUE;
}

/**
  * @brief  check the pccard info valid.
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @param  sector_count: sector count
  * @param  head_count:head count
  * @retval true or false.
  */
BOOL vaild_init_drive_para(pccard_handle_type *hpccard, uint16_t sector_count, uint8_t head_count)
{
  uint8_t CardInfo[PCCARD_SECTOR_SIZE * sizeof(uint8_t)] = {0};

  memset(CardInfo, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

  if(pccard_identify(hpccard))
  {
    while(1)
    {
      if(task_file_regis_valid(hpccard))
      {
        if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
        {
          read_sector(hpccard, CardInfo, PCCARD_SECTOR_SIZE);
          break;
        }
        else if(pccard_read_status_reg(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
        {
          printf("\r\nCommand Pass, but ERR bit is high");
          printf("\r\nCF Status Reg=%#x", pccard_read_status_reg(hpccard));
          printf("\r\nCF Error  Reg=%#x", pccard_read_error_reg(hpccard));
          return FALSE;
        }
      }
    }
  }

  while(1)
  {
    if(task_file_regis_valid(hpccard))
    {
      if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
      {
        break;
      }
      else if((pccard_read_status_reg(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        printf("\r\nSend Command Failed");
        printf("\r\nCF Status Reg=%#x", pccard_read_status_reg(hpccard));
        printf("\r\nCF Error  Reg=%#x", pccard_read_error_reg(hpccard));
        return FALSE;
      }
    }
  }

  if (((uint16_t)CardInfo[0] | (uint16_t)CardInfo[1] << 8) == 0x848A && ((uint16_t)CardInfo[110] | \
      (uint16_t)CardInfo[111] << 8) ==  head_count && ((uint16_t)CardInfo[112] | (uint16_t)CardInfo[113] << 8) ==  sector_count )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }

}

/**
  * @brief  printf the verification result.
  * @param  result: pointer to a verification_result structure that contains
  *                various kinds of verification.
  * @retval none.
  */
void show_verification_result(verification_result_struct *result)
{
  printf("\r\n");
  printf("\r\n----------------------------------------------------------------------");
  printf("\r\n%s    %-6s   ", "ATTRIBUTE_RW_PASS      ", ((result->attribute_rw_pass) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "READ_ATTRIBUTE_CIS     ", ((result->cis_read_pass) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "IDENTIFY_DRIVE         ", ((result->ata_identify_drive_pass) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "EXECUTE_DRIVE_DIAG     ", ((result->ata_execute_drive_diag_pass) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "write_sector           ", ((result->ata_write_sector_pass) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "read_sector            ", ((result->ata_read_sector_pass) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "ACESS_25_SECTORS       ", ((result->acess_25_sectors_pass) ? " PASS" : "FAILED *****"));
  printf("\r\n----------------------------------------------------------------------");
  printf("\r\n\r\n");
}


/**
  * @brief  this function handles pccard device interrupt request.
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval status
  */
void pccard_irqhandler(pccard_handle_type *hpccard)
{
}


/**
  * @brief  return the pccard controller state
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval state
  */
pccard_state_type pccard_get_state(pccard_handle_type *hpccard)
{
  return hpccard->state;
}

/**
  * @brief  get the pccard error reg value.
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval reg: error reg value.
  */
uint8_t pccard_read_error_reg(pccard_handle_type *hpccard)
{
  uint8_t Reg = 0;

  /* read error reg operation */
  Reg =  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_ERROR_REG);
  CF_print(ERR_REG_INFO, ("\r\nCF card ERROR Register value is %#x", Reg));
  return Reg;
}

/**
  * @brief  get the pccard status reg value.
  * @param  hpccard: pointer to a pccard_handle_type structure that contains
  *                the configuration information for pccard module.
  * @retval reg: status reg value.
  */
uint8_t pccard_read_status_reg(pccard_handle_type *hpccard)
{
  uint8_t Reg = 0;

  /* read status reg operation */
  Reg =  *(__IO uint8_t *)(hpccard->cf.io_addr | ATA_STATUS_CMD);
  CF_print(STS_REG_INFO, ("\r\nCF card STATUS Register value is %#x", Reg));

  return Reg;
}
/**
  * @}
  */

/**
  * @}
  */












