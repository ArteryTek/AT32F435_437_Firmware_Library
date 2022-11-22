/**
  **************************************************************************
  * @file     qspi_cmd_en25qh128a.c
  * @brief    qspi_cmd_en25qh128a program
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

#include "at32f435_437_board.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_QSPI_xip_port_read_flash
  * @{
  */

#define FLASH_PAGE_PROGRAM_SIZE          256

qspi_cmd_type en25qh128a_cmd_config;
qspi_xip_type en25qh128a_xip_init;

void qspi_busy_check(void);
void qspi_write_enable(void);

/**
  * @brief  en25qh128a cmd read config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: read start address
  * @param  counter: read data counter
  * @retval none
  */
void en25qh128a_cmd_read_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0xEB;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = counter;
  qspi_cmd_struct->second_dummy_cycle_num = 6;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_144;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = FALSE;
}

/**
  * @brief  en25qh128a cmd write config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: write start address
  * @param  counter: write data counter
  * @retval none
  */
void en25qh128a_cmd_write_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x32;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = counter;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_114;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  en25qh128a cmd erase config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: erase address
  * @retval none
  */
void en25qh128a_cmd_erase_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x20;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  en25qh128a cmd wren config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void en25qh128a_cmd_wren_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x06;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = 0;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_0_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  en25qh128a cmd rdsr config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void en25qh128a_cmd_rdsr_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x05;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = 0;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_0_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = TRUE;
  qspi_cmd_struct->write_data_enable = FALSE;
}

/**
  * @brief  qspi dma set
  * @param  dir: dma transfer direction
  * @param  buf: the pointer for dma data
  * @param  length: data length
  * @retval none
  */
void qspi_dma_set(dma_dir_type dir, uint8_t* buf, uint32_t length)
{
  dma_init_type dma_init_struct;
  dma_reset(DMA2_CHANNEL1);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = length / 4;  /* using word unit */
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init_struct.direction = dir;
  dma_init_struct.memory_base_addr = (uint32_t)buf;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)(&(QSPI1->dt));
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_WORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;

  dma_init(DMA2_CHANNEL1, &dma_init_struct);

  dmamux_init(DMA2MUX_CHANNEL1, DMAMUX_DMAREQ_ID_QSPI1);
  dmamux_enable(DMA2, TRUE);

  dma_channel_enable(DMA2_CHANNEL1, TRUE);
}

/**
  * @brief  qspi read data
  * @param  addr: the address for read
  * @param  total_len: the length for read
  * @param  buf: the pointer for read data
  * @retval none
  */
void qspi_data_read(uint32_t addr, uint32_t total_len, uint8_t* buf)
{
  /* config qspi's dma mode */
  qspi_dma_enable(QSPI1, TRUE);
  qspi_dma_rx_threshold_set(QSPI1, QSPI_DMA_FIFO_THOD_WORD08);

  /* config and enable dma */
  qspi_dma_set(DMA_DIR_PERIPHERAL_TO_MEMORY, buf, total_len);

  /* kick command */
  en25qh128a_cmd_read_config(&en25qh128a_cmd_config, addr, total_len);
  qspi_cmd_operation_kick(QSPI1, &en25qh128a_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);

  /* wait dma completed */
  while(dma_flag_get(DMA2_FDT1_FLAG) == RESET);
  dma_flag_clear(DMA2_FDT1_FLAG);
  qspi_dma_enable(QSPI1, FALSE);
}

/**
  * @brief  qspi write data for one time
  * @param  addr: the address for write
  * @param  sz: the length for write
  * @param  buf: the pointer for write data
  * @retval none
  */
static void qspi_data_once_write(uint32_t addr, uint32_t sz, uint8_t* buf)
{
  qspi_write_enable();

  /* config qspi's dma mode */
  qspi_dma_enable(QSPI1, TRUE);
  qspi_dma_tx_threshold_set(QSPI1, QSPI_DMA_FIFO_THOD_WORD08);

  /* config and enable dma */
  qspi_dma_set(DMA_DIR_MEMORY_TO_PERIPHERAL, buf, sz);

  /* kick command */
  en25qh128a_cmd_write_config(&en25qh128a_cmd_config, addr, sz);
  qspi_cmd_operation_kick(QSPI1, &en25qh128a_cmd_config);
  

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);

  /* wait dma completed */
  while(dma_flag_get(DMA2_FDT1_FLAG) == RESET);
  dma_flag_clear(DMA2_FDT1_FLAG);
  qspi_dma_enable(QSPI1, FALSE);

  qspi_busy_check();
}

