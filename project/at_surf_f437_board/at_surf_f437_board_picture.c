/**
  **************************************************************************
  * @file     at_surf_f437_board_picture.c
  * @brief    picture display.
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

uint8_t name_buf[FF_LFN_BUF + 1];
picture_type picture_info;

/**
  * @brief  compare whether the valus of buffer 1 and buffer 2 are equal.
  * @param  buffer1: buffer 1 addrets.
  *         buffer2: buffer 2 addrets.
  * @retval ERROR: equal.
  *         SUCCESS: unequal.
  */
error_status buf_cmp(uint8_t* buffer1, uint8_t* buffer2, uint32_t len)
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
  * @brief  find equal strings in buf.
  * @param  buf: buffer to be searched.
  *         find_str: strings.
  * @retval ERROR: target strings not found.
  *         SUCCESS: target strings found successfull.
  */
error_status str_find(const char *buf, const char *find_str)
{
  uint32_t i;
  uint32_t buf_len, find_len;

  buf_len = strlen(buf);
  find_len = strlen(find_str);

  if(buf_len < find_len)
  {
    return ERROR;
  }

  buf_len = buf_len - find_len + 1;

  for(i = 0; i < buf_len; i++)
  {
    if(buf_cmp((uint8_t *)&buf[i], (uint8_t *)find_str, find_len) == SUCCESS)
    {
      return SUCCESS;
    }
  }

  return ERROR;
}

/**
  * @brief  get the type of picture file.
  * @param  name: picture name.
  * @retval picture file type.
  */
picture_file_type picture_type_get(uint8_t *name)
{
  if((str_find((const char *)name, ".bmp") == SUCCESS) || (str_find((const char *)name, ".BMP") == SUCCESS))
  {
    return PICTURE_BMP;
  }

  if((str_find((const char *)name, ".jpg") == SUCCESS) || (str_find((const char *)name, ".JPG") == SUCCESS) ||
     (str_find((const char *)name, ".jpeg") == SUCCESS) || (str_find((const char *)name, ".JPEG") == SUCCESS))
  {
    return PICTURE_JPEG;
  }

  if((str_find((const char *)name, ".png") == SUCCESS) || (str_find((const char *)name, ".PNG") == SUCCESS))
  {
    return PICTURE_PNG;
  }

  if((str_find((const char *)name, ".gif") == SUCCESS) || (str_find((const char *)name, ".GIF") == SUCCESS))
  {
    return PICTURE_GIF;
  }

  return PICTURE_ERROR;
}


/**
  * @brief  get the number of valid picture file.
  * @param  path: picture file path.
  *         info: file information.
  * @retval number of valid picture file found.
  */
uint16_t picture_file_number_get(uint8_t *path, FILINFO *info)
{
  FRESULT ret;
  DIR dir;
  uint16_t cnt = 0;

  /* open the specified path */
  ret = f_opendir(&dir,(const TCHAR*)path);

  if(ret == FR_OK)
  {
    while(1)
    {
      /* read the file information under the path */
      ret = f_readdir(&dir, info);

      if((ret != FR_OK) || (info->fname[0]==0))
      {
        break;
      }

      /* find files in mp3 format */
      if((str_find(info->fname, ".jpg" )  == SUCCESS) ||
         (str_find(info->fname, ".JPG" )  == SUCCESS) ||
         (str_find(info->fname, ".jpeg")  == SUCCESS) ||
         (str_find(info->fname, ".JPEG")  == SUCCESS) ||
         (str_find(info->fname, ".png" )  == SUCCESS) ||
         (str_find(info->fname, ".PNG" )  == SUCCESS) ||
         (str_find(info->fname, ".gif" )  == SUCCESS) ||
         (str_find(info->fname, ".GIF" )  == SUCCESS) ||
         (str_find(info->fname, ".bmp" )  == SUCCESS) ||
         (str_find(info->fname, ".BMP" )  == SUCCESS) )
      {
        cnt++;
      }
    }
  }

  /* close path */
  f_closedir(&dir);

  return cnt;
}

/**
  * @brief  get the picture file names in the specified order.
  * @param  path: picture file path.
  *         info: picture file information.
  *         id: specified order.
  * @retval ERROR: specified order picture not found.
  *         SUCCESS: specified order picture found successfull.
  */
