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
#include "at_surf_f437_board_touch.h"
#include "at_surf_f437_board_lcd.h"

uint16_t x_dot[5], y_dot[5];

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

  /* initialize touch */
  touch_init(TOUCH_SCAN_VERTICAL);

  /* display information */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"Touch Test");

  while(1)
  {
    lcd_string_show(10, 60, 200, 24, 24, (uint8_t *)"X:");

    lcd_string_show(10, 90, 200, 24, 24, (uint8_t *)"Y:");

    /* read coordinate */
    if(touch_read_xy(x_dot, y_dot) == SUCCESS)
    {
      /* display x coordinate */
      lcd_num_show(40, 60, 200, 24, 24, x_dot[0], 3);

      /* display y coordinate */
      lcd_num_show(40, 90, 200, 24, 24, y_dot[0], 3);
    }
  }
}
