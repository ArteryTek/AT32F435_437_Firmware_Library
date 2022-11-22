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

/** @addtogroup 435_USART_synchronous USART_synchronous
  * @{
  */

#define COUNTOF(a)                       (sizeof(a) / sizeof(*(a)))
#define USART2_TX_BUFFER_SIZE            (COUNTOF(usart2_tx_buffer) - 1)
#define SPI1_TX_BUFFER_SIZE              (COUNTOF(spi1_tx_buffer) - 1)
#define DYMMY_BYTE                       0x00

uint8_t usart2_tx_buffer[] = "usart synchronous example: usart2 -> spi1";
uint8_t spi1_tx_buffer[] = "usart synchronous example: spi1 -> usart2";
uint8_t usart2_rx_buffer[SPI1_TX_BUFFER_SIZE];
uint8_t spi1_rx_buffer[USART2_TX_BUFFER_SIZE];
uint8_t usart2_tx_counter = 0x00;
uint8_t spi1_tx_counter = 0x00;
uint8_t usart2_rx_counter = 0x00;
uint8_t spi1_rx_counter = 0x00;
uint8_t usart2_tx_buffer_size = USART2_TX_BUFFER_SIZE;
uint8_t spi1_tx_buffer_size = SPI1_TX_BUFFER_SIZE;

/**
  * @brief  config usart and spi
  * @param  none
  * @retval none
  */
void usart_spi_configuration(void)
{
  gpio_init_type gpio_init_struct;
  spi_init_type spi_init_struct;

  /* enable the usart2 and gpio clock */
  crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* enable the spi1 and gpio clock */
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the usart2 tx, rx and ck pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_3 | GPIO_PINS_4;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_7);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE3, GPIO_MUX_7);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE4, GPIO_MUX_7);

  /* configure the spi1 mosi, miso and ck pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOA, &gpio_init_struct);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE5, GPIO_MUX_5);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE6, GPIO_MUX_5);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE7, GPIO_MUX_5);

  /* configure usart2 param */
  usart_init(USART2, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_clock_config(USART2, USART_CLOCK_POLARITY_HIGH, USART_CLOCK_PHASE_2EDGE, USART_CLOCK_LAST_BIT_OUTPUT);
  usart_clock_enable(USART2, TRUE);
  usart_transmitter_enable(USART2, TRUE);
  usart_receiver_enable(USART2, TRUE);
  usart_enable(USART2, TRUE);

  /* configure spi1 param */
  spi_default_para_init(&spi_init_struct);
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode =SPI_MODE_SLAVE;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_HIGH;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(SPI1, &spi_init_struct);
  spi_enable(SPI1, TRUE);
}

/**
  * @brief  compares two buffers.
  * @param  pbuffer1, pbuffer2: buffers to be compared.
  * @param  buffer_length: buffer's length
  * @retval 1: pbuffer1 identical to pbuffer2
  *         0: pbuffer1 differs from pbuffer2
  */
uint8_t buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t buffer_length)
{
  while(buffer_length--)
  {
    if(*pbuffer1 != *pbuffer2)
    {
      return 0;
    }
    pbuffer1++;
    pbuffer2++;
  }
  return 1;
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
  usart_spi_configuration();

  /* wait until end of transmission from usart2 to spi1 */
  while(spi1_rx_counter < usart2_tx_buffer_size)
  {
    /* usart transmit data */
    while(usart_flag_get(USART2, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART2, usart2_tx_buffer[usart2_tx_counter++]);

    /* wait until spi receive data */
    while(spi_i2s_flag_get(SPI1, SPI_I2S_RDBF_FLAG) == RESET);
    spi1_rx_buffer[spi1_rx_counter++] = spi_i2s_data_receive(SPI1);

    /* wait until usart receive data, and clear dummy byte in data register */
    while(usart_flag_get(USART2, USART_RDBF_FLAG) == RESET);
    usart_data_receive(USART2);
  }

  /* wait until end of transmission from spi1 to usart2 */
  while(usart2_rx_counter < spi1_tx_buffer_size)
  {
    /* spi put transmit data in data register */
    while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI1, spi1_tx_buffer[spi1_tx_counter++]);

    /* usart transmit a dummy byte to generate clock to slave */
    while(usart_flag_get(USART2, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART2, DYMMY_BYTE);

    /* wait until usart receive data */
    while(usart_flag_get(USART2, USART_RDBF_FLAG) == RESET);
    usart2_rx_buffer[usart2_rx_counter++] = usart_data_receive(USART2);
  }

  while(1)
  {
    /* compare data buffer */
    if(buffer_compare(usart2_tx_buffer, spi1_rx_buffer, USART2_TX_BUFFER_SIZE) && \
       buffer_compare(spi1_tx_buffer, usart2_rx_buffer, SPI1_TX_BUFFER_SIZE))
    {
      at32_led_toggle(LED2);
      at32_led_toggle(LED3);
      at32_led_toggle(LED4);
      delay_sec(1);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
