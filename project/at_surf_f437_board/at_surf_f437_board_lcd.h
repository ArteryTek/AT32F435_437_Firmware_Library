/**
  **************************************************************************
  * @file     at_surf_f437_board_lcd.h
  * @brief    header file for at-start board. set of firmware functions to
  *           manage leds and push-button. initialize delay function.
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

#ifndef __AT32_SURF_F437_BOARD_LCD_H
#define __AT32_SURF_F437_BOARD_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

/**
  * @brief lcd color definition
  */
#define WHITE                            0xFFFF
#define BLACK                            0x0000
#define BLUE                             0x001F
#define BRED                             0xF81F
#define GRED                             0xFFE0
#define GBLUE                            0x07FF
#define RED                              0xF800
#define MAGENTA                          0xF81F
#define GREEN                            0x07E0
#define CYAN                             0x7FFF
#define YELLOW                           0xFFE0
#define BROWN                            0xBC40
#define BRRED                            0xFC07
#define GRAY                             0x8430

#define DARKBLUE                         0x01CF
#define LIGHTBLUE                        0x7D7C
#define GRAYBLUE                         0x5458

#define LIGHTGREEN                       0x841F
#define LIGHTGRAY                        0xEF5B
#define LGRAY                            0xC618

#define LGRAYBLUE                        0xA651
#define LBBLUE                           0x2B12

/**
  * @brief lcd mode definition
  */
#define LCD_USE8BIT_MODEL                0

/**
  * @brief lcd size definition
  */
#define LCD_W                            320
#define LCD_H                            480

/**
  * @brief the address of write data & command (xmc_a10)
  */
#define XMC_LCD_COMMAND                  (0x60000000)
#define XMC_LCD_DATA                     (0x60000000 | 0x800)

/**
  * @brief lcd display direction
  */
typedef enum
{
  LCD_DISPLAY_VERTICAL                   = 0x00, /*!< vertical display */
  LCD_DISPLAY_HORIZONTAL                 = 0x01  /*!< horizontal display */
} lcd_display_type;

/**
  * @brief the lcd operate struct
  */
typedef struct
{
#if LCD_USE8BIT_MODEL
  volatile uint8_t LCD_REG;
  volatile uint8_t LCD_RAM;
#else
  volatile uint16_t LCD_REG;
  volatile uint16_t LCD_RAM;
#endif
} lcd_type;

/**
  * @brief the lcd device struct
  */
typedef struct
{
  uint16_t width;
  uint16_t height;
  uint16_t id;
  uint8_t  dir;
  uint16_t wramcmd;
  uint16_t rramcmd;
  uint16_t setxcmd;
  uint16_t setycmd;
} lcd_device_type;

extern lcd_device_type lcddev;

/**
  * @brief the lcd io definition
  */
#define LCD_D0_GPIO_PORT                 GPIOD
#define LCD_D0_GPIO_CLK                  CRM_GPIOD_PERIPH_CLOCK
#define LCD_D0_GPIO_PIN                  GPIO_PINS_14
#define LCD_D0_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE14
#define LCD_D0_GPIO_MUX                  GPIO_MUX_12

#define LCD_D1_GPIO_PORT                 GPIOD
#define LCD_D1_GPIO_CLK                  CRM_GPIOD_PERIPH_CLOCK
#define LCD_D1_GPIO_PIN                  GPIO_PINS_15
#define LCD_D1_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE15
#define LCD_D1_GPIO_MUX                  GPIO_MUX_12

#define LCD_D2_GPIO_PORT                 GPIOD
#define LCD_D2_GPIO_CLK                  CRM_GPIOD_PERIPH_CLOCK
#define LCD_D2_GPIO_PIN                  GPIO_PINS_0
#define LCD_D2_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE0
#define LCD_D2_GPIO_MUX                  GPIO_MUX_12

