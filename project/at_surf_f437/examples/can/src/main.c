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
#include "at_surf_f437_board_can.h"
#include "at_surf_f437_board_lcd.h"

/**
  * @brief  compare whether the valus of buffer 1 and buffer 2 are equal.
  * @param  buffer1: buffer 1 address.
            buffer2: buffer 2 address.
  * @retval the result of compare.
  */
error_status buffer_compare(uint8_t* buffer1, uint8_t* buffer2, uint32_t len)
{
  uint32_t i;

  for(i = 0; i < len; i++)
  {
    if(buffer1[i] != buffer2[i])
    {
      return ERROR;
    }
  }

  return SUCCESS;
}

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  uint32_t cnt = 0;

  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* can 1 init */
  can_init(CAN1);

  /* can 2 init */
  can_init(CAN2);

  /* display information */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"CAN Test");

  while(1)
  {
    /* can 1 data init */
    can_data_init(CAN1);

    /* transmit data through can 1 */
    can_transmit_data(CAN1, can1_tx_message);

    /* can 2 data init */
    can_data_init(CAN2);

    /* transmit data through can 2 */
    can_transmit_data(CAN2, can2_tx_message);

    /* can 1 receive data */
    if(can1_rx_flag != 0)
    {
      /* clear can 1 receive data flag */
      can1_rx_flag = 0;

      /* display title */
      lcd_string_show(10, 60, 200, 24, 24, (uint8_t *)"can 1 received");

      /* display filter index */
      lcd_string_show(10, 100, 200, 24, 24, (uint8_t *)"filter_index:");
      lcd_num_show(170, 100, 200, 24, 24, can1_rx_message.filter_index, 1);

      if (can1_rx_message.id_type == CAN_ID_EXTENDED )
      {
        /* display extended id */
        lcd_string_show(10, 130, 200, 24, 24, (uint8_t *)"extended_id:");
        lcd_num_show(170, 130, 200, 24, 24, can1_rx_message.extended_id, 1);
      }
      else
      {
        /* display standard id */
        lcd_string_show(10, 130, 200, 24, 24, (uint8_t *)"standard_id:");
        lcd_num_show(170, 130, 200, 24, 24, can1_rx_message.standard_id, 1);
      }

      if (can1_rx_message.frame_type == CAN_TFT_REMOTE )
      {
        /* display remote frame */
        lcd_string_show(10, 160, 200, 24, 24, (uint8_t *)"remote frame: no data");
      }
      else
      {
         /* data compare */
        if(buffer_compare(can2_tx_message.data, can1_rx_message.data, 8) == SUCCESS)
        {
          /* receive ok */
          lcd_string_show(10, 160, 310, 24, 24, (uint8_t *)"can 1 receive ok  ");
        }
        else
        {
          /* receive fail */
          lcd_string_show(10, 160, 310, 24, 24, (uint8_t *)"can 1 receive fail");
        }
      }
    }

    /* can 2 receive data */
    if(can2_rx_flag != 0)
    {
      /* clear can 2 receive data flag */
      can2_rx_flag = 0;

      /* display title */
      lcd_string_show(10, 220, 200, 24, 24, (uint8_t *)"can 2 received");

      /* display filter index */
      lcd_string_show(10, 260, 200, 24, 24, (uint8_t *)"filter_index:");
      lcd_num_show(170, 260, 200, 24, 24, can2_rx_message.filter_index, 1);

      if (can2_rx_message.id_type == CAN_ID_EXTENDED )
      {
        /* display extended id */
        lcd_string_show(10, 290, 200, 24, 24, (uint8_t *)"extended_id:");
        lcd_num_show(170, 290, 200, 24, 24, can2_rx_message.extended_id, 1);
      }
      else
      {
        /* display standard id */
        lcd_string_show(10, 290, 200, 24, 24, (uint8_t *)"standard_id:");
        lcd_num_show(170, 290, 200, 24, 24, can2_rx_message.standard_id, 1);
      }

      if (can2_rx_message.frame_type == CAN_TFT_REMOTE )
      {
        /* display remote frame */
        lcd_string_show(10, 320, 200, 24, 24, (uint8_t *)"remote frame: no data");
      }
      else
      {
         /* data compare */
        if(buffer_compare(can1_tx_message.data, can2_rx_message.data, 8) == SUCCESS)
        {
          /* receive ok */
          lcd_string_show(10, 320, 310, 24, 24, (uint8_t *)"can 2 receive ok  ");
        }
        else
        {
          /* receive fail */
          lcd_string_show(10, 320, 310, 24, 24, (uint8_t *)"can 2 receive fail");
        }
      }
    }

    cnt++;

    /* number of transfer */
    lcd_string_show(10, 380, 200, 24, 24, (uint8_t *)"transfer number:");
    lcd_num_show(206, 380, 250, 24, 24, cnt, 1);

    delay_ms(1000);
  }
}
