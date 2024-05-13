/**
  **************************************************************************
  * @file     run_in_qspi.c
  * @brief    run in qspi program
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
#include "run_in_qspi.h"


/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_QSPI_run_in_qspi_flash
  * @{
  */

/**
  * @brief  check the led toggle in qspi
  * @param  none
  * @retval none
  */
void qspi_run(void)
{
  while(1)
  {
    /* toggle led */
    at32_led_toggle(LED2);
    delay_ms(100);
    at32_led_toggle(LED3);
    delay_ms(100);
    at32_led_toggle(LED4);
    delay_ms(100);
  }
}

/**
  * @}
  */

/**
  * @}
  */
