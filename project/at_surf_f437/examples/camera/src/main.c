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
#include "at_surf_f437_board_ov2640.h"
#include "at_surf_f437_board_ov5640.h"

uint16_t camera_id = 0;
uint16_t camera_identify(void);

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
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"DVP Test");

  delay_ms(500);

  /* identify camera */
  camera_id = camera_identify();
  while((camera_id != OV5640_ID) && (camera_id != OV2640_PID))
  {
    lcd_string_show(30, 100, 240, 16, 16, (uint8_t *)"camer err");

    delay_ms(400);
  }
  
  if (camera_id == OV5640_ID)
  {
    /* initialize ov5640 */
    while(ov5640_init() != SUCCESS)
    {
      lcd_string_show(10, 140, 200, 24, 24, (uint8_t *)"ov5640 init err");

      delay_ms(400);
    }
  }
  else
  {
    /* initialize ov2640 */
    while(ov2640_init() != SUCCESS)
    {
      lcd_string_show(10, 140, 200, 24, 24, (uint8_t *)"ov2640 init err");

      delay_ms(400);
    }
  }

  /* display information */
  lcd_string_show(10, 60, 300, 24, 24, (uint8_t *)"camera init ok");

  lcd_string_show(10, 100, 300, 24, 24, (uint8_t *)"dvp initializing...");

  /* start image capture and display on lcd */
  if (camera_id == OV5640_ID)
  {
    ov5640_capture();
  }
  else
  {
    ov2640_capture();
  }

  while(1)
  {

  }
}

/**
  * @brief  camera identify
  * @param  none
  * @retval camera id
  */
uint16_t camera_identify(void)
{
  uint8_t temp = 0;
  uint16_t reg;
  gpio_init_type gpio_initure;

  crm_periph_clock_enable(OV_RST_GPIO_CLK, TRUE);

  gpio_default_para_init(&gpio_initure);
  gpio_initure.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_initure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initure.gpio_pull = GPIO_PULL_UP;
  gpio_initure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

  gpio_initure.gpio_pins = OV_RST_GPIO_PIN;
  gpio_init(OV_RST_GPIO_PORT, &gpio_initure);

  OV5640_RST_LOW;
  delay_ms(100);
  OV5640_RST_HIGH;
  delay_ms(20);
  ov5640_i2c_init();
  delay_ms(5);
  ov5640_reg_read(OV5640_CHIPIDH, &temp);
  reg  = (uint16_t)temp << 8;
  ov5640_reg_read(OV5640_CHIPIDL, &temp);
  reg |= temp;
  
  if(reg == OV5640_ID)
  {
    return reg;
  }
  
  OV2640_RST_LOW();    //POWER ON;
  delay_ms(10);
  OV2640_PWRON_LOW();
  delay_ms(10);
  OV2640_RST_HIGH();
  delay_ms(10);
  ov2640_i2c_init();
  ov2640_reg_write(OV2640_DSP_RA_DLMT, 0x01);
   ov2640_reg_write(OV2640_SENSOR_COM7, 0x80);
  delay_ms(50); 
  ov2640_reg_read(OV2640_SENSOR_MIDH, &temp);
  reg  = (uint16_t)temp << 8;
  ov2640_reg_read(OV2640_SENSOR_MIDL, &temp);
  reg |= temp;
  if(reg!=OV2640_MID)
  {
    return 0;
  }
  ov2640_reg_read(OV2640_SENSOR_PIDH, &temp);
  reg  = (uint16_t)temp << 8;
  ov2640_reg_read(OV2640_SENSOR_PIDL, &temp);  
  reg |= temp;
  if(reg==OV2640_PID)
  {
    return reg;
  }
  
  return 0;
}

