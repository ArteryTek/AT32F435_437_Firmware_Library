/**
  **************************************************************************
  * @file     xmc_lcd.c
  * @brief    xmc_lcd program file
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

#include "xmc_lcd.h"
#include "font.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_DVP_ov5640_capture
  * @{
  */

lcd_dev_type lcd_dev_struct =
{
  0,
  xmc_init,       /*!< function for xmc and gpios init */
  lcd_init,       /*!< function for configures the lcd */
  lcd_setblock,   /*!< lcd function to set block or set window */
  lcd_drawpoint,  /*!< lcd function to drawpoint */
  lcd_clear,      /*!< lcd function to clear */
};

lcd_dev_type *lcd_struct;

/**
  * @brief  configures the xmc and gpios to interface with the lcd.
  *         this function must be called before any write/read operation
  *         on the lcd.
  * @param  none
  * @retval none
  */
void xmc_init(void)
{
  gpio_init_type  gpio_init_struct = {0};
  xmc_norsram_init_type  xmc_norsram_init_struct;
  xmc_norsram_timing_init_type rw_timing_struct, w_timing_struct;

  /* enable the gpio clock */
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
  /* enable the xmc clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);

  /*-- gpio configuration ------------------------------------------------------*/
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE2, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE4, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE5, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE11, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE12, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE5, GPIO_MUX_10);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE15, GPIO_MUX_12);

  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE7, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE15, GPIO_MUX_12);

  /* lcd data lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_4 | GPIO_PINS_5  | GPIO_PINS_11| GPIO_PINS_12;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOC, &gpio_init_struct);


  gpio_init_struct.gpio_pins = GPIO_PINS_5 | GPIO_PINS_8 | GPIO_PINS_9  | GPIO_PINS_10 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins =  GPIO_PINS_7 | GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init(GPIOE, &gpio_init_struct);

  /*-- xmc configuration ------------------------------------------------------*/
  xmc_norsram_default_para_init(&xmc_norsram_init_struct);
  xmc_norsram_init_struct.subbank = XMC_BANK1_NOR_SRAM4;
  xmc_norsram_init_struct.data_addr_multiplex = XMC_DATA_ADDR_MUX_DISABLE;
  xmc_norsram_init_struct.device = XMC_DEVICE_SRAM;
  xmc_norsram_init_struct.bus_type = XMC_BUSTYPE_16_BITS;
  xmc_norsram_init_struct.burst_mode_enable = XMC_BURST_MODE_DISABLE;
  xmc_norsram_init_struct.asynwait_enable = XMC_ASYN_WAIT_DISABLE;
  xmc_norsram_init_struct.wait_signal_lv = XMC_WAIT_SIGNAL_LEVEL_LOW;
  xmc_norsram_init_struct.wrapped_mode_enable = XMC_WRAPPED_MODE_DISABLE;
  xmc_norsram_init_struct.wait_signal_config = XMC_WAIT_SIGNAL_SYN_BEFORE;
  xmc_norsram_init_struct.write_enable = XMC_WRITE_OPERATION_ENABLE;
  xmc_norsram_init_struct.wait_signal_enable = XMC_WAIT_SIGNAL_DISABLE;
  xmc_norsram_init_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  xmc_norsram_init_struct.write_burst_syn = XMC_WRITE_BURST_SYN_DISABLE;
  xmc_nor_sram_init(&xmc_norsram_init_struct);

  /* timing configuration */
  xmc_norsram_timing_default_para_init(&rw_timing_struct, &w_timing_struct);
  rw_timing_struct.subbank = XMC_BANK1_NOR_SRAM4;
  rw_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  rw_timing_struct.addr_setup_time = 0x2;
  rw_timing_struct.addr_hold_time = 0x0;
  rw_timing_struct.data_setup_time = 0x2;
  rw_timing_struct.bus_latency_time = 0x0;
  rw_timing_struct.clk_psc = 0x0;
  rw_timing_struct.data_latency_time = 0x0;
  rw_timing_struct.mode = XMC_ACCESS_MODE_A;
  w_timing_struct.subbank = XMC_BANK1_NOR_SRAM4;
  w_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  w_timing_struct.addr_setup_time = 0x2;
  w_timing_struct.addr_hold_time = 0x0;
  w_timing_struct.data_setup_time = 0x2;
  w_timing_struct.bus_latency_time = 0x0;
  w_timing_struct.clk_psc = 0x0;
  w_timing_struct.data_latency_time = 0x0;
  w_timing_struct.mode = XMC_ACCESS_MODE_A;
  xmc_nor_sram_timing_config(&rw_timing_struct, &w_timing_struct);

  /* enable xmc_bank1_nor_sram4 */
  xmc_nor_sram_enable(XMC_BANK1_NOR_SRAM4, TRUE);

  gpio_init_struct.gpio_pins = GPIO_PINS_0;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOC, &gpio_init_struct);
}

