/**
  **************************************************************************
  * @file     dvp.c
  * @brief    dvp program
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

#include "at_surf_f437_board_dvp.h"
#include "at_surf_f437_board_lcd.h"
#include "at_surf_f437_board_ov5640.h"

uint8_t ov_frame = 0;

/**
  * @brief  configure dvp
  * @param  none
  * @retval none
  */
void dvp_config(uint16_t id)
{
  dvp_io_init();

  nvic_irq_enable(DVP_IRQn, 0, 0);

  dvp_capture_mode_set(DVP_CAP_FUNC_MODE_CONTINUOUS);
  dvp_hsync_polarity_set(DVP_HSYNC_POLARITY_HIGH);
  if (id == OV5640_ID)
  {
    dvp_vsync_polarity_set(DVP_VSYNC_POLARITY_HIGH);
  }
  else
  {
    dvp_vsync_polarity_set(DVP_VSYNC_POLARITY_LOW);
  }
  dvp_pclk_polarity_set(DVP_CLK_POLARITY_RISING);
  dvp_zoomout_set(DVP_PCDC_ALL, DVP_PCDS_CAP_FIRST, DVP_LCDC_ALL, DVP_LCDS_CAP_FIRST);
  dvp_zoomout_select(DVP_PCDES_CAP_FIRST);
  dvp_pixel_data_length_set(DVP_PIXEL_DATA_LENGTH_8);

  //*(__IO uint32_t *)(0x50050040) = (uint32_t)0x1000;

  dvp_sync_mode_set(DVP_SYNC_MODE_HARDWARE);


  /* enable the interrupts */
  dvp_interrupt_enable(DVP_CFD_INT | DVP_OVR_INT | DVP_ESE_INT | DVP_VS_INT | DVP_HS_INT, TRUE);

  /* enable dvp by setting dvpen bit */
  dvp_enable(TRUE);
}

/**
  * @brief  initialize gpio for dvp
  * @param  none
  * @retval none
  */
