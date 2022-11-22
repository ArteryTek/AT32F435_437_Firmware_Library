/**
  **************************************************************************
  * @file     xmc_psram.c
  * @brief    xmc_psram program
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

#include "xmc_psram.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_XMC_psram
  * @{
  */

#define BANK1_SRAM3_ADDR    (uint32_t)0x68000000

/**
  * @brief  init psram interface
  * @param  none
  * @retval none
  */
void psram_init(void)
{
  gpio_init_type  gpio_init_struct = {0};
  xmc_norsram_init_type  xmc_norsram_init_struct;
  xmc_norsram_timing_init_type rw_timing_struct, w_timing_struct;

  /* enable the gpio clock */
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* enable the xmc clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);

  /*-- gpio configuration ------------------------------------------------------*/
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE15, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE3, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE6, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE2, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE7, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE15, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE7, GPIO_MUX_12);

  /* address lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_13 | GPIO_PINS_14;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOG, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_2;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOE, &gpio_init_struct);

  /* data lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_7 | GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOE, &gpio_init_struct);

  /* ne/noe/nwe/nbl0/nbl1 lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOE, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOG, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOB, &gpio_init_struct);


  /*-- xmc configuration ------------------------------------------------------*/
  xmc_norsram_default_para_init(&xmc_norsram_init_struct);
  xmc_norsram_init_struct.subbank = XMC_BANK1_NOR_SRAM3;
  xmc_norsram_init_struct.data_addr_multiplex = XMC_DATA_ADDR_MUX_ENABLE;
  xmc_norsram_init_struct.device = XMC_DEVICE_PSRAM;
  xmc_norsram_init_struct.bus_type = XMC_BUSTYPE_16_BITS;
  xmc_norsram_init_struct.burst_mode_enable = XMC_BURST_MODE_DISABLE;
  xmc_norsram_init_struct.asynwait_enable = XMC_ASYN_WAIT_DISABLE;
  xmc_norsram_init_struct.wait_signal_lv = XMC_WAIT_SIGNAL_LEVEL_LOW;
  xmc_norsram_init_struct.wrapped_mode_enable = XMC_WRAPPED_MODE_DISABLE;
  xmc_norsram_init_struct.wait_signal_config = XMC_WAIT_SIGNAL_SYN_BEFORE;
  xmc_norsram_init_struct.write_enable = XMC_WRITE_OPERATION_ENABLE;
  xmc_norsram_init_struct.wait_signal_enable = XMC_WAIT_SIGNAL_DISABLE;
  xmc_norsram_init_struct.write_timing_enable = XMC_WRITE_TIMING_DISABLE;
  xmc_norsram_init_struct.write_burst_syn = XMC_WRITE_BURST_SYN_DISABLE;
  xmc_nor_sram_init(&xmc_norsram_init_struct);

  /* timing configuration */
  xmc_norsram_timing_default_para_init(&rw_timing_struct, &w_timing_struct);
  rw_timing_struct.subbank = XMC_BANK1_NOR_SRAM3;
  rw_timing_struct.write_timing_enable = XMC_WRITE_TIMING_DISABLE;
  rw_timing_struct.addr_hold_time      = 0x08;
  rw_timing_struct.addr_setup_time     = 0x09;
  rw_timing_struct.data_setup_time     = 0x0F;
  rw_timing_struct.data_latency_time   = 0x0;
  rw_timing_struct.bus_latency_time    = 0x0;
  rw_timing_struct.clk_psc             = 0x0;
  rw_timing_struct.mode = XMC_ACCESS_MODE_A;
  w_timing_struct.subbank = XMC_BANK1_NOR_SRAM3;
  w_timing_struct.write_timing_enable = XMC_WRITE_TIMING_DISABLE;
  w_timing_struct.addr_hold_time      = 0x08;
  w_timing_struct.addr_setup_time     = 0x09;
  w_timing_struct.data_setup_time     = 0x0F;
  w_timing_struct.data_latency_time   = 0x0;
  w_timing_struct.bus_latency_time    = 0x0;
  w_timing_struct.clk_psc             = 0x0;
  w_timing_struct.mode = XMC_ACCESS_MODE_A;
  xmc_nor_sram_timing_config(&rw_timing_struct, &w_timing_struct);

  /* bus turnaround phase for consecutive read duration and consecutive write duration */
  xmc_ext_timing_config(XMC_BANK1_NOR_SRAM3, 0x08, 0x08);

  /* enable xmc_bank1_nor_sram3 */
  xmc_nor_sram_enable(XMC_BANK1_NOR_SRAM3, TRUE);
}

/**
  * @brief  writes a half-word buffer to the xmc psram memory.
  * @param  pbuffer : pointer to buffer.
  * @param  writeaddr : psram memory internal address from which the data will be
  *         written.
  * @param  numhalfwordtowrite : number of half-words to write.
  * @retval none
  */
void psram_writebuffer(uint16_t* pbuffer, uint32_t writeaddr, uint32_t numhalfwordtowrite)
{
  for(; numhalfwordtowrite != 0; numhalfwordtowrite--) /*!< while there is data to write */
  {
    /*!< Transfer data to the memory */
    *(__IO uint16_t *) (BANK1_SRAM3_ADDR + writeaddr) = *pbuffer++;

    /*!< increment the address*/
    writeaddr += 2;
  }
}

/**
  * @brief  reads a block of data from the xmc psram memory.
  * @param  pbuffer : pointer to the buffer that receives the data read from the
  *         psram memory.
  * @param  readaddr : psram memory internal address to read from.
  * @param  numhalfwordtoread : number of half-words to read.
  * @retval none
  */
void psram_readbuffer(uint16_t* pbuffer, uint32_t readaddr, uint32_t numhalfwordtoread)
{
  for(; numhalfwordtoread != 0; numhalfwordtoread--) /*!< while there is data to read */
  {
    /*!< read a half-word from the memory */
    *pbuffer++ = *(__IO uint16_t*) (BANK1_SRAM3_ADDR + readaddr);

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

