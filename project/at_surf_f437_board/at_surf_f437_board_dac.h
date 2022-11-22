/**
  **************************************************************************
  * @file     at_surf_f437_board_dac.h
  * @brief    dac application libray header file
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
#ifndef __AT_SURF_F437_BOARD_DAC_H
#define __AT_SURF_F437_BOARD_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

/**
  * @brief define dac io
  */
#define DAC_CLK                          CRM_DAC_PERIPH_CLOCK
#define DACx                             DAC2
#define DAC_GPIO_CLK                     CRM_GPIOA_PERIPH_CLOCK
#define DAC_GPIO_PIN                     GPIO_PINS_5
#define DAC_GPIO_PORT                    GPIOA

void dac_init(void);
void dac_output_voltage_set(uint16_t voltage);

#ifdef __cplusplus
}
#endif

#endif

