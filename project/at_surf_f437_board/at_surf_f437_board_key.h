/**
  **************************************************************************
  * @file     at_surf_f437_board_key.h
  * @brief    key application libray header file
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
#ifndef __AT_SURF_F437_BOARD_KEY_H
#define __AT_SURF_F437_BOARD_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

/**
  * @brief define key type
  */
typedef enum
{
  KEY_1                                  = 0x00,
  KEY_2                                  = 0x01,
  NO_KEY                                 = 0x02
} key_type;

/**
  * @brief define key io
  */
#define KEY_1_GPIO_CLK                   CRM_GPIOA_PERIPH_CLOCK
#define KEY_1_GPIO_PIN                   GPIO_PINS_0
#define KEY_1_GPIO_PORT                  GPIOA

#define KEY_2_GPIO_CLK                   CRM_GPIOC_PERIPH_CLOCK
#define KEY_2_GPIO_PIN                   GPIO_PINS_13
#define KEY_2_GPIO_PORT                  GPIOC

#define KEY_1_DOWN()                     (gpio_input_data_bit_read(KEY_1_GPIO_PORT, KEY_1_GPIO_PIN) == SET)
#define KEY_2_DOWN()                     (gpio_input_data_bit_read(KEY_2_GPIO_PORT, KEY_2_GPIO_PIN) == SET)

#define KEY_1_RELEASE()                  (gpio_input_data_bit_read(KEY_1_GPIO_PORT, KEY_1_GPIO_PIN) == RESET)
#define KEY_2_RELEASE()                  (gpio_input_data_bit_read(KEY_2_GPIO_PORT, KEY_2_GPIO_PIN) == RESET)

void key_init(void);
key_type key_press(void);

#ifdef __cplusplus
}
#endif

#endif

