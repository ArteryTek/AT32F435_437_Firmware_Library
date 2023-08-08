/**
  **************************************************************************
  * @file     at_surf_f437_board_rgbled.h
  * @brief    header file for rgbled.
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

#ifndef __AT32_SURF_F437_BOARD_ERGLED_H
#define __AT32_SURF_F437_BOARD_ERGLED_H

#ifdef __cplusplus
extern "C" {
#endif
#include "at32f435_437.h"

#define RGB_LED_R_PIN                    GPIO_PINS_10
#define RGB_LED_R_GPIO                   GPIOB
#define RGB_LED_R_GPIO_CRM_CLK           CRM_GPIOB_PERIPH_CLOCK

#define RGB_LED_G_PIN                    GPIO_PINS_13
#define RGB_LED_G_GPIO                   GPIOD
#define RGB_LED_G_GPIO_CRM_CLK           CRM_GPIOD_PERIPH_CLOCK

#define RGB_LED_B_PIN                    GPIO_PINS_5
#define RGB_LED_B_GPIO                   GPIOB
#define RGB_LED_B_GPIO_CRM_CLK           CRM_GPIOB_PERIPH_CLOCK

#define RGB_LED_R_LOW()                   gpio_bits_reset(RGB_LED_R_GPIO, RGB_LED_R_PIN)
#define RGB_LED_R_HIGH()                 gpio_bits_set(RGB_LED_R_GPIO, RGB_LED_R_PIN)

#define RGB_LED_G_LOW()                   gpio_bits_reset(RGB_LED_G_GPIO, RGB_LED_G_PIN)
#define RGB_LED_G_HIGH()                 gpio_bits_set(RGB_LED_G_GPIO, RGB_LED_G_PIN)

#define RGB_LED_B_LOW()                   gpio_bits_reset(RGB_LED_B_GPIO, RGB_LED_B_PIN)
#define RGB_LED_B_HIGH()                 gpio_bits_set(RGB_LED_B_GPIO, RGB_LED_B_PIN)

/**
  * @brief rgb led color type
  */
typedef enum
{
  RGB_LED_RED                            = RGB_LED_R_PIN,
  RGB_LED_GREEN                          = RGB_LED_G_PIN,
  RGB_LED_BLUE                           = RGB_LED_B_PIN,
  RGB_LED_YELLOE                         = (RGB_LED_R_PIN | RGB_LED_G_PIN),
  RGB_LED_GBLUE                          = (RGB_LED_G_PIN | RGB_LED_B_PIN),
  RGB_LED_PURPLE                         = (RGB_LED_R_PIN | RGB_LED_B_PIN),
  RGB_LED_WHITE                          = (RGB_LED_R_PIN | RGB_LED_G_PIN | RGB_LED_B_PIN)
} rgb_led_color_type;

void rgb_led_init(void);
void rgb_led_set(uint16_t color);
void rgb_led_off(void);
void rgb_led_toggle(uint16_t color);


#ifdef __cplusplus
}
#endif

#endif
