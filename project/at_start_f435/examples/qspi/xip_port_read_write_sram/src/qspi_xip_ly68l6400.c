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

/** @addtogroup 435_QSPI_xip_port_read_write_sram
  * @{
  */

qspi_xip_type ly68l6400_xip_init;
qspi_cmd_type ly68l6400_cmd_config;

/**
  * @brief  xip init ly68l6400 config
  * @param  qspi_xip_struct: the pointer for qspi_xip_type parameter
  * @retval none
  */
void xip_init_ly68l6400_config(qspi_xip_type *qspi_xip_struct)
{
  qspi_xip_struct->read_instruction_code = 0xEB;
  qspi_xip_struct->read_address_length = QSPI_XIP_ADDRLEN_3_BYTE;
  qspi_xip_struct->read_operation_mode = QSPI_OPERATE_MODE_144;
  qspi_xip_struct->read_second_dummy_cycle_num = 6;
  qspi_xip_struct->write_instruction_code = 0x38;
  qspi_xip_struct->write_address_length = QSPI_XIP_ADDRLEN_3_BYTE;
  qspi_xip_struct->write_operation_mode = QSPI_OPERATE_MODE_144;
  qspi_xip_struct->write_second_dummy_cycle_num = 0;
  qspi_xip_struct->write_select_mode = QSPI_XIPW_SEL_MODED;
  qspi_xip_struct->write_time_counter = 0x7F;
  qspi_xip_struct->write_data_counter = 0x1F;
  qspi_xip_struct->read_select_mode = QSPI_XIPR_SEL_MODED;
  qspi_xip_struct->read_time_counter = 0x7F;
  qspi_xip_struct->read_data_counter = 0x1F;
}

/**
  * @brief  cmd rsten ly68l6400 config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void cmd_rsten_ly68l6400_config(qspi_cmd_type *qspi_cmd_struct)
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
  * @brief  cmd rst ly68l6400 config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void cmd_rst_ly68l6400_config(qspi_cmd_type *qspi_cmd_struct)
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

void qspi_cmd_ly68l6400_send(qspi_cmd_type* cmd)
{
  qspi_cmd_operation_kick(QSPI1, cmd);
  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}


void qspi_xip_init_ly68l6400(void)
{
  /* switch to command-port mode */
  qspi_xip_enable(QSPI1, FALSE);

  /* issue reset command */
  cmd_rsten_ly68l6400_config(&ly68l6400_cmd_config);
  qspi_cmd_ly68l6400_send(&ly68l6400_cmd_config);
  cmd_rst_ly68l6400_config(&ly68l6400_cmd_config);
  qspi_cmd_ly68l6400_send(&ly68l6400_cmd_config);

  /* initial xip */
  xip_init_ly68l6400_config(&ly68l6400_xip_init);
  qspi_xip_init(QSPI1, &ly68l6400_xip_init);
  qspi_xip_cache_bypass_set(QSPI1,TRUE);
  qspi_xip_enable(QSPI1, TRUE);
}

/**
  * @}
  */

/**
  * @}
  */
