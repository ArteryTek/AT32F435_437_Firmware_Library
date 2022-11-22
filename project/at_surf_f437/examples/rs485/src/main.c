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

#include "at32f435_437.h"
#include "at32f435_437_clock.h"
#include "at_surf_f437_board_delay.h"
#include "at_surf_f437_board_rs485.h"
#include "at_surf_f437_board_lcd.h"

uint8_t tx_buf[RS485_RX_BUFFER_SIZE] = "AT32-SUFR-F437\r\n";
uint8_t rx_buf[RS485_RX_BUFFER_SIZE];

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

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* initialize rs485 */
  rs485_init();

  /* display information */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"RS485 Test");

  lcd_string_show(10, 60, 200, 24, 24, (uint8_t *)"RX data:");

  while(1)
  {
    /* data received */
    if(rs485_rx_number)
    {
      /* clear the end data to 0 in order to print */
      rx_buf[rs485_rx_number] = 0;

      /* copy data to rx_buf */
      rs485_data_receive(rx_buf, rs485_rx_number);

      /* clear the display area */
      lcd_fill(10, 100, 310, 124, BLACK);

      /* display the received data */
      lcd_string_show(10, 100, 200, 24, 24, rx_buf);

      /* send data */
      rs485_data_send(tx_buf, 17);
    }
  }
}

