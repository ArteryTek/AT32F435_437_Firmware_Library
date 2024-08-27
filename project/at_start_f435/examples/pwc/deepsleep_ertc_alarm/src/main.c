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

/** @addtogroup 435_PWC_deepsleep_ertc_alarm PWC_deepsleep_ertc_alarm
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
  exint_init_type exint_init_struct;

  /* config the exint line of the ertc alarm */
  exint_init_struct.line_select   = EXINT_LINE_17;
  exint_init_struct.line_enable   = TRUE;
  exint_init_struct.line_mode     = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  /* set the alarm 05h:20min:10s */
  ertc_alarm_mask_set(ERTC_ALA, ERTC_ALARM_MASK_DATE_WEEK | ERTC_ALARM_MASK_HOUR | ERTC_ALARM_MASK_MIN);
  ertc_alarm_week_date_select(ERTC_ALA, ERTC_SLECT_DATE);
  ertc_alarm_set(ERTC_ALA, 31, 6, 20, 5, ERTC_AM);

  /* enable the ertc interrupt */
  nvic_irq_enable(ERTCAlarm_IRQn, 0, 0);

  /* enable ertc alarm a interrupt */
  ertc_interrupt_enable(ERTC_ALA_INT, TRUE);

  /* enable the alarm */
  ertc_alarm_enable(ERTC_ALA, TRUE);
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
  * @brief  system clock recover.
  * @param  none
  * @retval none
  */
void system_clock_recover(void)
{
  /* enable external high-speed crystal oscillator - hext */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

  /* wait till hext is ready */
  while(crm_hext_stable_wait() == ERROR);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) == RESET);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t systick_index = 0;
  __IO uint32_t delay_index = 0;

  /* congfig the system clock */
  system_clock_config();

  /* init at start board */
  at32_board_init();

  /* config priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* enable pwc and bpr clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* config ertc */
  ertc_config();

  /* set the wakeup time: 06h:20min:5s */
  ertc_alarm_config();

  while(1)
  {
    /* turn off the led light */
    at32_led_off(LED2);

    /* save systick register configuration */
    systick_index = SysTick->CTRL;
    systick_index &= ~((uint32_t)0xFFFFFFFE);

    /* disable systick */
    SysTick->CTRL &= (uint32_t)0xFFFFFFFE;

    ertc_alarm_value_set(1);

    /* select system clock source as hick before ldo set */
    crm_sysclk_switch(CRM_SCLK_HICK);

    /* wait till hick is used as system clock source */
    while(crm_sysclk_switch_status_get() != CRM_SCLK_HICK)
    {
    }

    pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V0);

    /* congfig the voltage regulator mode */
    pwc_voltage_regulate_set(PWC_REGULATOR_LOW_POWER);

    /* enter deep sleep mode */
    pwc_deep_sleep_mode_enter(PWC_DEEP_SLEEP_ENTER_WFI);

    /* wake up from deep sleep mode, restore systick register configuration */
    SysTick->CTRL |= systick_index;

    /* turn on the led light */
    at32_led_on(LED2);

    /* wait clock stable */
    delay_us(120);

    /* resume ldo before system clock source enhance */
    pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V3);

    /* congfig the system clock */
    system_clock_recover();

    delay_ms(500);
  }
}

/**
  * @}
  */

/**
  * @}
  */
