/**
  **************************************************************************
  * @file     at_surf_f437_board_low_power_mode.c
  * @brief    the driver library of the low power mode
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

#include "at_surf_f437_board_low_power_mode.h"
#include "at_surf_f437_board_delay.h"

/**
  * @brief  wakeup pin init.
  * @param  none.
  * @retval none.
  */
void wakeup_pin_init(void)
{
  gpio_init_type gpio_init_struct;
  exint_init_type exint_init_struct;

  /* enable the wakeup pin gpio clock */
  crm_periph_clock_enable(WAKEUP_GPIO_CLK, TRUE);
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure wakeup pin as input with pull-down */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_init_struct.gpio_pins = WAKEUP_GPIO_PIN;
  gpio_init(WAKEUP_GPIO_PORT, &gpio_init_struct);

  /* config exint line */
  scfg_exint_line_config(WAKEUP_EXINT_PORT_SOURCE, WAKEUP_EXINT_PINS_SOURCE);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_select = WAKEUP_EXINT_LINE;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  /* enable wakeup pin interrupt */
  nvic_irq_enable(WAKEUP_EXINT_IRQn, 1, 0);
}
