/**
  **************************************************************************
  * @file     at_surf_f437_board_rebled.c
  * @brief    set of firmware functions to manage rgbled.
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

#include "at_surf_f437_board_rgbled.h"

static __IO uint8_t rgb_led_toggle_flag;

/**
  * @brief  initialize rgb led
  * @param  none
  * @retval none
  */
void rgb_led_init(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the tmr and gpio clock */
  crm_periph_clock_enable(RGB_LED_R_GPIO_CRM_CLK, TRUE);
  crm_periph_clock_enable(RGB_LED_G_GPIO_CRM_CLK, TRUE);
  crm_periph_clock_enable(RGB_LED_B_GPIO_CRM_CLK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the rgb led pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = RGB_LED_R_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(RGB_LED_R_GPIO, &gpio_init_struct);

  gpio_init_struct.gpio_pins = RGB_LED_G_PIN;
  gpio_init(RGB_LED_G_GPIO, &gpio_init_struct);

  gpio_init_struct.gpio_pins = RGB_LED_B_PIN;
  gpio_init(RGB_LED_B_GPIO, &gpio_init_struct);
}


/**
  * @brief  set rgb led color,and turn on.
  * @param  color: rgb led corlor
  *         this parameter can be one of the following values:
  *         - RGB_LED_RED
  *         - RGB_LED_GREEN
  *         - RGB_LED_BLUE
  *         - RGB_LED_YELLOE
  *         - RGB_LED_GBLUE
  *         - RGB_LED_PURPLE
  *         - RGB_LED_WHITE
  * @retval flag_status (SET or RESET)
  */
void rgb_led_set(uint16_t color)
{
  RGB_LED_R_GPIO->scr = RGB_LED_R_PIN;
  RGB_LED_G_GPIO->scr = RGB_LED_G_PIN;
  RGB_LED_B_GPIO->scr = RGB_LED_B_PIN;

  RGB_LED_R_GPIO->clr = color & RGB_LED_R_PIN;
  RGB_LED_G_GPIO->clr = color & RGB_LED_G_PIN;
  RGB_LED_B_GPIO->clr = color & RGB_LED_B_PIN;
}

/**
  * @brief  turn off reg led.
  * @param  none
  * @retval none
  */
void rgb_led_off(void)
{
  RGB_LED_R_HIGH();
  RGB_LED_G_HIGH();
  RGB_LED_B_HIGH();
}

/**
  * @brief  reg led toggle.
  * @param  none
  * @retval none
  */
void rgb_led_toggle(uint16_t color)
{
  if(rgb_led_toggle_flag == 1)
  {
    rgb_led_set(color);
    rgb_led_toggle_flag = 0;
  }
  else
  {
    rgb_led_off();
    rgb_led_toggle_flag = 1;
  }
}


