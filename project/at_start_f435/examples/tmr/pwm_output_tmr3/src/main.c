/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
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

#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_TMR_pwm_output_tmr3 TMR_pwm_output_tmr3
  * @{
  */

tmr_output_config_type tmr_oc_init_structure;
crm_clocks_freq_type crm_clocks_freq_struct = {0};

uint16_t ccr1_val = 333;
uint16_t ccr2_val = 249;
uint16_t ccr3_val = 166;
uint16_t ccr4_val = 83;
uint16_t prescalervalue = 0;

void crm_configuration(void);
void gpio_configuration(void);

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  /* system peripheral configuration */
  crm_configuration();

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  /* gpio configuration */
  gpio_configuration();

  /* compute the prescaler value */
  prescalervalue = (uint16_t) ((crm_clocks_freq_struct.apb1_freq * 2) / 24000000) - 1;

  /* tmr3 time base configuration */
  tmr_base_init(TMR3, 665, prescalervalue);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR3, TMR_CLOCK_DIV1);

  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_output_channel_config(TMR3, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_1, ccr1_val);
  tmr_output_channel_buffer_enable(TMR3, TMR_SELECT_CHANNEL_1, TRUE);

  tmr_output_channel_config(TMR3, TMR_SELECT_CHANNEL_2, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_2, ccr2_val);
  tmr_output_channel_buffer_enable(TMR3, TMR_SELECT_CHANNEL_2, TRUE);

  tmr_output_channel_config(TMR3, TMR_SELECT_CHANNEL_3, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_3, ccr3_val);
  tmr_output_channel_buffer_enable(TMR3, TMR_SELECT_CHANNEL_3, TRUE);

  tmr_output_channel_config(TMR3, TMR_SELECT_CHANNEL_4, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_4, ccr4_val);
  tmr_output_channel_buffer_enable(TMR3, TMR_SELECT_CHANNEL_4, TRUE);

  tmr_period_buffer_enable(TMR3, TRUE);

  /* tmr enable counter */
  tmr_counter_enable(TMR3, TRUE);

  while(1)
  {
  }
}

/**
  * @brief  configure the tmr1 pins.
  * @param  none
  * @retval none
  */
void gpio_configuration(void)
{
  gpio_init_type gpio_init_struct;

  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1;
  gpio_init(GPIOB, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE6, GPIO_MUX_2);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE7, GPIO_MUX_2);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE0, GPIO_MUX_2);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE1, GPIO_MUX_2);
}

/**
  * @brief  configures the different peripheral clocks.
  * @param  none
  * @retval none
  */
void crm_configuration(void)
{
  /* tmr3 clock enable */
  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);

  /* gpioa gpiob clock enable */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
}

/**
  * @}
  */

/**
  * @}
  */
