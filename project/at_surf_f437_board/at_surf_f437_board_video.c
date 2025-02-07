/**
  **************************************************************************
  * @file     at_surf_f437_board_player.c
  * @brief    video player.
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

#include "at_surf_f437_board_video.h"

int16_t music_output_buffer[DMA_BUFFER_SIZE];

uint8_t name_buf[FF_LFN_BUF + 1];
video_type video_info;

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
  * @brief  get the type of video file.
  * @param  name: video name.
  * @retval video file type.
  */
video_file_type video_type_get(uint8_t *name)
{
  if((str_find((const char *)name, ".avi") == SUCCESS) || (str_find((const char *)name, ".AVI") == SUCCESS))
  {
    return VIDEO_AVI;
  }

  return VIDEO_ERROR;
}


/**
  * @brief  get the number of valid video file.
  * @param  path: video file path.
  *         info: file information.
  * @retval number of valid video file found.
  */
uint16_t video_file_number_get(uint8_t *path, FILINFO *info)
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
      if((str_find(info->fname, ".avi")  == SUCCESS) ||
         (str_find(info->fname, ".AVI")  == SUCCESS))
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
  * @brief  get the video file names in the specified order.
  * @param  path: video file path.
  *         info: video file information.
  *         id: specified order.
  * @retval ERROR: specified order video not found.
  *         SUCCESS: specified order video found successfull.
  */
error_status video_name_get(uint8_t *path, FILINFO *info, uint16_t id)
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
      if((str_find(info->fname, ".avi")  == SUCCESS) ||
         (str_find(info->fname, ".AVI")  == SUCCESS) )
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
  * @brief  get player volume.
  * @param  none.
  * @retval volume.
  */
uint16_t volume_value_get(void)
{
  static uint16_t last = 0;
  uint16_t vol = 0;

  /* software filtering */
  last = last * 0.6 + VR_ADC->odt * 0.4;

  vol = last / 40.96 + 0.5;

  if(vol > 100)
  {
    vol = 100;
  }

  return vol;
}

/**
  * @brief  set play volume.
  * @param  none.
  * @retval none.
  */
void volume_value_set(void)
{
  static uint16_t vol_last = 0xFFFF;
  uint16_t vol;

  /* calculate play volume */
  vol = volume_value_get() * 1.59 + 96;

  /* the volume value changes */
  if(vol_last != vol)
  {
    /* set volume */
    audio_spk_volume_set(vol, vol);

    vol_last = vol;
  }
}

/**
  * @brief  set the frequency of audio.
  * @param  freq: frequency
  * @retval none.
  */
void audio_freq_set(uint32_t freq)
{
  static uint32_t freq_last = 0xFFFF;

  /* the frequency value changes */
  if(freq_last != freq)
  {
    switch(freq)
    {
      case 8000:   audio_codec_modify_freq(WM8988_REG_FREQ_8_K     ); break;
      case 11025:  audio_codec_modify_freq(WM8988_REG_FREQ_11_025_K); break;
      case 12000:  audio_codec_modify_freq(WM8988_REG_FREQ_12_K    ); break;
      case 16000:  audio_codec_modify_freq(WM8988_REG_FREQ_16K     ); break;
      case 22050:  audio_codec_modify_freq(WM8988_REG_FREQ_22_05K  ); break;
      case 24000:  audio_codec_modify_freq(WM8988_REG_FREQ_24K     ); break;
      case 32000:  audio_codec_modify_freq(WM8988_REG_FREQ_32_K    ); break;
      case 44100:  audio_codec_modify_freq(WM8988_REG_FREQ_44_1_K  ); break;
      case 48000:  audio_codec_modify_freq(WM8988_REG_FREQ_48_K    ); break;
      case 96000:  audio_codec_modify_freq(WM8988_REG_FREQ_96_K    ); break;
      default:break;
    }

    freq_last = freq;
  }
}

/**
  * @brief  init video struct.
  * @param  video: video information structure.
  * @retval none.
  */
