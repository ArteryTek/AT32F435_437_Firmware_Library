/**
  **************************************************************************
  * @file     at_surf_f437_board_player.c
  * @brief    music player.
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

#include "at_surf_f437_board_player.h"

int16_t music_output_buffer[DMA_BUFFER_SIZE];

uint8_t name_buf[FF_LFN_BUF + 1];
audio_type audio_info; 

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
  * @brief  get the type of music file.
  * @param  name: music name.
  * @retval music file type.
  */
music_file_type music_type_get(uint8_t *name)
{    
  if((str_find((const char *)name, ".mp3") == SUCCESS) || (str_find((const char *)name, ".MP3") == SUCCESS))
  {
    return MUSIC_MP3;
  } 
  
  if((str_find((const char *)name, ".wav") == SUCCESS) || (str_find((const char *)name, ".WAV") == SUCCESS))
  {
    return MUSIC_WAV;
  }

  if((str_find((const char *)name, ".ape") == SUCCESS) || (str_find((const char *)name, ".APE") == SUCCESS))
  {
    return MUSIC_APE;
  }
  
  if((str_find((const char *)name, ".flac") == SUCCESS) || (str_find((const char *)name, ".FLAC") == SUCCESS))
  {
    return MUSIC_FLAC;
  }
  
  return MUSIC_ERROR;
}


/**
  * @brief  get the number of valid music file.
  * @param  path: music file path.
  *         info: file information.
  * @retval number of valid music file found.
  */
uint16_t music_file_number_get(uint8_t *path, FILINFO *info)
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
      if((str_find(info->fname, ".mp3")  == SUCCESS) || 
         (str_find(info->fname, ".MP3")  == SUCCESS) || 
         (str_find(info->fname, ".wav")  == SUCCESS) || 
         (str_find(info->fname, ".WAV")  == SUCCESS) ||
         (str_find(info->fname, ".ape")  == SUCCESS) || 
         (str_find(info->fname, ".APE")  == SUCCESS) || 
         (str_find(info->fname, ".flac") == SUCCESS) || 
         (str_find(info->fname, ".FLAC") == SUCCESS))
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
  * @brief  get the music file names in the specified order.
  * @param  path: music file path.
  *         info: music file information.
  *         id: specified order.
  * @retval ERROR: specified order music not found.
  *         SUCCESS: specified order music found successfull.
  */
error_status music_name_get(uint8_t *path, FILINFO *info, uint16_t id)
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
      if((str_find(info->fname, ".mp3")  == SUCCESS) || 
         (str_find(info->fname, ".MP3")  == SUCCESS) || 
         (str_find(info->fname, ".wav")  == SUCCESS) || 
         (str_find(info->fname, ".WAV")  == SUCCESS) ||
         (str_find(info->fname, ".ape")  == SUCCESS) || 
         (str_find(info->fname, ".APE")  == SUCCESS) || 
         (str_find(info->fname, ".flac") == SUCCESS) || 
         (str_find(info->fname, ".FLAC") == SUCCESS))
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
  * @brief  set the frequency of music playback.
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
  * @brief  audio data conversion.
  * @param  audio: audio information structure.
  *         decode_left: left channel data.
  *         decode_right: right channel data.
  *         sample: number of data.
  * @retval none.
  */
void audio_data_convert_to_i2s(audio_type *audio, int32_t *decode_left, int32_t *decode_right, uint32_t sample)
{
  uint32_t i = 0;
  int32_t *left_ch, *right_ch;
  
  left_ch = decode_left;
  right_ch = decode_right;
  
  /* waiting for the completion of the previous frame data transmission */
  audio_wait_data_tx_end();
  
  while (sample--) 
  {
    /* output sample(s) in 16-bit signed little-endian pcm */
    music_output_buffer[i++] = ((*left_ch++) >> (audio->bps - 16)) & 0xFFFF;
    
    if(audio->channel == 2) 
    {
      music_output_buffer[i++] = ((*right_ch++) >> (audio->bps - 16)) & 0xFFFF;
    }
    else
    {
      i++;
    }
  }  
}

/**
  * @brief  init audio struct.
  * @param  audio: audio information structure.
  * @retval none.
  */
void music_play_init(audio_type *audio)
{
  /* init audio struct */ 
  audio->decode_cnt = 0;
  audio->error_cnt = 0;
  audio->file_size = 0;
  audio->decode_size = 0;
  audio->total_sec = 0;
  audio->current_sec = 0;
  
  audio->bps = 0;
  audio->channel = 0;
  audio->samplerate = 0;
  audio->totalsamples = 0;

  audio->tx_size = 0;
  
  audio->key = MUSIC_NO_KEY;
}

/**
  * @brief  get key status.
  * @param  none.
  * @retval key value.
  *         MUSIC_NO_KEY      
  *         MUSIC_KEY_PREVIOUS
  *         MUSIC_KEY_NEXT    
  *         MUSIC_KEY_PAUSE   
  */
music_key_type music_key_get(void)
{
  key_type key;
  joy_type joy_ley;
  
  /* check button status */
  key = key_press();
  
  /* read io input status */
  pca9555_io_scan();

  /* read joystick key value */
  joy_ley = joystick_press();
  
  if(key == KEY_1)
  {
    return MUSIC_KEY_PREVIOUS;
  }
  
  if(key == KEY_2)
  {
    return MUSIC_KEY_NEXT;
  }
  
  if(joy_ley == JOY_ENTER)
  {
    return MUSIC_KEY_PAUSE;
  }
  
  return MUSIC_NO_KEY;
}

