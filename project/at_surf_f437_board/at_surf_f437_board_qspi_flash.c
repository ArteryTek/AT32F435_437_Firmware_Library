/**
  **************************************************************************
  * @file     at_surf_f437_board_qspi_flash.c
  * @brief    the driver library of the qspi flash flash
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

#include "at_surf_f437_board_qspi_flash.h"

qspi_cmd_type qspi_flash_cmd_config;

void qspi_flash_busy_check(void);
void qspi_flash_write_enable(void);

/**
  * @brief  initializes quad spi flash.
  * @param  none
  * @retval none
  */
void qspi_flash_init(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the qspi flash clock */
  crm_periph_clock_enable(QSPI_FLASH_QSPI_CLK, TRUE);

  /* enable the pin clock */
  crm_periph_clock_enable(QSPI_FLASH_CS_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_CLK_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_D0_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_D1_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_D2_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_D3_GPIO_CLK, TRUE);

  /* configure gpio */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

  gpio_init_struct.gpio_pins = QSPI_FLASH_CS_GPIO_PIN;
  gpio_init(QSPI_FLASH_CS_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_CLK_GPIO_PIN;
  gpio_init(QSPI_FLASH_CLK_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_D0_GPIO_PIN;
  gpio_init(QSPI_FLASH_D0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_D1_GPIO_PIN;
  gpio_init(QSPI_FLASH_D1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_D2_GPIO_PIN;
  gpio_init(QSPI_FLASH_D2_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_D3_GPIO_PIN;
  gpio_init(QSPI_FLASH_D3_GPIO_PORT, &gpio_init_struct);

  /* gpio mux config */
  gpio_pin_mux_config(QSPI_FLASH_CS_GPIO_PORT,  QSPI_FLASH_CS_GPIO_PINS_SOURCE,  QSPI_FLASH_CS_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_CLK_GPIO_PORT, QSPI_FLASH_CLK_GPIO_PINS_SOURCE, QSPI_FLASH_CLK_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_D0_GPIO_PORT,  QSPI_FLASH_D0_GPIO_PINS_SOURCE,  QSPI_FLASH_D0_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_D1_GPIO_PORT,  QSPI_FLASH_D1_GPIO_PINS_SOURCE,  QSPI_FLASH_D1_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_D2_GPIO_PORT,  QSPI_FLASH_D2_GPIO_PINS_SOURCE,  QSPI_FLASH_D2_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_D3_GPIO_PORT,  QSPI_FLASH_D3_GPIO_PINS_SOURCE,  QSPI_FLASH_D3_GPIO_MUX);

  /* switch to cmd port */
  qspi_xip_enable(QSPI_FLASH_QSPIx, FALSE);

  /* set sclk */
  qspi_clk_division_set(QSPI_FLASH_QSPIx, QSPI_CLK_DIV_4);

  /* set sck idle mode 0 */
  qspi_sck_mode_set(QSPI_FLASH_QSPIx, QSPI_SCK_MODE_0);

  /* set wip in bit 0 */
  qspi_busy_config(QSPI_FLASH_QSPIx, QSPI_BUSY_OFFSET_0);
}

/**
  * @brief  qspi_flash cmd read config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: read start address
  * @param  counter: read data counter
  * @retval none
  */
void qspi_flash_cmd_read_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter)
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
  * @brief  qspi_flash cmd write config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: write start address
  * @param  counter: write data counter
  * @retval none
  */
void qspi_flash_cmd_write_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter)
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
  * @brief  qspi_flash cmd erase config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: erase address
  * @retval none
  */
void qspi_flash_cmd_erase_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr)
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
  * @brief  qspi_flash cmd wren config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void qspi_flash_cmd_wren_config(qspi_cmd_type *qspi_cmd_struct)
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
  * @brief  qspi_flash cmd rdsr config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void qspi_flash_cmd_rdsr_config(qspi_cmd_type *qspi_cmd_struct)
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
  * @brief  qspi flash read data
  * @param  addr: the address for read
  * @param  total_len: the length for read
  * @param  buf: the pointer for read data
  * @retval none
  */
void qspi_flash_data_read(uint32_t addr, uint8_t* buf, uint32_t total_len)
{
  uint32_t i, len = total_len;

  qspi_flash_cmd_read_config(&qspi_flash_cmd_config, addr, total_len);
  qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

  /* read data */
  do
  {
    if(total_len >= QSPI_FLASH_FIFO_DEPTH)
    {
      len = QSPI_FLASH_FIFO_DEPTH;
    }
    else
    {
      len = total_len;
    }
    while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_RXFIFORDY_FLAG) == RESET);
    for(i = 0; i < len; ++i)
    {
      *buf++ = qspi_byte_read(QSPI_FLASH_QSPIx);
    }
    total_len -= len;
  }while(total_len);

  /* wait command completed */
  while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);
}

/**
  * @brief  qspi flash write data
  * @param  addr: the address for write
  * @param  total_len: the length for write
  * @param  buf: the pointer for write data
  * @retval none
  */
void qspi_flash_data_write(uint32_t addr, uint8_t* buf, uint32_t total_len)
{
  uint32_t i, len;

  do
  {
    qspi_flash_write_enable();
    /* send up to 256 bytes at one time, and only one page */
    len = (addr / QSPI_FLASH_PAGE_SIZE + 1) * QSPI_FLASH_PAGE_SIZE - addr;
    if(total_len < len)
      len = total_len;

    qspi_flash_cmd_write_config(&qspi_flash_cmd_config, addr, len);
    qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

    for(i = 0; i < len; ++i)
    {
      while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_TXFIFORDY_FLAG) == RESET);
      qspi_byte_write(QSPI_FLASH_QSPIx, *buf++);
    }
    total_len -= len;
    addr += len;

    /* wait command completed */
    while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET);
    qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);

    qspi_flash_busy_check();

  }while(total_len);
}

/**
  * @brief  qspi flash erase data
  * @param  sec_addr: the sector address for erase
  * @retval none
  */
void qspi_flash_erase(uint32_t sec_addr)
{
  qspi_flash_write_enable();

  qspi_flash_cmd_erase_config(&qspi_flash_cmd_config, sec_addr);
  qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);

  qspi_flash_busy_check();
}

/**
  * @brief  qspi flash check busy
  * @param  none
  * @retval none
  */
void qspi_flash_busy_check(void)
{
  qspi_flash_cmd_rdsr_config(&qspi_flash_cmd_config);
  qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);
}

/**
  * @brief  qspi flash write enable
  * @param  none
  * @retval none
  */
void qspi_flash_write_enable(void)
{
  qspi_flash_cmd_wren_config(&qspi_flash_cmd_config);
  qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);
}
