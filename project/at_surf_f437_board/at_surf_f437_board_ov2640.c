/**
  **************************************************************************
  * @file     at_surf_f437_board_ov2640.c
  * @brief    ov2640 program
  **************************************************************************
  *                       Copyright notice  &  Disclaimer
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

#include "at_surf_f437_board_ov2640.h"
#include "at_surf_f437_board_ov2640cfg.h"
#include "at_surf_f437_board_delay.h"
#include "at_surf_f437_board_lcd.h"
#include "i2c_application.h"

extern uint16_t camera_id;
i2c_handle_type hi2c_ov;

/**
  * @brief  write register to ov2640
  * @param  reg: register write to ov2640
  * @param  data: data write to ov2640
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov2640_reg_write(uint8_t reg, uint8_t data)
{
  uint8_t temp[2];
  temp[0] = reg;
  temp[1] = data;

  if(i2c_master_transmit(&hi2c_ov, (uint16_t)OV2640_ADDR, (uint8_t*)temp, 2, 1000000)  != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  read register from ov2640
  * @param  reg: register read from ov2640
  * @param  data: data read from ov2640
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov2640_reg_read(uint8_t reg, uint8_t *data)
{
  uint8_t temp[1];
  temp[0] = reg;

  if(i2c_master_transmit(&hi2c_ov, (uint16_t)OV2640_ADDR, (uint8_t*)temp, 1, 10000)  != I2C_OK)
  {
    return ERROR;
  }

  if(i2c_master_receive(&hi2c_ov, (uint16_t)OV2640_ADDR, data, 1, 10000)  != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  ov2640 jpeg mode configure
  * @param  none
  * @retval none
  */
void ov2640_jpeg_mode(void)
{
  uint16_t i = 0;

  for(i = 0; i < (sizeof(ov2640_yuv422_reg_tbl) / 2); i++)
  {
    ov2640_reg_write(ov2640_yuv422_reg_tbl[i][0], ov2640_yuv422_reg_tbl[i][1]);
  }

  for(i = 0; i < (sizeof(ov2640_jpeg_reg_tbl) / 2); i++)
  {
    ov2640_reg_write(ov2640_jpeg_reg_tbl[i][0], ov2640_jpeg_reg_tbl[i][1]);
  }
}

/**
  * @brief  ov2640 rgb565 mode configure
  * @param  none
  * @retval none
  */
void ov2640_rgb565_mode(void)
{
  uint16_t i = 0;

  for(i = 0; i < (sizeof(ov2640_rgb565_reg_tbl) / 2); i++)
  {
    ov2640_reg_write(ov2640_rgb565_reg_tbl[i][0], ov2640_rgb565_reg_tbl[i][1]);
  }
}

/**
  * @brief  ov2640 exposure configure
  * @param  exposure: exposure value
  * @retval none
  */
void ov2640_auto_exposure(uint8_t level)
{
  uint8_t i;

  uint8_t *p = (uint8_t*)OV2640_AUTOEXPOSURE_LEVEL[level];

  for(i = 0; i < 4; i++)
  {
    ov2640_reg_write(p[i*2], p[i*2+1]);
  }
}

/**
  * @brief  this function is draw point to lcd
  * @param  mode : the light mode.
  *         0:auto
  *         1:sunny
  *         2:office
  *         3:cloudy
  *         4:home
  * @retval none
  */
void ov2640_light_mode(uint8_t mode)
{
  uint8_t regccval = 0x5E;  // Sunny
  uint8_t regcdval = 0x41;
  uint8_t regceval = 0x54;

  switch(mode)
  {
    case 0: // auto
      ov2640_reg_write(0xFF, 0x00);
      ov2640_reg_write(0xC7, 0x10);  // AWB ON
      return;
    case 2: // cloudy
      regccval = 0x65;
      regcdval = 0x41;
      regceval = 0x4F;
      break;
    case 3: // office
      regccval = 0x52;
      regcdval = 0x41;
      regceval = 0x66;
      break;
    case 4: // home
      regccval = 0x42;
      regcdval = 0x3F;
      regceval = 0x71;
      break;
  }

  ov2640_reg_write(0xFF, 0x00);
  ov2640_reg_write(0xC7, 0x40);    // AWB OFF
  ov2640_reg_write(0xCC, regccval);
  ov2640_reg_write(0xCD, regcdval);
  ov2640_reg_write(0xCE, regceval);
}

