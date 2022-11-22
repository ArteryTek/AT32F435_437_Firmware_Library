/**
  **************************************************************************
  * @file     at_surf_f437_board_rs485.h
  * @brief    rs485 application libray header file
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT_SURF_F437_BOARD_RS485_H
#define __AT_SURF_F437_BOARD_RS485_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "i2c_application.h"

#define RS485_USARTx                     USART3
#define RS485_USART_CLK                  CRM_USART3_PERIPH_CLOCK

#define RS485_TX_GPIO_CLK                CRM_GPIOC_PERIPH_CLOCK
#define RS485_TX_GPIO_PIN                GPIO_PINS_10
#define RS485_TX_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE10
#define RS485_TX_GPIO_PORT               GPIOC
#define RS485_TX_GPIO_MUX                GPIO_MUX_7

#define RS485_RX_GPIO_CLK                CRM_GPIOC_PERIPH_CLOCK
#define RS485_RX_GPIO_PIN                GPIO_PINS_11
#define RS485_RX_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE11
#define RS485_RX_GPIO_PORT               GPIOC
#define RS485_RX_GPIO_MUX                GPIO_MUX_7

#define RS485_DE_GPIO_CLK                CRM_GPIOD_PERIPH_CLOCK
#define RS485_DE_GPIO_PIN                GPIO_PINS_12
#define RS485_DE_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE12
#define RS485_DE_GPIO_PORT               GPIOD
#define RS485_DE_GPIO_MUX                GPIO_MUX_7

#define RS485_USART_IRQn                 USART3_IRQn
#define RS485_USART_IRQHandler           USART3_IRQHandler

#define RS485_RX_BUFFER_SIZE             200

extern volatile uint8_t rs485_tx_counter;
extern volatile uint8_t rs485_rx_counter;
extern volatile uint8_t rs485_tx_number;
extern volatile uint8_t rs485_rx_number;

extern uint8_t  rs485_rx_buffer[RS485_RX_BUFFER_SIZE];
extern uint8_t* rs485_tx_buffer;

/**
  * @brief rs485 exported functions
  */
void rs485_init(void);
void rs485_data_receive(uint8_t* pdata, uint16_t num);
void rs485_data_send(uint8_t* pdata, uint16_t num);


#ifdef __cplusplus
}
#endif

#endif

