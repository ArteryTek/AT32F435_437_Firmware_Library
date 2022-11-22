/**
  **************************************************************************
  * @file     at_surf_f437_board_qspi_sram.c
  * @brief    the driver library of the qspi sram
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

#include "at_surf_f437_board_qspi_sram.h"
qspi_xip_type sram_xip_init;
qspi_cmd_type sram_cmd_config;

/**
  * @brief  xip init sram config
  * @param  qspi_xip_struct: the pointer for qspi_xip_type parameter
  * @retval none
  */
void qspi_sram_xip_init_para(qspi_xip_type *qspi_xip_struct)
{
  qspi_xip_struct->read_instruction_code = 0xEB;
  qspi_xip_struct->read_address_length = QSPI_XIP_ADDRLEN_3_BYTE;
  qspi_xip_struct->read_operation_mode = QSPI_OPERATE_MODE_444;
  qspi_xip_struct->read_second_dummy_cycle_num = 6;
  qspi_xip_struct->write_instruction_code = 0x38;
  qspi_xip_struct->write_address_length = QSPI_XIP_ADDRLEN_3_BYTE;
  qspi_xip_struct->write_operation_mode = QSPI_OPERATE_MODE_444;
  qspi_xip_struct->write_second_dummy_cycle_num = 0;
  qspi_xip_struct->write_select_mode = QSPI_XIPW_SEL_MODED;
  qspi_xip_struct->write_time_counter = 0x7F;
  qspi_xip_struct->write_data_counter = 0x1F;
  qspi_xip_struct->read_select_mode = QSPI_XIPR_SEL_MODED;
  qspi_xip_struct->read_time_counter = 0x7F;
  qspi_xip_struct->read_data_counter = 0x1F;
}

/**
  * @brief  cmd rsten sram config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void qspi_sram_cmd_rsten_config(qspi_cmd_type *qspi_cmd_struct)
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
  * @brief  cmd quad mode enable config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void qspi_sram_cmd_enquadmode_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x35;
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
  * @brief  cmd quad mode exit config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void qspi_sram_cmd_exitquadmode_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0xF5;
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
  * @brief  cmd rst sram config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void qspi_sram_cmd_rst_config(qspi_cmd_type *qspi_cmd_struct)
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
  * @brief  sram cmd send
  * @param  qspi_cmd_type: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void qspi_sram_cmd_send(qspi_cmd_type* cmd)
{
  qspi_cmd_operation_kick(QSPI_SRAM_QSPIx, cmd);
  /* wait command completed */
  while(qspi_flag_get(QSPI_SRAM_QSPIx, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI_SRAM_QSPIx, QSPI_CMDSTS_FLAG);
}

/**
  * @brief  qspi sram read data
  * @param  addr: the address for read
  * @param  total_len: the length for read
  * @param  buf: the pointer for read data
  * @retval none
  */
void qspi_sram_data_read(uint32_t addr, uint8_t* buf, uint32_t total_len)
{
  uint32_t i;
  uint8_t *paddr;

  paddr = (uint8_t*)(QSPI_SRAM_MEM_BASE + addr);

  for(i = 0; i < total_len; i++)
  {
    buf[i] = paddr[i];
  }
}

/**
  * @brief  qspi sram write data
  * @param  addr: the address for write
  * @param  total_len: the length for write
  * @param  buf: the pointer for write data
  * @retval none
  */
void qspi_sram_data_write(uint32_t addr, uint8_t* buf, uint32_t total_len)
{
  uint32_t i;
  uint8_t *paddr;

  paddr = (uint8_t*)(QSPI_SRAM_MEM_BASE + addr);

  for(i = 0; i < total_len; i++)
  {
    paddr[i] = buf[i];
  }
}

/**
  * @brief  initializes quad spi sram.
  * @param  none
  * @retval none
  */
