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

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_EDMA_i2s_halfduplex_edma_doublebuffer EDMA_i2s_halfduplex_edma_doublebuffer
  * @{
  */

uint16_t i2s3_buffer1_tx[32] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C,
                               0x0D0E, 0x0F10, 0x1112, 0x1314, 0x1516, 0x1718,
                               0x191A, 0x1B1C, 0x1D1E, 0x1F20, 0x2122, 0x2324,
                               0x2526, 0x2728, 0x292A, 0x2B2C, 0x2D2E, 0x2F30,
                               0x3132, 0x3334, 0x3536, 0x3738, 0x393A, 0x3B3C,
                               0x3D3E, 0x3F40};
uint16_t i2s3_buffer2_tx[32] = {0x4142, 0x4344, 0x4546, 0x4748, 0x494A, 0x4B4C,
                               0x4D4E, 0x4F50, 0x5152, 0x5354, 0x5556, 0x5758,
                               0x595A, 0x5B5C, 0x5D5E, 0x5F60, 0x6162, 0x6364,
                               0x6566, 0x6768, 0x696A, 0x6B6C, 0x6D6E, 0x6F70,
                               0x7172, 0x7374, 0x7576, 0x7778, 0x797A, 0x7B7C,
                               0x7D7E, 0x7F80};
uint16_t i2s2_buffer1_rx[32];
uint16_t i2s2_buffer2_rx[32];
volatile int transfer_count = 0;

static void gpio_config(void);
static void i2s_config(void);
error_status buffer_compare(uint16_t* pbuffer1, uint16_t* pbuffer2, uint16_t buffer_length);

/**
  * @brief  buffer_compare function.
  * @param  none
  * @retval the result of compare
  */
error_status buffer_compare(uint16_t* pbuffer1, uint16_t* pbuffer2, uint16_t buffer_length)
{
  while(buffer_length--)
  {
    if(*pbuffer1 != *pbuffer2)
    {
      return ERROR;
    }

    pbuffer1++;
    pbuffer2++;
  }
  return SUCCESS;
}

/**
  * @brief  this function handles edma stream1 handler.
  * @param  none
  * @retval none
  */
void EDMA_Stream1_IRQHandler(void)
{
  if(edma_interrupt_flag_get(EDMA_FDT1_FLAG) != RESET)
  {
    if(transfer_count == 0)
    {
      if(buffer_compare(i2s3_buffer1_tx, i2s2_buffer1_rx, 32) == SUCCESS)
      {
        transfer_count = 1;
      }
      else
      {
        /* some error */
        at32_led_off(LED2);
        at32_led_off(LED3);
        at32_led_off(LED4);
      }
    }
    if(transfer_count == 1)
    {
      if(buffer_compare(i2s3_buffer2_tx, i2s2_buffer2_rx, 32) == SUCCESS)
      {
        /* edma double buffer mode transfer data pass */
        at32_led_on(LED2);
        at32_led_on(LED3);
        at32_led_on(LED4);
        /* end of transfer,close edma stream */
        edma_stream_enable(EDMA_STREAM1, FALSE);
        edma_stream_enable(EDMA_STREAM2, FALSE);
      }
      else
      {
        /* some error */
        at32_led_off(LED2);
        at32_led_off(LED3);
        at32_led_off(LED4);
      }
    }
    edma_flag_clear(EDMA_FDT1_FLAG);
  }
}

/**
  * @brief  i2s configuration.
  * @param  none
  * @retval none
  */
