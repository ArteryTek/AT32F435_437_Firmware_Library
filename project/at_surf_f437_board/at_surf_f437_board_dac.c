/**
  **************************************************************************
  * @file     at_surf_f437_board_dac.c
  * @brief    the driver library of the dac
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

#include "at_surf_f437_board_dac.h"
#include "at_surf_f437_board_delay.h"

/**
  * @brief  dac init.
  * @param  none.
  * @retval none.
  */
void dac_init(void)
{
  gpio_init_type gpio_init_struct;

  /* enable dac/gpioa clock */
  crm_periph_clock_enable(DAC_CLK, TRUE);
  crm_periph_clock_enable(DAC_GPIO_CLK, TRUE);

  /* once the dac is enabled, the corresponding gpio pin is automatically
     connected to the dac converter. in order to avoid parasitic consumption,
     the gpio pin should be configured in analog */
  gpio_init_struct.gpio_pins = DAC_GPIO_PIN;
  gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(DAC_GPIO_PORT, &gpio_init_struct);

  /* dac2 configuration */
  dac_output_buffer_enable(DAC2_SELECT, FALSE);

  /* enable dac2: once the dac2 is enabled, pa.05 is
     automatically connected to the dac converter. */
  dac_enable(DAC2_SELECT, TRUE);
}

/**
  * @brief  dac output voltage set.
  * @param  voltage: output voltage
  *         the range is 0~3300 representing 0~3.300V.
  * @retval none.
  */
void dac_output_voltage_set(uint16_t voltage)
{
  uint16_t temp;

  temp = (voltage / 3300.0) * 4095;

  dac_2_data_set(DAC2_12BIT_RIGHT, temp);
}

