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
#include "at_surf_f437_board_sdram.h"
#include "at_surf_f437_board_lcd.h"

#define BUF_SIZE                         4096
uint16_t write_buf[BUF_SIZE];
uint16_t read_buf[BUF_SIZE];

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
  uint16_t i;

  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* initialize sdram */
  sdram_init();

  /* display information */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"SDRAM Test");

  /* initialize write buffer */
  for(i = 0; i < BUF_SIZE; i++)
  {
    write_buf[i] = i;
  }

  /* write data to sdram */
  sdram_data_write(0, write_buf, BUF_SIZE);

  /* read data from sdram */
  sdram_data_read(0, read_buf, BUF_SIZE);

  /* compare data */
  if(buffer_compare((uint8_t *)write_buf, (uint8_t *)read_buf, BUF_SIZE * 2) == SUCCESS)
  {
    lcd_string_show(10, 60, 310, 24, 24, (uint8_t *)"sdram write/read ok");
  }
  else
  {
    lcd_string_show(10, 60, 310, 24, 24, (uint8_t *)"sdram write/read error");
  }

  while(1)
  {

  }
}
