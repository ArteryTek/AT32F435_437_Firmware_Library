/**
  **************************************************************************
  * @file     at_surf_f437_board_joystick.h
  * @brief    joystick application libray header file
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT_SURF_F437_BOARD_JOYSTICK_H
#define __AT_SURF_F437_BOARD_JOYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at_surf_f437_board_pca9555.h"

/**
  * @brief define button type
  */
typedef enum
{
  JOY_NONE                               = 0x00,
  JOY_LEFT                               = 0x01,
  JOY_RIGHT                              = 0x02,
  JOY_UP                                 = 0x03,
  JOY_DOWN                               = 0x04,
  JOY_ENTER                              = 0x05
} joy_type;


#define JOY_LEFT_PIN                     PCA_IO1_PINS_4
#define JOY_RIGHT_PIN                    PCA_IO1_PINS_1
#define JOY_UP_PIN                       PCA_IO1_PINS_0
#define JOY_DOWN_PIN                     PCA_IO1_PINS_2
#define JOY_ENTER_PIN                    PCA_IO1_PINS_3

#define JOY_LEFT_DOWN()                  (pca9555_input_data_bit_read(JOY_LEFT_PIN ) == RESET)
#define JOY_RIGHT_DOWN()                 (pca9555_input_data_bit_read(JOY_RIGHT_PIN) == RESET)
#define JOY_UP_DOWN()                    (pca9555_input_data_bit_read(JOY_UP_PIN   ) == RESET)
#define JOY_DOWN_DOWN()                  (pca9555_input_data_bit_read(JOY_DOWN_PIN ) == RESET)
#define JOY_ENTER_DOWN()                 (pca9555_input_data_bit_read(JOY_ENTER_PIN) == RESET)

#define JOY_LEFT_RELEASE()               (pca9555_input_data_bit_read(JOY_LEFT_PIN ) == SET)
#define JOY_RIGHT_RELEASE()              (pca9555_input_data_bit_read(JOY_RIGHT_PIN) == SET)
#define JOY_UP_RELEASE()                 (pca9555_input_data_bit_read(JOY_UP_PIN   ) == SET)
#define JOY_DOWN_RELEASE()               (pca9555_input_data_bit_read(JOY_DOWN_PIN ) == SET)
#define JOY_ENTER_RELEASE()              (pca9555_input_data_bit_read(JOY_ENTER_PIN) == SET)

void joystick_init(void);
joy_type joystick_press(void);

#ifdef __cplusplus
}
#endif

#endif

