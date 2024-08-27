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

/** @addtogroup 437_ERTC_wakeup_timer ERTC_wakeup_timer
  * @{
  */

void ertc_config(void);
void wakeup_timer_config(void);

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

  /* ertc wakeup timer configuration */
  wakeup_timer_config();

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
  * @brief  configure the ertc peripheral by selecting the clock source.
  * @param  none
  * @retval none
  */
void ertc_config(void)
{
  /* enable the pwc clock interface */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* allow access to ertc */
  pwc_battery_powered_domain_access(TRUE);

  /* reset ertc domain */
  crm_battery_powered_domain_reset(TRUE);
  crm_battery_powered_domain_reset(FALSE);

  /* enable the lext osc */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LEXT, TRUE);

  /* wait till lext is ready */
  while(crm_flag_get(CRM_LEXT_STABLE_FLAG) == RESET)
  {
  }

  /* select the ertc clock source */
  crm_ertc_clock_select(CRM_ERTC_CLOCK_LEXT);

  /* enable the ertc clock */
  crm_ertc_clock_enable(TRUE);

  /* deinitializes the ertc registers */
  ertc_reset();

  /* wait for ertc apb registers update */
  ertc_wait_update();

  /* configure the ertc divider */
  /* ertc second(1hz) = ertc_clk / (div_a + 1) * (div_b + 1) */
  ertc_divider_set(127, 255);

  /* configure the ertc hour mode */
  ertc_hour_mode_set(ERTC_HOUR_MODE_24);

  /* set date: 2021-05-01 */
  ertc_date_set(21, 5, 1, 5);

  /* set time: 12:00:00 */
  ertc_time_set(12, 0, 0, ERTC_AM);
}


/**
  * @brief  configure the ertc peripheral by selecting the clock source.
  * @param  none
  * @retval none
  */
void wakeup_timer_config(void)
{
  exint_init_type exint_init_struct;

  /* select the wakeup timer clock source */
  ertc_wakeup_clock_set(ERTC_WAT_CLK_CK_B_16BITS);

  /* set wakeup time: 5s */
  ertc_wakeup_counter_set(5 - 1);

  /* enable ertc wakeup timer interrupt */
  ertc_interrupt_enable(ERTC_WAT_INT, TRUE);

  /* enable the wakeup timer */
  ertc_wakeup_enable(TRUE);

  /* ertc wakeup timer interrupt configuration */
  /* exint configuration */
  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable   = TRUE;
  exint_init_struct.line_mode     = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_select   = EXINT_LINE_22;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  /* enable the ertc interrupt */
  nvic_irq_enable(ERTC_WKUP_IRQn, 0, 1);
}

/**
  * @}
  */

/**
  * @}
  */
