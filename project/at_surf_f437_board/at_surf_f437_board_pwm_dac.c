/**
  **************************************************************************
  * @file     at_surf_f437_board_pwm_dac.c
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

#include "at_surf_f437_board_pwm_dac.h"

/**
  * @brief  pwm dac init.
  * @param  none.
  * @retval none.
  */
void pwm_dac_init(void)
{
  gpio_init_type gpio_init_struct;
  tmr_output_config_type tmr_oc_init_structure;
  crm_clocks_freq_type   crm_clock_freq;

  /* enable tmr3/gpioa clock */
  crm_periph_clock_enable(PWM_DAC_TMR_CLK, TRUE);
  crm_periph_clock_enable(PWM_DAC_GPIO_CLK, TRUE);

  /* timer input pin Configuration */
  gpio_init_struct.gpio_pins = PWM_DAC_GPIO_PIN;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(PWM_DAC_GPIO_PORT, &gpio_init_struct);

  gpio_pin_mux_config(PWM_DAC_GPIO_PORT, PWM_DAC_GPIO_PINS_SOURCE, PWM_DAC_GPIO_MUX);

  /* get the system frequency value */
  crm_clocks_freq_get(&crm_clock_freq);

  /* tmr counter mode configuration (10kHz) */
  tmr_base_init(PWM_DAC_TMRx, 1000, crm_clock_freq.apb1_freq / 10000000 * 2);
  tmr_cnt_dir_set(PWM_DAC_TMRx, TMR_COUNT_UP);

  /* configure tmr channel */
  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_output_channel_config(PWM_DAC_TMRx, PWM_DAC_TMR_CHANNEL, &tmr_oc_init_structure);

  /* channel value set */
  tmr_channel_value_set(PWM_DAC_TMRx, PWM_DAC_TMR_CHANNEL, 0);

  /*output channel buffer enable */
  tmr_output_channel_buffer_enable(PWM_DAC_TMRx, PWM_DAC_TMR_CHANNEL, TRUE);

  /* period buffer enable */
  tmr_period_buffer_enable(PWM_DAC_TMRx, TRUE);

  /* enable pwm output */
  tmr_output_enable(PWM_DAC_TMRx, TRUE);

  /* enable tmr */
  tmr_counter_enable(PWM_DAC_TMRx, TRUE);
}

/**
  * @brief  pwm dac output voltage set.
  * @param  voltage: output voltage
  *         the range is 0~3300 representing 0~3.300V.
  * @retval none.
  */
void pwm_dac_output_voltage_set(uint16_t voltage)
{
  uint16_t temp;

  temp = (voltage / 3300.0) * 1000;

  tmr_channel_value_set(PWM_DAC_TMRx, PWM_DAC_TMR_CHANNEL, temp);
}

