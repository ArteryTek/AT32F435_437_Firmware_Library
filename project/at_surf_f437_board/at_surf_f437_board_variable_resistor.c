/**
  **************************************************************************
  * @file     at_surf_f437_board_variable_resistor.c
  * @brief    the driver library of the variable resistor
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

#include "at_surf_f437_board_variable_resistor.h"


/**
  * @brief  tmr configuration.
  * @param  none
  * @retval none
  */
void variable_resistor_tmr_init(void)
{
  tmr_output_config_type tmr_oc_init_structure;

  /* timer clock enable */
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

  /* trigger frequency: (systemclock / 28800) / 1000 = 10Hz */
  tmr_base_init(TMR1, 1000, 28800);

  /* config timer count direction */
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

  /* config timer clock source division */
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);

  /* trigger chananel config */
  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);

  /* chananel value set */
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 500);

  /* chananel enable */
  tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_1, TRUE);

  /* output enable */
  tmr_output_enable(TMR1, TRUE);
}

/**
  * @brief  adc configuration.
  * @param  none
  * @retval none
  */
void variable_resistor_adc_init(void)
{
  adc_common_config_type adc_common_struct;
  gpio_init_type gpio_initstructure;
  adc_base_config_type adc_base_struct;

  /* adc clock enable */
  crm_periph_clock_enable(VR_ADC_CLK, TRUE);

  /* adc channel gpio clock enable */
  crm_periph_clock_enable(VR_ADC_CHANNEL_GPIO_CLK, TRUE);

  gpio_default_para_init(&gpio_initstructure);
  /* config adc pin as analog input mode */
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull = GPIO_PULL_NONE;
  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
  gpio_initstructure.gpio_pins = VR_ADC_CHANNEL_GPIO_PIN;
  gpio_init(VR_ADC_CHANNEL_GPIO_PORT, &gpio_initstructure);

  adc_common_default_para_init(&adc_common_struct);

  /* config combine mode */
  adc_common_struct.combine_mode = ADC_INDEPENDENT_MODE;

  /* config division,adcclk is division by hclk */
  adc_common_struct.div = ADC_HCLK_DIV_4;

  /* config common dma mode,it's not useful in independent mode */
  adc_common_struct.common_dma_mode = ADC_COMMON_DMAMODE_DISABLE;

  /* config common dma request repeat */
  adc_common_struct.common_dma_request_repeat_state = FALSE;

  /* config adjacent adc sampling interval,it's useful for ordinary shifting mode */
  adc_common_struct.sampling_interval = ADC_SAMPLING_INTERVAL_5CYCLES;

  /* config inner temperature sensor and vintrv */
  adc_common_struct.tempervintrv_state = FALSE;

  /* config voltage battery */
  adc_common_struct.vbat_state = FALSE;
  adc_common_config(&adc_common_struct);

  adc_base_default_para_init(&adc_base_struct);

  adc_base_struct.sequence_mode = TRUE;
  adc_base_struct.repeat_mode = FALSE;
  adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
  adc_base_struct.ordinary_channel_length = 1;
  adc_base_config(VR_ADC, &adc_base_struct);
  adc_resolution_set(VR_ADC, ADC_RESOLUTION_12B);

  /* config ordinary channel */
  adc_ordinary_channel_set(VR_ADC, VR_ADC_CHANNELx, 1, ADC_SAMPLETIME_47_5);

  /* config ordinary trigger source and trigger edge */
  adc_ordinary_conversion_trigger_set(VR_ADC, ADC_ORDINARY_TRIG_TMR1CH1, ADC_ORDINARY_TRIG_EDGE_RISING);

  /* enable adc overflow interrupt */
  adc_interrupt_enable(VR_ADC, ADC_OCCO_INT, TRUE);

  /* set oversampling ratio and shift */
  adc_oversample_ratio_shift_set(VR_ADC, ADC_OVERSAMPLE_RATIO_8, ADC_OVERSAMPLE_SHIFT_3);

  /* disable ordinary oversampling trigger mode */
  adc_ordinary_oversample_trig_enable(VR_ADC, FALSE);

  /* set ordinary oversample restart mode */
  adc_ordinary_oversample_restart_set(VR_ADC, ADC_OVERSAMPLE_CONTINUE);

  /* enable ordinary oversampling */
  adc_ordinary_oversample_enable(VR_ADC, TRUE);


  /* adc enable */
  adc_enable(VR_ADC, TRUE);
  while(adc_flag_get(VR_ADC, ADC_RDY_FLAG) == RESET);

  /* adc calibration */
  adc_calibration_init(VR_ADC);
  while(adc_calibration_init_status_get(VR_ADC));

  adc_calibration_start(VR_ADC);
  while(adc_calibration_status_get(VR_ADC));
}

/**
  * @brief  variable resistor init.
  * @param  none.
  * @retval none.
  */
void variable_resistor_init(void)
{
  /* trigger timer init */
  variable_resistor_tmr_init();

  /* adc init */
  variable_resistor_adc_init();

  /* trigger timer enable */
  tmr_counter_enable(TMR1, TRUE);
}

