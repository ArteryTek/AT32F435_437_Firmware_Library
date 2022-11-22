/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
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

#include "at32f435_437.h"
#include "at32f435_437_clock.h"
#include "at_surf_f437_board_delay.h"
#include "at_surf_f437_board_low_power_mode.h"
#include "at_surf_f437_board_lcd.h"

#define TEST_SLEEP_MODE
//#define TEST_DEEPSLEEP_MODE

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* initialize wakeup pin */
  wakeup_pin_init();

  /* enable pwc clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* display information */
  lcd_string_show(5, 20, 310, 24, 24, (uint8_t *)"Low power mode test");

  /* delay 1 s */
  delay_ms(1000);

#if defined TEST_SLEEP_MODE

  /* display information */
  lcd_string_show(5, 60, 310, 24, 24, (uint8_t *)"Enter sleep mode");
  lcd_string_show(5, 90, 310, 24, 24, (uint8_t *)"Prese key 1 to wakeup");

  /* enter sleep mode */
  pwc_sleep_mode_enter(PWC_SLEEP_ENTER_WFI);

  /* wakeup from sleep mode */
  lcd_string_show(5, 140, 310, 24, 24, (uint8_t *)"Wakeup from sleep mode");

#elif defined TEST_DEEPSLEEP_MODE

  /* display information */
  lcd_string_show(5, 60, 310, 24, 24, (uint8_t *)"Enter deepsleep mode");
  lcd_string_show(5, 90, 310, 24, 24, (uint8_t *)"Prese key 1 to wakeup");

  /* congfig the voltage regulator mode */
  pwc_voltage_regulate_set(PWC_REGULATOR_LOW_POWER);

  /* enter deepsleep mode */
  pwc_deep_sleep_mode_enter(PWC_DEEP_SLEEP_ENTER_WFI);

  /* initial system clock */
  system_clock_config();

  /* wakeup from deepsleep mode */
  lcd_string_show(5, 140, 310, 24, 24, (uint8_t *)"Wakeup from deepsleep mode");

#endif

  while(1)
  {

  }
}

