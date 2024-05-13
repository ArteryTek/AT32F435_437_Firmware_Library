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
#include <string.h>

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_QSPI_xip_port_read_flash QSPI_xip_port_read_flash
  * @{
  */

extern void qspi_data_write(uint32_t addr, uint32_t total_len, uint8_t* buf);
extern void qspi_erase(uint32_t sec_addr);
extern void en25qh128a_qspi_xip_init(void);

#define TEST_SIZE                        4096
/* use dma transmit must align at word */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD uint8_t wbuf[TEST_SIZE] ALIGNED_TAIL;

#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD uint8_t rbuf[TEST_SIZE] ALIGNED_TAIL;

/**
  * @brief  qspi config
  * @param  none
  * @retval none
  */
void qspi_config(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the qspi clock */
  crm_periph_clock_enable(CRM_QSPI1_PERIPH_CLOCK, TRUE);

  /* enable the pin clock */
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure the io0 gpio */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE8, GPIO_MUX_10);

  /* configure the io1 gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE9, GPIO_MUX_10);

  /* configure the io2 gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE7, GPIO_MUX_9);

  /* configure the io3 gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE6, GPIO_MUX_9);

  /* configure the sck gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init(GPIOF, &gpio_init_struct);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE10, GPIO_MUX_9);

  /* configure the cs gpio */
  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init(GPIOG, &gpio_init_struct);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE6, GPIO_MUX_10);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  uint16_t i, err = 0;

  system_clock_config();
  at32_board_init();

  for(i = 0; i < TEST_SIZE; i++)
  {
    wbuf[i] = (uint8_t)i;
    rbuf[i] = 0;
  }
  /* qspi config */
  qspi_config();

  /* switch to cmd port */
  qspi_xip_enable(QSPI1, FALSE);

  /* set sclk */
  qspi_clk_division_set(QSPI1, QSPI_CLK_DIV_4);

  /* set sck idle mode 0 */
  qspi_sck_mode_set(QSPI1, QSPI_SCK_MODE_0);

  /* set wip in bit 0 */
  qspi_busy_config(QSPI1, QSPI_BUSY_OFFSET_0);

  /* enable auto ispc */
  qspi_auto_ispc_enable(QSPI1);

  /* erase */
  qspi_erase(0);

  /* program */
  qspi_data_write(0, TEST_SIZE, wbuf);

  /*configure xip mode*/
  en25qh128a_qspi_xip_init();
  
  /* read */
  memcpy(rbuf, (uint8_t*)QSPI1_MEM_BASE, TEST_SIZE);

  if(memcmp(rbuf, wbuf, TEST_SIZE))
  {
    err = 1;
  }

  while(1)
  {
    if(err == 0)
    {
      at32_led_toggle(LED3);
      delay_ms(300);
    }
    else
    {
      at32_led_toggle(LED2);
      delay_ms(300);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
