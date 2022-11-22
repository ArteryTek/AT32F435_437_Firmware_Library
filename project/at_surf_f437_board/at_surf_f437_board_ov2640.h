/**
  **************************************************************************
  * @file     at_surf_f437_board_ov2640.c
  * @brief    header file of ov2640 program
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

#ifndef _AT_SURF_F437_BOARD_OV2640_H
#define _AT_SURF_F437_BOARD_OV2640_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at_surf_f437_board_dvp.h"

#define OV2640_ADDR                      0x60    //OV2640 ID
#define OV2640_MID                       0x7FA2
#define OV2640_PID                       0x2642

#define OV2640_DSP_R_BYPASS              0x05
#define OV2640_DSP_Qs                    0x44
#define OV2640_DSP_CTRL                  0x50
#define OV2640_DSP_HSIZE1                0x51
#define OV2640_DSP_VSIZE1                0x52
#define OV2640_DSP_XOFFL                 0x53
#define OV2640_DSP_YOFFL                 0x54
#define OV2640_DSP_VHYX                  0x55
#define OV2640_DSP_DPRP                  0x56
#define OV2640_DSP_TEST                  0x57
#define OV2640_DSP_ZMOW                  0x5A
#define OV2640_DSP_ZMOH                  0x5B
#define OV2640_DSP_ZMHH                  0x5C
#define OV2640_DSP_BPADDR                0x7C
#define OV2640_DSP_BPDATA                0x7D
#define OV2640_DSP_CTRL2                 0x86
#define OV2640_DSP_CTRL3                 0x87
#define OV2640_DSP_SIZEL                 0x8C
#define OV2640_DSP_HSIZE2                0xC0
#define OV2640_DSP_VSIZE2                0xC1
#define OV2640_DSP_CTRL0                 0xC2
#define OV2640_DSP_CTRL1                 0xC3
#define OV2640_DSP_R_DVP_SP              0xD3
#define OV2640_DSP_IMAGE_MODE            0xDA
#define OV2640_DSP_RESET                 0xE0
#define OV2640_DSP_MS_SP                 0xF0
#define OV2640_DSP_SS_ID                 0x7F
#define OV2640_DSP_SS_CTRL               0xF8
#define OV2640_DSP_MC_BIST               0xF9
#define OV2640_DSP_MC_AL                 0xFA
#define OV2640_DSP_MC_AH                 0xFB
#define OV2640_DSP_MC_D                  0xFC
#define OV2640_DSP_P_STATUS              0xFE
#define OV2640_DSP_RA_DLMT               0xFF

#define OV2640_SENSOR_GAIN               0x00
#define OV2640_SENSOR_COM1               0x03
#define OV2640_SENSOR_REG04              0x04
#define OV2640_SENSOR_REG08              0x08
#define OV2640_SENSOR_COM2               0x09
#define OV2640_SENSOR_PIDH               0x0A
#define OV2640_SENSOR_PIDL               0x0B
#define OV2640_SENSOR_COM3               0x0C
#define OV2640_SENSOR_COM4               0x0D
#define OV2640_SENSOR_AEC                0x10
#define OV2640_SENSOR_CLKRC              0x11
#define OV2640_SENSOR_COM7               0x12
#define OV2640_SENSOR_COM8               0x13
#define OV2640_SENSOR_COM9               0x14
#define OV2640_SENSOR_COM10              0x15
#define OV2640_SENSOR_HREFST             0x17
#define OV2640_SENSOR_HREFEND            0x18
#define OV2640_SENSOR_VSTART             0x19
#define OV2640_SENSOR_VEND               0x1A
#define OV2640_SENSOR_MIDH               0x1C
#define OV2640_SENSOR_MIDL               0x1D
#define OV2640_SENSOR_AEW                0x24
#define OV2640_SENSOR_AEB                0x25
#define OV2640_SENSOR_W                  0x26
#define OV2640_SENSOR_REG2A              0x2A
#define OV2640_SENSOR_FRARL              0x2B
#define OV2640_SENSOR_ADDVSL             0x2D
#define OV2640_SENSOR_ADDVHS             0x2E
#define OV2640_SENSOR_YAVG               0x2F
#define OV2640_SENSOR_REG32              0x32
#define OV2640_SENSOR_ARCOM2             0x34
#define OV2640_SENSOR_REG45              0x45
#define OV2640_SENSOR_FLL                0x46
#define OV2640_SENSOR_FLH                0x47
#define OV2640_SENSOR_COM19              0x48
#define OV2640_SENSOR_ZOOMS              0x49
#define OV2640_SENSOR_COM22              0x4B
#define OV2640_SENSOR_COM25              0x4E
#define OV2640_SENSOR_BD50               0x4F
#define OV2640_SENSOR_BD60               0x50
#define OV2640_SENSOR_REG5D              0x5D
#define OV2640_SENSOR_REG5E              0x5E
#define OV2640_SENSOR_REG5F              0x5F
#define OV2640_SENSOR_REG60              0x60
#define OV2640_SENSOR_HISTO_LOW          0x61
#define OV2640_SENSOR_HISTO_HIGH         0x62


#define OV_I2C_TIMEOUT                   0xFFFFFFF

#define OV_I2C_CLKCTRL_100K              0x70F03C6B   //100K

#define OV_I2C                           I2C2
#define OV_I2C_CLK                       CRM_I2C2_PERIPH_CLOCK

#define OV_I2C_SCL_GPIO_CLK              CRM_GPIOH_PERIPH_CLOCK
#define OV_I2C_SCL_GPIO_PIN              GPIO_PINS_2
#define OV_I2C_SCL_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE2
#define OV_I2C_SCL_GPIO_PORT             GPIOH
#define OV_I2C_SCL_GPIO_MUX              GPIO_MUX_4

#define OV_I2C_SDA_GPIO_CLK              CRM_GPIOH_PERIPH_CLOCK
#define OV_I2C_SDA_GPIO_PIN              GPIO_PINS_3
#define OV_I2C_SDA_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE3
#define OV_I2C_SDA_GPIO_PORT             GPIOH
#define OV_I2C_SDA_GPIO_MUX              GPIO_MUX_4

#define OV_RST_GPIO_CLK                  CRM_GPIOD_PERIPH_CLOCK
#define OV_RST_GPIO_PIN                  GPIO_PINS_6
#define OV_RST_GPIO_PORT                 GPIOD

#define OV2640_RST_HIGH()                gpio_bits_set(OV_RST_GPIO_PORT, OV_RST_GPIO_PIN)
#define OV2640_RST_LOW()                 gpio_bits_reset(OV_RST_GPIO_PORT, OV_RST_GPIO_PIN)

#define OV2640_PWRON_HIGH()
#define OV2640_PWRON_LOW()


error_status ov2640_reg_write(uint8_t reg, uint8_t data);
error_status ov2640_reg_read(uint8_t reg, uint8_t *data);
error_status ov2640_init(void);
void ov2640_jpeg_mode(void);
void ov2640_rgb565_mode(void);
void ov2640_auto_exposure(uint8_t level);
void ov2640_light_mode(uint8_t mode);
void ov2640_color_saturation(uint8_t sat);
void ov2640_brightness(uint8_t bright);
void ov2640_contrast(uint8_t contrast);
void ov2640_special_effects(uint8_t eft);
void ov2640_color_bar(uint8_t sw);
void ov2640_window_set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
uint8_t ov2640_outsize_set(uint16_t width,uint16_t height);
uint8_t ov2640_imagewin_set(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height);
uint8_t ov2640_imagesize_set(uint16_t width,uint16_t height);
void ov2640_i2c_init(void);
void ov2640_capture(void);

#ifdef __cplusplus
}
#endif

#endif
