/**
  **************************************************************************
  * @file     at_surf_f437_board_pwm_dac.h
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
#ifndef __AT_SURF_F437_BOARD_PWM_DAC_H
#define __AT_SURF_F437_BOARD_PWM_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

/**
  * @brief define dac io
  */

#define PWM_DAC_GPIO_PORT               GPIOA
#define PWM_DAC_GPIO_CLK                CRM_GPIOA_PERIPH_CLOCK
#define PWM_DAC_GPIO_PIN                GPIO_PINS_15
#define PWM_DAC_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE15
#define PWM_DAC_GPIO_MUX                GPIO_MUX_1

#define PWM_DAC_TMRx                    TMR2
#define PWM_DAC_TMR_CLK                 CRM_TMR2_PERIPH_CLOCK
#define PWM_DAC_TMR_CHANNEL             TMR_SELECT_CHANNEL_1


void pwm_dac_init(void);
void pwm_dac_output_voltage_set(uint16_t voltage);

#ifdef __cplusplus
}
#endif

#endif

