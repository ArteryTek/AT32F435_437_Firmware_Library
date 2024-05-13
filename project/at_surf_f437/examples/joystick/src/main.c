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
#include "at_surf_f437_board_joystick.h"
#include "at_surf_f437_board_lcd.h"

#define PI  3.141F
#define MUL 2
float pitch = 5, roll = 5, yaw = 5;
float pitch_buf, roll_buf, yaw_buf;
float cube[8][3]={{-20*MUL,-20*MUL,-20*MUL},{-20*MUL,20*MUL,-20*MUL},{20*MUL,20*MUL,-20*MUL},{20*MUL,-20*MUL,-20*MUL},{-20*MUL,-20*MUL,20*MUL},{-20*MUL,20*MUL,20*MUL},{20*MUL,20*MUL,20*MUL},{20*MUL,-20*MUL,20*MUL}};
int lineid[]={1,2,2,3,3,4,4,1,5,6,6,7,7,8,8,5,8,4,7,3,6,2,5,1,1,3,2,4};

#define DRAW_W 150
#define DRAW_H 150

uint32_t lcd_buf[DRAW_W][DRAW_H];
  
/**
  * @brief  math convert
  * @param  a:point to result
  * @param  b:the matrix to be convert
  * @retval a:the result of convert
  */
float *matconv(float *a, float b[3][3])
{
  float res[3];
  int i;
  for(i=0; i<3; ++i)
  res[i] = b[i][0] * a[0] + b[i][1] * a[1] + b[i][2] * a[2];
  for(i=0; i<3; ++i)
  a[i] = res[i];
  return a;
}

/**
  * @brief  object rotate
  * @param  obj:point the cube cell
  * @param  x:the x coordinate of the cube
  * @param  y:the y coordinate of the cube
  * @param  z:the z coordinate of the cube
  * @retval none
  */
void rotate(float* obj, float x, float y, float z)
{
  float rz[3][3], ry[3][3], rx[3][3];
  x /= PI; y /= PI; z /= PI;

  rz[0][0] = cos(z);  rz[0][1] = -sin(z); rz[0][2] = 0;
  rz[1][0] = sin(z);  rz[1][1] = cos(z);  rz[1][2] = 0;
  rz[2][0] = 0;       rz[2][1] = 0;       rz[2][2] = 1;

  ry[0][0] = 1;       ry[0][1] = 0;       ry[0][2] = 0;
  ry[1][0] = 0;       ry[1][1] = cos(y);  ry[1][2] = -sin(y);
  ry[2][0] = 0;       ry[2][1] = sin(y);  ry[2][2] = cos(y);

  rx[0][0] = cos(x);  rx[0][1] = 0;       rx[0][2] = sin(x);
  rx[1][0] = 0;       rx[1][1] = 1;       rx[1][2] = 0;
  rx[2][0] = -sin(x); rx[2][1] = 0;       rx[2][2] = cos(x);

  matconv(matconv(matconv(obj,rz), ry), rx);
}

/**
  * @brief  write a pixel data at memory
  * @param  x:the x coordinate of the pixel
  * @param  y:the y coordinate of the pixel
  * @param  color:the color of the pixel
  * @retval none
  */
void lcd_point_draw_in_memory(uint16_t x, uint16_t y, uint16_t color)
{
  lcd_buf[x][y] = color;
}

/**
  * @brief  draw a line at memory
  * @param  x_start:the x start coordinate of the line
  * @param  x_end  :the x end   coordinate of the line
  * @param  y_start:the y start coordinate of the line
  * @param  y_end  :the x end   coordinate of the line
  * @param  color  :the color of the line
  * @retval none
  */
