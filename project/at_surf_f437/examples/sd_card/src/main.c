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
#include "at_surf_f437_board_sd_card.h"
#include "at_surf_f437_board_lcd.h"
#include "ff.h"

FATFS fs;
FIL file;
BYTE work[FF_MAX_SS];
char filename[] = "1:/test1.txt";
const char write_buf[] = "this is my file for test fatfs!\r\n";
char read_buf[50];

/**
  * @brief  error handler program
  * @param  i2c_status
  * @retval none
  */
void error_handler(uint32_t error_code)
{
  lcd_string_show(10, 200, 310, 24, 24, (uint8_t *)"file write/read error");

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
  * @brief  sd card / fatfs file system init
  * @param  none
  * @retval error_status
  */
error_status file_system_init(void)
{
  FRESULT ret;

  /* registers work area to the fatfs module */
  ret = f_mount(&fs, "1:", 1);

  if(ret)
  {
    /* registers work area fail*/
    if(ret == FR_NO_FILESYSTEM)
    {
      /* create fatfs file system */
      ret = f_mkfs("1:", 0, work, sizeof(work));

      if(ret)
      {
        /* create fatfs file system fail*/
        return ERROR;
      }

      /* unregisters work area */
      ret = f_mount(NULL, "1:", 1);

      /* registers work area to the fatfs module */
      ret = f_mount(&fs, "1:", 1);

      if(ret)
      {
        /* registers work area fail */
        return ERROR;
      }
    }
    else
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
  FRESULT ret;
  UINT bytes_written = 0;
  UINT bytes_read = 0;
  DWORD fre_clust, fre_sect, tot_sect;
  FATFS* pt_fs;

  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* display information */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"SD Card Test");

  /* sd card / fatfs file system init */
  if(file_system_init() == SUCCESS)
  {
    lcd_string_show(10, 55, 280, 24, 24, (uint8_t *)"sd card init ok");
  }
  else
  {
    lcd_string_show(10, 55, 280, 24, 24, (uint8_t *)"sd card init error");
  }

  /* open file, if doesn't exist, create the file */
  if((ret = f_open(&file, filename, FA_READ | FA_WRITE | FA_CREATE_ALWAYS)) != 0)
  {
    error_handler(ret);
  }

  /* write data to file */
  if((ret = f_write(&file, write_buf, sizeof(write_buf), &bytes_written)) != 0)
  {
    error_handler(ret);
  }

  /* move pointer to the start */
  f_lseek(&file, 0);

  /* read data from file */
  if((ret = f_read(&file, read_buf, sizeof(read_buf), &bytes_read)) != 0)
  {
    error_handler(ret);
  }

  /* close file */
  if((ret = f_close(&file)) != 0)
  {
    error_handler(ret);
  }

  pt_fs = &fs;

  /* get volume information and free clusters of drive 1 */
  ret = f_getfree("1:", &fre_clust, &pt_fs);

  if(ret == FR_OK)
  {
    /* get total sectors and free sectors */
    tot_sect = (pt_fs->n_fatent - 2) * pt_fs->csize;
    fre_sect = fre_clust * pt_fs->csize;

    /* calculate capacity */
    tot_sect = tot_sect / 2 / 1024;
    fre_sect = fre_sect / 2 / 1024;

    /* display the total capacity */
    lcd_string_show(10, 100, 300, 24, 24, (uint8_t *)"card capacity:      MB");
    lcd_num_show(182, 100, 200, 24, 24, tot_sect, 1);

    /* display the free capacity */
    lcd_string_show(10, 130, 300, 24, 24, (uint8_t *)"free capacity:      MB");
    lcd_num_show(182, 130, 200, 24, 24, fre_sect, 1);
  }

  /* unregisters work area */
  ret = f_mount(NULL, "1:", 1);

  /* compare written and read data */
  if(buffer_compare((uint8_t*)read_buf, (uint8_t*)write_buf, sizeof(write_buf)) == SUCCESS)
  {
    lcd_string_show(10, 175, 310, 24, 24, (uint8_t *)"file write/read ok");
  }
  else
  {
    lcd_string_show(10, 175, 310, 24, 24, (uint8_t *)"file write/read fail");
  }

  while(1)
  {

  }
}
