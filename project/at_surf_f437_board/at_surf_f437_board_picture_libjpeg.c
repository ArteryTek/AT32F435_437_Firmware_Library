/**
  **************************************************************************
  * @file     at_surf_f437_board_libjpeg.c
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

#include "jpeglib.h"

/* this struct contains the JPEG decompression parameters */
struct jpeg_decompress_struct cinfo;

/* this struct represents a JPEG error handler */
struct jpeg_error_mgr jerr;

uint32_t line_counter;

/**
  * @brief  draw point callback function.
  * @param  picture: image information structure.
  *         cinfo: decoder information structure.
  *         buf: data.
  *         data_len: data len.
  * @retval none.
  */
void libjpeg_display_function(j_decompress_ptr cinfo, picture_type *picture, uint8_t* buf, uint32_t data_len)
{
  uint32_t x, width;

  rgb888_to_rgb565(buf, data_len, picture->out_buf);

  if(cinfo->image_width > LCD_W)
  {
    width = LCD_W;
  }
  else
  {
    width = cinfo->image_width;
  }

  lcd_windows_set(0, line_counter, width, line_counter);

  for(x = 0; x < width; x++)
  {
    lcd_data_16bit_write(picture->out_buf[x]);
  }

  line_counter++;
}

/**
  * @brief  display .JPEG image through libjpeg.
  * @param  picture: image information structure.
  *         pname: image file path.
  *         x_pos: the x-axis displays the starting point.
  *         y_pos: the y-axis displays the starting point.
  * @retval ERROR: did not display successfully.
  *         SUCCESS: display completed successfully .
  */
error_status jpeg_libjpeg_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos)
{
  error_status status = SUCCESS;

  /* output row buffer */
  JSAMPROW buffer[2] = {0};

  /* display parameter initialization */
  picture->display_xpos = x_pos;
  picture->display_ypos = y_pos;

  /* malloc memory */
  picture->read_buf = at32_malloc(picture->read_buf_size);
  picture->out_buf  = at32_malloc(picture->out_buf_size);

  buffer[0] = picture->read_buf;

  /* open the specified file */
  if(f_open(&picture->file,(const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;

    goto end;
  }

  /* step 1: allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr);

  /* initialize the JPEG decompression object */
  jpeg_create_decompress(&cinfo);

  /* prepare for input from a stdio stream */
  jpeg_stdio_src(&cinfo, &picture->file);

  /* step 3: read image parameters with jpeg_read_header() */
  if(jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK)
  {
    status = ERROR;

    goto end;
  }

  /* step 4: set parameters for decompression */
  cinfo.dct_method = JDCT_FLOAT;
  cinfo.out_color_space = JCS_RGB;

  /* step 5: start decompressor */
  if(jpeg_start_decompress(&cinfo) != TRUE )
  {
    status = ERROR;

    goto end;
  }

  picture->image_width  = cinfo.image_width ;
  picture->image_height = cinfo.image_height;

  line_counter = 0;

  while(cinfo.output_scanline < cinfo.output_height)
  {
    /* image decoding */
    jpeg_read_scanlines(&cinfo, buffer, 1);

    libjpeg_display_function(&cinfo, picture, buffer[0], cinfo.image_width *3);
  }

  end:

  /* step 6: finish decompression */
  jpeg_finish_decompress(&cinfo);

  /* step 7: release jpeg decompression object */
  jpeg_destroy_decompress(&cinfo);

  /* close file */
  f_close(&picture->file);

  /* free memory */
  at32_free(picture->read_buf);
  at32_free(picture->out_buf);

  return status;
}


