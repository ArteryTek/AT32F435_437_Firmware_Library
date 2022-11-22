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
#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"
#include "xmc_lcd.h"
#include "ov5640.h"
#include "dvp.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_DVP_ov5640_capture DVP_ov5640_capture
  * @{
  */

uint16_t point_color;
void ov5640_capture(void);

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  lcd_struct = &lcd_dev_struct;
  system_clock_config();

  at32_board_init();
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  lcd_struct->lcd_init();
  point_color = WHITE;
  lcd_struct->lcd_clear(point_color);

  lcd_string_show(30, 50, 200, 16, 16, (uint8_t *)"Artery AT32F435/F437");
  lcd_string_show(30, 70, 200, 16, 16, (uint8_t *)"OV5640 TEST");
  lcd_string_show(30, 90, 200, 16, 16, (uint8_t *)"2021/6/21");

  delay_ms(500);
  while(ov5640_init() != SUCCESS)
  {
    lcd_string_show(30, 130, 240, 16, 16, (uint8_t *)"OV5640 ERR");
    delay_ms(200);
    lcd_fill(30, 130, 239, 170, WHITE);
    delay_ms(200);
    at32_led_toggle(LED2);
  }

  lcd_string_show(30, 130, 200, 16, 16, (uint8_t *)"OV5640 OK");

  delay_ms(500);
  ov5640_capture();
  while(1)
  {

  }
}

void ov5640_capture(void)
{
  uint8_t reg_val = 0;
  lcd_clear(WHITE);

  lcd_string_show(30, 50, 200, 16, 16, (uint8_t *)"Artery AT32F435/F437");
  lcd_string_show(30, 70, 200, 16, 16, (uint8_t *)"OV5640 RGB565 Mode");
  lcd_string_show(30, 90, 200, 16, 16, (uint8_t *)"DVP Initializing...");

  ov5640_rgb565_mode();
  ov5640_focus_init();
  ov5640_light_mode(0);
  ov5640_color_saturation(3);
  ov5640_brightness(4);
  ov5640_contrast(3);
  ov5640_sharpness(33);
  ov5640_focus_constant();

  dvp_config();

  dvp_dma_init((uint32_t)XMC_LCD_DATA, 0, 38400);

  delay_ms(100);
  ov5640_outsize_set(0, 0, LCD_W, LCD_H);
  ov5640_reg_read(0x4730, &reg_val);
  dvp_start();
}

/**
  * @}
  */

/**
  * @}
  */
