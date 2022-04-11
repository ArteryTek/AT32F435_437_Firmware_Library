/**
  **************************************************************************
  * @file_music     at_surf_f437_board_player_wav.c
  * @brief    wav file decode.
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

#include "at_surf_f437_board_audio.h"
#include "at_surf_f437_board_player.h"

/**
  * @brief wav data change
  */
typedef union
{
  uint8_t buf[2];
  int16_t data;
}wav_data_change_type;

/**
  * @brief wav riff format
  */
typedef struct
{
  uint32_t id;
  uint32_t size;
  uint32_t format;
}riff_type;

/**
  * @brief wav fmt format
  */
typedef struct
{
  uint32_t id;
  uint32_t size;
  uint16_t audioformat;
  uint16_t numofchannels;
  uint32_t samplerate;
  uint32_t byterate;
  uint16_t blockalign;
  uint16_t bitspersample;
}fmt_type;

/**
  * @brief wav data format
  */
typedef struct
{
  uint32_t id;
  uint32_t size;
}data_type;

/**
  * @brief wav file header format
  */
typedef struct
{
	riff_type riff;
	fmt_type  fmt;
	data_type data;
	uint16_t data_size;
	uint16_t read_size;
}wav_header_type;

wav_header_type wav_header;

/**
  * @brief  find equal strings in buf.
  * @param  buf: buffer to be searched.
  *         find_str: strings.
  * @retval -1: target strings not found.
  *         x: location.
  */
int32_t wav_str_find(uint8_t *buf, char *find_str, uint16_t buf_len)
{
  uint32_t i;
  uint32_t find_len;

  find_len = strlen(find_str);

  if(buf_len < find_len)
  {
    return -1;
  }

  buf_len = buf_len - find_len + 1;

  for(i = 0; i < buf_len; i++)
  {
    if(buf_cmp((uint8_t *)&buf[i], (uint8_t *)find_str, find_len) == SUCCESS)
    {
      return i;
    }
  }

  return -1;
}

/**
  * @brief  decode wav header.
  * @param  header: wav file infromation.
  *         buf: decode data.
  * @retval -1: wav header not found.
  *         x: data start location.
  */
int32_t wav_header_decode(wav_header_type *header, uint8_t *buf)
{
  int32_t riff_start;
  int32_t fmt_start;
  int32_t data_start;

  /* riff fmt data location find */
  riff_start = wav_str_find(buf, (char *)"RIFF", 200);
  fmt_start  = wav_str_find(buf, (char *)"fmt ", 200);
  data_start = wav_str_find(buf, (char *)"data", 200);

  if((riff_start >= 0) && (fmt_start > 0) && (data_start > 0))
  {
    memcpy((uint8_t *)&header->riff, &buf[riff_start], sizeof(riff_type));
    memcpy((uint8_t *)&header->fmt, &buf[fmt_start], sizeof(fmt_type));
    memcpy((uint8_t *)&header->data, &buf[data_start], sizeof(data_type));

    if((header->fmt.bitspersample != 8) && (header->fmt.bitspersample != 16) && (header->fmt.bitspersample != 24) && (header->fmt.bitspersample != 32))
    {
      return -1;
    }

    if((header->fmt.numofchannels != 1) &&  (header->fmt.numofchannels != 2))
    {
      return -1;
    }

    header->data_size = (header->fmt.bitspersample / 8) * header->fmt.numofchannels;
    header->read_size = header->data_size * MP3_DECODE_BUFFER_SIZE / 2;

    return data_start + 8;
  }

  return -1;
}

/**
  * @brief  decode wav data.
  * @param  header: wav file information.
  *         input_buf: raw data.
  *         output_buf: decode data.
  * @retval none.
  */
void wav_data_decode(wav_header_type *header, int8_t *input_buf, int16_t *output_buf, uint32_t number)
{
  uint32_t  i, data_num;
  wav_data_change_type left, righ;

  data_num = number / header->data_size;

  /* play music */
  switch(header->fmt.bitspersample)
  {
    /* 8 bits per sample */
    case 8:
      for(i = 0; i < data_num; i++)
      {
        if(header->fmt.numofchannels == 1)
        {
          output_buf[i * 2]     = (int16_t)input_buf[i + 0];
          output_buf[i * 2 + 1] = 0;
        }
        else
        {
          output_buf[i * 2]     = (int16_t)input_buf[i * 2 + 0];
          output_buf[i * 2 + 1] = (int16_t)input_buf[i * 2 + 1];
        }
      }
      break;

    /* 16 bits per sample */
    case 16:
      for(i = 0; i < data_num; i++)
      {
        if(header->fmt.numofchannels == 1)
        {
          left.buf[0] = input_buf[i * 2 + 0];
          left.buf[1] = input_buf[i * 2 + 1];

          output_buf[i * 2]     = left.data;
          output_buf[i * 2 + 1] = 0;
        }
        else
        {
          left.buf[0] = input_buf[i * 4 + 0];
          left.buf[1] = input_buf[i * 4 + 1];
          righ.buf[0] = input_buf[i * 4 + 2];
          righ.buf[1] = input_buf[i * 4 + 3];

          output_buf[i * 2]     = left.data;
          output_buf[i * 2 + 1] = righ.data;
        }
      }
      break;

    /* 24 bits per sample */
    case 24:
      for(i = 0; i < data_num; i++)
      {
        if(header->fmt.numofchannels == 1)
        {
          left.buf[0] = input_buf[i * 3 + 1];
          left.buf[1] = input_buf[i * 3 + 2];

          output_buf[i * 2]     = left.data;
          output_buf[i * 2 + 1] = 0;
        }
        else
        {
          left.buf[0] = input_buf[i * 6 + 1];
          left.buf[1] = input_buf[i * 6 + 2];
          righ.buf[0] = input_buf[i * 6 + 4];
          righ.buf[1] = input_buf[i * 6 + 5];

          output_buf[i * 2]     = left.data;
          output_buf[i * 2 + 1] = righ.data;
        }
      }
      break;

    /* 32 bits per sample */
    case 32:
      for(i = 0; i < data_num; i++)
      {
        if(header->fmt.numofchannels == 1)
        {
          left.buf[0] = input_buf[i * 4 + 2];
          left.buf[1] = input_buf[i * 4 + 3];

          output_buf[i * 2]     = left.data;
          output_buf[i * 2 + 1] = 0;
        }
        else
        {
          left.buf[0] = input_buf[i * 8 + 2];
          left.buf[1] = input_buf[i * 8 + 3];
          righ.buf[0] = input_buf[i * 8 + 6];
          righ.buf[1] = input_buf[i * 8 + 7];

          output_buf[i * 2]     = left.data;
          output_buf[i * 2 + 1] = righ.data;
        }
      }
      break;
    default:
      break;
  }
}