/**
  * @brief  ov2640 color saturation configure
  * @param  sat: color saturation value
  * @retval none
  */
void ov2640_color_saturation(uint8_t sat)
{
  uint8_t reg7dval = ((sat+2) << 4) | 0x08;
  ov2640_reg_write(0xFF, 0x00);
  ov2640_reg_write(0x7C, 0x00);
  ov2640_reg_write(0x7D, 0x02);
  ov2640_reg_write(0x7C, 0x03);
  ov2640_reg_write(0x7D, reg7dval);
  ov2640_reg_write(0x7D, reg7dval);
}

/**
  * @brief  ov2640 brightness configure
  * @param  bright: brightness value
  * @retval none
  */
void ov2640_brightness(uint8_t bright)
{
  ov2640_reg_write(0xff, 0x00);
  ov2640_reg_write(0x7c, 0x00);
  ov2640_reg_write(0x7d, 0x04);
  ov2640_reg_write(0x7c, 0x09);
  ov2640_reg_write(0x7d, bright << 4);
  ov2640_reg_write(0x7d, 0x00);
}

/**
  * @brief  ov2640 contrast configure
  * @param  contrast: contrast value
  * @retval none
  */
void ov2640_contrast(uint8_t contrast)
{
  uint8_t reg7d0val = 0x20;
  uint8_t reg7d1val = 0x20;

  switch(contrast)
  {
    case 0: // -2
      reg7d0val = 0x18;
      reg7d1val = 0x34;
      break;
    case 1: // -1
      reg7d0val = 0x1C;
      reg7d1val = 0x2A;
      break;
    case 3: // 1
      reg7d0val = 0x24;
      reg7d1val = 0x16;
      break;
    case 4: // 2
      reg7d0val = 0x28;
      reg7d1val = 0x0C;
      break;
  }

  ov2640_reg_write(0xff, 0x00);
  ov2640_reg_write(0x7c, 0x00);
  ov2640_reg_write(0x7d, 0x04);
  ov2640_reg_write(0x7c, 0x07);
  ov2640_reg_write(0x7d, 0x20);
  ov2640_reg_write(0x7d, reg7d0val);
  ov2640_reg_write(0x7d, reg7d1val);
  ov2640_reg_write(0x7d, 0x06);
}

/**
  * @brief  ov2640 special effects configure
  * @param  eft: special effects value
  * @retval none
  */
void ov2640_special_effects(uint8_t eft)
{
  uint8_t reg7d0val = 0x00;
  uint8_t reg7d1val = 0x80;
  uint8_t reg7d2val = 0x80;

  switch(eft)
  {
    case 1: // negative film
      reg7d0val = 0x40;
      break;
    case 2: // black and white
      reg7d0val = 0x18;
      break;
    case 3: // red
      reg7d0val = 0x18;
      reg7d1val = 0x40;
      reg7d2val = 0xC0;
      break;
    case 4: // green
      reg7d0val = 0x18;
      reg7d1val = 0x40;
      reg7d2val = 0x40;
      break;
    case 5: // blue
      reg7d0val = 0x18;
      reg7d1val = 0xA0;
      reg7d2val = 0x40;
      break;
    case 6: // retro
      reg7d0val = 0x18;
      reg7d1val = 0x40;
      reg7d2val = 0xA6;
      break;
  }

  ov2640_reg_write(0xff, 0x00);
  ov2640_reg_write(0x7c, 0x00);
  ov2640_reg_write(0x7d, reg7d0val);
  ov2640_reg_write(0x7c, 0x05);
  ov2640_reg_write(0x7d, reg7d1val);
  ov2640_reg_write(0x7d, reg7d2val);
}

void ov2640_color_bar(uint8_t sw)
{
  uint8_t reg;

  ov2640_reg_write(0xFF, 0x01);
  ov2640_reg_read(0x12, & reg);

  reg &= ~(1 << 1);
  if(sw)reg|= 1 << 1;

  ov2640_reg_write(0x12, reg);
}

