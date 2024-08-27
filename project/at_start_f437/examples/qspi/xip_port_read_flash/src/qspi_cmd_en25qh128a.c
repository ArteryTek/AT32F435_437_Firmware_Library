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

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_QSPI_xip_port_read_flash
  * @{
  */

#define FLASH_PAGE_PROGRAM_SIZE          256

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
FALSE,0,0x05,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_0_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,TRUE,FALSE};

/* en25qh128a cmd rsten parameters */
static const qspi_cmd_type en25qh128a_rsten_para = {
FALSE,0,0x66,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_0_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* en25qh128a cmd rst parameters */
static const qspi_cmd_type en25qh128a_rst_para = {
FALSE,0,0x99,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_0_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* en25qh128a xip init parameters */
static const qspi_xip_type en25qh128a_xip_init_para = {
0x6B,QSPI_XIP_ADDRLEN_3_BYTE,QSPI_OPERATE_MODE_114,8,0x32,QSPI_XIP_ADDRLEN_3_BYTE,QSPI_OPERATE_MODE_114,0,QSPI_XIPW_SEL_MODED,0x7F,0x1F,QSPI_XIPR_SEL_MODET,0x7F,0x1F};

qspi_cmd_type en25qh128a_cmd_config;

void qspi_busy_check(void);
void qspi_write_enable(void);
void qspi_cmd_send(qspi_cmd_type* qspi_cmd_struct);

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
  qspi_cmd_send((qspi_cmd_type*)&en25qh128a_rdsr_para);
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

void en25qh128a_qspi_xip_init(void)
{
  /* switch to command-port mode */
  qspi_xip_enable(QSPI1, FALSE);

  /* system reset */
  qspi_cmd_send((qspi_cmd_type*)&en25qh128a_rsten_para);
  qspi_cmd_send((qspi_cmd_type*)&en25qh128a_rst_para);
 
  /* initial xip */
  qspi_xip_init(QSPI1, (qspi_xip_type*)&en25qh128a_xip_init_para);
  qspi_xip_enable(QSPI1, TRUE);
}

/**
  * @}
  */

/**
  * @}
  */