error_status picture_name_get(uint8_t *path, FILINFO *info, uint16_t id)
{
  uint8_t ret;
  uint16_t cnt = 0;
  DIR dir;

  /* open the specified path */
  ret = f_opendir(&dir,(const TCHAR*)path);

  if(ret == FR_OK)
  {
    while(1)
    {
      /* read the file information under the path */
      ret = f_readdir(&dir, info);

      if((ret != FR_OK) || (info->fname[0]==0))
      {
        break;
      }

      /* find the corresponding serial number song */
      if((str_find(info->fname, ".jpg" ) == SUCCESS) ||
         (str_find(info->fname, ".JPG" ) == SUCCESS) ||
         (str_find(info->fname, ".jpeg") == SUCCESS) ||
         (str_find(info->fname, ".JPEG") == SUCCESS) ||
         (str_find(info->fname, ".png")  == SUCCESS) ||
         (str_find(info->fname, ".PNG")  == SUCCESS) ||
         (str_find(info->fname, ".gif")  == SUCCESS) ||
         (str_find(info->fname, ".GIF")  == SUCCESS) ||
         (str_find(info->fname, ".bmp" ) == SUCCESS) ||
         (str_find(info->fname, ".BMP" ) == SUCCESS))
      {
        if(id == cnt)
        {
          return SUCCESS;
        }

        cnt++;
      }
    }
  }

  return ERROR;
}

/**
  * @brief  convert rgb888 format to rgb565 format.
  * @param  rgb888: rgb888 bufer.
  *         rgb565: rgb565 bufer.
  *         rgb888_size: bufer size.
  * @retval count
  */
uint32_t rgb888_to_rgb565(uint8_t *rgb888, uint32_t rgb888_size, uint16_t *rgb565)
{
  uint32_t i = 0, count = 0;

  for(i = 0; i < rgb888_size; i += 3)
  {
    rgb565[count++] = (((rgb888[i + 2] & 0xF8) << 8) | ((rgb888[i + 1] & 0xFC) << 3) | (rgb888[i] >> 3));;
  }

  return count;
}

/**
  * @brief  convert argb888 format to rgb565 format.
  * @param  argb888: argb888 bufer.
  *         rgb565: rgb565 bufer.
  *         rgb888_size: bufer size.
  * @retval count
  */
uint32_t rgba888_to_rgb565(uint8_t *rgb888, uint32_t rgb888_size, uint16_t *rgb565)
{
  uint32_t i = 0, count = 0;

  /* data 0:B  1:G  2:R  3:A */
  for(i = 0; i < rgb888_size; i += 4)
  {
    rgb565[count++] = (((rgb888[i + 2] & 0xF8) << 8) | ((rgb888[i + 1] & 0xFC) << 3) | (rgb888[i] >> 3));;
  }

  return count;
}

/**
  * @brief  convert rgb555 format to rgb565 format.
  * @param  rgb555: rgb555 bufer.
  *         rgb565: rgb565 bufer.
  *         rgb555_size: bufer size.
  * @retval count
  */
uint32_t rgb555_to_rgb565(uint8_t *rgb555, uint32_t rgb555_size, uint16_t *rgb565)
{
  uint32_t i = 0, count = 0;

  uint16_t *temp = (uint16_t *)rgb555;

  for(i = 0; i < (rgb555_size/2); i++)
  {
    temp[i] =

    rgb565[i] = (((temp[i] & 0xFFE0) << 1) | (temp[i] & 0x001F));
  }

  return count;
}

/**
  * @brief  init fps tmer.
  * @param  none.
  * @retval none
  */
void fps_tmer_init(void)
{
  /* tmr clock enable */
  crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);

  tmr_32_bit_function_enable(TMR2, TRUE);

  /* tmr2 time base configuration */
  tmr_base_init(TMR2, 0XFFFFFFFF, system_core_clock / 1000000 - 1);
  tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR2, TMR_CLOCK_DIV1);

  /* tmr2 enable counter */
  tmr_counter_enable(TMR2, TRUE);
}

/**
  * @brief  fps tmer reset.
  * @param  none.
  * @retval none
  */