void qspi_sram_init(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the qspi clock */
  crm_periph_clock_enable(QSPI_SRAM_QSPI_CLK, TRUE);

  /* enable the pin clock */
  crm_periph_clock_enable(QSPI_SRAM_CS_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_SRAM_CLK_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_SRAM_D0_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_SRAM_D1_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_SRAM_D2_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_SRAM_D3_GPIO_CLK, TRUE);

  /* configure gpio */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

  gpio_init_struct.gpio_pins = QSPI_SRAM_CS_GPIO_PIN;
  gpio_init(QSPI_SRAM_CS_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_SRAM_CLK_GPIO_PIN;
  gpio_init(QSPI_SRAM_CLK_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_SRAM_D0_GPIO_PIN;
  gpio_init(QSPI_SRAM_D0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_SRAM_D1_GPIO_PIN;
  gpio_init(QSPI_SRAM_D1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_SRAM_D2_GPIO_PIN;
  gpio_init(QSPI_SRAM_D2_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_SRAM_D3_GPIO_PIN;
  gpio_init(QSPI_SRAM_D3_GPIO_PORT, &gpio_init_struct);

  /* gpio mux config */
  gpio_pin_mux_config(QSPI_SRAM_CS_GPIO_PORT,  QSPI_SRAM_CS_GPIO_PINS_SOURCE,  QSPI_SRAM_CS_GPIO_MUX);
  gpio_pin_mux_config(QSPI_SRAM_CLK_GPIO_PORT, QSPI_SRAM_CLK_GPIO_PINS_SOURCE, QSPI_SRAM_CLK_GPIO_MUX);
  gpio_pin_mux_config(QSPI_SRAM_D0_GPIO_PORT,  QSPI_SRAM_D0_GPIO_PINS_SOURCE,  QSPI_SRAM_D0_GPIO_MUX);
  gpio_pin_mux_config(QSPI_SRAM_D1_GPIO_PORT,  QSPI_SRAM_D1_GPIO_PINS_SOURCE,  QSPI_SRAM_D1_GPIO_MUX);
  gpio_pin_mux_config(QSPI_SRAM_D2_GPIO_PORT,  QSPI_SRAM_D2_GPIO_PINS_SOURCE,  QSPI_SRAM_D2_GPIO_MUX);
  gpio_pin_mux_config(QSPI_SRAM_D3_GPIO_PORT,  QSPI_SRAM_D3_GPIO_PINS_SOURCE,  QSPI_SRAM_D3_GPIO_MUX);

  /* switch to command-port mode */
  qspi_xip_enable(QSPI_SRAM_QSPIx, FALSE);

  /* set sclk */
  qspi_clk_division_set(QSPI_SRAM_QSPIx, QSPI_CLK_DIV_4);

  /* set sck idle mode 0 */
  qspi_sck_mode_set(QSPI_SRAM_QSPIx, QSPI_SCK_MODE_0);

  /* set wip in bit 0 */
  qspi_busy_config(QSPI_SRAM_QSPIx, QSPI_BUSY_OFFSET_0);

  /* exit quad mode */
  qspi_sram_cmd_exitquadmode_config(&sram_cmd_config);
  qspi_sram_cmd_send(&sram_cmd_config);

  /* issue reset command */
  qspi_sram_cmd_rsten_config(&sram_cmd_config);
  qspi_sram_cmd_send(&sram_cmd_config);
  qspi_sram_cmd_rst_config(&sram_cmd_config);
  qspi_sram_cmd_send(&sram_cmd_config);

  /* enable quad mode */
  qspi_sram_cmd_enquadmode_config(&sram_cmd_config);
  qspi_sram_cmd_send(&sram_cmd_config);

  /* initial xip */
  qspi_sram_xip_init_para(&sram_xip_init);
  qspi_xip_init(QSPI_SRAM_QSPIx, &sram_xip_init);
  qspi_xip_cache_bypass_set(QSPI_SRAM_QSPIx,TRUE);
  qspi_xip_enable(QSPI_SRAM_QSPIx, TRUE);
}
