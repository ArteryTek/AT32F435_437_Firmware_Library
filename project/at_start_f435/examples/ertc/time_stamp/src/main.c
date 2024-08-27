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

/** @addtogroup 435_ERTC_time_stamp ERTC_time_stamp
  * @{
  */

void ertc_time_show(void);
void ertc_timestamp_show(void);
void ertc_config(void);
void ertc_timestamp_config(void);

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
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

  /* configure the timestamp register */
  ertc_timestamp_config();

  /* display the date and time */
  ertc_time_show();

  while(1)
  {

  }
}

/**
  * @brief  configure the ertc peripheral by selecting the clock source.
  * @param  none
  * @retval none
  */
void ertc_config(void)
{
  /* enable the pwc clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* allow access to ertc */
  pwc_battery_powered_domain_access(TRUE);

  /* reset ertc bpr domain */
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
  * @brief  configure the ertc timestamp.
  * @param  none
  * @retval none
  */
void ertc_timestamp_config(void)
{
  exint_init_type exint_init_struct;

  /* configure nvic */
  nvic_irq_enable(TAMP_STAMP_IRQn, 0, 0);

  /* configure exint */
  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable   = TRUE;
  exint_init_struct.line_mode     = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_select   = EXINT_LINE_21;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  /* enable the timestamp */
  ertc_timestamp_valid_edge_set(ERTC_TIMESTAMP_EDGE_FALLING);
  ertc_timestamp_enable(TRUE);

  /* enable the timestamp int */
  ertc_interrupt_enable(ERTC_TS_INT, TRUE);
}

/**
  * @brief  display the current date.
  * @param  none
  * @retval none
  */
void ertc_time_show(void)
{
  ertc_time_type time;

  /* get the current time */
  ertc_calendar_get(&time);

  /* display the curent time */
  printf("current time: ");

  /* display date format : year-month-day */
  printf("%02d-%02d-%02d ",time.year, time.month, time.day);

  /* display time format : hour:min:sec */
  printf("%02d:%02d:%02d\r\n\r\n",time.hour, time.min, time.sec);
}

/**
  * @brief  display the current timestamp.
  * @param  none
  * @retval none
  */
void ertc_timestamp_show(void)
{
  ertc_time_type time;

  /* get the current timestamp */
  ertc_timestamp_get(&time);

  /* display the current timestamp */
  printf("timestamp:    ");

  /* display date format : month-day */
  printf("   %02d-%02d ", time.month, time.day);

  /* display time format : hour:min:sec */
  printf("%02d:%02d:%02d\r\n", time.hour, time.min, time.sec);
}

/**
  * @}
  */

/**
  * @}
  */
