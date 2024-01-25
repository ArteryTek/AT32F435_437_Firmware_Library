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

#include "at32f435_437.h"
#include "dvp.h"
#include "xmc_lcd.h"
#include "ov5640.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_DVP_ov5640_capture
  * @{
  */

edma_init_type edma_init_struct;
uint8_t ov_frame = 0;

extern void jpeg_data_process(void);
void dvp_int_handler(void);

typedef struct
{
  uint32_t CR_NDT;
  uint32_t PA;
  uint32_t M0A;
  uint32_t LLP;

}Descriptors_Format_def;

/**
  * @brief  configure dvp
  * @param  none
  * @retval none
  */
void dvp_config(void)
{
  dvp_io_init();

  nvic_irq_enable(DVP_IRQn, 0, 0);

  dvp_capture_mode_set(DVP_CAP_FUNC_MODE_CONTINUOUS);
  dvp_hsync_polarity_set(DVP_HSYNC_POLARITY_LOW);
  dvp_vsync_polarity_set(DVP_VSYNC_POLARITY_LOW);
  dvp_pclk_polarity_set(DVP_CLK_POLARITY_RISING);
  dvp_zoomout_set(DVP_PCDC_ALL, DVP_PCDS_CAP_FIRST, DVP_LCDC_ALL, DVP_LCDS_CAP_FIRST);
  dvp_zoomout_select(DVP_PCDES_CAP_FIRST);
  dvp_pixel_data_length_set(DVP_PIXEL_DATA_LENGTH_8);
#ifdef HARDWARE_MODE
  dvp_sync_mode_set(DVP_SYNC_MODE_HARDWARE);
#else
  dvp_sync_mode_set(DVP_SYNC_MODE_EMBEDDED);
  dvp_sync_code_set(0xFF, 0xFF, 0xC7, 0xDA);
#endif

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
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE4,  GPIO_MUX_13);//HS
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE6,  GPIO_MUX_13);//PCLK
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9,  GPIO_MUX_13);//D0
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE10,  GPIO_MUX_13);//D1
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE11,  GPIO_MUX_13);//D2
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE12, GPIO_MUX_13);//D3
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE4,  GPIO_MUX_13);//D5
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE7,  GPIO_MUX_13);//VS
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE9,  GPIO_MUX_13);//D7
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE4,  GPIO_MUX_13);//D4
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE5,  GPIO_MUX_13);//D6

  gpio_initure.gpio_pins = GPIO_PINS_4 | GPIO_PINS_6 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_11 | GPIO_PINS_12;
  gpio_initure.gpio_mode = GPIO_MODE_MUX;
  gpio_initure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initure.gpio_pull = GPIO_PULL_UP;
  gpio_initure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_initure);

  gpio_initure.gpio_pins = GPIO_PINS_4 | GPIO_PINS_7 | GPIO_PINS_9;
  gpio_init(GPIOB, &gpio_initure);

  gpio_initure.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init(GPIOE, &gpio_initure);
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

  edma_reset(EDMA_STREAM4);
  edma_init(EDMA_STREAM4, &edma_init_struct);

  edmamux_enable(TRUE);
  edmamux_init(EDMAMUX_CHANNEL4, EDMAMUX_DMAREQ_ID_DVP);

  if(mem1addr != 0)
  {
    edma_double_buffer_mode_init(EDMA_STREAM4, mem1addr, EDMA_MEMORY_0);
    edma_double_buffer_mode_enable(EDMA_STREAM4, TRUE);
  }

  edma_interrupt_enable(EDMA_STREAM4, EDMA_FDT_INT, TRUE);
  nvic_irq_enable(EDMA_Stream4_IRQn, 0, 0);
}

/**
  * @brief  start dvp transmission
  * @param  none
  * @retval none
  */
void dvp_start(void)
{
  lcd_struct->lcd_setblock(0, 0, LCD_W, LCD_H);
  edma_stream_enable(EDMA_STREAM4, TRUE);
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

  edma_stream_enable(EDMA_STREAM4, FALSE);
}

/**
  * @brief  dvp irq handler
  * @param  none
  * @retval none
  */
void DVP_IRQHandler(void)
{
  dvp_int_handler();
}

/**
  * @brief  dvp frame transmit done
  * @param  none
  * @retval none
  */
void dvp_frame_done(void)
{
  lcd_struct->lcd_setblock(0, 0, LCD_W, LCD_H);
  ov_frame++;
}

/**
  * @brief  edma stream4 irq handler
  * @param  none
  * @retval none
  */
void EDMA_Stream4_IRQHandler(void)
{
  if(edma_interrupt_flag_get(EDMA_FDT4_FLAG) != RESET)
  {
    edma_flag_clear(EDMA_FDT4_FLAG);
  }
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

/**
  * @}
  */

/**
  * @}
  */
