/**
  **************************************************************************
  * @file     at_surf_f437_board_infrared.h
  * @brief    infrared receiver application libray header file
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
#ifndef __AT_SURF_F437_BOARD_infrared_receiver_H
#define __AT_SURF_F437_BOARD_infrared_receiver_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

#define IR_FIFO_SIZE                    10

/**
  * @brief define infrared receiver data type
  */
typedef union
{
  uint32_t value;
  struct
  {
    uint8_t cmd1;
    uint8_t cmd0;
    uint8_t addr1;
    uint8_t addr0;
  } byte;
} infrared_receiver_data_type;

/**
  * @brief define infrared receiver fifo type
  */
typedef struct
{
  volatile uint16_t buf[IR_FIFO_SIZE];
  volatile uint8_t write;
  volatile uint8_t read;
} infrared_receiver_fifo_type;

/**
  * @brief define infrared receiver io
  */
#define IR_GPIO_PORT                    GPIOC
#define IR_GPIO_CLK                     CRM_GPIOC_PERIPH_CLOCK
#define IR_GPIO_PIN                     GPIO_PINS_8
#define IR_GPIO_PINS_SOURCE             GPIO_PINS_SOURCE8
#define IR_GPIO_MUX                     GPIO_MUX_2

#define IR_TMRx                         TMR3
#define IR_TMR_CLK                      CRM_TMR3_PERIPH_CLOCK
#define IR_TMR_CHANNEL                  TMR_SELECT_CHANNEL_3
#define IR_TMR_CHANNEL_INT              TMR_C3_INT
#define IR_TMR_CHANNEL_FLAG             TMR_C3_FLAG
#define IR_TMR_IRQn                     TMR3_GLOBAL_IRQn
#define IR_TMR_IRQHandler               TMR3_GLOBAL_IRQHandler

#define IR_GPIO_READ()                  (gpio_input_data_bit_read(IR_GPIO_PORT, IR_GPIO_PIN) != RESET)


void infrared_receiver_init(void);
error_status infrared_receiver_key_get(uint16_t *val);
void infrared_receiver_data_deal(void);


#ifdef __cplusplus
}
#endif

#endif

