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
#include <math.h>
#include "at32f435_437.h"
#include "at32f435_437_clock.h"
#include "at_surf_f437_board_delay.h"
#include "at_surf_f437_board_lcd.h"
#include "at_surf_f437_board_usart.h"




/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  uint16_t step = 0;
  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  while(1)
  {
    /* change color */
    switch(step)
    {
      case 0: lcd_clear(WHITE ); break;
      case 1: lcd_clear(BLUE ); break;
      case 2: lcd_clear(BRED ); break;
      case 3: lcd_clear(GBLUE ); break;
      case 4: lcd_clear(RED ); break;
      case 5: lcd_clear(BRRED ); break;
      case 6: lcd_clear(GREEN ); break;
      case 7: lcd_clear(YELLOW); break;
      default: step = 0; break;
    }
    /* display information */
    lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"TFT LCD Test");
    lcd_string_show(10, 60, 200, 24, 24, (uint8_t *)"2021-01-20");
    step++;
    if(step == 7)
    {
      step = 0;
    }
    delay_ms(1000);

  }
}

