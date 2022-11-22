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

/** @addtogroup 435_PWC_standby_ertc_wakeup PWC_standby_ertc_alarm
  * @{
  */

/**
  * @brief  ertc configuration.
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
  while(crm_flag_get(CRM_LEXT_STABLE_FLAG) == RESET);

  /* select the ertc clock source */
  crm_ertc_clock_select(CRM_ERTC_CLOCK_LEXT);

  /* enable the ertc clock */
  crm_ertc_clock_enable(TRUE);

  /* deinitializes the ertc registers */
  ertc_reset();

  /* wait for ertc apb registers synchronisation */
  ertc_wait_update();

  /* configure the ertc data register and ertc prescaler
     ck_spre(1hz) = ertcclk(lext) /(ertc_clk_div_a + 1)*(ertc_clk_div_b + 1)*/
  ertc_divider_set(127, 255);

  /* configure the hour format is 24-hour format*/
  ertc_hour_mode_set(ERTC_HOUR_MODE_24);

  /* set the date: friday june 11th 2021 */
  ertc_date_set(21, 6, 11, 5);

  /* set the time to 06h 20mn 00s am */
  ertc_time_set(6, 20, 0, ERTC_AM);
}

/**
  * @brief  ertc alarm configuration.
  * @param  none
  * @retval none
  */
void ertc_alarm_config(void)
{
  /* set the alarm 05h:20min:10s */
  ertc_alarm_mask_set(ERTC_ALA, ERTC_ALARM_MASK_DATE_WEEK | ERTC_ALARM_MASK_HOUR | ERTC_ALARM_MASK_MIN);
  ertc_alarm_week_date_select(ERTC_ALA, ERTC_SLECT_DATE);

  /* enable ertc alarm a interrupt */
  ertc_interrupt_enable(ERTC_ALA_INT, TRUE);
}

/**
  * @brief  ertc alarm value set.
  * @param  alam_index : ERTC Alarm Seconds value
  * @retval none
  */
void ertc_alarm_value_set(uint32_t alam_index)
{
  ertc_time_type ertc_time_struct;

  /* disable the alarm */
  ertc_alarm_enable(ERTC_ALA, FALSE);
  ertc_calendar_get(&ertc_time_struct);
  ertc_alarm_set(ERTC_ALA, ertc_time_struct.day, ertc_time_struct.hour, ertc_time_struct.min, (ertc_time_struct.sec + alam_index)% 60, ertc_time_struct.ampm);

  /* disable the alarm */
  ertc_alarm_enable(ERTC_ALA, TRUE);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index = 0;

  /* congfig the system clock */
  system_clock_config();

  /* init at start board */
  at32_board_init();

  /* enable pwc and bpr clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  if(pwc_flag_get(PWC_STANDBY_FLAG) != RESET)
  {
    /* wakeup from standby */
    pwc_flag_clear(PWC_STANDBY_FLAG);
    at32_led_on(LED2);
  }
  if(pwc_flag_get(PWC_WAKEUP_FLAG) != RESET)
  {
    /* wakeup event occurs */
    pwc_flag_clear(PWC_WAKEUP_FLAG);
    at32_led_on(LED3);
  }
  delay_sec(1);
  /* config ertc */
  ertc_config();
  at32_led_on(LED4);
  ertc_alarm_config();
  delay_sec(1);
  ertc_alarm_value_set(3);

  /* enter standby mode */
  pwc_standby_mode_enter();
  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */
