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
#include "at_surf_f437_board_lcd.h"
#include "at_surf_f437_board_calendar.h"

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  uint8_t temp = 0;

  ertc_time_type time;

  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* initialize lcd */
  calendar_init();

  /* display string */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"Calendar Test");

  /* display symbol */
  lcd_string_show(10, 60, 200, 24, 24, (uint8_t *)"    -  -     :  :  ");

  while(1)
  {
    /* get the current time */
    ertc_calendar_get(&time);

    if(temp != time.sec)
    {
      temp = time.sec;

      /* display year */
      lcd_num_show(10, 60, 200, 24, 24, time.year + 2000, 4);

      /* display month */
      lcd_num_show(70, 60, 200, 24, 24, time.month, 2);

      /* display day */
      lcd_num_show(106, 60, 200, 24, 24, time.day, 2);

      /* display hour */
      lcd_num_show(142, 60, 200, 24, 24, time.hour, 2);

      /* display minute */
      lcd_num_show(178, 60, 200, 24, 24, time.min, 2);

      /* display second */
      lcd_num_show(214, 60, 200, 24, 24, time.sec, 2);
    }
  }
}

