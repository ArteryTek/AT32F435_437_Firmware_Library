/**
  **************************************************************************
  * @file     at_surf_f437_board_sdram.c
  * @brief    the driver library of the sdram
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

#include "at_surf_f437_board_sdram.h"
#include "at_surf_f437_board_delay.h"

#define SDRAM_BANK1_ADDR    ((uint32_t)0xC0000000)
void sdram_init_sequence(xmc_cmd_bank1_2_type cmd_bank);

/**
  * @brief  init sdram interface
  * @param  none
  * @retval none
  */
void sdram_init(void)
{
  gpio_init_type  gpio_init_struct = {0};

  xmc_sdram_init_type sdram_init_struct;
  xmc_sdram_timing_type sdram_timing_struct;

  /* enable the gpio clock */
  crm_periph_clock_enable(SDRAM_D0_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D1_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D2_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D3_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D4_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D5_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D6_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D7_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D8_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D9_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_D10_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_D11_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_D12_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_D13_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_D14_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_D15_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_A0_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A0_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A1_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A2_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A3_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A4_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A5_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A6_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A7_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A8_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A9_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_A10_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_A11_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_A12_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_CKE_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_CLK_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_CS_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_WE_GPIO_CLK,   TRUE);
  crm_periph_clock_enable(SDRAM_CAS_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_RAS_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_BA0_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_BA1_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(SDRAM_NBL0_GPIO_CLK, TRUE);
  crm_periph_clock_enable(SDRAM_NBL1_GPIO_CLK, TRUE);

  /* enable the xmc clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);

  /* gpio mux configuration */
  gpio_pin_mux_config(SDRAM_D0_GPIO_PORT,   SDRAM_D0_GPIO_PINS_SOURCE,   SDRAM_D0_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D1_GPIO_PORT,   SDRAM_D1_GPIO_PINS_SOURCE,   SDRAM_D1_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D2_GPIO_PORT,   SDRAM_D2_GPIO_PINS_SOURCE,   SDRAM_D2_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D3_GPIO_PORT,   SDRAM_D3_GPIO_PINS_SOURCE,   SDRAM_D3_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D4_GPIO_PORT,   SDRAM_D4_GPIO_PINS_SOURCE,   SDRAM_D4_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D5_GPIO_PORT,   SDRAM_D5_GPIO_PINS_SOURCE,   SDRAM_D5_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D6_GPIO_PORT,   SDRAM_D6_GPIO_PINS_SOURCE,   SDRAM_D6_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D7_GPIO_PORT,   SDRAM_D7_GPIO_PINS_SOURCE,   SDRAM_D7_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D8_GPIO_PORT,   SDRAM_D8_GPIO_PINS_SOURCE,   SDRAM_D8_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D9_GPIO_PORT,   SDRAM_D9_GPIO_PINS_SOURCE,   SDRAM_D9_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D10_GPIO_PORT,  SDRAM_D10_GPIO_PINS_SOURCE,  SDRAM_D10_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D11_GPIO_PORT,  SDRAM_D11_GPIO_PINS_SOURCE,  SDRAM_D11_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D12_GPIO_PORT,  SDRAM_D12_GPIO_PINS_SOURCE,  SDRAM_D12_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D13_GPIO_PORT,  SDRAM_D13_GPIO_PINS_SOURCE,  SDRAM_D13_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D14_GPIO_PORT,  SDRAM_D14_GPIO_PINS_SOURCE,  SDRAM_D14_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_D15_GPIO_PORT,  SDRAM_D15_GPIO_PINS_SOURCE,  SDRAM_D15_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A0_GPIO_PORT,   SDRAM_A0_GPIO_PINS_SOURCE,   SDRAM_A0_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A0_GPIO_PORT,   SDRAM_A0_GPIO_PINS_SOURCE,   SDRAM_A0_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A1_GPIO_PORT,   SDRAM_A1_GPIO_PINS_SOURCE,   SDRAM_A1_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A2_GPIO_PORT,   SDRAM_A2_GPIO_PINS_SOURCE,   SDRAM_A2_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A3_GPIO_PORT,   SDRAM_A3_GPIO_PINS_SOURCE,   SDRAM_A3_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A4_GPIO_PORT,   SDRAM_A4_GPIO_PINS_SOURCE,   SDRAM_A4_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A5_GPIO_PORT,   SDRAM_A5_GPIO_PINS_SOURCE,   SDRAM_A5_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A6_GPIO_PORT,   SDRAM_A6_GPIO_PINS_SOURCE,   SDRAM_A6_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A7_GPIO_PORT,   SDRAM_A7_GPIO_PINS_SOURCE,   SDRAM_A7_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A8_GPIO_PORT,   SDRAM_A8_GPIO_PINS_SOURCE,   SDRAM_A8_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A9_GPIO_PORT,   SDRAM_A9_GPIO_PINS_SOURCE,   SDRAM_A9_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A10_GPIO_PORT,  SDRAM_A10_GPIO_PINS_SOURCE,  SDRAM_A10_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A11_GPIO_PORT,  SDRAM_A11_GPIO_PINS_SOURCE,  SDRAM_A11_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_A12_GPIO_PORT,  SDRAM_A12_GPIO_PINS_SOURCE,  SDRAM_A12_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_CKE_GPIO_PORT,  SDRAM_CKE_GPIO_PINS_SOURCE,  SDRAM_CKE_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_CLK_GPIO_PORT,  SDRAM_CLK_GPIO_PINS_SOURCE,  SDRAM_CLK_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_CS_GPIO_PORT,   SDRAM_CS_GPIO_PINS_SOURCE,   SDRAM_CS_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_WE_GPIO_PORT,   SDRAM_WE_GPIO_PINS_SOURCE,   SDRAM_WE_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_CAS_GPIO_PORT,  SDRAM_CAS_GPIO_PINS_SOURCE,  SDRAM_CAS_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_RAS_GPIO_PORT,  SDRAM_RAS_GPIO_PINS_SOURCE,  SDRAM_RAS_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_BA0_GPIO_PORT,  SDRAM_BA0_GPIO_PINS_SOURCE,  SDRAM_BA0_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_BA1_GPIO_PORT,  SDRAM_BA1_GPIO_PINS_SOURCE,  SDRAM_BA1_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_NBL0_GPIO_PORT, SDRAM_NBL0_GPIO_PINS_SOURCE, SDRAM_NBL0_GPIO_MUX);
  gpio_pin_mux_config(SDRAM_NBL1_GPIO_PORT, SDRAM_NBL1_GPIO_PINS_SOURCE, SDRAM_NBL1_GPIO_MUX);


  /* address lines configuration */
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

  gpio_init_struct.gpio_pins = SDRAM_D0_GPIO_PIN;
  gpio_init(SDRAM_D0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D1_GPIO_PIN;
  gpio_init(SDRAM_D1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D2_GPIO_PIN;
  gpio_init(SDRAM_D2_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D3_GPIO_PIN;
  gpio_init(SDRAM_D3_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D4_GPIO_PIN;
  gpio_init(SDRAM_D4_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D5_GPIO_PIN;
  gpio_init(SDRAM_D5_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D6_GPIO_PIN;
  gpio_init(SDRAM_D6_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D7_GPIO_PIN;
  gpio_init(SDRAM_D7_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D8_GPIO_PIN;
  gpio_init(SDRAM_D8_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D9_GPIO_PIN;
  gpio_init(SDRAM_D9_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D10_GPIO_PIN;
  gpio_init(SDRAM_D10_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D11_GPIO_PIN;
  gpio_init(SDRAM_D11_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D12_GPIO_PIN;
  gpio_init(SDRAM_D12_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D13_GPIO_PIN;
  gpio_init(SDRAM_D13_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D14_GPIO_PIN;
  gpio_init(SDRAM_D14_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_D15_GPIO_PIN;
  gpio_init(SDRAM_D15_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A0_GPIO_PIN;
  gpio_init(SDRAM_A0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A0_GPIO_PIN;
  gpio_init(SDRAM_A0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A1_GPIO_PIN;
  gpio_init(SDRAM_A1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A2_GPIO_PIN;
  gpio_init(SDRAM_A2_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A3_GPIO_PIN;
  gpio_init(SDRAM_A3_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A4_GPIO_PIN;
  gpio_init(SDRAM_A4_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A5_GPIO_PIN;
  gpio_init(SDRAM_A5_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A6_GPIO_PIN;
  gpio_init(SDRAM_A6_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A7_GPIO_PIN;
  gpio_init(SDRAM_A7_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A8_GPIO_PIN;
  gpio_init(SDRAM_A8_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A9_GPIO_PIN;
  gpio_init(SDRAM_A9_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A10_GPIO_PIN;
  gpio_init(SDRAM_A10_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A11_GPIO_PIN;
  gpio_init(SDRAM_A11_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_A12_GPIO_PIN;
  gpio_init(SDRAM_A12_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_CKE_GPIO_PIN;
  gpio_init(SDRAM_CKE_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_CLK_GPIO_PIN;
  gpio_init(SDRAM_CLK_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_CS_GPIO_PIN;
  gpio_init(SDRAM_CS_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_WE_GPIO_PIN;
  gpio_init(SDRAM_WE_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_CAS_GPIO_PIN;
  gpio_init(SDRAM_CAS_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_RAS_GPIO_PIN;
  gpio_init(SDRAM_RAS_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_BA0_GPIO_PIN;
  gpio_init(SDRAM_BA0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_BA1_GPIO_PIN;
  gpio_init(SDRAM_BA1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_NBL0_GPIO_PIN;
  gpio_init(SDRAM_NBL0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = SDRAM_NBL1_GPIO_PIN;
  gpio_init(SDRAM_NBL1_GPIO_PORT, &gpio_init_struct);

  /*-- xmc configuration ------------------------------------------------------*/
  xmc_sdram_default_para_init(&sdram_init_struct, &sdram_timing_struct);

  sdram_init_struct.sdram_bank                 = XMC_SDRAM_BANK1;
  sdram_init_struct.internel_banks             = XMC_INBK_4;
  sdram_init_struct.clkdiv                     = XMC_CLKDIV_4;
  sdram_init_struct.write_protection           = FALSE;
  sdram_init_struct.burst_read                 = FALSE;
  sdram_init_struct.read_delay                 = XMC_READ_DELAY_1;
  sdram_init_struct.column_address             = XMC_COLUMN_9;
  sdram_init_struct.row_address                = XMC_ROW_13;
  sdram_init_struct.cas                        = XMC_CAS_3;
  sdram_init_struct.width                      = XMC_MEM_WIDTH_16;

  sdram_timing_struct.tmrd                     = XMC_DELAY_CYCLE_2;
  sdram_timing_struct.txsr                     = XMC_DELAY_CYCLE_11;
  sdram_timing_struct.tras                     = XMC_DELAY_CYCLE_7;
  sdram_timing_struct.trc                      = XMC_DELAY_CYCLE_9;
  sdram_timing_struct.twr                      = XMC_DELAY_CYCLE_2;
  sdram_timing_struct.trp                      = XMC_DELAY_CYCLE_3;
  sdram_timing_struct.trcd                     = XMC_DELAY_CYCLE_3;

  xmc_sdram_init(&sdram_init_struct, &sdram_timing_struct);

  sdram_init_sequence(XMC_CMD_BANK1);
}

/**
  * @brief  init sdram sequence
  * @param  none
  * @retval none
  */
void sdram_init_sequence(xmc_cmd_bank1_2_type cmd_bank)
{
  xmc_sdram_cmd_type sdram_cmd_struct;
  uint32_t timeout = 0xFFFF;

  sdram_cmd_struct.cmd                   = XMC_CMD_CLK;
  sdram_cmd_struct.auto_refresh          = 1;
  sdram_cmd_struct.cmd_banks             = cmd_bank;
  sdram_cmd_struct.data                  = 0;
  xmc_sdram_cmd(&sdram_cmd_struct);

  while((xmc_flag_status_get(XMC_BANK5_6_SDRAM, XMC_BUSY_FLAG) != RESET) && (timeout > 0))
  {
    timeout --;
  }
  delay_ms(100);

  sdram_cmd_struct.cmd                   = XMC_CMD_PRECHARG_ALL;
  sdram_cmd_struct.auto_refresh          = 1;
  sdram_cmd_struct.cmd_banks             = cmd_bank;
  sdram_cmd_struct.data                  = 0;
  xmc_sdram_cmd(&sdram_cmd_struct);

  timeout = 0xFFFF;
  while((xmc_flag_status_get(XMC_BANK5_6_SDRAM, XMC_BUSY_FLAG) != RESET) && (timeout > 0))
  {
    timeout --;
  }

  /* counter = (refresh_count * 1000 * SDCLK) / row - 20 */
  /* counter = (64ms * 1000 * 144MHz) / 2^13 - 20 */
  xmc_sdram_refresh_counter_set(1105);

  sdram_cmd_struct.cmd                   = XMC_CMD_AUTO_REFRESH;
  sdram_cmd_struct.auto_refresh          = 8;
  sdram_cmd_struct.cmd_banks             = cmd_bank;
  sdram_cmd_struct.data                  = 0;
  xmc_sdram_cmd(&sdram_cmd_struct);

  timeout = 0xFFFF;
  while((xmc_flag_status_get(XMC_BANK5_6_SDRAM, XMC_BUSY_FLAG) != RESET) && (timeout > 0))
  {
    timeout --;
  }

  sdram_cmd_struct.cmd                   = XMC_CMD_LOAD_MODE;
  sdram_cmd_struct.auto_refresh          = 1;
  sdram_cmd_struct.cmd_banks             = cmd_bank;
  sdram_cmd_struct.data                  = (uint32_t)SDRAM_BURST_LEN_1 | SDRAM_BURST_SEQUENTIAL |
                                           SDRAM_CAS_LATENCY_3 | SDRAM_OPERATING_MODE_STANDARD |
                                           SDRAM_WR_BURST_SINGLE;
  xmc_sdram_cmd(&sdram_cmd_struct);

  timeout = 0xFFFF;
  while((xmc_flag_status_get(XMC_BANK5_6_SDRAM, XMC_BUSY_FLAG) != RESET) && (timeout > 0))
  {
    timeout --;
  }
}

/**
  * @brief  writes a half-word buffer to the sdram memory.
  * @param  pbuffer : pointer to buffer.
  * @param  writeaddr : sdram memory internal address from which the data will be
  *         written.
  * @param  numhalfwordtowrite : number of half-words to write.
  * @retval none
  */
void sdram_data_write(uint32_t writeaddr, uint16_t* pbuffer, uint32_t numhalfwordtowrite)
{
  for(; numhalfwordtowrite != 0; numhalfwordtowrite--) /*!< while there is data to write */
  {
    /*!< Transfer data to the memory */
    *(__IO uint16_t *) (SDRAM_BANK1_ADDR + writeaddr) = *pbuffer++;

    /*!< increment the address*/
    writeaddr += 2;
  }
}

/**
  * @brief  reads a block of data from the sdram.
  * @param  pbuffer : pointer to the buffer that receives the data read from the
  *         sdram memory.
  * @param  readaddr : sdram memory internal address to read from.
  * @param  numhalfwordtoread : number of half-words to read.
  * @retval none
  */
void sdram_data_read(uint32_t readaddr, uint16_t* pbuffer, uint32_t numhalfwordtoread)
{
  for(; numhalfwordtoread != 0; numhalfwordtoread--) /*!< while there is data to read */
  {
    /*!< read a half-word from the memory */
    *pbuffer++ = *(__IO uint16_t*) (SDRAM_BANK1_ADDR + readaddr);

    /*!< increment the address*/
    readaddr += 2;
  }
}