void video_play_init(video_type *video)
{
  /* init video struct */
  video->last_sec = 0xFFFF;
  video->last_vol = 0xFFFF;

  video->decode_cnt = 0;
  video->total_sec = 0;
  video->current_sec = 0;

  video->channel = 0;
  video->samplerate = 0;

  video->key = VIDEO_NO_KEY;
}

/**
  * @brief  get key status.
  * @param  none.
  * @retval key value.
  *         VIDEO_NO_KEY
  *         VIDEO_KEY_PREVIOUS
  *         VIDEO_KEY_NEXT
  */
video_key_type video_key_get(void)
{
  key_type key;

  /* check button status */
  key = key_press();

  /* read io input status */
  pca9555_io_scan();

  if(key == KEY_1)
  {
    return VIDEO_KEY_PREVIOUS;
  }

  if(key == KEY_2)
  {
    return VIDEO_KEY_NEXT;
  }

  return VIDEO_NO_KEY;
}

/**
  * @brief  check key while playing.
  * @param  video: video information structure.
  * @retval paly status.
  *         VIDEO_PLAY_CONTINUE
  *         VIDEO_PLAY_STOP
  */
video_play_type video_play_stop_check(video_type *video)
{
  /* check button status */
  video->key = video_key_get();

  if(video->key != VIDEO_NO_KEY)
  {
    return VIDEO_PLAY_STOP;
  }

  return VIDEO_PLAY_CONTINUE;
}

/**
  * @brief  play video.
  * @param  video: video information structure.
  * @retval none
  */
void video_play(video_type *video)
{
  uint8_t buf[30];
  DIR video_dir;

  video->video_id = 0;

  /* find the video folder */
  while(f_opendir(&video_dir, "1:/VIDEO"))
  {
    lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"VIDEO file not find");
  }

  /* get the number of video files */
  video->video_number = video_file_number_get((uint8_t *)"1:/VIDEO", &video->file_info);

  /* no video files */
  while(video->video_number == 0)
  {
    lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"video file not found");
  }

  lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"video file found success");

  /* set volume */
  volume_value_set();

  while(1)
  {
    /* play video */
    if(video_name_get((uint8_t *)"1:/VIDEO", &video->file_info, video->video_id) == SUCCESS)
    {
      strcpy((char *)name_buf,"1:/VIDEO/");
      strcat((char *)name_buf,(const char*)video->file_info.fname);

      lcd_clear(BLACK);

      /* display the number of video files */
      sprintf((char *)buf, "%02d/%02d", video->video_id + 1, video->video_number);
      lcd_string_show(10, 270, 200, 24, 24, buf);

      /* show video name */
      lcd_string_show(10, 300, 319, 24, 24, (uint8_t *)video->file_info.fname);

      lcd_string_show(10, 415, 300, 24, 24, (uint8_t *)"key1: Previous");
      lcd_string_show(10, 440, 300, 24, 24, (uint8_t *)"key2: Next");

      video->type = video_type_get((uint8_t *)video->file_info.fname);

      video->read_buf_size = READ_BUF_SIZE;
      video->out_buf_size  = OUT_BUF_SIZE;
      video->lcd_buf_size  = LCD_BUF_SIZE;

      video_play_init(video);

      /* play video */
      switch(video->type)
      {
        case VIDEO_AVI:
          video_play_mjpeg(video, name_buf);
          break;

        default:
          break;
      }

      /* video switch */
      switch(video->key)
      {
        /* key 1 down */
        case VIDEO_KEY_PREVIOUS:

          if(video->video_id)
          {
            video->video_id--;
          }
          else
          {
            video->video_id = video->video_number - 1;
          }
          break;

        /* key 2 down */
        case VIDEO_KEY_NEXT:
        case VIDEO_NO_KEY:

          video->video_id++;

          if(video->video_id >= video->video_number)
          {
            video->video_id = 0;
          }
          break;

        default:
          break;
      }
    }
  }
}

