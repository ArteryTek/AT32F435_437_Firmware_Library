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
#include "at_surf_f437_board_otg.h"
#include "at_surf_f437_board_lcd.h"

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  uint16_t data_len;
  uint32_t timeout;
  uint8_t send_zero_packet = 0;

  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* initialize otg */
  otg_init();

  /* display information */
  lcd_string_show(10, 20, 300, 24, 24, (uint8_t *)"OTG Test");

  /* display information */
  lcd_string_show(10, 50, 300, 24, 24, (uint8_t *)"Virtual Serial Port");

  while(1)
  {
    /* get usb vcp receive data */
    data_len = usb_vcp_get_rxdata(&otg_core_struct.dev, usb_buffer);

    if(data_len > 0 || send_zero_packet == 1)
    {
      /* display received data */
      lcd_fill(10, 80, 300, 140, BLACK);
      lcd_string_show(10, 80, 300, 24, 24, (uint8_t *)"Receive data:");
      lcd_string_show(10, 110, 300, 24, 24, usb_buffer);

      /* bulk transfer is complete when the endpoint does one of the following
         1 has transferred exactly the amount of data expected
         2 transfers a packet with a payload size less than wMaxPacketSize or transfers a zero-length packet
      */
      if(data_len > 0)
        send_zero_packet = 1;

      if(data_len == 0)
        send_zero_packet = 0;

      timeout = 5000000;
      do
      {
        /* send data to host */
        if(usb_vcp_send_data(&otg_core_struct.dev, usb_buffer, data_len) == SUCCESS)
        {
          break;
        }
      }while(timeout --);
    }
  }
}

