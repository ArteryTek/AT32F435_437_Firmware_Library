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

/** @addtogroup 435_PWC_deepsleep_ertc_tamper PWC_deepsleep_ertc_tamper
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
}

/**
  * @brief  ertc tamper configuration.
  * @param  none
  * @retval none
  */
void ertc_tamper_config(void)
{
  exint_init_type exint_init_struct;

  /* config the exint line of the ertc tamper */
  exint_init_struct.line_select   = EXINT_LINE_21;
  exint_init_struct.line_enable   = TRUE;
  exint_init_struct.line_mode     = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  /* enable tamper irqchannel */
  nvic_irq_enable(TAMP_STAMP_IRQn, 0, 0);

  /* disable the tamper 1 detection */
  ertc_tamper_enable(ERTC_TAMPER_1, FALSE);

  /* clear tamper 1 pin event(tamp1f) pending flag */
  ertc_flag_clear(ERTC_TP1F_FLAG);

  /* configure the tamper 1 trigger */
  ertc_tamper_valid_edge_set(ERTC_TAMPER_1, ERTC_TAMPER_EDGE_RISING);

  /* enable the tamper interrupt */
  ertc_interrupt_enable(ERTC_TP_INT, TRUE);

  /* enable the tamper 1 detection */
  ertc_tamper_enable(ERTC_TAMPER_1, TRUE);
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

  /* turn on the led light */
  at32_led_on(LED2);

  /* config ertc */
  ertc_config();

  /* set the wakeup time: 06h:20min:5s */
  ertc_tamper_config();

  /* save systick register configuration */
  systick_index = SysTick->CTRL;
  systick_index &= ~((uint32_t)0xFFFFFFFE);

  /* disable systick */
  SysTick->CTRL &= (uint32_t)0xFFFFFFFE;

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
  at32_led_on(LED3);

  /* wait clock stable */
  delay_us(120);

  /* resume ldo before system clock source enhance */
  pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V3);

  /* congfig the system clock */
  system_clock_recover();

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
