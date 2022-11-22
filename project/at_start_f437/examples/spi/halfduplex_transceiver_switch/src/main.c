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

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_SPI_halfduplex_transceiver_switch SPI_halfduplex_transceiver_switch
  * @{
  */

#define BUFFER_SIZE                      32

spi_init_type spi_init_struct;
uint8_t spi3_buffer_tx[BUFFER_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                       0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
                                       0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                       0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20};
uint8_t spi2_buffer_tx[BUFFER_SIZE] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
                                       0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60,
                                       0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
                                       0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70};
uint8_t spi3_buffer_rx[BUFFER_SIZE], spi2_buffer_rx[BUFFER_SIZE];
volatile uint32_t tx_index = 0, rx_index = 0;
volatile error_status transfer_status1 = ERROR, transfer_status2 = ERROR;

static void gpio_config(void);
static void spi_config(void);
error_status buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t buffer_length);

/**
  * @brief  buffer compare function.
  * @param  pbuffer1, pbuffer2: buffers to be compared.
  * @param  buffer_length: buffer's length
  * @retval the result of compare
  */
error_status buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t buffer_length)
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
  * @brief  spi configuration.
  * @param  none
  * @retval none
  */
static void spi_config(void)
{
  crm_periph_clock_enable(CRM_SPI3_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
  spi_default_para_init(&spi_init_struct);
  spi_init_struct.transmission_mode = SPI_TRANSMIT_HALF_DUPLEX_TX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_256;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(SPI3, &spi_init_struct);

  spi_init_struct.transmission_mode = SPI_TRANSMIT_HALF_DUPLEX_RX;
  spi_init_struct.master_slave_mode = SPI_MODE_SLAVE;
  spi_init(SPI2, &spi_init_struct);

  nvic_irq_enable(SPI3_I2S3EXT_IRQn, 0, 0);
  nvic_irq_enable(SPI2_I2S2EXT_IRQn, 0, 0);
  spi_i2s_interrupt_enable(SPI3, SPI_I2S_TDBE_INT, TRUE);
  spi_i2s_interrupt_enable(SPI2, SPI_I2S_RDBF_INT, TRUE);

  spi_enable(SPI3, TRUE);
  spi_enable(SPI2, TRUE);
}

/**
  * @brief  gpio configuration.
  * @param  none
  * @retval none
  */
static void gpio_config(void)
{
  gpio_init_type gpio_initstructure;
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);

  /* master sck pin */
  gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_initstructure.gpio_pins           = GPIO_PINS_10;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE10, GPIO_MUX_6);

    /* master mosi pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins           = GPIO_PINS_12;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE12, GPIO_MUX_6);

  /* slave sck pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_pins           = GPIO_PINS_1;
  gpio_init(GPIOD, &gpio_initstructure);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE1, GPIO_MUX_6);

  /* slave miso pin */
  gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins           = GPIO_PINS_2;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE2, GPIO_MUX_5);

  while(gpio_input_data_bit_read(GPIOD, GPIO_PINS_1)!=RESET);
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
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  gpio_config();
  /* config spi1 send spi2 receive */
  spi_config();
  while(rx_index < BUFFER_SIZE);

  /* test result:the data check */
  transfer_status1 = buffer_compare(spi2_buffer_rx, spi3_buffer_tx, BUFFER_SIZE);

  /* config spi2 send spi1 receive */
  spi_enable(SPI3, FALSE);
  spi_enable(SPI2, FALSE);
  rx_index = 0;
  tx_index = 0;
  spi_i2s_interrupt_enable(SPI3, SPI_I2S_TDBE_INT, FALSE);
  spi_i2s_interrupt_enable(SPI2, SPI_I2S_RDBF_INT, FALSE);

  spi_init_struct.transmission_mode = SPI_TRANSMIT_HALF_DUPLEX_RX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  spi_init(SPI3, &spi_init_struct);

  spi_init_struct.transmission_mode = SPI_TRANSMIT_HALF_DUPLEX_TX;
  spi_init_struct.master_slave_mode = SPI_MODE_SLAVE;
  spi_init(SPI2, &spi_init_struct);

  spi_i2s_interrupt_enable(SPI3, SPI_I2S_RDBF_INT, TRUE);
  spi_i2s_interrupt_enable(SPI2, SPI_I2S_TDBE_INT, TRUE);
  spi_enable(SPI2, TRUE);
  spi_enable(SPI3, TRUE);
  while(rx_index < BUFFER_SIZE);

  /* test result:the data check */
  transfer_status2 = buffer_compare(spi3_buffer_rx, spi2_buffer_tx, BUFFER_SIZE);

  /* test result indicate:if success ,led2 lights */
  if((transfer_status1 == SUCCESS) && (transfer_status2 == SUCCESS))
  {
    at32_led_on(LED2);
  }
  else
  {
    at32_led_off(LED2);
  }
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
