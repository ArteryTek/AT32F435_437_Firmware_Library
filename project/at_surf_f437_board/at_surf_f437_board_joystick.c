/**
  **************************************************************************
  * @file     at_surf_f437_board_joystick.c
  * @brief    the driver library of the joystick
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

#include "at_surf_f437_board_joystick.h"

/**
  * @brief  joystick init.
  * @param  none.
  * @retval none.
  */
void joystick_init(void)
{
  pca9555_init(PCA_I2C_CLKCTRL_100K);

  pca9555_input_mode_config(JOY_LEFT_PIN | JOY_RIGHT_PIN | JOY_UP_PIN  | JOY_DOWN_PIN | JOY_ENTER_PIN);
}

/**
  * @brief  returns which button have press down
  * @param  none
  * @retval the button have press down
  */
joy_type joystick_press(void)
{
  static uint8_t pressed = 1;
  static uint16_t io_last = 0xFFFF;
  uint16_t io_status;

  io_status = pca9555_input_data_read();

  io_status = io_status & (JOY_LEFT_PIN | JOY_RIGHT_PIN | JOY_UP_PIN | JOY_DOWN_PIN | JOY_ENTER_PIN);

  if(io_last != io_status)
  {
    io_last = io_status;

    /* get button state in at_start board */
    if((pressed == 1) && (JOY_LEFT_DOWN() || JOY_RIGHT_DOWN() || JOY_UP_DOWN() || JOY_DOWN_DOWN() || JOY_ENTER_DOWN()))
    {
      /* debounce */
      pressed = 0;

      if(JOY_LEFT_DOWN())
      {
        return JOY_LEFT;
      }
      else if(JOY_RIGHT_DOWN())
      {
        return JOY_RIGHT;
      }
      else if(JOY_UP_DOWN())
      {
        return JOY_UP;
      }
      else if(JOY_DOWN_DOWN())
      {
        return JOY_DOWN;
      }
      else if(JOY_ENTER_DOWN())
      {
        return JOY_ENTER;
      }
    }
    else if(JOY_LEFT_RELEASE() && JOY_RIGHT_RELEASE() && JOY_UP_RELEASE() && JOY_DOWN_RELEASE() && JOY_ENTER_RELEASE())
    {
      pressed = 1;
    }
  }

  return JOY_NONE;
}