/**
  * @brief  configures the lcd.
  *         this function must be called before any write/read operation
  *         on the lcd.
  * @param  none
  * @retval none
  */
void lcd_init(void)
{
  /* init xmc */
  lcd_struct->xmc_init();

  delay_ms(50);

  /* read id */
  lcd_wr_command(0x0000);
  delay_ms(5);

  lcd_struct->lcd_id = lcd_rd_data();
  lcd_wr_command(0xd3);

  lcd_struct->lcd_id = lcd_rd_data();
  lcd_struct->lcd_id = lcd_rd_data();
  lcd_struct->lcd_id = lcd_rd_data();
  lcd_struct->lcd_id = lcd_rd_data();
  lcd_struct->lcd_id = lcd_struct->lcd_id << 8;
  lcd_struct->lcd_id |= lcd_rd_data();

  lcd_wr_command(0xcf);
  lcd_wr_data(0x00);
  lcd_wr_data(0xc1);
  lcd_wr_data(0x30);
  lcd_wr_command(0xed);
  lcd_wr_data(0x64);
  lcd_wr_data(0x03);
  lcd_wr_data(0x12);
  lcd_wr_data(0x81);
  lcd_wr_command(0xe8);
  lcd_wr_data(0x85);
  lcd_wr_data(0x10);
  lcd_wr_data(0x7a);
  lcd_wr_command(0xcb);
  lcd_wr_data(0x39);
  lcd_wr_data(0x2c);
  lcd_wr_data(0x00);
  lcd_wr_data(0x34);
  lcd_wr_data(0x02);
  lcd_wr_command(0xf7);
  lcd_wr_data(0x20);
  lcd_wr_command(0xea);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_command(0xc0);
  lcd_wr_data(0x1b);
  lcd_wr_command(0xc1);
  lcd_wr_data(0x01);
  lcd_wr_command(0xc5);
  lcd_wr_data(0x30);
  lcd_wr_data(0x30);
  lcd_wr_command(0xc7);
  lcd_wr_data(0xb7);
  lcd_wr_command(0x36);
  lcd_wr_data(0x48);
  lcd_wr_command(0x3a);
  lcd_wr_data(0x55);
  lcd_wr_command(0xb1);
  lcd_wr_data(0x00);
  lcd_wr_data(0x1a);
  lcd_wr_command(0xb6);
  lcd_wr_data(0x0a);
  lcd_wr_data(0xa2);
  lcd_wr_command(0xf2);
  lcd_wr_data(0x00);
  lcd_wr_command(0x26);
  lcd_wr_data(0x01);
  lcd_wr_command(0xe0);
  lcd_wr_data(0x0f);
  lcd_wr_data(0x2a);
  lcd_wr_data(0x28);
  lcd_wr_data(0x08);
  lcd_wr_data(0x0e);
  lcd_wr_data(0x08);
  lcd_wr_data(0x54);
  lcd_wr_data(0xa9);
  lcd_wr_data(0x43);
  lcd_wr_data(0x0a);
  lcd_wr_data(0x0f);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_command(0xe1);
  lcd_wr_data(0x00);
  lcd_wr_data(0x15);
  lcd_wr_data(0x17);
  lcd_wr_data(0x07);
  lcd_wr_data(0x11);
  lcd_wr_data(0x06);
  lcd_wr_data(0x2b);
  lcd_wr_data(0x56);
  lcd_wr_data(0x3c);
  lcd_wr_data(0x05);
  lcd_wr_data(0x10);
  lcd_wr_data(0x0f);
  lcd_wr_data(0x3f);
  lcd_wr_data(0x3f);
  lcd_wr_data(0x0f);
  lcd_wr_command(0x2b);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x01);
  lcd_wr_data(0x3f);
  lcd_wr_command(0x2a);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0xef);
  lcd_wr_command(0x11);
  delay_ms(120);
  lcd_wr_command(0x29);
  lcd_wr_command(0x36);
  lcd_wr_data(0x08);
  lcd_wr_command(0x2a);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0xef >> 8);
  lcd_wr_data(0xef & 0xff);
  lcd_wr_command(0x2b);
  lcd_wr_data(0x00);
  lcd_wr_data(0x00);
  lcd_wr_data(0x13f >> 8);
  lcd_wr_data(0x13f & 0xff);
  lcd_wr_command(0x36);      //setting the display direction
  lcd_wr_data(0x68);         //90 degree
  LCD_BL_HIGH;
}

/**
  * @brief  this function is write command to lcd.
  * @param  command : the command to write.
  * @retval none
  */
void lcd_wr_command(uint16_t command)
{
  *(uint16_t*) XMC_LCD_COMMAND = command;
}