void ov2640_window_set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
  uint16_t endx;
  uint16_t endy;
  uint8_t temp;

  endx = sx+width / 2;    // V*2
  endy = sy+height / 2;

  ov2640_reg_write(0xFF, 0x01);
  ov2640_reg_read(0x03, & temp);
  temp &= 0xF0;
  temp|= ((endy & 0x03) << 2)|(sy & 0x03);

  ov2640_reg_write(0x03, temp);
  ov2640_reg_write(0x19, sy >> 2);
  ov2640_reg_write(0x1A, endy >> 2);

  ov2640_reg_read(0x32, & temp);
  temp &= 0xC0;
  temp|= ((endx & 0x07) << 3)|(sx & 0x07);

  ov2640_reg_write(0x32, temp);
  ov2640_reg_write(0x17, sx >> 3);
  ov2640_reg_write(0x18, endx >> 3);
}

/**
  * @brief  ov2640 output size set
  * @param  offx: image offset x
  * @param  offy: image offset y
  * @param  width: output width
  * @param  height: output height
  * @retval none
  */
uint8_t ov2640_outsize_set(uint16_t width, uint16_t height)
{
  uint16_t outh;
  uint16_t outw;
  uint8_t temp;

  if(width % 4)return 1;
  if(height % 4)return 2;

  outw = width / 4;
  outh = height / 4;

  ov2640_reg_write(0xFF, 0x00);
  ov2640_reg_write(0xE0, 0x04);
  ov2640_reg_write(0x5A, outw & 0xFF);
  ov2640_reg_write(0x5B, outh & 0xFF);

  temp = (outw >> 8) & 0x03;
  temp|= (outh >> 6) & 0x04;

  ov2640_reg_write(0x5C, temp);
  ov2640_reg_write(0xE0, 0x00);

  return 0;
}

/**
  * @brief  ov2640 image window set
  * @param  offx: image offset x
  * @param  offy: image offset y
  * @param  width: output width
  * @param  height: output height
  * @retval none
  */
uint8_t ov2640_imagewin_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
  uint16_t hsize;
  uint16_t vsize;
  uint8_t temp;

  if(width % 4)return 1;
  if(height % 4)return 2;

  hsize = width / 4;
  vsize = height / 4;

  ov2640_reg_write(0xFF, 0x00);
  ov2640_reg_write(0xE0, 0x04);
  ov2640_reg_write(0x51, hsize & 0xFF);
  ov2640_reg_write(0x52, vsize & 0xFF);
  ov2640_reg_write(0x53, offx & 0xFF);
  ov2640_reg_write(0x54, offy & 0xFF);

  temp = (vsize >> 1) & 0x80;
  temp|= (offy >> 4) & 0x70;
  temp|= (hsize >> 5) & 0x08;
  temp|= (offx >> 8) & 0x07;

  ov2640_reg_write(0x55, temp);
  ov2640_reg_write(0x57, (hsize >> 2) & 0x80);
  ov2640_reg_write(0xE0, 0x00);

  return 0;
}

uint8_t ov2640_imagesize_set(uint16_t width, uint16_t height)
{
  uint8_t temp;

  ov2640_reg_write(0xFF, 0x00);
  ov2640_reg_write(0xE0, 0x04);
  ov2640_reg_write(0xC0, (width) >> 3 & 0xFF);
  ov2640_reg_write(0xC1, (height) >> 3 & 0xFF);

  temp = (width & 0x07) << 3;
  temp|= height & 0x07;
  temp|= (width >> 4) & 0x80;

  ov2640_reg_write(0x8C, temp);
  ov2640_reg_write(0xE0, 0x00);

  return 0;
}

/**
  * @brief  eeprom i2c peripheral initialization.
  * @param  i2c_speed: i2c speed.
  * @retval none.
  */