/**
  * @brief  display wav music information.
  * @param  wav: wav file information.
  *         info: paly status information.
  * @retval none.
  */
void wav_info_display(wav_header_type *wav, audio_type *info)
{
  uint8_t buf[30];

  /* display wav file information */
  if(info->decode_cnt == 1)
  {
    sprintf((char *)buf, "%d bits per sample", wav->fmt.bitspersample);
    lcd_string_show(10, 270, 300, 24, 24, buf);

    sprintf((char *)buf, "%d Hz sample rate", wav->fmt.samplerate);
    lcd_string_show(10, 300, 300, 24, 24, buf);

    sprintf((char *)buf, "%d Chanel", wav->fmt.numofchannels);
    lcd_string_show(10, 330, 300, 24, 24, buf);
  }

  /* show play progress */
  sprintf((char *)buf, "%02d:%02d", info->total_time / 60, info->total_time % 60);

  lcd_string_show(100, 110, 200, 24, 24, buf);

  /* show play progress */
  sprintf((char *)buf, "%02d:%02d", info->current_time / 60, info->current_time % 60);

  lcd_string_show(10, 180, 200, 24, 24, buf);

  /* show volume */
  sprintf((char *)buf, "Volume:%d  ", volume_value_get());

  lcd_string_show(185, 110, 200, 24, 24, buf);
}

/**
  * @brief  play wav music files.
  * @param  pname: music file path.
  * @retval ERROR: did not play successfully.
  *         SUCCESS: play completed successfully .
  */
error_status wav_song_play(uint8_t *pname)
{
  int32_t ret = 0;
  UINT br;
  uint8_t *read_ptr;
  error_status status = SUCCESS;

  audio_info.key = NO_KEY;
  audio_info.decode_cnt = 0;
  audio_info.read_cnt = 0;
  audio_info.error_cnt = 0;
  audio_info.file_size = 0;
  audio_info.decode_size = 0;
  audio_info.total_time = 0;
  audio_info.current_time = 0;

  read_ptr = music_read_buffer;

  /* open the specified file */
  if(f_open(&file_music,(const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;

    goto end;
  }

  /* get file size */
  audio_info.file_size = f_size(&file_music);

  ret = f_read(&file_music, read_ptr, MP3_READBUF_SIZE, &br);

  if((ret != FR_OK) || (br == 0))
  {
    status = ERROR;

    goto end;
  }

  ret = wav_header_decode(&wav_header, read_ptr);

  /* wait for i2s to transmit a frame of data */
  i2s_dma_tx_end = 0;

  while(i2s_dma_tx_end == 0);

  i2s_dma_tx_end = 0;

  if(ret > 0)
  {
    f_lseek(&file_music, ret);

    while(1)
    {
      if(f_read(&file_music, read_ptr, wav_header.read_size, &br) != FR_OK)
      {
        status = ERROR;

        goto end;
      }

      if(br == 0)
      {
        goto end;
      }

      wav_data_decode(&wav_header, (int8_t *)music_read_buffer, music_output_buffer, br);

      audio_info.decode_cnt++;

      audio_info.decode_size += br;

      audio_info.total_time = audio_info.file_size / wav_header.fmt.byterate;
      audio_info.current_time = audio_info.decode_size / wav_header.fmt.byterate;

      /* check button status */
      audio_info.key = key_press();

      if(audio_info.key != NO_KEY)
      {
        goto end;
      }

      /* set volume */
      volume_value_set();

      wav_info_display(&wav_header, &audio_info);

      while(i2s_dma_tx_end == 0);

      i2s_dma_tx_end = 0;
    }
  }

  end:

  /* clear i2s data buffer */
  memset(music_output_buffer, 0, sizeof(music_output_buffer));

  /* close file */
  f_close(&file_music);

  return status;
}
