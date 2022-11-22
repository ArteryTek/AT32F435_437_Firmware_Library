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

/** @addtogroup 435_ADC_use_polling_get_conversion_data ADC_use_polling_get_conversion_data
  * @{
  */

__IO uint16_t adc1_ordinary_valuetab[3] = {0};
__IO uint16_t *p_adc1_ordinary = adc1_ordinary_valuetab;
__IO uint32_t adc1_overflow_flag = 0;

static void gpio_config(void);
static void adc_config(void);

/**
  * @brief  gpio configuration.
  * @param  none
  * @retval none
  */
static void gpio_config(void)
{
  gpio_init_type gpio_initstructure;
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_initstructure);

  /* config adc pin as analog input mode */
  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
  gpio_initstructure.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6;
  gpio_init(GPIOA, &gpio_initstructure);
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
  nvic_irq_enable(ADC1_2_3_IRQn, 0, 0);

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
  adc_base_struct.ordinary_channel_length = 3;
  adc_base_config(ADC1, &adc_base_struct);
  adc_resolution_set(ADC1, ADC_RESOLUTION_12B);

  /* config ordinary channel */
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4, 1, ADC_SAMPLETIME_640_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5, 2, ADC_SAMPLETIME_640_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6, 3, ADC_SAMPLETIME_640_5);

  /* config ordinary trigger source and trigger edge */
  adc_ordinary_conversion_trigger_set(ADC1, ADC_ORDINARY_TRIG_TMR1CH1, ADC_ORDINARY_TRIG_EDGE_NONE);

  /* config dma mode,it's not useful when common dma mode is use */
  adc_dma_mode_enable(ADC1, FALSE);

  /* config dma request repeat,it's not useful when common dma mode is use */
  adc_dma_request_repeat_enable(ADC1, FALSE);

  /* each ordinary channel conversion set occe flag */
  adc_occe_each_conversion_enable(ADC1, TRUE);

  /* enable adc overflow interrupt */
  adc_interrupt_enable(ADC1, ADC_OCCO_INT, TRUE);

  /* adc enable */
  adc_enable(ADC1, TRUE);
  while(adc_flag_get(ADC1, ADC_RDY_FLAG) == RESET);

  /* adc calibration */
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index = 0;
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* config the system clock */
  system_clock_config();

  /* init at start board */
  at32_board_init();
  at32_led_off(LED2);
  at32_led_off(LED3);
  at32_led_off(LED4);
  p_adc1_ordinary = adc1_ordinary_valuetab;
  uart_print_init(115200);
  gpio_config();
  adc_config();
  printf("use_polling_get_conversion_data \r\n");
  adc_ordinary_software_trigger_enable(ADC1, TRUE);
  while(1)
  {
    while(adc_flag_get(ADC1, ADC_OCCE_FLAG) == RESET);
    *(p_adc1_ordinary++) = adc_ordinary_conversion_data_get(ADC1);
    index++;
    if(adc1_overflow_flag != 0)
    {
      /* printf flag when error occur */
      at32_led_on(LED3);
      at32_led_on(LED4);
      printf("error occur\r\n");
      printf("adc1_overflow_flag = %d\r\n",adc1_overflow_flag);
      while(1);
    }
    if(index%3 == 0)
    {
      printf("conversion end without error\r\n");
      printf("adc1_ordinary_valuetab[0] = 0x%x\r\n", adc1_ordinary_valuetab[0]);
      printf("adc1_ordinary_valuetab[1] = 0x%x\r\n", adc1_ordinary_valuetab[1]);
      printf("adc1_ordinary_valuetab[2] = 0x%x\r\n", adc1_ordinary_valuetab[2]);
      printf("\r\n");
      delay_sec(1);
      p_adc1_ordinary = adc1_ordinary_valuetab;
      adc_ordinary_software_trigger_enable(ADC1, TRUE);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