/**
  * @brief  check key while playing.
  * @param  audio: audio information structure.
  * @retval paly status.
  *         MUSIC_PLAY_CONTINUE
  *         MUSIC_PLAY_STOP       
  */
music_play_type music_play_stop_check(audio_type *audio)
{
  /* check button status */
  audio->key = music_key_get();
  
  if(audio->key == MUSIC_KEY_PAUSE)
  {
    /* clear i2s data buffer */
    memset(music_output_buffer, 0, sizeof(music_output_buffer));
    
    lcd_string_show(100, 180, 300, 24, 24, (uint8_t *)"暂停"); 
    
    while(1)
    {
      /* check button status */
      audio->key = music_key_get();
      
      if(audio->key == MUSIC_KEY_PAUSE)
      {
        lcd_string_show(100, 180, 300, 24, 24, (uint8_t *)"    "); 
        break;
      }
      
      if(audio->key != MUSIC_NO_KEY)   
      {
        lcd_string_show(100, 180, 300, 24, 24, (uint8_t *)"    "); 
        return MUSIC_PLAY_STOP;
      }        
    }
  }
  else if(audio->key != MUSIC_NO_KEY) 
  {
    return MUSIC_PLAY_STOP;
  } 

  return MUSIC_PLAY_CONTINUE;
}

/**
  * @brief  display music information.
  * @param  audio: audio information structure.
  * @retval none.
  */
void music_info_display(audio_type *audio)
{
  uint8_t buf[30];
  
  /* display wav file information */
  if(audio->decode_cnt == 1)
  {
    if(audio->music_type == MUSIC_MP3)
    {
      sprintf((char *)buf, "%d bit per second", audio->bps);    
    }
    else
    {
      sprintf((char *)buf, "%d bit per sample", audio->bps);    
    }

    lcd_string_show(10, 240, 300, 24, 24, buf);  

    sprintf((char *)buf, "%d channel", audio->channel);
    lcd_string_show(10, 270, 300, 24, 24, buf);   
    
    sprintf((char *)buf, "%d hz sample rate", audio->samplerate);
    lcd_string_show(10, 300, 300, 24, 24, buf); 
  }
  
  /* show play progress */
  sprintf((char *)buf, "%02d:%02d", audio->total_sec / 60, audio->total_sec % 60);

  lcd_string_show(100, 110, 200, 24, 24, buf);  
  
  /* show play progress */
  sprintf((char *)buf, "%02d:%02d", audio->current_sec / 60, audio->current_sec % 60);

  lcd_string_show(10, 180, 200, 24, 24, buf);  
  
  /* show volume */
  sprintf((char *)buf, "Volume:%d  ", volume_value_get());
  
  lcd_string_show(185, 110, 200, 24, 24, buf);  
}

/**
  * @brief  play music.
  * @param  audio: audio information structure.
  * @retval none
  */
void music_play(audio_type *audio)
{
  uint8_t buf[30];
  DIR music_dir;

  audio->music_id = 0; 

  /* find the music folder */
  while(f_opendir(&music_dir, "1:/MUSIC"))
  {
    lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"MUSIC file not find");  
  }
     
  /* get the number of music files */
  audio->music_number = music_file_number_get((uint8_t *)"1:/MUSIC", &audio->file_info);

  /* no music files */
  while(audio->music_number == 0)
  {      
    lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"music file not found");        
  }

  lcd_string_show(10, 55, 300, 24, 24, (uint8_t *)"music file found success");        
  
  /* set volume */
  volume_value_set();
  
  while(1)
  {
    /* play music */
    if(music_name_get((uint8_t *)"1:/MUSIC", &audio->file_info, audio->music_id) == SUCCESS)
    {
      strcpy((char *)name_buf,"1:/MUSIC/");
      strcat((char *)name_buf,(const char*)audio->file_info.fname); 

      /* display the number of music files */
      sprintf((char *)buf, "%02d/%02d", audio->music_id + 1, audio->music_number);
      lcd_string_show(10, 110, 200, 24, 24, buf);    

      /* show music name */
      lcd_fill(10, 145, 319, 160 + 24, BLACK);
      lcd_string_show(10, 145, 319, 24, 24, (uint8_t *)audio->file_info.fname);  
      
      lcd_fill(10, 240, 319, 470, BLACK);
      
      lcd_string_show(10, 380, 300, 24, 24, (uint8_t *)"key1:上一首");     
      lcd_string_show(10, 410, 300, 24, 24, (uint8_t *)"key2:下一首"); 
      lcd_string_show(10, 440, 300, 24, 24, (uint8_t *)"joystick:暂停"); 
      
      audio->music_type = music_type_get((uint8_t *)audio->file_info.fname);
      
      /* play music */
      switch(audio->music_type)
      {
        case MUSIC_MP3:
          mp3_song_play(audio, name_buf);              
          break;
        case MUSIC_WAV:
          wav_song_play(audio, name_buf);                
          break;
        case MUSIC_FLAC:
          flac_song_play(audio, name_buf);
          break;
        case MUSIC_APE:
          ape_song_play(audio, name_buf);        
          break;
        
        default:
          break;
      }
     
      /* music switch */
      switch(audio->key)
      {
        /* key 1 down */  
        case MUSIC_KEY_PREVIOUS:
          
          if(audio->music_id)
          {
            audio->music_id--;  
          }
          else
          {
            audio->music_id = audio->music_number - 1; 
          }
          break;

        /* key 2 down */  
        case MUSIC_KEY_NEXT:
        case MUSIC_NO_KEY:
          
          audio->music_id++;
        
          if(audio->music_id >= audio->music_number)
          {
            audio->music_id = 0;
          }
          break;
        
        default:
          break;
      }
    }
  } 
}