#define LCD_D3_GPIO_PORT                 GPIOD
#define LCD_D3_GPIO_CLK                  CRM_GPIOD_PERIPH_CLOCK
#define LCD_D3_GPIO_PIN                  GPIO_PINS_1
#define LCD_D3_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE1
#define LCD_D3_GPIO_MUX                  GPIO_MUX_12

#define LCD_D4_GPIO_PORT                 GPIOE
#define LCD_D4_GPIO_CLK                  CRM_GPIOE_PERIPH_CLOCK
#define LCD_D4_GPIO_PIN                  GPIO_PINS_7
#define LCD_D4_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE7
#define LCD_D4_GPIO_MUX                  GPIO_MUX_12

#define LCD_D5_GPIO_PORT                 GPIOE
#define LCD_D5_GPIO_CLK                  CRM_GPIOE_PERIPH_CLOCK
#define LCD_D5_GPIO_PIN                  GPIO_PINS_8
#define LCD_D5_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE8
#define LCD_D5_GPIO_MUX                  GPIO_MUX_12

#define LCD_D6_GPIO_PORT                 GPIOE
#define LCD_D6_GPIO_CLK                  CRM_GPIOE_PERIPH_CLOCK
#define LCD_D6_GPIO_PIN                  GPIO_PINS_9
#define LCD_D6_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE9
#define LCD_D6_GPIO_MUX                  GPIO_MUX_12

#define LCD_D7_GPIO_PORT                 GPIOE
#define LCD_D7_GPIO_CLK                  CRM_GPIOE_PERIPH_CLOCK
#define LCD_D7_GPIO_PIN                  GPIO_PINS_10
#define LCD_D7_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE10
#define LCD_D7_GPIO_MUX                  GPIO_MUX_12

#define LCD_D8_GPIO_PORT                 GPIOE
#define LCD_D8_GPIO_CLK                  CRM_GPIOE_PERIPH_CLOCK
#define LCD_D8_GPIO_PIN                  GPIO_PINS_11
#define LCD_D8_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE11
#define LCD_D8_GPIO_MUX                  GPIO_MUX_12

#define LCD_D9_GPIO_PORT                 GPIOE
#define LCD_D9_GPIO_CLK                  CRM_GPIOE_PERIPH_CLOCK
#define LCD_D9_GPIO_PIN                  GPIO_PINS_12
#define LCD_D9_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE12
#define LCD_D9_GPIO_MUX                  GPIO_MUX_12

#define LCD_D10_GPIO_PORT                GPIOE
#define LCD_D10_GPIO_CLK                 CRM_GPIOE_PERIPH_CLOCK
#define LCD_D10_GPIO_PIN                 GPIO_PINS_13
#define LCD_D10_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE13
#define LCD_D10_GPIO_MUX                 GPIO_MUX_12

#define LCD_D11_GPIO_PORT                GPIOE
#define LCD_D11_GPIO_CLK                 CRM_GPIOE_PERIPH_CLOCK
#define LCD_D11_GPIO_PIN                 GPIO_PINS_14
#define LCD_D11_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE14
#define LCD_D11_GPIO_MUX                 GPIO_MUX_12

#define LCD_D12_GPIO_PORT                GPIOE
#define LCD_D12_GPIO_CLK                 CRM_GPIOE_PERIPH_CLOCK
#define LCD_D12_GPIO_PIN                 GPIO_PINS_15
#define LCD_D12_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE15
#define LCD_D12_GPIO_MUX                 GPIO_MUX_12

#define LCD_D13_GPIO_PORT                GPIOD
#define LCD_D13_GPIO_CLK                 CRM_GPIOD_PERIPH_CLOCK
#define LCD_D13_GPIO_PIN                 GPIO_PINS_8
#define LCD_D13_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE8
#define LCD_D13_GPIO_MUX                 GPIO_MUX_12

#define LCD_D14_GPIO_PORT                GPIOD
#define LCD_D14_GPIO_CLK                 CRM_GPIOD_PERIPH_CLOCK
#define LCD_D14_GPIO_PIN                 GPIO_PINS_9
#define LCD_D14_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE9
#define LCD_D14_GPIO_MUX                 GPIO_MUX_12