void fps_tmer_reset(void)
{
  tmr_counter_value_set(TMR2, 0);
}

/**
  * @brief  caclulate fps.
  * @param  picture: picture information structure.
  * @retval none
  */
void fps_caclulate(picture_type *picture)
{
  picture->fps = 1000000.0 / tmr_counter_value_get(TMR2);
}

/**
  * @brief  decoder infor picture.
  * @param  picture: picture information structure.
  *         decoder: decoder type.
  * @retval none
  */
void decoder_infor_display(picture_type *picture, decoder_type decoder)
{
  uint8_t buf[30];

  /* caclulate fps */
  fps_caclulate(picture);

  /* display the information of picture files */
  switch(decoder)
  {
    case DECODER_LIBJPEG:
      sprintf((char *)buf, "%02d/%02d fps:%.2f libjpeg", picture->picture_id + 1, picture->picture_number, picture->fps);
      lcd_string_show2(10, 428, 320, 24, 24, buf);
      break;
    case DECODER_TJPGD:
      sprintf((char *)buf, "%02d/%02d fps:%.2f tjpgd", picture->picture_id + 1, picture->picture_number, picture->fps);
      lcd_string_show2(10, 428, 320, 24, 24, buf);
      break;
    case DECODER_GIF:
      sprintf((char *)buf, "%02d/%02d", picture->picture_id + 1, picture->picture_number);
      lcd_string_show2(10, 428, 320, 24, 24, buf);
      break;
    default:
      sprintf((char *)buf, "%02d/%02d fps:%.2f", picture->picture_id + 1, picture->picture_number, picture->fps);
      lcd_string_show2(10, 428, 320, 24, 24, buf);
      break;
  }

  /* show picture name */
  lcd_string_show2(10, 452, 319, 24, 24, (uint8_t *)picture->file_info.fname);

  delay_ms(2000);

  lcd_clear(BLACK);
}

/**
  * @brief  display picture.
  * @param  picture: picture information structure.
  * @retval none
  */
void picture_display(picture_type *picture)
{
  DIR picture_dir;

  picture->picture_id = 0;

  /* initialize fps tmer */
  fps_tmer_init();

  /* find the picture folder */
  while(f_opendir(&picture_dir, "1:/PICTURE"))
  {
    lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"PICTURE file not find");
  }

  /* get the number of picture files */
  picture->picture_number = picture_file_number_get((uint8_t *)"1:/PICTURE", &picture->file_info);

  /* no picture files */
  while(picture->picture_number == 0)
  {
    lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"picture file not found");
  }

  lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"picture found success");

  while(1)
  {
    /* play picture */
    if(picture_name_get((uint8_t *)"1:/PICTURE", &picture->file_info, picture->picture_id) == SUCCESS)
    {
      strcpy((char *)name_buf,"1:/PICTURE/");
      strcat((char *)name_buf,(const char*)picture->file_info.fname);

      picture->file_type = picture_type_get((uint8_t *)picture->file_info.fname);

      picture->read_buf_size = READ_BUF_SIZE;
      picture->out_buf_size  = OUT_BUF_SIZE;

      fps_tmer_reset();

      /* play picture */
      switch(picture->file_type)
      {
        case PICTURE_BMP:
          bmp_decode(picture, name_buf, 0, 0);
          decoder_infor_display(picture, DECODER_BMP);
          break;
        case PICTURE_JPEG:
          jpeg_tjpgd_decode(picture, name_buf, 0, 0);
          decoder_infor_display(picture, DECODER_TJPGD);

          fps_tmer_reset();
          jpeg_libjpeg_decode(picture, name_buf, 0, 0);
          decoder_infor_display(picture, DECODER_LIBJPEG);
          break;
        case PICTURE_GIF:
          gif_decode(picture, name_buf, 0, 0, 320, 480);
          decoder_infor_display(picture, DECODER_GIF);
          break;
        case PICTURE_PNG:
          png_decode(picture, name_buf, 0, 0);
          decoder_infor_display(picture, DECODER_PNG);
          break;
        default:
          break;
      }

      picture->picture_id++;

      if(picture->picture_id >= picture->picture_number)
      {
        picture->picture_id = 0;
      }
    }
  }
}

