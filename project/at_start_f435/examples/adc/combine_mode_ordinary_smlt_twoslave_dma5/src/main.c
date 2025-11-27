/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *
  * Copyright (c) 2025, Artery Technology, All rights reserved.
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

/** @addtogroup 435_ADC_combine_mode_ordinary_smlt_twoslave_dma5 ADC_combine_mode_ordinary_smlt_twoslave_dma5
  * @{
  */

__IO uint32_t adccom_ordinary_valuetab[3][2];
__IO uint32_t dma1_trans_complete_flag = 0;
__IO uint32_t adc_conversion_times_index = 0;
__IO uint32_t adc1_overflow_flag = 0;
__IO uint32_t adc2_overflow_flag = 0;
__IO uint32_t adc3_overflow_flag = 0;
__IO uint32_t error_times_index = 0;

/**
  * @brief  gpio configuration.
  * @param  none
  * @retval none
  */
static void gpio_config(void)
{
  gpio_init_type gpio_initstructure;
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_initstructure);

  /* config adc pin as analog input mode */
  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
  gpio_initstructure.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init(GPIOA, &gpio_initstructure);

  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
  gpio_initstructure.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1;
  gpio_init(GPIOB, &gpio_initstructure);

  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
  gpio_initstructure.gpio_pins = GPIO_PINS_0 | GPIO_PINS_2 | GPIO_PINS_3;
  gpio_init(GPIOC, &gpio_initstructure);
}

/**
  * @brief  tmr1 configuration.
  * @param  none
  * @retval none
  */
static void tmr1_config(void)
{
  gpio_init_type gpio_initstructure;
  tmr_output_config_type tmr_oc_init_structure;
  crm_clocks_freq_type crm_clocks_freq_struct = {0};
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_initstructure);
  gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
  gpio_initstructure.gpio_pins = GPIO_PINS_8;
  gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull = GPIO_PULL_NONE;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_initstructure);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_1);

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

  /* (systemclock/(systemclock/10000))/10000 = 1Hz(1s) */
  tmr_base_init(TMR1, 9999, (crm_clocks_freq_struct.sclk_freq/10000 - 1));
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);

  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 5000);
  tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_1, TRUE);
  tmr_output_enable(TMR1, TRUE);
}

/**
  * @brief  dma configuration.
  * @param  none
  * @retval none
  */
static void dma_config(void)
{
  dma_init_type dma_init_struct;
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  nvic_irq_enable(DMA1_Channel1_IRQn, 0, 0);

  dma_reset(DMA1_CHANNEL1);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = 6;
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr = (uint32_t)adccom_ordinary_valuetab;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)&(ADCCOM->codt);
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_WORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(DMA1_CHANNEL1, &dma_init_struct);

  dmamux_enable(DMA1, TRUE);
  dmamux_init(DMA1MUX_CHANNEL1, DMAMUX_DMAREQ_ID_ADC1);

  /* enable dma transfer complete interrupt */
  dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, TRUE);
}

/**
  * @brief  adc configuration.
  * @param  none
  * @retval none
  */