void lcd_draw_line_in_memory(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end,uint16_t color)
{
  uint16_t t;
  int xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int incx, incy, uRow, uCol;
  
  delta_x = x_end - x_start;
  delta_y = y_end - y_start;
  uRow = x_start;
  uCol = y_start;
  
  if(delta_x > 0)
  {
    incx = 1;
  }
  else if(delta_x == 0)
  {
    incx = 0;
  }
  else 
  { 
    incx = -1;
    delta_x = -delta_x;
  }
  
  if(delta_y > 0)
  {
    incy = 1;
  }
  else if(delta_y == 0)
  {
    incy=0;
  }
  else
  {
    incy = -1;
    delta_y = -delta_y;
  }
  if(delta_x > delta_y)
  {
    distance = delta_x;
  }
  else 
  {
    distance = delta_y;
  }
  
  for(t = 0; t <= distance + 1; t++ )
  {
    lcd_point_draw_in_memory(uRow, uCol, color);
    
    xerr += delta_x;
    yerr += delta_y;
    
    if(xerr > distance)
    {
      xerr -= distance;
      uRow += incx;
    }
    
    if(yerr > distance)
    {
      yerr -= distance;
      uCol += incy;
    }
  }
}

/**
  * @brief  refresh memory data to lcd
  * @param  x_start:the x start coordinate of the line
  * @param  y_start:the y start coordinate of the line
  * @retval none
  */
void lcd_refresh(uint16_t x_start, uint16_t y_start)
{
  uint16_t x, y;
  
  for(y = 0; y < DRAW_H; y++)
  {
    lcd_windows_set(x_start, y_start + y, x + DRAW_W, y_start + y);
    
    for(x = 0; x < DRAW_W; x++)
    {
      lcd_data_16bit_write(lcd_buf[x][y]);
    }
  }
}

/**
  * @brief  clear memory
  * @param  color:the color of the line
  * @retval none
  */
void lcd_memory_clear(uint16_t color)
{
  uint16_t x, y;
  
  for(y = 0; y < DRAW_H; y++)
  {
    for(x = 0; x < DRAW_W; x++)
    {
      lcd_buf[x][y] = color;
    }
  }
}

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  uint16_t i;

  joy_type key;

  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* initialize joystick */
  joystick_init();

  lcd_clear(BLACK);

  /* display information */
  lcd_string_show(80, 20, 200, 24, 24, (uint8_t *)"Joystick Test");

  lcd_string_show(50, 400, 200, 24, 24, (uint8_t *)"key value:");


  while(1)
  {
    /* read io input status */
    pca9555_io_scan();

    /* read joystick key value */
    key = joystick_press();


    switch(key)
    {
      case JOY_LEFT:
        lcd_string_show(200, 400, 100, 24, 24, (uint8_t *)"left ");
        pitch = 10;
        roll= 0;
        yaw= 0;
        break;
      case JOY_RIGHT:
        lcd_string_show(200, 400, 100, 24, 24, (uint8_t *)"right");
        pitch = -10;
        roll= 0;
        yaw= 0;
        break;
      case JOY_UP:
        lcd_string_show(200, 400, 100, 24, 24, (uint8_t *)"up   ");
        pitch = 0;
        roll= -10;
        yaw=0;
        break;
      case JOY_DOWN:
        lcd_string_show(200, 400, 100, 24, 24, (uint8_t *)"down ");
        pitch = 0;
        roll= 10;
        yaw=0;
        break;
      case JOY_ENTER:
        lcd_string_show(200, 400, 100, 24, 24, (uint8_t *)"enter");
        pitch = 0;
        roll= 0;
        yaw = 10;
        break;
      case JOY_NONE:
        break;
      default:
        break;
    }
    
    for(i = 0; i < 8; i++)
    {
      rotate(cube[i], pitch/360, roll/360, yaw/360);    
    }

    lcd_memory_clear(BLACK);
    
    for(i = 0; i < 28; i += 2)
    {
      lcd_draw_line_in_memory(75 + cube[lineid[i]-1][0], 75 + cube[lineid[i]-1][1], 75 + cube[lineid[i+1]-1][0], 75 + cube[lineid[i+1]-1][1], WHITE);
    }
    
    lcd_refresh(80, 130);
    
    delay_ms(10);
  }
}

