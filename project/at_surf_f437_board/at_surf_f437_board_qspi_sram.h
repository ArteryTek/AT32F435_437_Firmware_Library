/**
  **************************************************************************
  * @file     at_surf_f437_board_qspi_sram.h
  * @brief    qspi sram application libray header file
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
#ifndef __AT_SURF_F437_BOARD_QSPI_SRAM_H
#define __AT_SURF_F437_BOARD_QSPI_SRAM_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

#define QSPI_SRAM_QSPIx                  QSPI2
#define QSPI_SRAM_MEM_BASE               QSPI2_MEM_BASE
#define QSPI_SRAM_QSPI_CLK               CRM_QSPI2_PERIPH_CLOCK

#define QSPI_SRAM_CS_GPIO_CLK            CRM_GPIOB_PERIPH_CLOCK
#define QSPI_SRAM_CS_GPIO_PIN            GPIO_PINS_8
#define QSPI_SRAM_CS_GPIO_PINS_SOURCE    GPIO_PINS_SOURCE8
#define QSPI_SRAM_CS_GPIO_PORT           GPIOB
#define QSPI_SRAM_CS_GPIO_MUX            GPIO_MUX_10

#define QSPI_SRAM_CLK_GPIO_CLK           CRM_GPIOB_PERIPH_CLOCK
#define QSPI_SRAM_CLK_GPIO_PIN           GPIO_PINS_1
#define QSPI_SRAM_CLK_GPIO_PINS_SOURCE   GPIO_PINS_SOURCE1
#define QSPI_SRAM_CLK_GPIO_PORT          GPIOB
#define QSPI_SRAM_CLK_GPIO_MUX           GPIO_MUX_10

#define QSPI_SRAM_D0_GPIO_CLK            CRM_GPIOB_PERIPH_CLOCK
#define QSPI_SRAM_D0_GPIO_PIN            GPIO_PINS_0
#define QSPI_SRAM_D0_GPIO_PINS_SOURCE    GPIO_PINS_SOURCE0
#define QSPI_SRAM_D0_GPIO_PORT           GPIOB
#define QSPI_SRAM_D0_GPIO_MUX            GPIO_MUX_9

#define QSPI_SRAM_D1_GPIO_CLK            CRM_GPIOG_PERIPH_CLOCK
#define QSPI_SRAM_D1_GPIO_PIN            GPIO_PINS_12
#define QSPI_SRAM_D1_GPIO_PINS_SOURCE    GPIO_PINS_SOURCE12
#define QSPI_SRAM_D1_GPIO_PORT           GPIOG
#define QSPI_SRAM_D1_GPIO_MUX            GPIO_MUX_5

#define QSPI_SRAM_D2_GPIO_CLK            CRM_GPIOG_PERIPH_CLOCK
#define QSPI_SRAM_D2_GPIO_PIN            GPIO_PINS_10
#define QSPI_SRAM_D2_GPIO_PINS_SOURCE    GPIO_PINS_SOURCE10
#define QSPI_SRAM_D2_GPIO_PORT           GPIOG
#define QSPI_SRAM_D2_GPIO_MUX            GPIO_MUX_5

#define QSPI_SRAM_D3_GPIO_CLK            CRM_GPIOA_PERIPH_CLOCK
#define QSPI_SRAM_D3_GPIO_PIN            GPIO_PINS_3
#define QSPI_SRAM_D3_GPIO_PINS_SOURCE    GPIO_PINS_SOURCE3
#define QSPI_SRAM_D3_GPIO_PORT           GPIOA
#define QSPI_SRAM_D3_GPIO_MUX            GPIO_MUX_9

/**
  * @brief qspi sram exported functions
  */
void qspi_sram_init(void);
void qspi_sram_data_read(uint32_t addr, uint8_t* buf, uint32_t total_len);
void qspi_sram_data_write(uint32_t addr, uint8_t* buf, uint32_t total_len);

#ifdef __cplusplus
}
#endif

#endif