static void adc_config(void)
{
  adc_common_config_type adc_common_struct;
  adc_base_config_type adc_base_struct;
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_ADC2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_ADC3_PERIPH_CLOCK, TRUE);
  adc_reset();
  nvic_irq_enable(ADC1_2_3_IRQn, 0, 0);
  adc_common_default_para_init(&adc_common_struct);

  /* config combine mode */
  adc_common_struct.combine_mode = ADC_ORDINARY_SMLT_ONLY_TWOSLAVE_MODE;

  /* config division,adcclk is division by hclk */
  adc_common_struct.div = ADC_HCLK_DIV_4;

  /* config common dma mode,it's useful for ordinary group in combine mode */
  adc_common_struct.common_dma_mode = ADC_COMMON_DMAMODE_5;

  /* config common dma request repeat */
  adc_common_struct.common_dma_request_repeat_state = TRUE;

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
  adc_base_struct.ordinary_channel_length = 3;
  adc_base_config(ADC1, &adc_base_struct);
  adc_resolution_set(ADC1, ADC_RESOLUTION_12B);

  /* config ordinary channel */
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4, 1, ADC_SAMPLETIME_47_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5, 2, ADC_SAMPLETIME_47_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6, 3, ADC_SAMPLETIME_47_5);

  /* config ordinary trigger source and trigger edge */
  adc_ordinary_conversion_trigger_set(ADC1, ADC_ORDINARY_TRIG_TMR1CH1, ADC_ORDINARY_TRIG_EDGE_RISING);

  /* config dma mode,it's not useful when common dma mode is use */
  adc_dma_mode_enable(ADC1, FALSE);

  /* config dma request repeat,it's not useful when common dma mode is use */
  adc_dma_request_repeat_enable(ADC1, FALSE);

  /* enable adc overflow interrupt */
  adc_interrupt_enable(ADC1, ADC_OCCO_INT, TRUE);

  adc_base_config(ADC2, &adc_base_struct);
  adc_resolution_set(ADC2, ADC_RESOLUTION_12B);
  adc_ordinary_channel_set(ADC2, ADC_CHANNEL_7, 1, ADC_SAMPLETIME_47_5);
  adc_ordinary_channel_set(ADC2, ADC_CHANNEL_8, 2, ADC_SAMPLETIME_47_5);
  adc_ordinary_channel_set(ADC2, ADC_CHANNEL_9, 3, ADC_SAMPLETIME_47_5);
  adc_ordinary_conversion_trigger_set(ADC2, ADC_ORDINARY_TRIG_TMR1CH1, ADC_ORDINARY_TRIG_EDGE_NONE);
  adc_dma_mode_enable(ADC2, FALSE);
  adc_dma_request_repeat_enable(ADC2, FALSE);
  adc_interrupt_enable(ADC2, ADC_OCCO_INT, TRUE);

  adc_base_config(ADC3, &adc_base_struct);
  adc_resolution_set(ADC3, ADC_RESOLUTION_12B);
  adc_ordinary_channel_set(ADC3, ADC_CHANNEL_10, 1, ADC_SAMPLETIME_47_5);
  adc_ordinary_channel_set(ADC3, ADC_CHANNEL_12, 2, ADC_SAMPLETIME_47_5);
  adc_ordinary_channel_set(ADC3, ADC_CHANNEL_13, 3, ADC_SAMPLETIME_47_5);
  adc_ordinary_conversion_trigger_set(ADC3, ADC_ORDINARY_TRIG_TMR1CH1, ADC_ORDINARY_TRIG_EDGE_NONE);
  adc_dma_mode_enable(ADC3, FALSE);
  adc_dma_request_repeat_enable(ADC3, FALSE);
  adc_interrupt_enable(ADC3, ADC_OCCO_INT, TRUE);

  /* adc enable */
  adc_enable(ADC1, TRUE);
  adc_enable(ADC2, TRUE);
  adc_enable(ADC3, TRUE);
  while(adc_flag_get(ADC1, ADC_RDY_FLAG) == RESET);
  while(adc_flag_get(ADC2, ADC_RDY_FLAG) == RESET);
  while(adc_flag_get(ADC3, ADC_RDY_FLAG) == RESET);

  /* adc calibration */
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));
  adc_calibration_init(ADC2);
  while(adc_calibration_init_status_get(ADC2));
  adc_calibration_start(ADC2);
  while(adc_calibration_status_get(ADC2));
  adc_calibration_init(ADC3);
  while(adc_calibration_init_status_get(ADC3));
  adc_calibration_start(ADC3);
  while(adc_calibration_status_get(ADC3));
}

/**
  * @brief  adc convert recovery process.
  * @param  none
  * @retval none
  */
