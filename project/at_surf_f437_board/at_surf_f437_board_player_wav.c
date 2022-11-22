/**
  **************************************************************************
  * @file     at_surf_f437_board_player_wav.c
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

#include "at_surf_f437_board_player.h"

#define WAV_READBUF_SIZE                 (1024 * 8)
#define WAV_DECODE_SIZE                  (1152)  /* single channel */
/**
  * @brief wav data change
  */
typedef union
{
  uint8_t buf[4];         
  int32_t data;
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
  * @param  audio: audio information structure.
  *         header: wav file information.
  *         buf: decode data.
  * @retval -1: wav header not found.
  *         x: data start location.
  */
int32_t wav_header_decode(audio_type *audio, wav_header_type *header, uint8_t *buf)
{
  int32_t riff_start;
  int32_t fmt_start;
  int32_t data_start;
  
  /* riff fmt data location find */
  riff_start = wav_str_find(buf, (char *)"RIFF", WAV_READBUF_SIZE);
  fmt_start  = wav_str_find(buf, (char *)"fmt ", WAV_READBUF_SIZE);
  data_start = wav_str_find(buf, (char *)"data", WAV_READBUF_SIZE);
  
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
    header->read_size = header->data_size * WAV_DECODE_SIZE;

    audio->bps = header->fmt.bitspersample;
    audio->channel = header->fmt.numofchannels;      
    audio->samplerate = header->fmt.samplerate;
    audio->totalsamples = header->data.size / ((audio->bps / 8) * audio->channel);
    audio->total_sec = audio->totalsamples / audio->samplerate;
    
    audio->tx_size = WAV_DECODE_SIZE;
    
    return data_start + 8;
  }
  
  return -1;
}

/**
  * @brief  decode wav data.
  * @param  header: wav file information.
  *         input_buf: data to be decode.
  *         decode_left: left channel data.
  *         decode_right: right channel data.
  *         read_size: input buffer data size.
  *         decode_size: decode data size.
  * @retval none.
  */
void wav_data_decode(wav_header_type *header, int8_t *input_buf, int32_t *decode_left, int32_t *decode_right, uint32_t read_size, uint32_t *decode_size)
{
  uint32_t i, idx = 0;
  wav_data_change_type temp;
  
  (*decode_size) = read_size / header->data_size;
  
  /* play music */
  switch(header->fmt.bitspersample)
  {
    /* 8 bits per sample */  
    case 8:
      for(i = 0; i < (*decode_size); i++)
      {
        temp.buf[0] = input_buf[idx++];
        temp.buf[1] = 0; 
        temp.buf[2] = 0; 
        temp.buf[3] = 0; 
        
        decode_left[i] = temp.data; 
        
        if(header->fmt.numofchannels == 2)
        {
          temp.buf[0] = input_buf[idx++];
          temp.buf[1] = 0; 
          temp.buf[2] = 0; 
          temp.buf[3] = 0; 
          
          decode_right[i] = temp.data; 
        }
      }
      break;
      
    /* 16 bits per sample */   
    case 16:
      for(i = 0; i < (*decode_size); i++)
      {
        temp.buf[0] = input_buf[idx++];
        temp.buf[1] = input_buf[idx++]; 
        temp.buf[2] = 0; 
        temp.buf[3] = 0; 
        
        decode_left[i] = temp.data; 
        
        if(header->fmt.numofchannels == 2)
        {
          temp.buf[0] = input_buf[idx++];
          temp.buf[1] = input_buf[idx++]; 
          temp.buf[2] = 0; 
          temp.buf[3] = 0; 
          
          decode_right[i] = temp.data; 
        }
      }
      break;
      
    /* 24 bits per sample */  
    case 24:
      for(i = 0; i < (*decode_size); i++)
      {
        temp.buf[0] = input_buf[idx++];
        temp.buf[1] = input_buf[idx++]; 
        temp.buf[2] = input_buf[idx++]; 
        temp.buf[3] = 0; 
        
        decode_left[i] = temp.data; 
        
        if(header->fmt.numofchannels == 2)
        {
          temp.buf[0] = input_buf[idx++];
          temp.buf[1] = input_buf[idx++]; 
          temp.buf[2] = input_buf[idx++]; 
          temp.buf[3] = 0; 
          
          decode_right[i] = temp.data; 
        }
      }     
      break;
      
    /* 32 bits per sample */  
    case 32:
      for(i = 0; i < (*decode_size); i++)
      {
        temp.buf[0] = input_buf[idx++];
        temp.buf[1] = input_buf[idx++]; 
        temp.buf[2] = input_buf[idx++]; 
        temp.buf[3] = input_buf[idx++]; 
        
        decode_left[i] = temp.data; 
        
        if(header->fmt.numofchannels == 2)
        {
          temp.buf[0] = input_buf[idx++];
          temp.buf[1] = input_buf[idx++]; 
          temp.buf[2] = input_buf[idx++]; 
          temp.buf[3] = input_buf[idx++]; 
          
          decode_right[i] = temp.data; 
        }
      }     
      break;
    default:
      break;
  }
}

