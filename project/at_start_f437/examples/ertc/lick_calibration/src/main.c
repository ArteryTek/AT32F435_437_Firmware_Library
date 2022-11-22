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

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_ERTC_lick_calibration ERTC_lick_calibration
  * @{
  */

__IO uint32_t   lick_freq = 0;
__IO uint32_t   capture_number = 0;
__IO uint32_t   period_value = 0;

void ertc_config(void);
uint32_t lick_frequency_get(void);

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  ertc_time_type time;
  uint32_t temp = 0;

  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* at board initial */
  at32_board_init();

  /* initialize uart */
  uart_print_init(115200);

  /* ertc configuration */
  ertc_config();

  /* get the lick frequency: tmr5 is used to measure the lick frequency */
  lick_freq = lick_frequency_get();

  /* configure the ertc divider */
  /* ertc second(1hz) = ertc_clk(lick) / (div_a + 1) * (div_b + 1) */
  ertc_divider_set(127, (lick_freq / 128) - 1);

  printf("lick_freq = %d\r\n", lick_freq);
  printf("div_a     = %d\r\n", 127);
  printf("div_b     = %d\r\n", (lick_freq / 128) - 1);
  printf("\r\n");

  while(1)
  {
    /* get the current time */
    ertc_calendar_get(&time);

    if(temp != time.sec)
    {
      temp = time.sec;

      /* display date format : year-month-day */
      printf("%02d-%02d-%02d ",time.year, time.month, time.day);

      /* display time format : hour:min:sec */
      printf("%02d:%02d:%02d\r\n",time.hour, time.min, time.sec);
    }
  }
}

/**
  * @brief  configures the ertc peripheral by selecting the clock source.
  * @param  none
  * @retval none
  */
void ertc_config(void)
{
  /* enable the pwc clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* allow access to ertc */
  pwc_battery_powered_domain_access(TRUE);

  /* reset ertc domain */
  crm_battery_powered_domain_reset(TRUE);
  crm_battery_powered_domain_reset(FALSE);

  /* enable the lick osc */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

  /* wait till lick is ready */
  while(crm_flag_get(CRM_LICK_STABLE_FLAG) == RESET)
  {
  }

  /* select the ertc clock source */
  crm_ertc_clock_select(CRM_ERTC_CLOCK_LICK);

  /* enable the ertc clock */
  crm_ertc_clock_enable(TRUE);

  /* deinitializes the ertc registers */
  ertc_reset();

  /* wait for ertc apb registers update */
  ertc_wait_update();

  /* configure the ertc divider */
  ertc_divider_set(0x7F, 0xFF);

  /* configure the ertc hour mode */
  ertc_hour_mode_set(ERTC_HOUR_MODE_24);

  /* set date: 2021-05-01 */
  ertc_date_set(21, 5, 1, 5);

  /* set time: 12:00:00 */
  ertc_time_set(12, 0, 0, ERTC_AM);
}

/**
  * @brief  configures tmr5 to measure the lick oscillator frequency.
  * @param  none
  * @retval lick frequency
  */
uint32_t lick_frequency_get(void)
{
  tmr_input_config_type  tmr_input_config_struct;
  crm_clocks_freq_type   crm_clock_freq;

  /* enable tmr5 apb1 clocks */
  crm_periph_clock_enable(CRM_TMR5_PERIPH_CLOCK, TRUE);

  /* connect internally the tmr5_ch4 to the lick clock output */
  tmr_iremap_config(TMR5, TMR2_PTP_TMR5_LICK);

  /* configure tmr5 divider */
  tmr_div_value_set(TMR5, 0);
  tmr_event_sw_trigger(TMR5, TMR_OVERFLOW_SWTRIG);

  /* tmr5 channel4 input capture mode configuration */
  tmr_input_config_struct.input_channel_select = TMR_SELECT_CHANNEL_4;
  tmr_input_config_struct.input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;
  tmr_input_config_struct.input_polarity_select = TMR_INPUT_RISING_EDGE;
  tmr_input_config_struct.input_filter_value = 0;
  tmr_input_channel_init(TMR5, &tmr_input_config_struct, TMR_CHANNEL_INPUT_DIV_1);
  tmr_input_channel_divider_set(TMR5, TMR_SELECT_CHANNEL_4, TMR_CHANNEL_INPUT_DIV_8);

  /* enable tmr5 interrupt channel */
  nvic_irq_enable(TMR5_GLOBAL_IRQn, 0, 0);

  /* enable tmr5 counter */
  tmr_counter_enable(TMR5, TRUE);

  /* reset the flags */
  TMR5->swevt  = 0;

  /* enable the cc4 interrupt request */
  tmr_interrupt_enable(TMR5, TMR_C4_INT, TRUE);

  /* wait the tmr5 measuring operation to be completed */
  while(capture_number != 2);

  /* deinitialize the tmr5 peripheral */
  tmr_reset(TMR5);

  /* get the frequency value */
  crm_clocks_freq_get(&crm_clock_freq);

  /* get pclk1 divider */
  if ((CRM->cfg_bit.apb1div) > 0)
  {
    /* pclk1 divider different from 1 => timclk = 2 * pclk1 */
    return (((2 * crm_clock_freq.apb1_freq) / period_value) * 8) ;
  }
  else
  {
    /* pclk1 divider equal to 1 => timclk = pclk1 */
    return ((crm_clock_freq.apb1_freq / period_value) * 8);
  }
}

/**
  * @}
  */

/**
  * @}
  */
