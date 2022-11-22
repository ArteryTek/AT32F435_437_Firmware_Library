/**
  **************************************************************************
  * @file     ov5640.c
  * @brief    header file of ov5640 program
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

#ifndef _OV5640_H
#define _OV5640_H

#include "at32f435_437.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_DVP_ov5640_capture
  * @{
  */

/** @defgroup ov5640_library_peripheral_info
  * @{
  */

#define LIGHT_SWITCH        0

#define OV5640_RST_HIGH  gpio_bits_set(GPIOC, GPIO_PINS_3);
#define OV5640_RST_LOW   gpio_bits_reset(GPIOC, GPIO_PINS_3);

#define OV5640_ID                        ((uint16_t)0x5640)  /*!< ov5640 id                */
#define OV5640_ADDR                      ((uint16_t)0x0078)  /*!< ov5640 address           */
#define OV5640_CHIPIDH                   ((uint16_t)0x300A)  /*!< ov5640 chip id high bits */
#define OV5640_CHIPIDL                   ((uint16_t)0x300B)  /*!< ov5640 chip id low bits  */

#define HARDWARE_MODE
//#define EMBEDDED_MODE

/**
  * @}
  */

/** @defgroup ov5640_library_exported_functions
  * @{
  */

error_status ov5640_reg_write(uint16_t reg, uint8_t data);
error_status ov5640_reg_read(uint16_t reg, uint8_t *val);
error_status ov5640_init(void);
void ov5640_jpeg_mode(void);
void ov5640_rgb565_mode(void);
void ov5640_exposure(uint8_t exposure);
void ov5640_light_mode(uint8_t mode);
void ov5640_color_saturation(uint8_t sat);
void ov5640_brightness(uint8_t bright);
void ov5640_contrast(uint8_t contrast);
void ov5640_sharpness(uint8_t sharp);
void ov5640_special_effects(uint8_t eft);
void ov5640_flash_ctrl(uint8_t sw);
void ov5640_outsize_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height);
void ov5640_imagewin_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height);
error_status ov5640_focus_init(void);
error_status ov5640_focus_single(void);
error_status ov5640_focus_constant(void);
void ov5640_i2c_init(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif
