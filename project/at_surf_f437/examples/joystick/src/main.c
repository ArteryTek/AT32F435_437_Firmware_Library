/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.7
  * @date     2022-04-02
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

#define PI  3.141
#define MUL 2
float pitch = 5, roll = 5, yaw = 5;
float pitch_buf, roll_buf, yaw_buf;
float cube[8][3]={{-20*MUL,-20*MUL,-20*MUL},{-20*MUL,20*MUL,-20*MUL},{20*MUL,20*MUL,-20*MUL},{20*MUL,-20*MUL,-20*MUL},{-20*MUL,-20*MUL,20*MUL},{-20*MUL,20*MUL,20*MUL},{20*MUL,20*MUL,20*MUL},{20*MUL,-20*MUL,20*MUL}};
int lineid[]={1,2,2,3,3,4,4,1,5,6,6,7,7,8,8,5,8,4,7,3,6,2,5,1,1,3,2,4};

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
  x /= PI; y /= PI; z /= PI;
  float rz[3][3] = {{cos(z),-sin(z),0}, {sin(z),cos(z),0}, {0,0,1}};
  float ry[3][3] = {{1,0,0}, {0,cos(y),-sin(y)}, {0,sin(y),cos(y)}};
  float rx[3][3] = {{cos(x),0,sin(x)}, {0,1,0}, {-sin(x),0,cos(x)}};
  matconv(matconv(matconv(obj,rz), ry), rx);
}

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  uint16_t x = 0, i;

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

    lcd_fill(80,80,230,220,BLACK);

    for(i=0; i<8; i++)
    rotate(cube[i], pitch/360, roll/360, yaw/360);
    for(i=0; i<28; i+=2)
    {
      lcd_draw_line(160+cube[lineid[i]-1][0], 150+cube[lineid[i]-1][1], 160+cube[lineid[i+1]-1][0], 150+cube[lineid[i+1]-1][1], WHITE);
    }

    delay_ms(10);

  }
}

