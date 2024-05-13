/**
  **************************************************************************
  * @file     qspi_cmd_esmt32m.c
  * @brief    qspi_cmd_esmt32m program
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

/** @addtogroup 435_QSPI_command_port_using_dma_in_qpi_mode
  * @{
  */

#define FLASH_PAGE_PROGRAM_SIZE          256

qspi_cmd_type esmt32m_cmd_config;

void qspi_busy_check(void);
void qspi_write_enable(void);

/**
  * @brief  esmt32m cmd read config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: read start address
  * @param  counter: read data counter
  * @retval none
  */
void esmt32m_cmd_read_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0xEB;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = counter;
  qspi_cmd_struct->second_dummy_cycle_num = 6;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_444;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = FALSE;
}

/**
  * @brief  esmt32m cmd write config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: write start address
  * @param  counter: write data counter
  * @retval none
  */
void esmt32m_cmd_write_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x2;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = counter;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_444;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  esmt32m cmd erase config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: erase address
  * @retval none
  */
void esmt32m_cmd_erase_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x20;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_444;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  esmt32m cmd wren config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void esmt32m_cmd_wren_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x06;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = 0;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_0_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_444;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  esmt32m cmd rdsr config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void esmt32m_cmd_rdsr_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x05;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = 0;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_0_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_444;
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
  esmt32m_cmd_read_config(&esmt32m_cmd_config, addr, total_len);
  qspi_cmd_operation_kick(QSPI1, &esmt32m_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET)
  {
  }
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);

  /* wait dma completed */
  while(dma_flag_get(DMA2_FDT1_FLAG) == RESET)
  {
  }
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
  esmt32m_cmd_write_config(&esmt32m_cmd_config, addr, sz);
  qspi_cmd_operation_kick(QSPI1, &esmt32m_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET)
  {
  }
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);

  /* wait dma completed */
  while(dma_flag_get(DMA2_FDT1_FLAG) == RESET)
  {
  }
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
  { /* send up to 256 bytes at one time, and only one page */
    blk_sz = (addr / FLASH_PAGE_PROGRAM_SIZE + 1) * FLASH_PAGE_PROGRAM_SIZE - addr;
    if(total_len < blk_sz)
      blk_sz = total_len;
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

  esmt32m_cmd_erase_config(&esmt32m_cmd_config, sec_addr);
  qspi_cmd_operation_kick(QSPI1, &esmt32m_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET)
  {
  }
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
  esmt32m_cmd_rdsr_config(&esmt32m_cmd_config);
  qspi_cmd_operation_kick(QSPI1, &esmt32m_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET)
  {
  }
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}

/**
  * @brief  qspi write enable
  * @param  none
  * @retval none
  */
void qspi_write_enable(void)
{
  esmt32m_cmd_wren_config(&esmt32m_cmd_config);
  qspi_cmd_operation_kick(QSPI1, &esmt32m_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET)
  {
  }
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}

/**
  * @brief  esmt32m cmd qpi enter config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void esmt32m_cmd_qpien_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x38;
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
  * @brief  esmt32m cmd rstqpi config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void esmt32m_cmd_rstqpi_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0xFF;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = 0;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_0_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_444;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  esmt32m cmd rst config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void esmt32m_cmd_rsten_config(qspi_cmd_type *qspi_cmd_struct)
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
  * @brief  esmt32m cmd rst config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void esmt32m_cmd_rst_config(qspi_cmd_type *qspi_cmd_struct)
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

/**
  * @brief  esmt32m cmd kick and wait completed
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void esmt32m_cmd_send(qspi_cmd_type* cmd)
{
  qspi_cmd_operation_kick(QSPI1, cmd);
  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}

/**
  * @brief  switch QSPI to cmd port and setup to qpi mode
  * @param  none
  * @retval none
  */
void esmt32m_qpi_mode_init(void)
{
  /* reset qpi */
  esmt32m_cmd_rstqpi_config(&esmt32m_cmd_config);
  esmt32m_cmd_send(&esmt32m_cmd_config);

  /* system reset */
  esmt32m_cmd_rsten_config(&esmt32m_cmd_config);
  esmt32m_cmd_send(&esmt32m_cmd_config);
  esmt32m_cmd_rst_config(&esmt32m_cmd_config);
  esmt32m_cmd_send(&esmt32m_cmd_config);

  /* once the reset command is accepted by the device, the device will
  take some time to reset. esmt32m will take about 28us in worst case.
  user can set delay or check wip status */
  delay_us(30);

  esmt32m_cmd_qpien_config(&esmt32m_cmd_config);
  esmt32m_cmd_send(&esmt32m_cmd_config);
}

/**
  * @}
  */

/**
  * @}
  */
