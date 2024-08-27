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

/** @addtogroup 435_QSPI_command_port_using_rdsr_sw
  * @{
  */

#define QSPI_FIFO_DEPTH                  (32*4)
#define FLASH_PAGE_PROGRAM_SIZE          256

/* en25qh128a cmd read parameters, the address_code and data_counter need to be set in application */
static const qspi_cmd_type en25qh128a_read_para = {
FALSE,0,0xEB,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_3_BYTE,0,6,QSPI_OPERATE_MODE_144,QSPI_RSTSC_HW_AUTO,FALSE,FALSE};

/* en25qh128a cmd write parameters, the address_code and data_counter need to be set in application */
static const qspi_cmd_type en25qh128a_write_para = {
FALSE,0,0x32,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_3_BYTE,0,0,QSPI_OPERATE_MODE_114,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* en25qh128a cmd sector erase parameters, the address_code need to be set in application */
static const qspi_cmd_type en25qh128a_erase_para = {
FALSE,0,0x20,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_3_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* en25qh128a cmd wren parameters */
static const qspi_cmd_type en25qh128a_wren_para = {
FALSE,0,0x06,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_0_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* en25qh128a cmd rdsr parameters */
static const qspi_cmd_type en25qh128a_rdsr_para = {
FALSE,0,0x05,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_0_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_SW_ONCE,TRUE,FALSE};

qspi_cmd_type en25qh128a_cmd_config;

void qspi_busy_check(void);
void qspi_write_enable(void);
void qspi_cmd_send(qspi_cmd_type* qspi_cmd_struct);

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

  en25qh128a_cmd_config = en25qh128a_read_para;
  en25qh128a_cmd_config.address_code = addr;
  en25qh128a_cmd_config.data_counter = total_len; 
  qspi_cmd_operation_kick(QSPI1, &en25qh128a_cmd_config);

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
    while(qspi_flag_get(QSPI1, QSPI_RXFIFORDY_FLAG) == RESET);

    for(i = 0; i < len; ++i)
    {
      *buf++ = qspi_byte_read(QSPI1);
    }
    total_len -= len;
  }while(total_len);

  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);

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

    en25qh128a_cmd_config = en25qh128a_write_para;
    en25qh128a_cmd_config.address_code = addr;
    en25qh128a_cmd_config.data_counter = len;
    qspi_cmd_operation_kick(QSPI1, &en25qh128a_cmd_config);

    for(i = 0; i < len; ++i)
    {
      while(qspi_flag_get(QSPI1, QSPI_TXFIFORDY_FLAG) == RESET);

      qspi_byte_write(QSPI1, *buf++);
    }
    total_len -= len;
    addr += len;

    /* wait command completed */
    while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);

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

  en25qh128a_cmd_config = en25qh128a_erase_para;
  en25qh128a_cmd_config.address_code = sec_addr;
  qspi_cmd_send(&en25qh128a_cmd_config);

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
    qspi_cmd_send((qspi_cmd_type*)&en25qh128a_rdsr_para);
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
  qspi_cmd_send((qspi_cmd_type*)&en25qh128a_wren_para);
}

/**
  * @brief  qspi cmd kick and wait completed
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void qspi_cmd_send(qspi_cmd_type* qspi_cmd_struct)
{
  /* kick command */
  qspi_cmd_operation_kick(QSPI1, qspi_cmd_struct);
 
  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}

/**
  * @}
  */

/**
  * @}
  */
