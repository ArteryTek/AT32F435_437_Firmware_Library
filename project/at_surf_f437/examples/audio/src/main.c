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
#include "at_surf_f437_board_lcd.h"
#include "at_surf_f437_board_player.h"
#include "at_surf_f437_board_delay.h"

FATFS fs;
BYTE work[FF_MAX_SS];

/**
  * @brief  sd card / fatfs file system init
  * @param  none
  * @retval error_status
  */
error_status file_system_init(void)
{
  FRESULT ret;

  /* registers work area to the fatfs module */
  ret = f_mount(&fs, "1:", 1);

  if(ret)
  {
    /* registers work area fail*/
    if(ret == FR_NO_FILESYSTEM)
    {
      /* create fatfs file system */
      ret = f_mkfs("1:", 0, work, sizeof(work));

      if(ret)
      {
        /* create fatfs file system fail*/
        return ERROR;
      }

      /* unregisters work area */
      ret = f_mount(NULL, "1:", 1);

      /* registers work area to the fatfs module */
      ret = f_mount(&fs, "1:", 1);

      if(ret)
      {
        /* registers work area fail */
        return ERROR;
      }
    }
    else
    {
      return ERROR;
    }
  }

  return SUCCESS;
}

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

  /* display information */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"Audio Test");

  if(file_system_init() != SUCCESS)
  {
    lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"sd card init error");
    while(1);
  }

  /* initialize audio */
  pca9555_init(PCA_I2C_CLKCTRL_100K);

  /* initialize key */
  key_init();

  /* initialize joystick */
  joystick_init();
  
  /* initialize variable resistor */
  variable_resistor_init();

  /* initialize audio */
  audio_init();

  /* paly music */
  music_play(&audio_info);

  while(1)
  {

  }
}

