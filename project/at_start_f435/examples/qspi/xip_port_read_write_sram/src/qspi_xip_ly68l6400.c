/**
  **************************************************************************
  * @file     qspi_xip_ly68l6400.c
  * @brief    qspi_xip_ly68l6400 program
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

/** @addtogroup 435_QSPI_xip_port_read_write_sram
  * @{
  */

/* ly68l6400 cmd rsten parameters */
static const qspi_cmd_type ly68l6400_rsten_para = {
FALSE,0,0x66,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_0_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* ly68l6400 cmd rst parameters */
static const qspi_cmd_type ly68l6400_rst_para = {
FALSE,0,0x99,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_0_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* ly68l6400 xip init parameters */
static const qspi_xip_type ly68l6400_xip_init_para = {
0xEB,QSPI_XIP_ADDRLEN_3_BYTE,QSPI_OPERATE_MODE_144,6,0x38,QSPI_XIP_ADDRLEN_3_BYTE,QSPI_OPERATE_MODE_144,0,QSPI_XIPW_SEL_MODED,0x7F,0x1F,QSPI_XIPR_SEL_MODET,0x7F,0x1F};

void qspi_cmd_send(qspi_cmd_type* qspi_cmd_struct);

void qspi_cmd_send(qspi_cmd_type* qspi_cmd_struct)
{
  /* kick command */ 
  qspi_cmd_operation_kick(QSPI1, qspi_cmd_struct);
 
  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}

void qspi_xip_init_ly68l6400(void)
{
  /* switch to command-port mode */
  qspi_xip_enable(QSPI1, FALSE);

  /* reset command */
  qspi_cmd_send((qspi_cmd_type*)&ly68l6400_rsten_para);
  qspi_cmd_send((qspi_cmd_type*)&ly68l6400_rst_para);

  /* initial xip */
  qspi_xip_init(QSPI1, (qspi_xip_type*)&ly68l6400_xip_init_para);
 
  if(DEBUGMCU->ser_id_bit.rev_id == 0x00)
  {
    qspi_xip_cache_bypass_set(QSPI1, TRUE);
  }
  qspi_xip_enable(QSPI1, TRUE);
}

/**
  * @}
  */

/**
  * @}
  */
