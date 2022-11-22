/**
  **************************************************************************
  * @file     at_surf_f437_board_qspi_flash.h
  * @brief    qspi flash application libray header file
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
#ifndef __AT_SURF_F437_BOARD_QSPI_FLASH_H
#define __AT_SURF_F437_BOARD_QSPI_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

#define QSPI_FLASH_FIFO_DEPTH            (32*4)
#define QSPI_FLASH_PAGE_SIZE             256

#define QSPI_FLASH_QSPIx                 QSPI1
#define QSPI_FLASH_QSPI_CLK              CRM_QSPI1_PERIPH_CLOCK

#define QSPI_FLASH_CS_GPIO_CLK           CRM_GPIOG_PERIPH_CLOCK
#define QSPI_FLASH_CS_GPIO_PIN           GPIO_PINS_6
#define QSPI_FLASH_CS_GPIO_PINS_SOURCE   GPIO_PINS_SOURCE6
#define QSPI_FLASH_CS_GPIO_PORT          GPIOG
#define QSPI_FLASH_CS_GPIO_MUX           GPIO_MUX_10

#define QSPI_FLASH_CLK_GPIO_CLK          CRM_GPIOF_PERIPH_CLOCK
#define QSPI_FLASH_CLK_GPIO_PIN          GPIO_PINS_10
#define QSPI_FLASH_CLK_GPIO_PINS_SOURCE  GPIO_PINS_SOURCE10
#define QSPI_FLASH_CLK_GPIO_PORT         GPIOF
#define QSPI_FLASH_CLK_GPIO_MUX          GPIO_MUX_9

#define QSPI_FLASH_D0_GPIO_CLK           CRM_GPIOF_PERIPH_CLOCK
#define QSPI_FLASH_D0_GPIO_PIN           GPIO_PINS_8
#define QSPI_FLASH_D0_GPIO_PINS_SOURCE   GPIO_PINS_SOURCE8
#define QSPI_FLASH_D0_GPIO_PORT          GPIOF
#define QSPI_FLASH_D0_GPIO_MUX           GPIO_MUX_10

#define QSPI_FLASH_D1_GPIO_CLK           CRM_GPIOF_PERIPH_CLOCK
#define QSPI_FLASH_D1_GPIO_PIN           GPIO_PINS_9
#define QSPI_FLASH_D1_GPIO_PINS_SOURCE   GPIO_PINS_SOURCE9
#define QSPI_FLASH_D1_GPIO_PORT          GPIOF
#define QSPI_FLASH_D1_GPIO_MUX           GPIO_MUX_10

#define QSPI_FLASH_D2_GPIO_CLK           CRM_GPIOF_PERIPH_CLOCK
#define QSPI_FLASH_D2_GPIO_PIN           GPIO_PINS_7
#define QSPI_FLASH_D2_GPIO_PINS_SOURCE   GPIO_PINS_SOURCE7
#define QSPI_FLASH_D2_GPIO_PORT          GPIOF
#define QSPI_FLASH_D2_GPIO_MUX           GPIO_MUX_9

#define QSPI_FLASH_D3_GPIO_CLK           CRM_GPIOF_PERIPH_CLOCK
#define QSPI_FLASH_D3_GPIO_PIN           GPIO_PINS_6
#define QSPI_FLASH_D3_GPIO_PINS_SOURCE   GPIO_PINS_SOURCE6
#define QSPI_FLASH_D3_GPIO_PORT          GPIOF
#define QSPI_FLASH_D3_GPIO_MUX           GPIO_MUX_9

/**
  * @brief qspi flash exported functions
  */
void qspi_flash_init(void);
void qspi_flash_data_read(uint32_t addr, uint8_t* buf, uint32_t total_len);
void qspi_flash_data_write(uint32_t addr, uint8_t* buf, uint32_t total_len);
void qspi_flash_erase(uint32_t sec_addr);

#ifdef __cplusplus
}
#endif

#endif

