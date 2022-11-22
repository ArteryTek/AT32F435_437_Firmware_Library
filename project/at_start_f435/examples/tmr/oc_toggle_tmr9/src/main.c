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

/** @addtogroup 435_TMR_oc_toggle_tmr9 TMR_oc_toggle_tmr9
  * @{
  */

tmr_output_config_type tmr_oc_init_structure;
crm_clocks_freq_type crm_clocks_freq_struct = {0};
__IO uint16_t ccr1_val = 32768;
__IO uint16_t ccr2_val = 16384;
uint16_t prescalervalue = 0;

void crm_configuration(void);
void gpio_configuration(void);
void nvic_configuration(void);

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

  /* nvic configuration */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(TMR1_BRK_TMR9_IRQn, 1, 0);

  /* gpio configuration */
  gpio_configuration();

  /* ---------------------------------------------------------------------------
    tmr9 configuration: output compare toggle mode:
    tmr9clk = system_core_clock (240mhz),
    the objective is to get tmr9 counter clock at 24 mhz:
     - prescalervalue = (tmr9clk / tmr9 counter clock) - 1
    cc1 update rate = tmr9 counter clock / ccr1val / 2 = 366.2 hz
    cc2 update rate = tmr9 counter clock / ccr2val / 2 = 732.4 hz
  ----------------------------------------------------------------------------*/
  /* compute the prescaler value */
  prescalervalue = (uint16_t) ((crm_clocks_freq_struct.apb1_freq * 2) / 24000000) - 1;

  /* tmr base configuration */
  tmr_base_init(TMR9, 65535, prescalervalue);
  tmr_cnt_dir_set(TMR9, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR9, TMR_CLOCK_DIV1);

  /* output compare toggle mode configuration: channel1 */
  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_SWITCH;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_output_channel_config(TMR9, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_1, ccr1_val);

  /* output toggle mode configuration: channel2 */
  tmr_output_channel_config(TMR9, TMR_SELECT_CHANNEL_2, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, ccr2_val);

  /* tmr enable counter */
  tmr_counter_enable(TMR9, TRUE);

  /* tmr interrupt enable */
  tmr_interrupt_enable(TMR9, TMR_C1_INT | TMR_C2_INT | TMR_C3_INT | TMR_C4_INT, TRUE);

  while(1)
  {}
}

/**
  * @brief  configure the tmr9 pins.
  * @param  none
  * @retval none
  */
void gpio_configuration(void)
{
  gpio_init_type gpio_init_struct;

  /* gpioa configuration:tmr9 channel1 and 2 as alternate function push-pull */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_3;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

  gpio_init(GPIOA, &gpio_init_struct);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_3);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE3, GPIO_MUX_3);
}

/**
  * @brief  configures the different peripheral clocks.
  * @param  none
  * @retval none
  */
void crm_configuration(void)
{
  /* tmr9 clock enable */
  crm_periph_clock_enable(CRM_TMR9_PERIPH_CLOCK, TRUE);

  /* gpioa clock enable */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
}

/**
  * @}
  */

/**
  * @}
  */
