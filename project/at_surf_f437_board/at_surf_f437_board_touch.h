/**
  **************************************************************************
  * @file     at_surf_f437_board_touch.h
  * @brief    touch application libray header file.
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
#ifndef __AT32_SURF_F437_BOARD_TOUCH_H
#define __AT32_SURF_F437_BOARD_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "i2c_application.h"

/**
  * @brief touch register definition
  */
#define GT_CTRL_REG                      0x8040 /* control register */
#define GT_CFG_REG                       0x8047 /* config version */
#define GT_CHECK_REG                     0x80FF /* config check sum */
#define GT_PID_REG                       0x8140 /* product ID */
#define GT_STS_REG                       0x814E /* touch status register */
#define GT_TP1_REG                       0x8150 /* point 1 coordinate */
#define GT_TP2_REG                       0x8158 /* point 2 coordinate */
#define GT_TP3_REG                       0x8160 /* point 3 coordinate */
#define GT_TP4_REG                       0x8168 /* point 4 coordinate */
#define GT_TP5_REG                       0x8170 /* point 5 coordinate */

/**
  * @brief touch scan direction
  */
typedef enum
{
  TOUCH_SCAN_VERTICAL                   = 0x00, /*!< vertical display */
  TOUCH_SCAN_HORIZONTAL                 = 0x01  /*!< horizontal display */
} touch_scan_type;

/**
  * @brief touch device type
  */
typedef struct
{
  error_status (*init)(touch_scan_type direction);         /* pin init */
  error_status (*touch_read_xy)(uint16_t *x, uint16_t *y); /* read x/y coordinate */
  void (*touch_scan)(void);                                /* touch scan func */
  error_status (*touch_adjust)(void);                      /* touch adjust func */
  touch_scan_type direction;                               /* touch scan direction */
  uint16_t x_p[6];                                         /* save x coordinate */
  uint16_t y_p[6];                                         /* save y coordinate */
}touch_dev_type;

extern touch_dev_type touch_dev_struct;

/**
  * @brief io definition
  */
#define GT_I2C_TIMEOUT                   0xFFFFFFF

#define GT_I2C_CLKCTRL_100K              0x80504C4E
#define GT_I2C_CLKCTRL_200K              0x30F03C6B

#define GT_I2C_ADDRESS                   0x28

#define GT_I2C_PORT                      I2C1
#define GT_I2C_CLK                       CRM_I2C1_PERIPH_CLOCK

#define GT_SCL_GPIO_CLK                  CRM_GPIOB_PERIPH_CLOCK
#define GT_SCL_GPIO_PIN                  GPIO_PINS_6
#define GT_SCL_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE6
#define GT_SCL_GPIO_PORT                 GPIOB
#define GT_SCL_GPIO_MUX                  GPIO_MUX_4

#define GT_SDA_GPIO_CLK                  CRM_GPIOB_PERIPH_CLOCK
#define GT_SDA_GPIO_PIN                  GPIO_PINS_7
#define GT_SDA_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE7
#define GT_SDA_GPIO_PORT                 GPIOB
#define GT_SDA_GPIO_MUX                  GPIO_MUX_4

#define GT_RST_GPIO_CLK                  CRM_GPIOD_PERIPH_CLOCK
#define GT_RST_GPIO_PIN                  GPIO_PINS_11
#define GT_RST_GPIO_PORT                 GPIOD

#define GT_INT_GPIO_CLK                  CRM_GPIOE_PERIPH_CLOCK
#define GT_INT_GPIO_PIN                  GPIO_PINS_3
#define GT_INT_GPIO_PORT                 GPIOE

#define GT_RST_LOW()                     gpio_bits_reset(GT_RST_GPIO_PORT, GT_RST_GPIO_PIN)
#define GT_RST_HIGH()                    gpio_bits_set(GT_RST_GPIO_PORT, GT_RST_GPIO_PIN)

#define GT_INT_LOW()                     gpio_bits_reset(GT_INT_GPIO_PORT, GT_INT_GPIO_PIN)
#define GT_INT_HIGH()                    gpio_bits_set(GT_INT_GPIO_PORT, GT_INT_GPIO_PIN)
#define GT_INT_READ()                    gpio_input_data_bit_read(GT_INT_GPIO_PORT, GT_INT_GPIO_PIN)

extern touch_dev_type touch_dev_struct;
extern touch_dev_type *touch_struct;

/**
  * @brief touch exported functions
  */
error_status touch_init(touch_scan_type direction);
error_status touch_read_xy(uint16_t *x, uint16_t *y);
error_status touch_read_x_or_y(uint8_t xy);
error_status touch_adjust(void);
void touch_scan(void);

#ifdef __cplusplus
}
#endif

#endif
