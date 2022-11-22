/**
  **************************************************************************
  * @file     at_surf_f437_board_usart.h
  * @brief    header file usart.
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

#ifndef __AT32_SURF_F437_BOARD_USART_H
#define __AT32_SURF_F437_BOARD_USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "at32f435_437.h"


void uart_print_init(uint32_t baudrate);

#define PRINT_USARTx                     USART1
#define PRINT_USART_CLK                  CRM_USART1_PERIPH_CLOCK

#define PRINT_USART_TX_GPIO_CLK          CRM_GPIOA_PERIPH_CLOCK
#define PRINT_USART_TX_GPIO_PIN          GPIO_PINS_9
#define PRINT_USART_TX_GPIO_PINS_SOURCE  GPIO_PINS_SOURCE9
#define PRINT_USART_TX_GPIO_PORT         GPIOA
#define PRINT_USART_TX_GPIO_MUX          GPIO_MUX_7


#ifdef __cplusplus
}
#endif

#endif
