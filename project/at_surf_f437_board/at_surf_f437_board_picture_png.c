/**
  **************************************************************************
  * @file     at_surf_f437_board_png.c
  * @brief    png image display.
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
#include "pngle.h"

/**
  * @brief  decoder initialization callback function.
  * @param  pngle_t: pngle decpder structure.
  * @retval none.
  */
void png_init(pngle_t *pngle, uint32_t w, uint32_t h)
{
}

/**
  * @brief  decoder drawing point callback function.
  * @param  pngle_t: pngle decpder structure.
  *         x: the x-axis displays the starting point.
  *         y: the y-axis displays the starting point.
  *         w: image width.
  *         h: image height.
  * @retval none.
  */
void png_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
  lcd_point_draw(x, y, RGB888_TO_RGB565(rgba[0], rgba[1], rgba[2]));
}

/**
  * @brief  decoding completed callback function.
  * @param  picture: image information structure.
  *         pngle_t: pngle decpder structure.
  * @retval none.
  */
void png_finish(pngle_t *pngle)
{
}

/**
  * @brief  png decode.
  * @param  picture: image information structure.
  *         pngle_t: pngle decpder structure.
  * @retval none.
  */
void pngle_decode(picture_type *picture,  pngle_t *pngle)
{
  int32_t fed = 0, remain = 0;
  UINT len;

  pngle_set_init_callback(pngle, png_init);
  pngle_set_draw_callback(pngle, png_draw);
  pngle_set_done_callback(pngle, png_finish);
  pngle_set_display_gamma(pngle, 2.2);

  while(!f_eof(&picture->file))
  {
    if(remain >= picture->read_buf_size)
    {
      break;
    }

    /* read image */
    if(f_read(&picture->file, picture->read_buf + remain, picture->read_buf_size - remain, &len) != FR_OK)
    {
      break;
    }

    /* image decoding */
    fed = pngle_feed(pngle, picture->read_buf, remain + len);

    if(fed < 0)
    {
      break;
    }

    remain = remain + len - fed;

    if(remain > 0)
    {
      memmove(picture->read_buf, picture->read_buf + fed, remain);
    }
  }

  pngle_reset(pngle);
}

/**
  * @brief  display .ING image.
  * @param  picture: image information structure.
  *         pname: image file path.
  *         x_pos: the x-axis displays the starting point.
  *         y_pos: the y-axis displays the starting point.
  * @retval ERROR: did not display successfully.
  *         SUCCESS: display completed successfully .
  */
error_status png_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos)
{
  error_status status = SUCCESS;
  pngle_t *pngle;

  /* display parameter initialization */
  picture->display_xpos = x_pos;
  picture->display_ypos = y_pos;

  /* malloc memory */
  picture->read_buf = at32_malloc(picture->read_buf_size);
  pngle = at32_calloc(1, sizeof(pngle_t));

  /* open the specified file */
  if(f_open(&picture->file, (const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;

    goto end;
  }

  /* image decoding */
	pngle_decode(picture, pngle);

  end:

  /* close file */
  f_close(&picture->file);

  /* free memory */
  at32_free(picture->read_buf);
  at32_free(pngle);

  return status;
}
