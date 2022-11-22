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

#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"
#include "xmc_lcd.h"
#include "touch.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_XMC_lcd_touch_16bit XMC_lcd_touch_16bit
  * @{
  */

uint16_t point_color;

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  touch_struct = &touch_dev_struct;
  lcd_struct = &lcd_dev_struct;
  system_clock_config();
  at32_board_init();

  lcd_struct->lcd_init();
  point_color = GBLUE;
  lcd_struct->lcd_clear(point_color);

  touch_struct->init();

  while(1)
  {
    touch_struct->touch_scan();
  }
}

/**
  * @}
  */

/**
  * @}
  */