void ov2640_i2c_lowlevel_init(i2c_handle_type* hi2c, uint32_t i2c_speed)
{
  gpio_init_type gpio_init_structure;

  /* i2c periph clock enable */
  crm_periph_clock_enable(OV_I2C_CLK, TRUE);
  crm_periph_clock_enable(OV_I2C_SCL_GPIO_CLK, TRUE);
  crm_periph_clock_enable(OV_I2C_SDA_GPIO_CLK, TRUE);

  /* gpio configuration */
  gpio_pin_mux_config(OV_I2C_SCL_GPIO_PORT, OV_I2C_SCL_GPIO_PINS_SOURCE, OV_I2C_SCL_GPIO_MUX);

  gpio_pin_mux_config(OV_I2C_SDA_GPIO_PORT, OV_I2C_SDA_GPIO_PINS_SOURCE, OV_I2C_SDA_GPIO_MUX);

  /* configure i2c pins: scl */
  gpio_init_structure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_structure.gpio_mode          = GPIO_MODE_MUX;
  gpio_init_structure.gpio_out_type      = GPIO_OUTPUT_OPEN_DRAIN;
  gpio_init_structure.gpio_pull          = GPIO_PULL_UP;

  gpio_init_structure.gpio_pins          = OV_I2C_SCL_GPIO_PIN;
  gpio_init(OV_I2C_SCL_GPIO_PORT, & gpio_init_structure);

  /* configure i2c pins: sda */
  gpio_init_structure.gpio_pins          = OV_I2C_SDA_GPIO_PIN;
  gpio_init(OV_I2C_SDA_GPIO_PORT, & gpio_init_structure);

  /* config i2c */
  i2c_init(hi2c->i2cx, 0x0F, i2c_speed);
}

/**
  * @brief  ov2640 i2c initialize
  * @param  none
  * @retval none
  */
void ov2640_i2c_init(void)
{
  hi2c_ov.i2cx = OV_I2C;

  /* reset i2c peripheral */
  i2c_reset(hi2c_ov.i2cx);

  /* i2c peripheral initialization */
  ov2640_i2c_lowlevel_init(&hi2c_ov, OV_I2C_CLKCTRL_100K);

  /* i2c peripheral enable */
  i2c_enable(hi2c_ov.i2cx, TRUE);
}

/**
  * @brief  initialize ov2640
  * @param  none
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov2640_init(void)
{
  uint16_t i = 0;
  uint16_t reg;
  uint8_t temp;

  OV2640_RST_LOW();
  delay_ms(10);
  OV2640_PWRON_LOW();
  delay_ms(10);
  OV2640_RST_HIGH();
  delay_ms(10);

  ov2640_i2c_init();

  ov2640_reg_write(OV2640_DSP_RA_DLMT, 0x01);
  ov2640_reg_write(OV2640_SENSOR_COM7, 0x80);

  delay_ms(50);
  ov2640_reg_read(OV2640_SENSOR_MIDH, & temp);
  reg = (uint16_t)temp  <<  8;
  ov2640_reg_read(OV2640_SENSOR_MIDL, & temp);

  reg |= temp;
  if(reg != OV2640_MID)
  {
    return ERROR;
  }

  ov2640_reg_read(OV2640_SENSOR_PIDH, & temp);
  reg = (uint16_t)temp  <<  8;
  ov2640_reg_read(OV2640_SENSOR_PIDL, & temp);
  reg |= temp;
  if(reg != OV2640_PID)
  {
    return ERROR;
  }

  for(i = 0; i < sizeof(ov2640_svga_init_reg_tbl) / 2; i++)
  {
    ov2640_reg_write(ov2640_svga_init_reg_tbl[i][0], ov2640_svga_init_reg_tbl[i][1]);
  }

  return SUCCESS;
}

/**
  * @brief  start image capture and display on lcd
  * @param  none
  * @retval none
  */
void ov2640_capture(void)
{
  delay_ms(100);

  ov2640_rgb565_mode();
  ov2640_light_mode(3);
  ov2640_color_saturation(2);
  ov2640_brightness(4);
  ov2640_contrast(2);

  dvp_config(camera_id);

  dvp_dma_init((uint32_t)XMC_LCD_DATA, 0, 38400);

  delay_ms(100);

  lcd_clear(BLACK);

  ov2640_outsize_set(DVP_LCD_W, DVP_LCD_H);

  dvp_start();
}
