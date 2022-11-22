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

#include "at32f435_437_clock.h"
#include "at_surf_f437_board_lcd.h"
#include "at_surf_f437_board_usart.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "at_surf_f437_board_delay.h"
#include "at_surf_f437_board_touch.h"
#include "lv_tick_custom.h"
#include <stdint.h>
#include <string.h>
#include "lvgl.h"

extern void lv_demo_music(void);

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{

  system_clock_config();
  delay_init();
  uart_print_init(115200);
  printf("init ok\r\n");

  /* for littlevgl gui tick increase */
  tmr7_int_init(287, 999);

  lcd_init(LCD_DISPLAY_VERTICAL);
  touch_init(TOUCH_SCAN_VERTICAL);
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
  lv_demo_music();
  while(1)
  {
    lv_task_handler();
  }
}


