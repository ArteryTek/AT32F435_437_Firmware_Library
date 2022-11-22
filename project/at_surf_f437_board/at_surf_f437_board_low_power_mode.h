/**
  **************************************************************************
  * @file     at_surf_f437_board_low_power_mode.h
  * @brief    low power mode application libray header file
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
#ifndef __AT_SURF_F437_BOARD_LOW_POWER_MODE_H
#define __AT_SURF_F437_BOARD_LOW_POWER_MODE_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

/**
  * @brief define wakeup pin io
  */
#define WAKEUP_GPIO_CLK                  CRM_GPIOA_PERIPH_CLOCK
#define WAKEUP_GPIO_PIN                  GPIO_PINS_0
#define WAKEUP_GPIO_PORT                 GPIOA
#define WAKEUP_EXINT_LINE                EXINT_LINE_0
#define WAKEUP_EXINT_PORT_SOURCE         SCFG_PORT_SOURCE_GPIOA
#define WAKEUP_EXINT_PINS_SOURCE         SCFG_PINS_SOURCE0
#define WAKEUP_EXINT_IRQn                EXINT0_IRQn
#define WAKEUP_EXINT_IRQHandler          EXINT0_IRQHandler


void wakeup_pin_init(void);

#ifdef __cplusplus
}
#endif

#endif

