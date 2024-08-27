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

/** @addtogroup 435_ERTC_calendar ERTC_calendar
  * @{
  */

/* select the ertc clock source */
#define ERTC_CLOCK_SOURCE_LEXT           /* select lext as the ertc clock */
//#define ERTC_CLOCK_SOURCE_LICK         /* select lick as the ertc clock */

__IO uint16_t ertc_clk_div_a = 0;
__IO uint16_t ertc_clk_div_b = 0;

void ertc_config(void);
void ertc_alarm_show(void);
void ertc_time_show(void);

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  exint_init_type exint_init_struct;
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

  /* enable the pwc clock interface */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* allow access to ertc */
  pwc_battery_powered_domain_access(TRUE);

  if (ertc_bpr_data_read(ERTC_DT1) != 0x1234)
  {
    /* printf ertc status */
    printf("ertc has not been initialized\r\n\r\n");

    /* ertc configuration */
    ertc_config();
  }
  else
  {
    /* printf ertc status */
    printf("ertc has been initialized\r\n\r\n");

    /* wait for ertc registers update */
    ertc_wait_update();

    /* clear the ertc alarm flag */
    ertc_flag_clear(ERTC_ALAF_FLAG);

    /* clear the exint line 17 pending bit */
    exint_flag_clear(EXINT_LINE_17);
  }

  /* display the ertc alarm and time */
  ertc_time_show();
  ertc_alarm_show();

  printf("\r\n");

  /* ertc alarm interrupt configuration */
  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable   = TRUE;
  exint_init_struct.line_mode     = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_select   = EXINT_LINE_17;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  /* enable the ertc interrupt */
  nvic_irq_enable(ERTCAlarm_IRQn, 0, 1);

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

#if defined (ERTC_CLOCK_SOURCE_LICK)
  /* enable the lick osc */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

  /* wait till lick is ready */
  while(crm_flag_get(CRM_LICK_STABLE_FLAG) == RESET)
  {
  }

  /* select the ertc clock source */
  crm_ertc_clock_select(CRM_ERTC_CLOCK_LICK);

  /* ertc second(1hz) = ertc_clk(lick) / (ertc_clk_div_a + 1) * (ertc_clk_div_b + 1) */
  ertc_clk_div_b = 255;
  ertc_clk_div_a = 127;
#elif defined (ERTC_CLOCK_SOURCE_LEXT)
  /* enable the lext osc */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LEXT, TRUE);

  /* wait till lext is ready */
  while(crm_flag_get(CRM_LEXT_STABLE_FLAG) == RESET)
  {
  }

  /* select the ertc clock source */
  crm_ertc_clock_select(CRM_ERTC_CLOCK_LEXT);

  /* ertc second(1hz) = ertc_clk / (ertc_clk_div_a + 1) * (ertc_clk_div_b + 1) */
  ertc_clk_div_b = 255;
  ertc_clk_div_a = 127;
#endif

  /* enable the ertc clock */
  crm_ertc_clock_enable(TRUE);

  /* deinitializes the ertc registers */
  ertc_reset();

  /* wait for ertc registers update */
  ertc_wait_update();

  /* configure the ertc divider */
  ertc_divider_set(ertc_clk_div_a, ertc_clk_div_b);

  /* configure the ertc hour mode */
  ertc_hour_mode_set(ERTC_HOUR_MODE_24);

  /* set date: 2021-05-01 */
  ertc_date_set(21, 5, 1, 5);

  /* set time: 12:00:00 */
  ertc_time_set(12, 0, 0, ERTC_AM);

  /* set the alarm 12:00:10 */
  ertc_alarm_mask_set(ERTC_ALA, ERTC_ALARM_MASK_DATE_WEEK);
  ertc_alarm_week_date_select(ERTC_ALA, ERTC_SLECT_DATE);
  ertc_alarm_set(ERTC_ALA, 1, 12, 0, 10, ERTC_AM);

  /* enable ertc alarm a interrupt */
  ertc_interrupt_enable(ERTC_ALA_INT, TRUE);

  /* enable the alarm */
  ertc_alarm_enable(ERTC_ALA, TRUE);

  ertc_flag_clear(ERTC_ALAF_FLAG);

  /* indicator for the ertc configuration */
  ertc_bpr_data_write(ERTC_DT1, 0x1234);
}

/**
  * @brief  display the current time.
  * @param  none
  * @retval none
  */
void ertc_time_show(void)
{
  ertc_time_type time;

  /* get the current time */
  ertc_calendar_get(&time);

  /* display date format : year-month-day */
  printf("Time:  %02d-%02d-%02d ",time.year, time.month, time.day);

  /* display time format : hour:min:sec */
  printf("%02d:%02d:%02d\r\n",time.hour, time.min, time.sec);
}

/**
  * @brief  display the current alarm.
  * @param  none
  * @retval none
  */
void ertc_alarm_show(void)
{
  ertc_alarm_value_type alarm;

  /* get the current alarm */
  ertc_alarm_get(ERTC_ALA, &alarm);

  /* display alarm format : hour:min:sec */
  printf("Alarm: %02d:%02d:%02d\r\n",alarm.hour, alarm.min, alarm.sec);
}

/**
  * @}
  */

/**
  * @}
  */
