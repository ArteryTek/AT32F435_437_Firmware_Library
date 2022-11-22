/**
  ******************************************************************************
  * @file     xmc_sdram.c
  * @brief    sdram program.
  ******************************************************************************
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

#include "xmc_sdram.h"
#include "at32f435_437_board.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_XMC_sdram_basic
  * @{
  */

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
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);

  /* enable the xmc clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);

  /*-- gpio configuration ------------------------------------------------------*/

  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE6, GPIO_MUX_12);

  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE2, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE3, GPIO_MUX_12);

  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE15, GPIO_MUX_12);

  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE7, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE15, GPIO_MUX_12);

  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE2, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE3, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE15, GPIO_MUX_12);

  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE2, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE15, GPIO_MUX_12);

  /* address lines configuration */
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

  gpio_init_struct.gpio_pins = GPIO_PINS_5 | GPIO_PINS_6;
  gpio_init(GPIOB, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_2 | GPIO_PINS_3;
  gpio_init(GPIOC, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_7 | GPIO_PINS_8 | GPIO_PINS_10 |
                               GPIO_PINS_9 | GPIO_PINS_11 | GPIO_PINS_11 | GPIO_PINS_12 |
                               GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init(GPIOE, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2 | GPIO_PINS_3 |
                               GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_11 | GPIO_PINS_12 |
                               GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init(GPIOF, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2 | GPIO_PINS_4 |
                               GPIO_PINS_5 | GPIO_PINS_8 | GPIO_PINS_15;
  gpio_init(GPIOG, &gpio_init_struct);


  /*-- xmc configuration ------------------------------------------------------*/
  xmc_sdram_default_para_init(&sdram_init_struct, &sdram_timing_struct);

  sdram_init_struct.sdram_bank                 = XMC_SDRAM_BANK1;
  sdram_init_struct.internel_banks             = XMC_INBK_4;
  sdram_init_struct.clkdiv                     = XMC_CLKDIV_3;
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
void sdram_writebuffer(uint16_t* pbuffer, uint32_t writeaddr, uint32_t numhalfwordtowrite)
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
void sdram_readbuffer(uint16_t* pbuffer, uint32_t readaddr, uint32_t numhalfwordtoread)
{
  for(; numhalfwordtoread != 0; numhalfwordtoread--) /*!< while there is data to read */
  {
    /*!< read a half-word from the memory */
    *pbuffer++ = *(__IO uint16_t*) (SDRAM_BANK1_ADDR + readaddr);

    /*!< increment the address*/
    readaddr += 2;
  }
}

/**
  * @brief  fill the global buffer
  * @param  pbuffer: pointer on the buffer to fill
  * @param  buffersize: size of the buffer to fill
  * @param  offset: first value to fill on the buffer
  */
void fill_buffer(uint16_t *pbuffer, uint16_t bufferlenght, uint32_t offset)
{
    uint16_t indextmp = 0;

    /* put in global buffer same values */
    for (indextmp = 0; indextmp < bufferlenght; indextmp++ )
    {
        pbuffer[indextmp] = indextmp + offset;
    }
}

/**
  * @}
  */

/**
  * @}
  */