/**
  * @brief  qspi write data
  * @param  addr: the address for write
  * @param  total_len: the length for write
  * @param  buf: the pointer for write data
  * @retval none
  */
void qspi_data_write(uint32_t addr, uint32_t total_len, uint8_t* buf)
{
  uint32_t blk_sz;
  do
  { /* send up to 256 bytes at one time */
    if(total_len > FLASH_PAGE_PROGRAM_SIZE)
    {
      blk_sz = FLASH_PAGE_PROGRAM_SIZE;
    }
    else
    {
      blk_sz = total_len;
    }
    qspi_data_once_write(addr, blk_sz, buf);
    addr += blk_sz;
    buf += blk_sz;
    total_len -= blk_sz;
  }while(total_len > 0);
}

/**
  * @brief  qspi erase data
  * @param  sec_addr: the sector address for erase
  * @retval none
  */
void qspi_erase(uint32_t sec_addr)
{
  qspi_write_enable();

  en25qh128a_cmd_erase_config(&en25qh128a_cmd_config, sec_addr);
  qspi_cmd_operation_kick(QSPI1, &en25qh128a_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);

  qspi_busy_check();
}

/**
  * @brief  qspi check busy
  * @param  none
  * @retval none
  */
void qspi_busy_check(void)
{
  en25qh128a_cmd_rdsr_config(&en25qh128a_cmd_config);
  qspi_cmd_operation_kick(QSPI1, &en25qh128a_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}

/**
  * @brief  qspi write enable
  * @param  none
  * @retval none
  */
void qspi_write_enable(void)
{
  en25qh128a_cmd_wren_config(&en25qh128a_cmd_config);
  qspi_cmd_operation_kick(QSPI1, &en25qh128a_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}


/**
  * @brief  en25qh128a cmd rsten config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void  en25qh128a_cmd_rsten_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x66;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = 0;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_0_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  en25qh128a cmd rst config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void en25qh128a_cmd_rst_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x99;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = 0;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_0_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

void en25qh128a_qspi_cmd_send(qspi_cmd_type* cmd)
{
  qspi_cmd_operation_kick(QSPI1, cmd);
  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}


/**
  * @brief  en25qh128a xip init config
  * @param  qspi_xip_struct: the pointer for qspi_xip_type parameter
  * @retval none
  */
void en25qh128a_xip_init_config(qspi_xip_type *qspi_xip_struct)
{
  qspi_xip_struct->read_instruction_code = 0x6B; 
  qspi_xip_struct->read_address_length = QSPI_XIP_ADDRLEN_3_BYTE;
  qspi_xip_struct->read_operation_mode = QSPI_OPERATE_MODE_114;
  qspi_xip_struct->read_second_dummy_cycle_num = 8;
  qspi_xip_struct->write_instruction_code = 0x32;
  qspi_xip_struct->write_address_length = QSPI_XIP_ADDRLEN_3_BYTE;
  qspi_xip_struct->write_operation_mode = QSPI_OPERATE_MODE_114;
  qspi_xip_struct->write_second_dummy_cycle_num = 0;
  qspi_xip_struct->write_select_mode = QSPI_XIPW_SEL_MODED;
  qspi_xip_struct->write_time_counter = 0x7F;
  qspi_xip_struct->write_data_counter = 0x1F;
  qspi_xip_struct->read_select_mode = QSPI_XIPR_SEL_MODED;
  qspi_xip_struct->read_time_counter = 0x7F;
  qspi_xip_struct->read_data_counter = 0x1F;
}


void en25qh128a_qspi_xip_init(void)
{
  /* switch to command-port mode */
  qspi_xip_enable(QSPI1, FALSE);

  /* issue reset command */
  en25qh128a_cmd_rsten_config(&en25qh128a_cmd_config);
  en25qh128a_qspi_cmd_send(&en25qh128a_cmd_config);
  en25qh128a_cmd_rst_config(&en25qh128a_cmd_config);
  en25qh128a_qspi_cmd_send(&en25qh128a_cmd_config);
  
  /* initial xip */
  en25qh128a_xip_init_config(&en25qh128a_xip_init);
  qspi_xip_init(QSPI1, &en25qh128a_xip_init);
  qspi_xip_cache_bypass_set(QSPI1, FALSE);
  qspi_xip_enable(QSPI1, TRUE);
}


/**
  * @}
  */

/**
  * @}
  */