/**
  * @brief  this function is write data to lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_wr_data(uint16_t data)
{
  *(uint16_t*) XMC_LCD_DATA = data;
}

uint16_t lcd_rd_data(void)
{
  uint16_t data;

  data = *(uint16_t*)XMC_LCD_DATA;

  return data;
}

/**
  * @brief  this function is set row&column coordinates for lcd.
  * @param  xstart : row coordinates starting vaule.
  * @param  ystart : column coordinates starting vaule.
  * @param  xend : row coordinates ending vaule.
  * @param  yend : column coordinates ending vaule.
  * @retval none
  */
void lcd_setblock(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend)
{
  /* set row coordinates */
  lcd_wr_command(0x2a);
  lcd_wr_data(xstart >> 8);
  lcd_wr_data(xstart);
  lcd_wr_data(xend >> 8);
  lcd_wr_data(xend);

  /* set column coordinates */
  lcd_wr_command(0x2b);
  lcd_wr_data(ystart >> 8);
  lcd_wr_data(ystart);
  lcd_wr_data(yend >> 8);
  lcd_wr_data(yend);

  /* enable write menory */
  lcd_wr_command(0x2c);
}

/**
  * @brief  this function is write one point to lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_writeonepoint(uint16_t color)
{
  lcd_wr_data(color);
}

/**
  * @brief  this function is draw point to lcd.
  * @param  data : the data to write.
  * @retval None
  */
void lcd_drawpoint(uint16_t x, uint16_t y, uint16_t color)
{
  lcd_struct->lcd_setblock(x, y ,x ,y);

  lcd_writeonepoint(color);
}

/**
  * @brief  this function is clear the lcd.
  * @param  data : the data to write.
  * @retval none
  */
void lcd_clear(uint16_t color)
{
  uint32_t i;

  lcd_struct->lcd_setblock(0, 0, LCD_W, LCD_H);

  for(i = 0; i < 76800; i++)
  {
    lcd_writeonepoint(color);
  }
}

/**
  * @brief  this function is show char to lcd
  * @param  x : row coordinates starting vaule
  * @param  y : column coordinates starting vaule
  * @param  num : number of char
  * @param  size : size of char
  * @param  mode : color
  * @retval none
  */
void lcd_char_show(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode)
{
  uint8_t temp, t1, t;
  uint16_t y0 = y;
  uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
  num = num - ' ';

  for(t = 0; t < csize; t++)
  {
    if(size == 12)
    {
      temp = asc2_1206[num][t];
    }
    else if(size == 16)
    {
      temp = asc2_1608[num][t];
    }
    else if(size == 24)
    {
      temp = asc2_2412[num][t];
    }
    else if(size == 32)
    {
      temp = asc2_3216[num][t];
    }
    else
    {
      return;
    }

    for(t1 = 0; t1 < 8; t1++)
    {
      if(temp & 0x80)
      {
        lcd_drawpoint(x, y, RED);
      }
      else if(mode == 0)
      {
        lcd_drawpoint(x, y, WHITE);
      }

      temp <<= 1;
      y++;

      if(y >= LCD_H)
      {
        return;
      }

      if((y - y0) == size)
      {
        y = y0;
        x++;

        if(x >= LCD_W)
        {
          return;
        }

        break;
      }
    }
  }
}

/**
  * @brief  this function is show string to lcd
  * @param  x : row coordinates starting vaule
  * @param  y : column coordinates starting vaule
  * @param  width : width of string
  * @param  height : height of string
  * @param  p : pointer of string
  * @retval none
  */
void lcd_string_show(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p)
{
  uint8_t x0 = x;
  width += x;
  height += y;

  while((*p <= '~') && (*p >= ' '))
  {
    if(x >= width)
    {
      x = x0;
      y += size;
    }

    if(y >= height)
    {
      break;
    }

    lcd_char_show(x, y, *p, size, 0);
    x += size / 2;
    p++;
  }
}

/**
  * @brief  this function is fill in lcd with concolorous
  * @param  sx : row coordinates starting vaule
  * @param  sy : column coordinates starting vaule
  * @param  ex : row coordinates ending vaule
  * @param  ey : column coordinates ending vaule
  * @param  color : color for fill in lcd
  * @retval none
  */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
  uint32_t i, num;
  num = (ex - sx) * (ey - sy);
  lcd_struct->lcd_setblock(sx, sy, ex, ey);

  for(i = 0; i < num; i++)
  {
    lcd_writeonepoint(color);
  }
}

/**
  * @brief  this function is fill in lcd with color-pointer
  * @param  sx : row coordinates starting vaule
  * @param  sy : column coordinates starting vaule
  * @param  ex : row coordinates ending vaule
  * @param  ey : column coordinates ending vaule
  * @param  color : color-point for fill in lcd
  * @retval none
  */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
  uint16_t height, width;
  uint16_t i, j;

  width = ex - sx + 1;
  height = ey - sy + 1;
  lcd_struct->lcd_setblock(sx, sy, ex, ey);

  for(i = 0; i < height; i++)
  {
    for(j = 0; j < width; j++)
    {
      lcd_writeonepoint(color[i * width + j]);
    }
  }
}
/**
  * @}
  */

/**
  * @}
  */
