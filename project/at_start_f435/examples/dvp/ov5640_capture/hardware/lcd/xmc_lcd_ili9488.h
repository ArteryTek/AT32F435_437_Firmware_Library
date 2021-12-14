/**
  **************************************************************************
  * @file     xmc_lcd_ili9488.h
  * @version  v2.0.2
  * @date     2021-11-26
  * @brief    xmc_lcd program header
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
#ifndef __XMC_LCD_ili9488_H
#define __XMC_LCD_ili9488_H

#include "at32f435_437_board.h"

/* the address of write data & command (xmc_a16) */
#define  XMC_LCD_COMMAND                 0x60000000  
#define  XMC_LCD_DATA                    0x60000002

#define LCD_BL_HIGH                      gpio_bits_set(GPIOB,GPIO_PINS_0)
#define LCD_BL_LOW                       gpio_bits_reset(GPIOB,GPIO_PINS_0)

/* the lcd reset line */                 
#define LCD_RESET_HIGH                   gpio_bits_set(GPIOD,GPIO_PINS_3)
#define LCD_RESET_LOW                    gpio_bits_reset(GPIOD,GPIO_PINS_3)

typedef struct  
{										    
	u16 width;	
	u16 height;	
	u16 id;			
	u8  dir;
	u16	wramcmd;
	u16 rramcmd;
	u16 setxcmd;
	u16 setycmd;
}_lcd_dev; 	

extern _lcd_dev lcddev; 
#define USE_HORIZONTAL  	               2
#define LCD_USE8BIT_MODEL                0

#define LCD_W                            320
#define LCD_H                            480
   
extern u16  POINT_COLOR;   
extern u16  BACK_COLOR;

#define LED                              0 

#define	LCD_LED PBout(LED)

typedef struct
{
#if LCD_USE8BIT_MODEL 
	vu8 LCD_REG;
	vu8 LCD_RAM;
#else
	vu16 LCD_REG;
	vu16 LCD_RAM;		
#endif
} LCD_TypeDef;

#if LCD_USE8BIT_MODEL
#define LCD_BASE                         ((u32)(0x6C000000 | 0x000003FF))
#else
#define LCD_BASE                         ((u32)(0x6C000000 | 0x000007FE))
#endif

#define LCD                              ((LCD_TypeDef *) LCD_BASE)		  

#define WHITE                            0xFFFF
#define BLACK      	                     0x0000	  
#define BLUE       	                     0x001F  
#define BRED                             0xF81F
#define GRED 			 	                     0xFFE0
#define GBLUE			 	                     0x07FF
#define RED                              0xF800
#define MAGENTA                          0xF81F
#define GREEN                            0x07E0
#define CYAN                             0x7FFF
#define YELLOW                           0xFFE0
#define BROWN 			                     0xBC40
#define BRRED 			                     0xFC07
#define GRAY  			                     0x8430

#define DARKBLUE      	                 0x01CF
#define LIGHTBLUE      	                 0x7D7C
#define GRAYBLUE       	                 0x5458
 
#define LIGHTGREEN     	                 0x841F
#define LIGHTGRAY                        0xEF5B
#define LGRAY 			 		                 0xC618
                                         
#define LGRAYBLUE      	                 0xA651
#define LBBLUE                           0x2B12
	    															  
void xmc_init(void);
uint16_t lcd_read(void);
void lcd_reg_write(uint16_t data);
void lcd_data_write(uint16_t data);
uint16_t lcd_data_read(void);
void lcd_command_write(uint16_t lcd_comm, uint16_t lcd_regvalue);
void lcd_command_read(uint16_t lcd_comm, uint8_t *rval, int n);
void lcd_ram_prepare_write(void);
void lcd_ram_prepare_read(void);
void lcd_data_16bit_write(uint16_t data);
uint16_t color_to_565(uint8_t r, uint8_t g, uint8_t b);
uint16_t lcd_data_16bit_read(void);
void lcd_point_draw(uint16_t x,uint16_t y, uint16_t color);
uint16_t lcd_point_read(uint16_t x,uint16_t y);
void lcd_clear(uint16_t color);
void lcd_init(void);
void lcd_windows_set(uint16_t xstar, uint16_t ystar,uint16_t xend,uint16_t yend);
void lcd_cursor_set(uint16_t xpos, uint16_t ypos);
void lcd_direction(uint8_t direction);
uint16_t lcd_id_read(void);
void lcd_char_show(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);
void lcd_string_show(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p);
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);

#endif
