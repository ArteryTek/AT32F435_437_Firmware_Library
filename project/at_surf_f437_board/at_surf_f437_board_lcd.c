/**
  **************************************************************************
  * @file     at_surf_f437_board_lcd.c
  * @brief    set of firmware functions to manage leds and push-button.
  *           initialize delay function.
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

#include "at_surf_f437_board_lcd.h"
#include "at_surf_f437_board_delay.h"
#include "at_surf_f437_board_font.h"

lcd_device_type lcddev;

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
  crm_periph_clock_enable(LCD_D0_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D1_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D2_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D3_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D4_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D5_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D6_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D7_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D8_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D9_GPIO_CLK,  TRUE);
  crm_periph_clock_enable(LCD_D10_GPIO_CLK, TRUE);
  crm_periph_clock_enable(LCD_D11_GPIO_CLK, TRUE);
  crm_periph_clock_enable(LCD_D12_GPIO_CLK, TRUE);
  crm_periph_clock_enable(LCD_D13_GPIO_CLK, TRUE);
  crm_periph_clock_enable(LCD_D14_GPIO_CLK, TRUE);
  crm_periph_clock_enable(LCD_D15_GPIO_CLK, TRUE);
  crm_periph_clock_enable(LCD_NE1_GPIO_CLK, TRUE);
  crm_periph_clock_enable(LCD_NWE_GPIO_CLK, TRUE);
  crm_periph_clock_enable(LCD_NOE_GPIO_CLK, TRUE);
  crm_periph_clock_enable(LCD_A10_GPIO_CLK,  TRUE);

  /* enable the xmc clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);

  /*-- gpio configuration ------------------------------------------------------*/
  gpio_pin_mux_config(LCD_D0_GPIO_PORT,  LCD_D0_GPIO_PINS_SOURCE,  LCD_D0_GPIO_MUX);
  gpio_pin_mux_config(LCD_D1_GPIO_PORT,  LCD_D1_GPIO_PINS_SOURCE,  LCD_D1_GPIO_MUX);
  gpio_pin_mux_config(LCD_D2_GPIO_PORT,  LCD_D2_GPIO_PINS_SOURCE,  LCD_D2_GPIO_MUX);
  gpio_pin_mux_config(LCD_D3_GPIO_PORT,  LCD_D3_GPIO_PINS_SOURCE,  LCD_D3_GPIO_MUX);
  gpio_pin_mux_config(LCD_D4_GPIO_PORT,  LCD_D4_GPIO_PINS_SOURCE,  LCD_D4_GPIO_MUX);
  gpio_pin_mux_config(LCD_D5_GPIO_PORT,  LCD_D5_GPIO_PINS_SOURCE,  LCD_D5_GPIO_MUX);
  gpio_pin_mux_config(LCD_D6_GPIO_PORT,  LCD_D6_GPIO_PINS_SOURCE,  LCD_D6_GPIO_MUX);
  gpio_pin_mux_config(LCD_D7_GPIO_PORT,  LCD_D7_GPIO_PINS_SOURCE,  LCD_D7_GPIO_MUX);
  gpio_pin_mux_config(LCD_D8_GPIO_PORT,  LCD_D8_GPIO_PINS_SOURCE,  LCD_D8_GPIO_MUX);
  gpio_pin_mux_config(LCD_D9_GPIO_PORT,  LCD_D9_GPIO_PINS_SOURCE,  LCD_D9_GPIO_MUX);
  gpio_pin_mux_config(LCD_D10_GPIO_PORT, LCD_D10_GPIO_PINS_SOURCE, LCD_D10_GPIO_MUX);
  gpio_pin_mux_config(LCD_D11_GPIO_PORT, LCD_D11_GPIO_PINS_SOURCE, LCD_D11_GPIO_MUX);
  gpio_pin_mux_config(LCD_D12_GPIO_PORT, LCD_D12_GPIO_PINS_SOURCE, LCD_D12_GPIO_MUX);
  gpio_pin_mux_config(LCD_D13_GPIO_PORT, LCD_D13_GPIO_PINS_SOURCE, LCD_D13_GPIO_MUX);
  gpio_pin_mux_config(LCD_D14_GPIO_PORT, LCD_D14_GPIO_PINS_SOURCE, LCD_D14_GPIO_MUX);
  gpio_pin_mux_config(LCD_D15_GPIO_PORT, LCD_D15_GPIO_PINS_SOURCE, LCD_D15_GPIO_MUX);
  gpio_pin_mux_config(LCD_NE1_GPIO_PORT, LCD_NE1_GPIO_PINS_SOURCE, LCD_NE1_GPIO_MUX);
  gpio_pin_mux_config(LCD_NWE_GPIO_PORT, LCD_NWE_GPIO_PINS_SOURCE, LCD_NWE_GPIO_MUX);
  gpio_pin_mux_config(LCD_NOE_GPIO_PORT, LCD_NOE_GPIO_PINS_SOURCE, LCD_NOE_GPIO_MUX);
  gpio_pin_mux_config(LCD_A10_GPIO_PORT,  LCD_A10_GPIO_PINS_SOURCE,  LCD_A10_GPIO_MUX);

  /* lcd data lines configuration */
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

  gpio_init_struct.gpio_pins = LCD_D0_GPIO_PIN;
  gpio_init(LCD_D0_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D1_GPIO_PIN;
  gpio_init(LCD_D1_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D2_GPIO_PIN;
  gpio_init(LCD_D2_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D3_GPIO_PIN;
  gpio_init(LCD_D3_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D4_GPIO_PIN;
  gpio_init(LCD_D4_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D5_GPIO_PIN;
  gpio_init(LCD_D5_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D6_GPIO_PIN;
  gpio_init(LCD_D6_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D7_GPIO_PIN;
  gpio_init(LCD_D7_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D8_GPIO_PIN;
  gpio_init(LCD_D8_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D9_GPIO_PIN;
  gpio_init(LCD_D9_GPIO_PORT,  &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D10_GPIO_PIN;
  gpio_init(LCD_D10_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D11_GPIO_PIN;
  gpio_init(LCD_D11_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D12_GPIO_PIN;
  gpio_init(LCD_D12_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D13_GPIO_PIN;
  gpio_init(LCD_D13_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D14_GPIO_PIN;
  gpio_init(LCD_D14_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_D15_GPIO_PIN;
  gpio_init(LCD_D15_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_NE1_GPIO_PIN;
  gpio_init(LCD_NE1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_NWE_GPIO_PIN;
  gpio_init(LCD_NWE_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_NOE_GPIO_PIN;
  gpio_init(LCD_NOE_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = LCD_A10_GPIO_PIN;
  gpio_init(LCD_A10_GPIO_PORT,  &gpio_init_struct);

  /*-- xmc configuration ------------------------------------------------------*/
  xmc_norsram_default_para_init(&xmc_norsram_init_struct);
  xmc_norsram_init_struct.subbank = XMC_BANK1_NOR_SRAM1;
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
  rw_timing_struct.subbank = XMC_BANK1_NOR_SRAM1;
  rw_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  rw_timing_struct.addr_setup_time = 0x8;
  rw_timing_struct.addr_hold_time = 0x0;
  rw_timing_struct.data_setup_time = 0x8;
  rw_timing_struct.bus_latency_time = 0x0;
  rw_timing_struct.clk_psc = 0x0;
  rw_timing_struct.data_latency_time = 0x0;
  rw_timing_struct.mode = XMC_ACCESS_MODE_A;
  w_timing_struct.subbank = XMC_BANK1_NOR_SRAM1;
  w_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  w_timing_struct.addr_setup_time = 0x2;
  w_timing_struct.addr_hold_time = 0x0;
  w_timing_struct.data_setup_time = 0x2;
  w_timing_struct.bus_latency_time = 0x0;
  w_timing_struct.clk_psc = 0x0;
  w_timing_struct.data_latency_time = 0x0;
  w_timing_struct.mode = XMC_ACCESS_MODE_A;
  xmc_nor_sram_timing_config(&rw_timing_struct, &w_timing_struct);

  xmc_ext_timing_config(XMC_BANK1_NOR_SRAM1, 0, 0);
  /* enable xmc_bank1_nor_sram4 */
  xmc_nor_sram_enable(XMC_BANK1_NOR_SRAM1, TRUE);

}

/**
  * @brief  lcd data read
  * @param  none
  * @retval data
  */
uint16_t lcd_read(void)
{
  uint16_t data;

  data = *(__IO uint16_t*)XMC_LCD_DATA;

  return data;
}

/**
  * @brief  write an 16-bit command to the lcd screen
  * @param  data:command value to be written
  * @retval none
  */
void lcd_reg_write(uint16_t data)
{
  *(__IO uint16_t*) XMC_LCD_COMMAND = data;
}

/**
  * @brief  write an 16-bit data to the lcd screen
  * @param  data:data value to be written
  * @retval none
  */
void lcd_data_write(uint16_t data)
{
  *(__IO uint16_t*) XMC_LCD_DATA = data;
}

/**
  * @brief  read an 16-bit value from the lcd screen
  * @param  none
  * @retval read value
  */
uint16_t lcd_data_read(void)
{
  return lcd_read();
}

/**
  * @brief  write data into registers
  * @param  lcd_reg: register address
  * @param  lcd_regvalue: data to be written
  * @retval none
  */
void lcd_command_write(uint16_t lcd_comm, uint16_t lcd_regvalue)
{
  *(__IO uint16_t*) XMC_LCD_COMMAND = lcd_comm;
  *(__IO uint16_t*) XMC_LCD_DATA = lcd_regvalue;
}

/**
  * @brief  read value from specially registers
  * @param  lcd_reg:register address
  * @retval read value
  */
void lcd_command_read(uint16_t lcd_comm, uint8_t *rval, int32_t n)
{
  lcd_reg_write(lcd_comm);

  while(n--)
  {
    *(rval++) = lcd_data_read();
  }
}

/**
  * @brief  write gram
  * @param  none
  * @retval none
  */
void lcd_ram_prepare_write(void)
{
  lcd_reg_write(lcddev.wramcmd);
}

/**
  * @brief  read gram
  * @param  none
  * @retval none
  */
void lcd_ram_prepare_read(void)
{
  lcd_reg_write(lcddev.rramcmd);
}

/**
  * @brief  write an 16-bit command to the lcd screen
  * @param  data:data to be written
  * @retval none
  */
void lcd_data_16bit_write(uint16_t data)
{
  #if LCD_USE8BIT_MODEL
  LCD->LCD_RAM = data >> 8;
  LCD->LCD_RAM = data;
  #else
  *(__IO uint16_t*) XMC_LCD_DATA = data;
  #endif
}

/**
  * @brief  transform rgb format to 565
  * @param  red: color red
  * @param  green: color green
  * @param  blue: color blue
  * @retval none
  */
uint16_t color_to_565(uint8_t red, uint8_t green, uint8_t blue)
{
  return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}

/**
  * @brief  read an 16-bit value from the lcd screen
  * @param  none
  * @retval read value
  */
uint16_t lcd_data_16bit_read(void)
{
  uint16_t r, g, b;

  r = lcd_data_read();
  delay_us(1);

  r = lcd_data_read();
  delay_us(1);

  g = lcd_data_read();

#if LCD_USE8BIT_MODEL
  delay_us(1);
  b = lcd_data_read();
#else
  b = g >> 8;
  g = r & 0xFF;
  r = r >> 8;
#endif

  return color_to_565(r, g, b);
}

/**
  * @brief  write a pixel data at a specified location
  * @param  x:the x coordinate of the pixel
  * @param  y:the y coordinate of the pixel
  * @param  color:the color of the pixel
  * @retval none
  */
void lcd_point_draw(uint16_t x, uint16_t y, uint16_t color)
{
  lcd_cursor_set(x, y);
  lcd_data_16bit_write(color);
}

/**
  * @brief  draw a line at a specified location
  * @param  x_start:the x start coordinate of the line
  * @param  x_end  :the x end   coordinate of the line
  * @param  y_start:the y start coordinate of the line
  * @param  y_end  :the x end   coordinate of the line
  * @param  color  :the color of the line
  * @retval none
  */
void lcd_draw_line(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end,uint16_t color)
{
  u16 t;
  int xerr=0,yerr=0,delta_x,delta_y,distance;
  int incx,incy,uRow,uCol;
  delta_x=x_end-x_start;
  delta_y=y_end-y_start;
  uRow=x_start;
  uCol=y_start;
  if(delta_x>0)incx=1;
  else if(delta_x==0)incx=0;
  else {incx=-1;delta_x=-delta_x;}
  if(delta_y>0)incy=1;
  else if(delta_y==0)incy=0;
  else{incy=-1;delta_y=-delta_y;}
  if( delta_x>delta_y)distance=delta_x;
  else distance=delta_y;
  for(t=0;t<=distance+1;t++ )
  {
    lcd_point_draw(uRow,uCol,color);
    xerr+=delta_x ;
    yerr+=delta_y ;
    if(xerr>distance)
    {
      xerr-=distance;
      uRow+=incx;
    }
    if(yerr>distance)
    {
      yerr-=distance;
      uCol+=incy;
    }
  }
}

/**
  * @brief  read a pixel color value at a specified location
  * @param  x:the x coordinate of the pixel
  * @param  y:the y coordinate of the pixel
  * @retval the read color value
  */
uint16_t lcd_point_read(uint16_t x, uint16_t y)
{
  uint16_t color;
  if(x >= lcddev.width || y >= lcddev.height)
  {
    return 0;
  }

  lcd_cursor_set(x, y);

  lcd_ram_prepare_read();

  color = lcd_data_16bit_read();

  return color;
}

/**
  * @brief  full screen filled lcd screen
  * @param  color:filled color
  * @retval none
  */
void lcd_clear(uint16_t color)
{
  uint32_t i;

//  uint32_t total_point = lcddev.width * lcddev.height;
  uint32_t total_point = 320 * 480;

//  lcd_windows_set(0, 0, lcddev.width - 1, lcddev.height - 1);
  lcd_windows_set(0, 0, 320 - 1, 480 - 1);

  for(i = 0 ;i < total_point; i++)
  {
    #if LCD_USE8BIT_MODEL
    LCD->LCD_RAM = color >> 8;
    LCD->LCD_RAM = color;
    #else
    *(__IO uint16_t*) XMC_LCD_DATA = color;
    #endif
  }
}

/**
  * @brief  initialization lcd screen
  * @param  direction: display direction
  *         this parameter can be one of the following values:
  *         - LCD_DISPLAY_VERTICAL: vertical display.
  *         - LCD_DISPLAY_HORIZONTAL: horizontal display.
  * @retval none
  */
void lcd_init(lcd_display_type direction)
{
  xmc_init();
  LCD_RESET_HIGH();
  delay_ms(120);
  LCD_RESET_LOW();
  delay_ms(120);
  LCD_RESET_HIGH();
  delay_ms(120);
  
  /* read lcd id */
  lcd_id_read();

  lcd_reg_write(0xF7);
  lcd_data_write(0xA9);
  lcd_data_write(0x51);
  lcd_data_write(0x2C);
  lcd_data_write(0x82);

  lcd_reg_write(0xC0);
  lcd_data_write(0x11);
  lcd_data_write(0x09);

  lcd_reg_write(0xC1);
  lcd_data_write(0x41);

  lcd_reg_write(0xC5);
  lcd_data_write(0x00);
  lcd_data_write(0x0A);
  lcd_data_write(0x80);

  lcd_reg_write(0xB1);
  lcd_data_write(0xB0);
  lcd_data_write(0x11);

  lcd_reg_write(0xB4);
  lcd_data_write(0x02);

  lcd_reg_write(0xB6);
  lcd_data_write(0x02);
  lcd_data_write(0x22);

  lcd_reg_write(0xB7);
  lcd_data_write(0xc6);

  lcd_reg_write(0xBE);
  lcd_data_write(0x00);
  lcd_data_write(0x04);

  lcd_reg_write(0xE9);
  lcd_data_write(0x00);

  lcd_reg_write(0x36);
  lcd_data_write(0x08);

  lcd_reg_write(0x3A);
  lcd_data_write(0x55);

  lcd_reg_write(0xE0);
  lcd_data_write(0x00);
  lcd_data_write(0x07);
  lcd_data_write(0x10);
  lcd_data_write(0x09);
  lcd_data_write(0x17);
  lcd_data_write(0x0B);
  lcd_data_write(0x41);
  lcd_data_write(0x89);
  lcd_data_write(0x4B);
  lcd_data_write(0x0A);
  lcd_data_write(0x0C);
  lcd_data_write(0x0E);
  lcd_data_write(0x18);
  lcd_data_write(0x1B);
  lcd_data_write(0x0F);

  lcd_reg_write(0xE1);
  lcd_data_write(0x00);
  lcd_data_write(0x17);
  lcd_data_write(0x1A);
  lcd_data_write(0x04);
  lcd_data_write(0x0E);
  lcd_data_write(0x06);
  lcd_data_write(0x2F);
  lcd_data_write(0x45);
  lcd_data_write(0x43);
  lcd_data_write(0x02);
  lcd_data_write(0x0A);
  lcd_data_write(0x09);
  lcd_data_write(0x32);
  lcd_data_write(0x36);
  lcd_data_write(0x0F);

  lcd_reg_write(0x11);
  delay_ms(120);
  lcd_reg_write(0x29);
  delay_ms(120);

  lcd_direction(direction);
  delay_ms(120);
  LCD_BL_HIGH();
  delay_ms(120);

  lcd_clear(BLACK);
}

/**
  * @brief  setting lcd display window
  * @param  xstar:the bebinning x coordinate of the lcd display window
  * @param  ystar:the bebinning y coordinate of the lcd display window
  * @param  xend:the endning x coordinate of the lcd display window
  * @param  yend:the endning y coordinate of the lcd display window
  * @retval none
  */
void lcd_windows_set(uint16_t xstar, uint16_t ystar, uint16_t xend, uint16_t yend)
{
  lcd_reg_write(lcddev.setxcmd);
  lcd_data_write(xstar >> 8);
  lcd_data_write(0x00FF & xstar);
  lcd_data_write(xend >> 8);
  lcd_data_write(0x00FF & xend);

  lcd_reg_write(lcddev.setycmd);
  lcd_data_write(ystar >> 8);
  lcd_data_write(0x00FF & ystar);
  lcd_data_write(yend >> 8);
  lcd_data_write(0x00FF & yend);

  lcd_ram_prepare_write();
}

/**
  * @brief  set coordinate value
  * @param  xpos:the  x coordinate of the pixel
  * @param  ypos:the  y coordinate of the pixel
  * @retval none
  */
void lcd_cursor_set(uint16_t xpos, uint16_t ypos)
{
  lcd_windows_set(xpos, ypos, xpos, ypos);
}

/**
  * @brief  setting the display direction of lcd screen
  * @param  direction:0-0 degree
  *                   1-90 degree
  *                   2-180 degree
  *                   3-270 degree
  * @retval none
  */
void lcd_direction(uint8_t direction)
{
  lcddev.setxcmd = 0x2A;
  lcddev.setycmd = 0x2B;
  lcddev.wramcmd = 0x2C;
  lcddev.rramcmd = 0x2E;
  lcddev.dir = direction % 4;

  switch(lcddev.dir)
  {
    case LCD_DISPLAY_HORIZONTAL:
      lcddev.width = LCD_H;
      lcddev.height = LCD_W;
      lcd_command_write(0x36, (1 << 3) | (1 << 6) | (1 << 5));
    break;
    case LCD_DISPLAY_VERTICAL:
      lcddev.width = LCD_W;
      lcddev.height = LCD_H;
      lcd_command_write(0x36, (1 << 3) | (1 << 6) | (1 << 7));
    break;
    default:break;
  }
}

/**
  * @brief  read id
  * @param  none
  * @retval id value
  */
uint16_t lcd_id_read(void)
{
  uint8_t val[4] = {0};

  lcd_command_read(0xD3, val, 4);

  return (val[2] << 8) | val[3];
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
        lcd_point_draw(x, y, WHITE);
      }
      else if(mode == 0)
      {
        lcd_point_draw(x, y, BLACK);
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
  * @brief  this function is show chinese to lcd
  * @param  x : row coordinates starting vaule
  * @param  y : column coordinates starting vaule
  * @param  gbk : chinese code
  * @param  size : size of char
  * @param  mode : color
  * @retval none
  */
void lcd_font_show(uint16_t x, uint16_t y, uint8_t *gbk, uint8_t size, uint8_t mode)
{
  uint32_t temp, t1, t, flag;
  uint16_t x0 = x;
  uint8_t buf[72];

  if(size == 24)
  {
    chinese_font_24x24_read(gbk, buf);
    flag = 0x00800000;
  }
  else
  {
    return;
  }

  for(t = 0; t < size; t++)
  {
    if(size == 24)
    {
      temp = (buf[t * 3 + 0] << 16) | (buf[t * 3 + 1] << 8) | buf[t * 3 + 2];
    }

    for(t1 = 0; t1 < size; t1++)
    {
      if(temp & flag)
      {
        lcd_point_draw(x, y, WHITE);
      }
      else if(mode == 0)
      {
        lcd_point_draw(x, y, BLACK);
      }

      temp <<= 1;

      x++;

      if(x >= LCD_W)
      {
        return;
      }
    }

    x = x0;
    y++;

    if(y >= LCD_H)
    {
      return;
    }
  }
}

/**
  * @brief  this function is show string to lcd
  * @param  x : row coordinates starting vaule
  * @param  y : column coordinates starting vaule
  * @param  width : width of area
  * @param  height : height of area
  * @param  size : size of font
  * @param  p : pointer of string
  * @retval none
  */
void lcd_string_show(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p)
{
  width += x;
  height += y;

  while(*p)
  {
    if(x >= width)
    {
      break;
    }

    if(y >= height)
    {
      break;
    }

    if(*p < 127)
    {
      lcd_char_show(x, y, *p, size, BLACK);
      x += size / 2;
      p++;
    }
    else
    {
      lcd_font_show(x, y, p, size, BLACK);
      x += size;
      p += 2;
    }
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
  uint16_t height, width;
  uint16_t i, j;

  width = ex - sx + 1;
  height = ey - sy + 1;

  lcd_windows_set(sx, sy, ex, ey);

  for(i = 0; i < height; i++)
  {
    for(j = 0; j < width; j++)
    {
      lcd_data_16bit_write(color);
    }
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

  lcd_windows_set(sx, sy, ex, ey);

  for(i = 0; i < height; i++)
  {
    for(j = 0; j < width; j++)
    {
      lcd_data_16bit_write(color[i * width + j]);
    }
  }
}

/**
  * @brief  this function is show int number to lcd
  * @param  x : row coordinates starting vaule
  * @param  y : column coordinates starting vaule
  * @param  width : width of string
  * @param  height : height of string
  * @param  num : number
  * @retval num_bit : display number of digits
  */
void lcd_num_show(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, int32_t num, uint8_t num_bit)
{
  uint8_t i, buf[12], start;

  buf[11] = 0;

  if(num < 0)
  {
    num = -num;
    buf[0] = '-';
  }
  else
  {
    buf[0] = '0';
  }

  for(i = 0; i < 10; i++)
  {
    buf[10 - i] =  num % 10 + '0';
    num /= 10;

    if(buf[10 - i] > '0')
    {
      start = 10 - i;
    }
  }

  if(buf[0] == '-')
  {
    start -= 1;

    buf[start] = '-';
  }
  else
  {
    if(start > (11 - num_bit))
    {
      start = 11 - num_bit;
    }
  }

  lcd_string_show(x, y, width, height, size, &buf[start]);
}

/**
  * @brief  this function is show float number to lcd
  * @param  x : row coordinates starting vaule
  * @param  y : column coordinates starting vaule
  * @param  width : width of string
  * @param  height : height of string
  * @param  num : number
  * @retval num_bit : display number of digits
  */
void lcd_float_num_show(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, float num, uint8_t num_bit)
{
  uint8_t buf[12];
  int32_t int_num;

  int_num = num;

  buf[0] = int_num + '0';

  num = num - int_num;

  num *= 1000;

  int_num = num;

  buf[1] = '.';
  buf[2] = (int_num / 100) + '0';
  buf[3] = (int_num % 100 / 10) + '0';
  buf[4] = (int_num % 10) + '0';
  buf[5] = 0;

  buf[num_bit + 2] = 0;

  lcd_string_show(x, y, width, height, size, buf);
}