/**
  * @brief  play wav music files.
  * @param  audio: audio information structure.
  *         pname: music file path.
  * @retval ERROR: did not play successfully.
  *         SUCCESS: play completed successfully .
  */
error_status wav_song_play(audio_type *audio, uint8_t *pname)
{   
  int32_t ret = 0; 
  UINT br;   
  uint8_t *read_ptr;    
  uint32_t decode_size;
  error_status status = SUCCESS;

  /* init audio struct */  
  music_play_init(audio);
  
  /* malloc memory */
  audio->read_buf = at32_malloc(WAV_READBUF_SIZE);
  audio->decoded0 = at32_malloc(WAV_DECODE_SIZE * 4);
  audio->decoded1 = at32_malloc(WAV_DECODE_SIZE * 4); 
  
  read_ptr = audio->read_buf;
  
  /* open the specified file */
  if(f_open(&audio->file,(const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;
    
    goto end;
  }    
  
  /* get file size */
  audio->file_size = f_size(&audio->file);  
  
  /* read file */
  ret = f_read(&audio->file, read_ptr, WAV_READBUF_SIZE, &br);

  if((ret != FR_OK) || (br == 0))
  {
    status = ERROR;
    
    goto end;
  }
  
  /* wav file header decode */
  ret = wav_header_decode(audio, &wav_header, read_ptr);

  /* set sample rate */
  audio_freq_set(audio->samplerate);
  
  /* set i2s transmit counter */
  audio_dma_counter_set(audio->tx_size);  
  
  /* waiting for the completion of the previous frame data transmission */
  audio_wait_data_tx_end();
    
  if(ret > 0)
  {
    f_lseek(&audio->file, ret);

    while(1)
    {
      if(f_read(&audio->file, read_ptr, wav_header.read_size, &br) != FR_OK)
      {
        status = ERROR;
        
        goto end;
      }
      
      if(br == 0)
      {
        goto end;
      }
    
      /* music decode */
      wav_data_decode(&wav_header, (int8_t *)read_ptr, audio->decoded0, audio->decoded1, br, &decode_size);
      
      /* play music */
      audio_data_convert_to_i2s(audio, audio->decoded0, audio->decoded1, decode_size);
      
      /* check button status */
      if(music_play_stop_check(audio) == MUSIC_PLAY_STOP)
      {
        goto end;
      }
          
      /* set volume */
      volume_value_set();
      
      audio->decode_cnt++;
      
      audio->decode_size += decode_size;
      
      audio->total_sec = audio->totalsamples / audio->samplerate;
      audio->current_sec = audio->decode_size / audio->samplerate;      
      
      /* display music information */
      music_info_display(audio);
    }
  }
  
  end:
  
  /* clear i2s data buffer */
  memset(music_output_buffer, 0, sizeof(music_output_buffer));
  
  /* close file */
  f_close(&audio->file);

  /* free memory */
  at32_free(audio->read_buf);  
  at32_free(audio->decoded0);  
  at32_free(audio->decoded1);  
  
  return status;  
}
