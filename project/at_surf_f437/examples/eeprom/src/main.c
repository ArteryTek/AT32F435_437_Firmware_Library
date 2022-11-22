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
#include "at_surf_f437_board_eeprom.h"
#include "at_surf_f437_board_lcd.h"

#define BUF_SIZE  10
uint8_t tx_buf1[BUF_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
uint8_t tx_buf2[BUF_SIZE] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0};
uint8_t tx_buf3[BUF_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
uint8_t rx_buf1[BUF_SIZE] = {0};
uint8_t rx_buf2[BUF_SIZE] = {0};
uint8_t rx_buf3[BUF_SIZE] = {0};

/**
  * @brief  error handler program
  * @param  i2c_status
  * @retval none
  */
void error_handler(uint32_t error_code)
{
  lcd_string_show(10, 60, 310, 24, 24, (uint8_t *)"eeprom write/read error");

  while(1)
  {
  }
}

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
  i2c_status_type i2c_status;

  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* initialize eeprom */
  eeprom_init(EE_I2C_CLKCTRL_400K);

  /* display information */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"EEPROM Test");

  /* write data to eeprom through polling mode */
  if((i2c_status = eeprom_data_write(&hi2c_ee, EE_MODE_POLL, EE_I2C_ADDRESS, 0, tx_buf1, BUF_SIZE, EE_I2C_TIMEOUT)) != I2C_OK)
  {
    error_handler(i2c_status);
  }

  delay_ms(1);

  /* read data from eeprom through polling mode */
  if((i2c_status = eeprom_data_read(&hi2c_ee, EE_MODE_POLL, EE_I2C_ADDRESS, 0, rx_buf1, BUF_SIZE, EE_I2C_TIMEOUT)) != I2C_OK)
  {
    error_handler(i2c_status);
  }

  delay_ms(1);

  /* write data to eeprom through interrupt mode */
  if((i2c_status = eeprom_data_write(&hi2c_ee, EE_MODE_INT, EE_I2C_ADDRESS, 0, tx_buf2, BUF_SIZE, EE_I2C_TIMEOUT)) != I2C_OK)
  {
    error_handler(i2c_status);
  }

  delay_ms(1);

  /* read data from eeprom through interrupt mode */
  if((i2c_status = eeprom_data_read(&hi2c_ee, EE_MODE_INT, EE_I2C_ADDRESS, 0, rx_buf2, BUF_SIZE, EE_I2C_TIMEOUT)) != I2C_OK)
  {
    error_handler(i2c_status);
  }

  delay_ms(1);

  /* write data to eeprom through dma mode */
  if((i2c_status = eeprom_data_write(&hi2c_ee, EE_MODE_DMA, EE_I2C_ADDRESS, 0, tx_buf3, BUF_SIZE, EE_I2C_TIMEOUT)) != I2C_OK)
  {
    error_handler(i2c_status);
  }

  delay_ms(1);

  /* read data from eeprom through dma mode */
  if((i2c_status = eeprom_data_read(&hi2c_ee, EE_MODE_DMA, EE_I2C_ADDRESS, 0, rx_buf3, BUF_SIZE, EE_I2C_TIMEOUT)) != I2C_OK)
  {
    error_handler(i2c_status);
  }

  if((buffer_compare(tx_buf1, rx_buf1, BUF_SIZE) == SUCCESS) &&
     (buffer_compare(tx_buf2, rx_buf2, BUF_SIZE) == SUCCESS) &&
     (buffer_compare(tx_buf3, rx_buf3, BUF_SIZE) == SUCCESS))
  {
    lcd_string_show(10, 60, 310, 24, 24, (uint8_t *)"eeprom write/read ok");
  }
  else
  {
    error_handler(i2c_status);
  }

  while(1)
  {

  }
}