static void i2s_config(void)
{
  edma_init_type edma_init_struct;
  i2s_init_type i2s_init_struct;

  /* enable dma1 clock */
  crm_periph_clock_enable(CRM_EDMA_PERIPH_CLOCK, TRUE);

  /* i2s2_rx:edma stream1 configuration */
  edma_default_para_init(&edma_init_struct);
  edma_init_struct.direction = EDMA_DIR_PERIPHERAL_TO_MEMORY;
  edma_init_struct.buffer_size = (uint32_t)32;
  edma_init_struct.peripheral_data_width = EDMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  edma_init_struct.peripheral_base_addr = (uint32_t)&(SPI2->dt);
  edma_init_struct.peripheral_inc_enable = FALSE;
  edma_init_struct.memory_data_width = EDMA_MEMORY_DATA_WIDTH_HALFWORD;
  edma_init_struct.memory0_base_addr = (uint32_t)i2s2_buffer1_rx;
  edma_init_struct.memory_inc_enable = TRUE;
  edma_init_struct.peripheral_burst_mode = EDMA_PERIPHERAL_SINGLE;
  edma_init_struct.memory_burst_mode = EDMA_MEMORY_SINGLE;
  edma_init_struct.fifo_mode_enable = FALSE;
  edma_init_struct.fifo_threshold = EDMA_FIFO_THRESHOLD_1QUARTER;
  edma_init_struct.priority = EDMA_PRIORITY_HIGH;
  edma_init_struct.loop_mode_enable = TRUE;
  edma_init(EDMA_STREAM1, &edma_init_struct);

  /* edma stream1 double buffer mode init */
  edma_double_buffer_mode_init(EDMA_STREAM1, (uint32_t)i2s2_buffer2_rx, EDMA_MEMORY_0);
  edma_double_buffer_mode_enable(EDMA_STREAM1, TRUE);

  /* i2s3_tx:edma stream2 configuration */
  edma_init_struct.direction = EDMA_DIR_MEMORY_TO_PERIPHERAL;
  edma_init_struct.peripheral_base_addr = (uint32_t)&(SPI3->dt);
  edma_init_struct.memory0_base_addr = (uint32_t)i2s3_buffer1_tx;
  edma_init(EDMA_STREAM2, &edma_init_struct);

  /* edma stream2 double buffer mode init */
  edma_double_buffer_mode_init(EDMA_STREAM2, (uint32_t)i2s3_buffer2_tx, EDMA_MEMORY_0);
  edma_double_buffer_mode_enable(EDMA_STREAM2, TRUE);

  /* enable transfer full data interrupt */
  edma_interrupt_enable(EDMA_STREAM1, EDMA_FDT_INT, TRUE);

  /* edma stream1 interrupt nvic init */
  nvic_irq_enable(EDMA_Stream1_IRQn, 1, 0);

  /* edmamux init */
  edmamux_enable(TRUE);
  edmamux_init(EDMAMUX_CHANNEL1, EDMAMUX_DMAREQ_ID_SPI2_RX);
  edmamux_init(EDMAMUX_CHANNEL2, EDMAMUX_DMAREQ_ID_SPI3_TX);

  crm_periph_clock_enable(CRM_SPI3_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
  i2s_default_para_init(&i2s_init_struct);
  i2s_init_struct.audio_protocol = I2S_AUDIO_PROTOCOL_PHILLIPS;
  i2s_init_struct.data_channel_format = I2S_DATA_16BIT_CHANNEL_32BIT;
  i2s_init_struct.mclk_output_enable = TRUE;
  i2s_init_struct.audio_sampling_freq = I2S_AUDIO_FREQUENCY_48K;
  i2s_init_struct.clock_polarity = I2S_CLOCK_POLARITY_LOW;
  i2s_init_struct.operation_mode = I2S_MODE_MASTER_TX;
  i2s_init(SPI3, &i2s_init_struct);

  i2s_init_struct.operation_mode =I2S_MODE_SLAVE_RX;
  i2s_init(SPI2, &i2s_init_struct);

  edma_stream_enable(EDMA_STREAM1, TRUE);
  edma_stream_enable(EDMA_STREAM2, TRUE);
  spi_i2s_dma_receiver_enable(SPI2, TRUE);

  spi_i2s_dma_transmitter_enable(SPI3, TRUE);

  i2s_enable(SPI2, TRUE);

 i2s_enable(SPI3, TRUE);
}

/**
  * @brief  gpio configuration.
  * @param  none
  * @retval none
  */
static void gpio_config(void)
{
  gpio_init_type gpio_initstructure;
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);

  /* master ws pin */
  gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_initstructure.gpio_pins           = GPIO_PINS_4;
  gpio_init(GPIOA, &gpio_initstructure);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE4, GPIO_MUX_6);

  /* master ck pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_pins           = GPIO_PINS_10;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE10, GPIO_MUX_6);

  /* master sd pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins           = GPIO_PINS_12;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE12, GPIO_MUX_6);

  /* master mck pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins           = GPIO_PINS_7;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE7, GPIO_MUX_6);

  /* slave ws pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins           = GPIO_PINS_0;
  gpio_init(GPIOD, &gpio_initstructure);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE0, GPIO_MUX_7);

  /* slave ck pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_pins           = GPIO_PINS_1;
  gpio_init(GPIOD, &gpio_initstructure);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE1, GPIO_MUX_6);

  /* slave sd pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins           = GPIO_PINS_4;
  gpio_init(GPIOD, &gpio_initstructure);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE4, GPIO_MUX_6);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  system_clock_config();
  at32_board_init();

  gpio_config();
  i2s_config();


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
