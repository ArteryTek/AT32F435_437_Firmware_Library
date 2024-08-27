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

/** @addtogroup 437_ERTC_tamper ERTC_tamper
  * @{
  */

#define ERTC_BPR_DT_NUMBER              20

ertc_dt_type bpr_addr_tab[ERTC_BPR_DT_NUMBER] =
{
  ERTC_DT1,  ERTC_DT2,  ERTC_DT3,  ERTC_DT4,  ERTC_DT5,
  ERTC_DT6,  ERTC_DT7,  ERTC_DT8,  ERTC_DT9,  ERTC_DT10,
  ERTC_DT11, ERTC_DT12, ERTC_DT13, ERTC_DT14, ERTC_DT15,
  ERTC_DT16, ERTC_DT17, ERTC_DT18, ERTC_DT19, ERTC_DT20,
};

void ertc_config(void);
void bpr_reg_write(void);
uint32_t bpr_reg_check(void);
uint32_t is_bpr_reg_reset(void);
void ertc_tamper_config(void);

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

  /* ertc tamper configuration */
  ertc_tamper_config();

  /* write to ertc bpr data registers */
  bpr_reg_write();

  /* check if the written data are correct */
  if(bpr_reg_check() == TRUE)
  {
    printf("init: bpr data registers are not reset\r\n");
  }
  else
  {
    printf("init: bpr data registers are cleared\r\n");
  }

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

  /* reset bpr domain */
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
  * @brief  configure the ertc tamper.
  * @param  none
  * @retval none
  */
void ertc_tamper_config(void)
{
  exint_init_type exint_init_struct;

  /* enable the external line 21 interrupt */
  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable   = TRUE;
  exint_init_struct.line_mode     = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_select   = EXINT_LINE_21;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  /* enable tamper interrupt */
  nvic_irq_enable(TAMP_STAMP_IRQn, 0, 0);

  /* disable the tamper 1 detection */
  ertc_tamper_enable(ERTC_TAMPER_1, FALSE);

  /* clear tamper 1 pin event(tamp1f) pending flag */
  ertc_flag_clear(ERTC_TP1F_FLAG);

  /* configure the tamper 1 trigger */
  ertc_tamper_valid_edge_set(ERTC_TAMPER_1, ERTC_TAMPER_EDGE_RISING);

  /* enable the tamper interrupt */
  ertc_interrupt_enable(ERTC_TP_INT, TRUE);

  /* clear tamper 1 pin interrupt pending bit */
  ertc_flag_clear(ERTC_TP1F_FLAG);

  /* enable the tamper 1 detection */
  ertc_tamper_enable(ERTC_TAMPER_1, TRUE);
}

/**
  * @brief  write data to bpr dt register.
  * @param  none.
  * @retval none
  */
void bpr_reg_write(void)
{
  uint32_t index = 0;

  for (index = 0; index < ERTC_BPR_DT_NUMBER; index++)
  {
    ertc_bpr_data_write(bpr_addr_tab[index], 0x5A00 + (index * 0x5A));
  }
}

/**
  * @brief  checks if the bpr dt register values are correct or not.
  * @param  none.
  * @retval
  *          - TRUE: all bpr dt register values are correct
  *          - FALSE: number of the bpr register
  *            which value is not correct
  */
uint32_t bpr_reg_check(void)
{
  uint32_t index = 0;

  for (index = 0; index < ERTC_BPR_DT_NUMBER; index++)
  {
    if (ertc_bpr_data_read(bpr_addr_tab[index]) != (0x5A00 + (index * 0x5A)))
    {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  * @brief  checks if the ertc bpr dtx registers are reset or not.
  * @param  none
  * @retval - 0: all ertc bpr dtx registers are reset
  *         - value different from 0: number of the first bpr register
  *           not reset
  */
uint32_t is_bpr_reg_reset(void)
{
  uint32_t index = 0;

  for (index = 0; index < ERTC_BPR_DT_NUMBER; index++)
  {
     /* read from bpr data register */
    if (ertc_bpr_data_read(bpr_addr_tab[index]) != 0)
    {
      return (index + 1);
    }
  }
  return 0;
}

/**
  * @}
  */

/**
  * @}
  */
