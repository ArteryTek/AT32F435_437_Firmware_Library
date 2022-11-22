/**
  **************************************************************************
  * @file     at_surf_f437_board_can.h
  * @brief    can application libray header file
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
#ifndef __AT_SURF_F437_BOARD_CAN_H
#define __AT_SURF_F437_BOARD_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"


#define CAN1_TX_GPIO_CLK                 CRM_GPIOB_PERIPH_CLOCK
#define CAN1_TX_GPIO_PIN                 GPIO_PINS_9
#define CAN1_TX_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE9
#define CAN1_TX_GPIO_PORT                GPIOB
#define CAN1_TX_GPIO_MUX                 GPIO_MUX_9

#define CAN1_RX_GPIO_CLK                 CRM_GPIOB_PERIPH_CLOCK
#define CAN1_RX_GPIO_PIN                 GPIO_PINS_8
#define CAN1_RX_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE8
#define CAN1_RX_GPIO_PORT                GPIOB
#define CAN1_RX_GPIO_MUX                 GPIO_MUX_9

#define CAN2_TX_GPIO_CLK                 CRM_GPIOB_PERIPH_CLOCK
#define CAN2_TX_GPIO_PIN                 GPIO_PINS_13
#define CAN2_TX_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE13
#define CAN2_TX_GPIO_PORT                GPIOB
#define CAN2_TX_GPIO_MUX                 GPIO_MUX_9

#define CAN2_RX_GPIO_CLK                 CRM_GPIOB_PERIPH_CLOCK
#define CAN2_RX_GPIO_PIN                 GPIO_PINS_12
#define CAN2_RX_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE12
#define CAN2_RX_GPIO_PORT                GPIOB
#define CAN2_RX_GPIO_MUX                 GPIO_MUX_9

extern can_rx_message_type can1_rx_message;
extern can_rx_message_type can2_rx_message;
extern can_tx_message_type can1_tx_message;
extern can_tx_message_type can2_tx_message;

extern uint8_t can1_rx_flag;
extern uint8_t can2_rx_flag;

void can_init(can_type *can_x);
void can_data_init(can_type *can_x);
void can_transmit_data(can_type *CAN_x, can_tx_message_type tx_message_struct);

#ifdef __cplusplus
}
#endif

#endif

