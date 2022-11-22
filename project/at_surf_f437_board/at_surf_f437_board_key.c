/**
  **************************************************************************
  * @file     at_surf_f437_board_key.c
  * @brief    the driver library of the key
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

#include "at_surf_f437_board_key.h"
#include "at_surf_f437_board_delay.h"

/**
  * @brief  key init.
  * @param  none.
  * @retval none.
  */
void key_init(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the key clock */
  crm_periph_clock_enable(KEY_1_GPIO_CLK, TRUE);
  crm_periph_clock_enable(KEY_2_GPIO_CLK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure key pin as input with pull-down */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;

  gpio_init_struct.gpio_pins = KEY_1_GPIO_PIN;
  gpio_init(KEY_1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = KEY_2_GPIO_PIN;
  gpio_init(KEY_2_GPIO_PORT, &gpio_init_struct);
}

/**
  * @brief  returns which key have press down
  * @param  none
  * @retval the key have press down
  */
key_type key_press(void)
{
  static uint8_t pressed = 0;

  /* get key state in at_start board */
  if((pressed == 0) && (KEY_1_DOWN() || KEY_2_DOWN()))
  {
    delay_ms(10);

    if(KEY_1_DOWN())
    {
      pressed = 1;
      return KEY_1;
    }
    else if(KEY_2_DOWN())
    {
      pressed = 1;
      return KEY_2;
    }
  }
  else if(KEY_1_RELEASE() && KEY_2_RELEASE())
  {
    pressed = 0;
  }

  return NO_KEY;
}

