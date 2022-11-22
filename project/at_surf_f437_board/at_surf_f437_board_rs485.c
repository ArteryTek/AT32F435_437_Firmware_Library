/**
  **************************************************************************
  * @file     at_surf_f437_board_rs485.c
  * @brief    the driver library of the rs485
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

#include "at_surf_f437_board_rs485.h"

volatile uint8_t rs485_tx_counter = 0x00;
volatile uint8_t rs485_rx_counter = 0x00;
volatile uint8_t rs485_tx_number = 0x00;
volatile uint8_t rs485_rx_number = 0x00;

uint8_t  rs485_rx_buffer[RS485_RX_BUFFER_SIZE];
uint8_t* rs485_tx_buffer;

/**
  * @brief  send data.
  * @param  pdata: data buffer.
  * @param  num: data size.
  * @retval none.
  */
void rs485_data_send(uint8_t* pdata, uint16_t num)
{
  /* initializes transmit conuter */
  rs485_tx_counter = 0;
  rs485_tx_number = num;

  /* initializes data bufer */
  rs485_tx_buffer = pdata;

  /* enable the rs485 transmit interrupt */
  usart_interrupt_enable(RS485_USARTx, USART_TDBE_INT, TRUE);
}

/**
  * @brief  receive data.
  * @param  pdata: data buffer.
  * @param  num: data size.
  * @retval none.
  */
void rs485_data_receive(uint8_t* pdata, uint16_t num)
{
  uint16_t i;

  /* copy data */
  for(i = 0; i < num; i++)
  {
    pdata[i] = rs485_rx_buffer[i];
  }

  /* initializes receive conuter */
  rs485_rx_number = 0;
}

/**
  * @brief  initializes rs485.
  * @param  none
  * @retval none
  */
void rs485_init(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the usart and gpio clock */
  crm_periph_clock_enable(RS485_USART_CLK, TRUE);
  crm_periph_clock_enable(RS485_TX_GPIO_CLK, TRUE);
  crm_periph_clock_enable(RS485_RX_GPIO_CLK, TRUE);
  crm_periph_clock_enable(RS485_DE_GPIO_CLK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

  /* configure the usart tx, rx pin */
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;

  gpio_init_struct.gpio_pins = RS485_DE_GPIO_PIN;
  gpio_init(RS485_DE_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = RS485_TX_GPIO_PIN;
  gpio_init(RS485_TX_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = RS485_RX_GPIO_PIN;
  gpio_init(RS485_RX_GPIO_PORT, &gpio_init_struct);

  gpio_pin_mux_config(RS485_TX_GPIO_PORT, RS485_TX_GPIO_PINS_SOURCE, RS485_TX_GPIO_MUX);
  gpio_pin_mux_config(RS485_RX_GPIO_PORT, RS485_RX_GPIO_PINS_SOURCE, RS485_RX_GPIO_MUX);
  gpio_pin_mux_config(RS485_DE_GPIO_PORT, RS485_DE_GPIO_PINS_SOURCE, RS485_DE_GPIO_MUX);

  /* config usart nvic interrupt */
  nvic_irq_enable(RS485_USART_IRQn, 0, 0);

  /* configure usart param */
  usart_rs485_delay_time_config(RS485_USARTx, 2, 2);
  usart_de_polarity_set(RS485_USARTx, USART_DE_POLARITY_HIGH);
  usart_rs485_mode_enable(RS485_USARTx, TRUE);
  usart_init(RS485_USARTx, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(RS485_USARTx, TRUE);
  usart_receiver_enable(RS485_USARTx, TRUE);

  /* enable usart rdbf interrupt */
  usart_interrupt_enable(RS485_USARTx, USART_RDBF_INT, TRUE);
  usart_enable(RS485_USARTx, TRUE);

  /* enable usart idle interrupt */
  usart_interrupt_enable(RS485_USARTx, USART_IDLE_INT, TRUE);
}

