/**
  **************************************************************************
  * @file     at_surf_f437_board_tgpgd.c
  * @brief    jpg image display.
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
#include "tjpgd.h"

JDEC TjpgLB_Jpeg;

/**
  * @brief  read data callback function.
  * @param  jd: decompressor object structure.
  *         buff: databufer.
  *         ndata: data number.
  * @retval none.
  */
UINT tjpgd_input_function( JDEC *jd, BYTE* buff, UINT ndata )
{
  FIL *jpeg = (FIL *)jd->device;
  UINT rb;

  if( buff )
  {
    f_read( jpeg, buff, ndata, &rb);

    return rb;
  }
  else
  {
    return (f_lseek( jpeg, f_tell(jpeg) + ndata) == FR_OK) ? ndata : 0;
  }
}

/**
  * @brief  draw point callback function.
  * @param  jd: decompressor object structure.
  *         rect: rectangular region in the output image.
  * @retval result.
  */
int tjpgd_display_function( JDEC *jd, void* bitmap, JRECT *rect )
{
  uint16_t *temp = (uint16_t *)bitmap;

  uint32_t x,y;

  lcd_windows_set(rect->left, rect->top, rect->right, rect->bottom);

  for(y = rect->top; y <= rect->bottom; y++)
  {
    for(x = rect->left; x <= rect->right; x++)
    {
      lcd_data_16bit_write(*temp);
      temp++;
    }
  }

  return 1;
}

/**
  * @brief  display .JPEG image through tjpgd.
  * @param  picture: image information structure.
  *         pname: image file path.
  *         x_pos: the x-axis displays the starting point.
  *         y_pos: the y-axis displays the starting point.
  * @retval ERROR: did not display successfully.
  *         SUCCESS: display completed successfully .
  */
error_status jpeg_tjpgd_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos)
{
  error_status status = SUCCESS;

  /* display parameter initialization */
  picture->display_xpos = x_pos;
  picture->display_ypos = y_pos;

  /* malloc memory */
  picture->read_buf = at32_malloc(picture->read_buf_size);

  /* open the specified file */
  if(f_open(&picture->file,(const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;

    goto end;
  }

  /* decoding prepare */
  if(jd_prepare(&TjpgLB_Jpeg, tjpgd_input_function, picture->read_buf, picture->read_buf_size, &picture->file) != JDR_OK)
  {
    status = ERROR;

    goto end;
  }

  /* image decoding */
  jd_decomp(&TjpgLB_Jpeg, tjpgd_display_function, 0 );

  end:

  /* close file */
  f_close(&picture->file);

  /* free memory */
  at32_free(picture->read_buf);

  return status;
}
