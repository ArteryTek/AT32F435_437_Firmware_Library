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

#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"
#include "run_in_qspi.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_QSPI_run_in_qspi_flash QSPI_run_in_qspi_flash
  * @{
  */

/**
  * @brief  qspi config
  * @param  none
  * @retval none
  */
void qspi_config(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the qspi clock */
  crm_periph_clock_enable(CRM_QSPI1_PERIPH_CLOCK, TRUE);

  /* enable the pin clock */
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure the io0 gpio */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE8, GPIO_MUX_10);

  /* configure the io1 gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE9, GPIO_MUX_10);

  /* configure the io2 gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE7, GPIO_MUX_9);

  /* configure the io3 gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE6, GPIO_MUX_9);

  /* configure the sck gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE10, GPIO_MUX_9);

  /* configure the cs gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init(GPIOG, &gpio_init_struct);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE6, GPIO_MUX_10);
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
  qspi_cmd_type en25qh128a_cmd_config;
  qspi_xip_type en25qh128a_xip_init;

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
  qspi_xip_enable(QSPI1, TRUE);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  /* qspi config */
  qspi_config();

  /* switch to cmd port */
  qspi_xip_enable(QSPI1, FALSE);

  /* set sclk */
  qspi_clk_division_set(QSPI1, QSPI_CLK_DIV_4);

  /* set sck idle mode 0 */
  qspi_sck_mode_set(QSPI1, QSPI_SCK_MODE_0);

  /* set wip in bit 0 */
  qspi_busy_config(QSPI1, QSPI_BUSY_OFFSET_0);

  /* enable auto ispc */
  qspi_auto_ispc_enable(QSPI1);

  /*configure xip mode*/
  en25qh128a_qspi_xip_init();

  /* check the led toggle in qspi */
  qspi_run();

  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */
