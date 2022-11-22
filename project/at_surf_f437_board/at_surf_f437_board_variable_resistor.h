/**
  **************************************************************************
  * @file     at_surf_f437_board_variable_resistor.h
  * @brief    variable resistor application libray header file
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
#ifndef __AT_SURF_F437_BOARD_VARIABLE_RESISTOR_H
#define __AT_SURF_F437_BOARD_VARIABLE_RESISTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at_surf_f437_board_pca9555.h"


#define VR_ADC                           ADC1
#define VR_ADC_CLK                       CRM_ADC1_PERIPH_CLOCK

#define VR_ADC_CHANNEL_GPIO_CLK          CRM_GPIOA_PERIPH_CLOCK
#define VR_ADC_CHANNEL_GPIO_PIN          GPIO_PINS_5
#define VR_ADC_CHANNEL_GPIO_PORT         GPIOA
#define VR_ADC_CHANNELx                  ADC_CHANNEL_5


void variable_resistor_init(void);

#ifdef __cplusplus
}
#endif

#endif