void dvp_io_init(void)
{
  gpio_init_type gpio_initure;

  crm_periph_clock_enable(CRM_DVP_PERIPH_CLOCK, TRUE);

  crm_periph_clock_enable(DVP_D0_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_D1_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_D2_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_D3_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_D4_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_D5_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_D6_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_D7_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_PIXCLK_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_VSYNC_GPIO_CLK, TRUE);
  crm_periph_clock_enable(DVP_HSYNC_GPIO_CLK, TRUE);

  gpio_pin_mux_config(DVP_D0_GPIO_PORT,     DVP_D0_GPIO_PINS_SOURCE,     DVP_D0_GPIO_MUX);
  gpio_pin_mux_config(DVP_D1_GPIO_PORT,     DVP_D1_GPIO_PINS_SOURCE,     DVP_D1_GPIO_MUX);
  gpio_pin_mux_config(DVP_D2_GPIO_PORT,     DVP_D2_GPIO_PINS_SOURCE,     DVP_D2_GPIO_MUX);
  gpio_pin_mux_config(DVP_D3_GPIO_PORT,     DVP_D3_GPIO_PINS_SOURCE,     DVP_D3_GPIO_MUX);
  gpio_pin_mux_config(DVP_D4_GPIO_PORT,     DVP_D4_GPIO_PINS_SOURCE,     DVP_D4_GPIO_MUX);
  gpio_pin_mux_config(DVP_D5_GPIO_PORT,     DVP_D5_GPIO_PINS_SOURCE,     DVP_D5_GPIO_MUX);
  gpio_pin_mux_config(DVP_D6_GPIO_PORT,     DVP_D6_GPIO_PINS_SOURCE,     DVP_D6_GPIO_MUX);
  gpio_pin_mux_config(DVP_D7_GPIO_PORT,     DVP_D7_GPIO_PINS_SOURCE,     DVP_D7_GPIO_MUX);
  gpio_pin_mux_config(DVP_PIXCLK_GPIO_PORT, DVP_PIXCLK_GPIO_PINS_SOURCE, DVP_PIXCLK_GPIO_MUX);
  gpio_pin_mux_config(DVP_VSYNC_GPIO_PORT,  DVP_VSYNC_GPIO_PINS_SOURCE,  DVP_VSYNC_GPIO_MUX);
  gpio_pin_mux_config(DVP_HSYNC_GPIO_PORT,  DVP_HSYNC_GPIO_PINS_SOURCE,  DVP_HSYNC_GPIO_MUX);

  gpio_initure.gpio_mode = GPIO_MODE_MUX;
  gpio_initure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initure.gpio_pull = GPIO_PULL_UP;
  gpio_initure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

  gpio_initure.gpio_pins = DVP_D0_GPIO_PIN;
  gpio_init(DVP_D0_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_D1_GPIO_PIN;
  gpio_init(DVP_D1_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_D2_GPIO_PIN;
  gpio_init(DVP_D2_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_D3_GPIO_PIN;
  gpio_init(DVP_D3_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_D4_GPIO_PIN;
  gpio_init(DVP_D4_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_D5_GPIO_PIN;
  gpio_init(DVP_D5_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_D6_GPIO_PIN;
  gpio_init(DVP_D6_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_D7_GPIO_PIN;
  gpio_init(DVP_D7_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_PIXCLK_GPIO_PIN;
  gpio_init(DVP_PIXCLK_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_VSYNC_GPIO_PIN;
  gpio_init(DVP_VSYNC_GPIO_PORT, &gpio_initure);

  gpio_initure.gpio_pins = DVP_HSYNC_GPIO_PIN;
  gpio_init(DVP_HSYNC_GPIO_PORT, &gpio_initure);
}

/**
  * @brief  initialize dma for dvp
  * @param  mem0addr: memory0 base address
  * @param  mem1addr: memory1 base address
  * @param  memsize: memory size
  * @param  memblen: memory data width
  * @param  meminc: memory increase
  * @retval none
  */
void dvp_dma_init(uint32_t mem0addr, uint32_t mem1addr, uint16_t memsize)
{
  edma_init_type edma_init_struct;

  crm_periph_clock_enable(CRM_EDMA_PERIPH_CLOCK, TRUE);

  edma_init_struct.peripheral_base_addr       = (uint32_t)&DVP->dt;
  edma_init_struct.memory0_base_addr          = (uint32_t)mem0addr;
  edma_init_struct.buffer_size                = memsize;
  edma_init_struct.direction                  = EDMA_DIR_PERIPHERAL_TO_MEMORY;
  edma_init_struct.peripheral_inc_enable      = FALSE;
  edma_init_struct.memory_inc_enable          = FALSE;
  edma_init_struct.peripheral_data_width      = EDMA_PERIPHERAL_DATA_WIDTH_WORD;
  edma_init_struct.memory_data_width          = EDMA_MEMORY_DATA_WIDTH_HALFWORD;
  edma_init_struct.loop_mode_enable           = TRUE;
  edma_init_struct.priority                   = EDMA_PRIORITY_HIGH;
  edma_init_struct.fifo_mode_enable           = TRUE;
  edma_init_struct.fifo_threshold             = EDMA_FIFO_THRESHOLD_FULL;
  edma_init_struct.memory_burst_mode          = EDMA_MEMORY_SINGLE;
  edma_init_struct.peripheral_burst_mode      = EDMA_PERIPHERAL_SINGLE;

  edma_reset(DVP_EDMA_STREAM);
  edma_init(DVP_EDMA_STREAM, &edma_init_struct);

  edmamux_enable(TRUE);
  edmamux_init(DVP_EDMA_DMAMUX_CHANNEL, DVP_EDMA_DMAREQ);

  if(mem1addr != 0)
  {
    edma_double_buffer_mode_init(DVP_EDMA_STREAM, mem1addr, EDMA_MEMORY_0);
    edma_double_buffer_mode_enable(DVP_EDMA_STREAM, TRUE);
  }

  edma_interrupt_enable(DVP_EDMA_STREAM, EDMA_FDT_INT, TRUE);
  nvic_irq_enable(DVP_EDMA_IRQn, 0, 0);
}

/**
  * @brief  start dvp transmission
  * @param  none
  * @retval none
  */
void dvp_start(void)
{
  lcd_windows_set(0, 120, DVP_LCD_W, 120+DVP_LCD_H);
  edma_stream_enable(DVP_EDMA_STREAM, TRUE);
  dvp_capture_enable(TRUE);
}

/**
  * @brief  stop dvp transmission
  * @param  none
  * @retval none
  */
void dvp_stop(void)
{
  dvp_capture_enable(FALSE);

  while(DVP->ctrl & 0x01);

  edma_stream_enable(DVP_EDMA_STREAM, FALSE);
}

/**
  * @brief  dvp frame transmit done
  * @param  none
  * @retval none
  */
void dvp_frame_done(void)
{
  lcd_windows_set(0, 120, DVP_LCD_W, 120+DVP_LCD_H);
  ov_frame++;
}

/**
  * @brief  handles dvp interrupt request.
  * @param  none
  * @retval none
  */
void dvp_int_handler(void)
{
  /* synchronization error interrupt management */
  if(dvp_interrupt_flag_get(DVP_ESE_INT_FLAG) != RESET)
  {
    /* clear the synchronization error flag */
    dvp_flag_clear(DVP_ESE_INT_FLAG);
  }

  /* overflow interrupt management */
  if(dvp_interrupt_flag_get(DVP_OVR_INT_FLAG) != RESET)
  {
    /* clear the overflow flag */
    dvp_flag_clear(DVP_OVR_INT_FLAG);
  }

  /* line interrupt management */
  if(dvp_interrupt_flag_get(DVP_HS_INT_FLAG) != RESET)
  {
    /* clear the line interrupt flag */
    dvp_flag_clear(DVP_HS_INT_FLAG);
  }

  /* vsync interrupt management */
  if(dvp_interrupt_flag_get(DVP_VS_INT_FLAG) != RESET)
  {
    /* clear the vsync flag */
    dvp_flag_clear(DVP_VS_INT_FLAG);
  }

  /* end of frame interrupt management */
  if(dvp_interrupt_flag_get(DVP_CFD_INT_FLAG) != RESET)
  {
    /* clear the end of frame flag */
    dvp_flag_clear(DVP_CFD_INT_FLAG);

    dvp_frame_done();
  }
}
