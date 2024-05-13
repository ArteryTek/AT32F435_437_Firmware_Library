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

/** @addtogroup 435_QSPI_command_port_using_rdsr_sw
  * @{
  */

#define QSPI_FIFO_DEPTH                  (32*4)
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
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_144;
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
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
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
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
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
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_SW_ONCE;
  qspi_cmd_struct->read_status_enable = TRUE;
  qspi_cmd_struct->write_data_enable = FALSE;
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
  uint32_t i, len = total_len;

  esmt32m_cmd_read_config(&esmt32m_cmd_config, addr, total_len);
  qspi_cmd_operation_kick(QSPI1, &esmt32m_cmd_config);

  /* read data */
  do
  {
    if(total_len >= QSPI_FIFO_DEPTH)
    {
      len = QSPI_FIFO_DEPTH;
    }
    else
    {
      len = total_len;
    }
    while(qspi_flag_get(QSPI1, QSPI_RXFIFORDY_FLAG) == RESET)
    {
    }
    for(i = 0; i < len; ++i)
    {
      *buf++ = qspi_byte_read(QSPI1);
    }
    total_len -= len;
  }while(total_len);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET)
  {
  }
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
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
  uint32_t i, len;

  do
  {
    qspi_write_enable();
    /* send up to 256 bytes at one time, and only one page */
    len = (addr / FLASH_PAGE_PROGRAM_SIZE + 1) * FLASH_PAGE_PROGRAM_SIZE - addr;
    if(total_len < len)
      len = total_len;

    esmt32m_cmd_write_config(&esmt32m_cmd_config, addr, len);
    qspi_cmd_operation_kick(QSPI1, &esmt32m_cmd_config);

    for(i = 0; i < len; ++i)
    {
      while(qspi_flag_get(QSPI1, QSPI_TXFIFORDY_FLAG) == RESET)
      {
      }
      qspi_byte_write(QSPI1, *buf++);
    }
    total_len -= len;
    addr += len;

    /* wait command completed */
    while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET)
    {
    }
    qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);

    qspi_busy_check();

  }while(total_len);
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
  do
  {
    esmt32m_cmd_rdsr_config(&esmt32m_cmd_config);
    qspi_cmd_operation_kick(QSPI1, &esmt32m_cmd_config);

    /* wait command completed */
    while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET)
    {
    }
    qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
    /* check wip status */
  }while(QSPI1->rsts & (1 << QSPI_BUSY_OFFSET_0));
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
  * @}
  */

/**
  * @}
  */
