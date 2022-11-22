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
#include "at_surf_f437_board_key.h"
#include "at_surf_f437_board_lcd.h"

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  key_type key_value;

  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* initialize key */
  key_init();

  /* display information */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"Key Test");
  lcd_string_show(10, 60, 280, 24, 24, (uint8_t *)"Press any key to begin");

  while(1)
  {
    /* key detection */
    key_value = key_press();

    switch(key_value)
    {
      /* key 2 down */
      case KEY_1:
        lcd_string_show(10, 100, 310, 24, 24, (uint8_t *)"key value: key 1");
        break;

      /* key 3 down */
      case KEY_2:
        lcd_string_show(10, 100, 310, 24, 24, (uint8_t *)"key value: key 2");
        break;

      default:
        break;
    }
  }
}