void adc_convert_recovery_process(void)
{
  uint32_t recovery_index = 0;

  /* disable adc */
  adc_enable(ADC1, FALSE);
  adc_enable(ADC2, FALSE);
  adc_enable(ADC3, FALSE);

  /* record adc mode configuration */
  recovery_index = adc_combine_mode_get();

  /* clear adc mode configuration */
  adc_combine_mode_set(ADC_INDEPENDENT_MODE);

  /* reinitialize dma */
  dma_channel_enable(DMA1_CHANNEL1, FALSE);
  dma_flag_clear(DMA1_FDT1_FLAG);
  dma_data_number_set(DMA1_CHANNEL1, 6);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  /* recovery adc mode configuration */
  adc_combine_mode_set((adc_combine_mode_type)recovery_index);

  /* enable adc to detection trigger */
  adc_enable(ADC1, TRUE);
  adc_enable(ADC2, TRUE);
  adc_enable(ADC3, TRUE);
}

/**
  * @brief  this function handles dma1_channel1 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel1_IRQHandler(void)
{
  if(dma_interrupt_flag_get(DMA1_FDT1_FLAG) != RESET)
  {
    dma_flag_clear(DMA1_FDT1_FLAG);
    dma1_trans_complete_flag++;
  }
}

/**
  * @brief  this function handles adc1_2_3 handler.
  * @param  none
  * @retval none
  */
void ADC1_2_3_IRQHandler(void)
{
  if(adc_interrupt_flag_get(ADC1, ADC_OCCO_FLAG) != RESET)
  {
    adc_flag_clear(ADC1, ADC_OCCO_FLAG);
    adc1_overflow_flag++;

    /* to avoid data wrong,it is recommended to add the following recovery code */
    adc_convert_recovery_process();
  }
  if(adc_interrupt_flag_get(ADC2, ADC_OCCO_FLAG) != RESET)
  {
    adc_flag_clear(ADC2, ADC_OCCO_FLAG);
    adc2_overflow_flag++;

    /* to avoid data wrong,it is recommended to add the following recovery code */
    adc_convert_recovery_process();
  }
  if(adc_interrupt_flag_get(ADC3, ADC_OCCO_FLAG) != RESET)
  {
    adc_flag_clear(ADC3, ADC_OCCO_FLAG);
    adc3_overflow_flag++;

    /* to avoid data wrong,it is recommended to add the following recovery code */
    adc_convert_recovery_process();
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index1 = 0;
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* config the system clock */
  system_clock_config();

  /* init at start board */
  at32_board_init();
  at32_led_off(LED2);
  at32_led_off(LED3);
  at32_led_off(LED4);
  uart_print_init(115200);
  gpio_config();
  tmr1_config();
  dma_config();
  adc_config();

  /* enable DMA after ADC activation */
  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  printf("combine_mode_ordinary_smlt_twoslave_dma5 \r\n");
  tmr_counter_enable(TMR1, TRUE);
  while(1)
  {
    if(adc_conversion_times_index != dma1_trans_complete_flag)
    {
      /* printf data when conversion end without error */
      adc_conversion_times_index = dma1_trans_complete_flag;
      printf("adc_conversion_times_index = %d\r\n",adc_conversion_times_index);
      for(index1 = 0; index1 < 3; index1++)
      {
        printf("adccom_ordinary_valuetab[%d][0] = 0x%x\r\n",index1, adccom_ordinary_valuetab[index1][0]);
        printf("adccom_ordinary_valuetab[%d][1] = 0x%x\r\n",index1, adccom_ordinary_valuetab[index1][1]);
      }
      printf("\r\n");
      at32_led_toggle(LED2);
    }
    if(error_times_index != (adc1_overflow_flag + adc2_overflow_flag + adc3_overflow_flag))
    {
      /* printf flag when error occur */
      error_times_index = adc1_overflow_flag + adc2_overflow_flag + adc3_overflow_flag;
      at32_led_on(LED3);
      at32_led_on(LED4);
      printf("error occur\r\n");
      printf("error_times_index = %d\r\n",error_times_index);
      printf("adc1_overflow_flag = %d\r\n",adc1_overflow_flag);
      printf("adc2_overflow_flag = %d\r\n",adc2_overflow_flag);
      printf("adc3_overflow_flag = %d\r\n",adc3_overflow_flag);
      printf("\r\n");
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