#define LCD_D15_GPIO_PORT                GPIOD
#define LCD_D15_GPIO_CLK                 CRM_GPIOD_PERIPH_CLOCK
#define LCD_D15_GPIO_PIN                 GPIO_PINS_10
#define LCD_D15_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE10
#define LCD_D15_GPIO_MUX                 GPIO_MUX_12

#define LCD_NE1_GPIO_PORT                GPIOD
#define LCD_NE1_GPIO_CLK                 CRM_GPIOD_PERIPH_CLOCK
#define LCD_NE1_GPIO_PIN                 GPIO_PINS_7
#define LCD_NE1_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE7
#define LCD_NE1_GPIO_MUX                 GPIO_MUX_12

#define LCD_NWE_GPIO_PORT                GPIOD
#define LCD_NWE_GPIO_CLK                 CRM_GPIOD_PERIPH_CLOCK
#define LCD_NWE_GPIO_PIN                 GPIO_PINS_5
#define LCD_NWE_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE5
#define LCD_NWE_GPIO_MUX                 GPIO_MUX_12

#define LCD_NOE_GPIO_PORT                GPIOD
#define LCD_NOE_GPIO_CLK                 CRM_GPIOD_PERIPH_CLOCK
#define LCD_NOE_GPIO_PIN                 GPIO_PINS_4
#define LCD_NOE_GPIO_PINS_SOURCE         GPIO_PINS_SOURCE4
#define LCD_NOE_GPIO_MUX                 GPIO_MUX_12

#define LCD_A10_GPIO_PORT                 GPIOG
#define LCD_A10_GPIO_CLK                  CRM_GPIOG_PERIPH_CLOCK
#define LCD_A10_GPIO_PIN                  GPIO_PINS_0
#define LCD_A10_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE0
#define LCD_A10_GPIO_MUX                  GPIO_MUX_12

#define LCD_BL_GPIO_CLK
#define LCD_BL_GPIO_PIN
#define LCD_BL_GPIO_PORT

#define LCD_RESET_GPIO_CLK
#define LCD_RESET_GPIO_PIN
#define LCD_RESET_GPIO_PORT

/* the lcd back light line */
#define LCD_BL_HIGH()
#define LCD_BL_LOW()

/* the lcd reset line */
#define LCD_RESET_HIGH()
#define LCD_RESET_LOW()

void xmc_init(void);
uint16_t lcd_read(void);
void lcd_reg_write(uint16_t data);
void lcd_data_write(uint16_t data);
uint16_t lcd_data_read(void);
void lcd_command_write(uint16_t lcd_comm, uint16_t lcd_regvalue);
void lcd_command_read(uint16_t lcd_comm, uint8_t *rval, int32_t n);
void lcd_ram_prepare_write(void);
void lcd_ram_prepare_read(void);
void lcd_data_16bit_write(uint16_t data);
uint16_t color_to_565(uint8_t r, uint8_t g, uint8_t b);
uint16_t lcd_data_16bit_read(void);
void lcd_point_draw(uint16_t x,uint16_t y, uint16_t color);
uint16_t lcd_point_read(uint16_t x,uint16_t y);
void lcd_draw_line(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end,uint16_t color);
void lcd_clear(uint16_t color);
void lcd_init(lcd_display_type direction);
void lcd_windows_set(uint16_t xstar, uint16_t ystar,uint16_t xend,uint16_t yend);
void lcd_cursor_set(uint16_t xpos, uint16_t ypos);
void lcd_direction(uint8_t direction);
uint16_t lcd_id_read(void);
void lcd_char_show(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);
void lcd_string_show(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p);
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);
void lcd_num_show(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, int32_t num, uint8_t num_bit);
void lcd_float_num_show(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, float num, uint8_t num_bit);

#ifdef __cplusplus
}
#endif

#endif

