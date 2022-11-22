/**
  **************************************************************************
  * @file     at_surf_f437_board_calendar.c
  * @brief    the driver library of the calendar
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

#include "at_surf_f437_board_calendar.h"

__IO uint16_t ertc_clk_div_a = 0;
__IO uint16_t ertc_clk_div_b = 0;


/**
  * @brief  calendar init.
  * @param  none.
  * @retval none.
  */
void calendar_init(void)
{
  /* enable the pwc clock interface */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* allow access to ertc */
  pwc_battery_powered_domain_access(TRUE);

  if (ertc_bpr_data_read(ERTC_DT1) != 0x1234)
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

    /* indicator for the ertc configuration */
    ertc_bpr_data_write(ERTC_DT1, 0x1234);
  }
}
