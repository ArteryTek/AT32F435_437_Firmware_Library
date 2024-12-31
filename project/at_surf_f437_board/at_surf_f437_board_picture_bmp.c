/**
  **************************************************************************
  * @file     at_surf_f437_board_bmp.c
  * @brief    bmp image display.
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
  * GUARANTEES OR REPretENTATIONS OF ANY KIND. ARTERY EXPretSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPretS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPretENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at_surf_f437_board_picture.h"

/**
  * @brief  decoder drawing point callback function.
  * @param  pngle_t: pngle decpder structure.
  *         buf: data.
  *         data_len: data len.
  * @retval none.
  */
error_status bmp_callback_function(picture_type *picture, uint16_t* buf, uint32_t data_len)
{
  uint32_t x;

  lcd_windows_set(0, picture->image_line_cnt, picture->image_width, picture->image_line_cnt);

  for(x = 0; x < picture->image_width ; x++)
  {
    lcd_data_16bit_write(buf[x]);
  }

  return SUCCESS;
}

/**
  * @brief  display .BMP image.
  * @param  picture: image information structure.
  *         pname: image file path.
  *         x_pos: the x-axis displays the starting point.
  *         y_pos: the y-axis displays the starting point.
  * @retval ERROR: did not display successfully.
  *         SUCCESS: display completed successfully .
  */
error_status bmp_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos)
{
  UINT br;
  error_status status = SUCCESS;
  uint16_t high, row_data_len;

  /* display parameter initialization */
  picture->display_xpos = x_pos;
  picture->display_ypos = y_pos;

  /* malloc memory */
  picture->read_buf = at32_malloc(picture->read_buf_size);
  picture->out_buf  = at32_malloc(picture->out_buf_size);

  /* open the specified file */
  if(f_open(&picture->file, (const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;

    goto end;
  }

  /* read bmp header */
  if(f_read(&picture->file, (uint8_t * )&picture->bmp, 54, &br) != FR_OK)
  {
    status = ERROR;

    goto end;
  }

  f_lseek(&picture->file, picture->bmp.bfOffBits);

  row_data_len = picture->bmp.biBitCount / 8 * picture->bmp.biWidth;

  picture->image_width  = picture->bmp.biWidth;
  picture->image_height = picture->bmp.biHeight;

  for(high = 0; high < picture->bmp.biHeight; high++)
  {
    /* read bmp image */
    if(f_read(&picture->file, picture->read_buf, row_data_len, &br) != FR_OK)
    {
      status = ERROR;

      goto end;
    }

    switch(picture->bmp.biBitCount)
    {
      case 32: /* 32 bit bmp */
        rgba888_to_rgb565(picture->read_buf, row_data_len, picture->out_buf);
        break;
      case 24: /* 24 bit bmp */
        rgb888_to_rgb565(picture->read_buf, row_data_len, picture->out_buf);
        break;
      case 16: /* 16 bit bmp */

        /* RGB555 */
        if(picture->bmp.biCompression == BI_RGB)
        {
          rgb555_to_rgb565(picture->read_buf, row_data_len, picture->out_buf);
        }
        /* RGB565 */
        else
        {
          memcpy(picture->out_buf, picture->read_buf, row_data_len);
        }
        break;
      case  8: /* 8 bit bmp */
        goto end;

        default:break;
    }

    picture->image_line_cnt = picture->bmp.biHeight - high - 1;
    bmp_callback_function(picture, picture->out_buf, picture->bmp.biWidth);
  }

  end:

  /* close file */
  f_close(&picture->file);

  /* free memory */
  at32_free(picture->read_buf);
  at32_free(picture->out_buf);

  return status;
}

