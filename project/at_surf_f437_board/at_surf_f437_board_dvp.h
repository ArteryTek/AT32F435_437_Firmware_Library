/**
  **************************************************************************
  * @file     dvp.h
  * @brief    header file of dvp program
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT_SURF_F437_BOARD_DVP_H
#define __AT_SURF_F437_BOARD_DVP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

#define DVP_LCD_W                            320
#define DVP_LCD_H                            240

#define DVP_D0_GPIO_CLK                  CRM_GPIOC_PERIPH_CLOCK
#define DVP_D0_GPIO_PIN                  GPIO_PINS_6
#define DVP_D0_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE6
#define DVP_D0_GPIO_PORT                 GPIOC
#define DVP_D0_GPIO_MUX                  GPIO_MUX_13

#define DVP_D1_GPIO_CLK                  CRM_GPIOC_PERIPH_CLOCK
#define DVP_D1_GPIO_PIN                  GPIO_PINS_7
#define DVP_D1_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE7
#define DVP_D1_GPIO_PORT                 GPIOC
#define DVP_D1_GPIO_MUX                  GPIO_MUX_13

#define DVP_D2_GPIO_CLK                  CRM_GPIOC_PERIPH_CLOCK
#define DVP_D2_GPIO_PIN                  GPIO_PINS_8
#define DVP_D2_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE8
#define DVP_D2_GPIO_PORT                 GPIOC
#define DVP_D2_GPIO_MUX                  GPIO_MUX_13

#define DVP_D3_GPIO_CLK                  CRM_GPIOG_PERIPH_CLOCK
#define DVP_D3_GPIO_PIN                  GPIO_PINS_11
#define DVP_D3_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE11
#define DVP_D3_GPIO_PORT                 GPIOG
#define DVP_D3_GPIO_MUX                  GPIO_MUX_13

#define DVP_D4_GPIO_CLK                  CRM_GPIOB_PERIPH_CLOCK
#define DVP_D4_GPIO_PIN                  GPIO_PINS_3
#define DVP_D4_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE3
#define DVP_D4_GPIO_PORT                 GPIOB
#define DVP_D4_GPIO_MUX                  GPIO_MUX_13

#define DVP_D5_GPIO_CLK                  CRM_GPIOD_PERIPH_CLOCK
#define DVP_D5_GPIO_PIN                  GPIO_PINS_3
#define DVP_D5_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE3
#define DVP_D5_GPIO_PORT                 GPIOD
#define DVP_D5_GPIO_MUX                  GPIO_MUX_13

#define DVP_D6_GPIO_CLK                  CRM_GPIOE_PERIPH_CLOCK
#define DVP_D6_GPIO_PIN                  GPIO_PINS_5
#define DVP_D6_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE5
#define DVP_D6_GPIO_PORT                 GPIOE
#define DVP_D6_GPIO_MUX                  GPIO_MUX_13

#define DVP_D7_GPIO_CLK                  CRM_GPIOE_PERIPH_CLOCK
#define DVP_D7_GPIO_PIN                  GPIO_PINS_6
#define DVP_D7_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE6
#define DVP_D7_GPIO_PORT                 GPIOE
#define DVP_D7_GPIO_MUX                  GPIO_MUX_13

#define DVP_PIXCLK_GPIO_CLK              CRM_GPIOA_PERIPH_CLOCK
#define DVP_PIXCLK_GPIO_PIN              GPIO_PINS_6
#define DVP_PIXCLK_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE6
#define DVP_PIXCLK_GPIO_PORT             GPIOA
#define DVP_PIXCLK_GPIO_MUX              GPIO_MUX_13

#define DVP_VSYNC_GPIO_CLK               CRM_GPIOG_PERIPH_CLOCK
#define DVP_VSYNC_GPIO_PIN               GPIO_PINS_9
#define DVP_VSYNC_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE9
#define DVP_VSYNC_GPIO_PORT              GPIOG
#define DVP_VSYNC_GPIO_MUX               GPIO_MUX_13

#define DVP_HSYNC_GPIO_CLK               CRM_GPIOA_PERIPH_CLOCK
#define DVP_HSYNC_GPIO_PIN               GPIO_PINS_4
#define DVP_HSYNC_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE4
#define DVP_HSYNC_GPIO_PORT              GPIOA
#define DVP_HSYNC_GPIO_MUX               GPIO_MUX_13

#define DVP_EDMA_STREAM                  EDMA_STREAM4
#define DVP_EDMA_DMAMUX_CHANNEL          EDMAMUX_CHANNEL4
#define DVP_EDMA_DMAREQ                  EDMAMUX_DMAREQ_ID_DVP
#define DVP_EDMA_FDT_FLAG                EDMA_FDT4_FLAG
#define DVP_EDMA_IRQn                    EDMA_Stream4_IRQn
#define DVP_EDMA_IRQHandler              EDMA_Stream4_IRQHandler

void dvp_config(uint16_t id);
void dvp_dma_init(uint32_t mem0addr,uint32_t mem1addr,uint16_t memsize);
void dvp_start(void);
void dvp_stop(void);
void dvp_io_init(void);
void dvp_int_handler(void);

#ifdef __cplusplus
}
#endif

#endif /*__DVP_H */
