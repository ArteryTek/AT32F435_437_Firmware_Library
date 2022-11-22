/**
  **************************************************************************
  * @file     random.c
  * @brief    set of firmware functions to random function
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
#include "random.h"
#include <stdlib.h>

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup GEN_random_number_demo
  * @{
  */

/* define at32 mcu uid address */
#define DEVICE_ID_ADDR1 0x1FFFF7E8
ertc_time_type *time;

uint16_t ertc_clk_div_a = 0;
uint16_t ertc_clk_div_b = 0;

/**
  * @brief  get uid value as seed
  * @param  none
  * @retval uid_one_word
  */
int get_uid_for_seed (void)
{
  uint32_t   id[3] = {0};
  uint32_t   uid_one_word;

  /* get uid */
  id[0] = *(int*)DEVICE_ID_ADDR1;
  id[2] = *(int*)(DEVICE_ID_ADDR1+8);
  uid_one_word=id[0]&0x7fffffff+((id[2]&(~0x7fffffff))<<16);
  return uid_one_word;
}

/**
  * @brief  enable ertc as seed
  * @param  none
  * @retval none
  */
#if ENABLE_ERTC_ASSEED
void ertc_init_for_seed (void)
{
 /* enable the pwc clock interface */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* allow access to ertc */
  pwc_battery_powered_domain_access(TRUE);

  /* reset ertc domain */
  crm_battery_powered_domain_reset(TRUE);
  crm_battery_powered_domain_reset(FALSE);


  /* the ertc clock may varies due to lick frequency dispersion. */
  /* enable the lick osc */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

  /* wait till lick is ready */
  while(crm_flag_get(CRM_LICK_STABLE_FLAG) == RESET)
  {
  }

  /* select the ertc clock source */
  crm_ertc_clock_select(CRM_ERTC_CLOCK_LICK);

  /* ck_spre(1hz) = ertcclk(lick) /(ertc_clk_div_a + 1)*(ertc_clk_div_b + 1)*/
  ertc_clk_div_b = 0xFF;
  ertc_clk_div_a = 0x7F;


  /* enable the ertc clock */
  crm_ertc_clock_enable(TRUE);

  /* deinitializes the ertc registers */
  ertc_reset();

  /* wait for ertc apb registers synchronisation */
  ertc_wait_update();

  /* configure the ertc data register and ertc prescaler */
  ertc_divider_set(ertc_clk_div_a, ertc_clk_div_b);
  ertc_hour_mode_set(ERTC_HOUR_MODE_24);

  /* set the alarm 05h:20min:30s */
  ertc_alarm_mask_set(ERTC_ALA, ERTC_ALARM_MASK_DATE_WEEK);
  ertc_alarm_week_date_select(ERTC_ALA, ERTC_SLECT_DATE);
  ertc_alarm_set(ERTC_ALA, 31, 5, 20, 30, ERTC_AM);

  /* enable ertc alarm a interrupt */
  ertc_interrupt_enable(ERTC_ALA_INT, TRUE);

  /* enable the alarm */
  ertc_alarm_enable(ERTC_ALA, TRUE);

  ertc_flag_clear(ERTC_ALAF_FLAG);

  /* set the date: friday january 11th 2013 */
  ertc_date_set(13, 1, 11, 5);

  /* set the time to 05h 20mn 00s am */
  ertc_time_set(5, 20, 0, ERTC_AM);

  /* indicator for the ertc configuration */
  ertc_bpr_data_write(ERTC_DT1, 0x32F1);
}

#endif

/**
  * @brief  random number test
  * @param  none
  * @retval none
  */
void randnum_test( void)
{
#if ENABLE_ERTC_ASSEED

  if (ertc_bpr_data_read(ERTC_DT1) != 0x32F1)
  {
    ertc_init_for_seed();
  }
  ertc_calendar_get(time);
  /* set uid and ertc as seed for random */
  srand(ERTC->time + get_uid_for_seed());
#else
  /* set only  uid as seed for random */
  srand(get_uid_for_seed());
#endif
  while(1)
  {
    delay_ms(500);
    printf("%d\r\n",rand());
  }
}

/**
  * @}
  */

/**
  * @}
  */
