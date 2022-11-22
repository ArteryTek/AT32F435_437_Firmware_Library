/**
  **************************************************************************
  * @file     at_surf_f437_board_player_ape.c
  * @brief    ape file decode.
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
#include "apedecoder.h"

/* monkey's audio files have one seekpoint per frame.  the framesize
   varies between 73728 and 1179648 samples.  

   at the smallest framesize, 30000 frames would be 50155 seconds of
   audio - almost 14 hours.  this should be enough for any file a user
   would want to play in rockbox, given the 2gb fat filesize (and 4gb
   seektable entry size) limit.

   this means the seektable is 120000 bytes, but we have a lot of
   spare room in the codec buffer - the ape codec itself is small.
*/

struct ape_ctx_t *apex; 

extern filter_int *filterbuf64;          /* 2432 or 4864 bytes */
extern filter_int *filterbuf256;         /* 5120 or 10240 bytes */
extern filter_int *filterbuf1280;        /* 17408 or 34816 bytes */

#define APE_MAX_DECODE_SIZE              1152
#define APE_READBUF_SIZE                 (1152 * 30)

/**
  * @brief  calculate the number of decoding each time.
  * @param  audio: audio information structure.
  *         ape_ctx: ape file information.
  * @retval none.
  */
void ape_calculate_blocks_per_loop(audio_type *audio, struct ape_ctx_t* ape_ctx)
{
  uint16_t i;
  
  audio->tx_size = APE_MAX_DECODE_SIZE;
  
  for(i = APE_MAX_DECODE_SIZE; i > 250; i --)
  {
    if((ape_ctx->blocksperframe % i) == 0)
    {
      audio->tx_size = i;
      break;
    }
  }
}

/**
  * @brief  play ape format music files.
  * @param  audio: audio information structure.
  *         pname: music file path.
  * @retval ERROR: did not play successfully.
  *         SUCCESS: play completed successfully .
  */
error_status ape_song_play(audio_type *audio, uint8_t *pname)
{
  int currentframe;
  int nblocks;
  int bytesconsumed;
  int bytes_left;
  int decode_size;
  int firstbyte; 
  
  UINT br;   
  uint8_t *readptr;
  int32_t ret = 0; 
  error_status status = SUCCESS;
  
  /* init audio struct */  
  music_play_init(audio);
   
  /* malloc memory */
  apex            = at32_malloc(sizeof(struct ape_ctx_t));  
  filterbuf64     = at32_malloc(4864);  
  filterbuf256    = at32_malloc(10240);  
  filterbuf1280   = at32_malloc(34816);  
  audio->decoded0 = at32_malloc(APE_MAX_DECODE_SIZE * 4);
  audio->decoded1 = at32_malloc(APE_MAX_DECODE_SIZE * 4); 
  audio->read_buf = at32_malloc(APE_READBUF_SIZE);

  /* open the specified file */
  if(f_open(&audio->file,(const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;
    
    goto end;
  }    

  /* get file size */
  audio->file_size = f_size(&audio->file);  
  
  /* read the file headers to populate the ape_ctx struct */
  ret = ape_parseheader(&audio->file, apex);
  
  if(ret == 0)
  {  
    if((apex->fileversion < APE_MIN_VERSION) || (apex->fileversion>APE_MAX_VERSION))
    {
      status = ERROR;
      
      goto end;
    }
    else
    {
      audio->bps = apex->bps;
      audio->channel = apex->channels;      
      audio->samplerate = apex->samplerate;
      audio->totalsamples = apex->totalsamples;
      audio->total_sec = audio->totalsamples / audio->samplerate;
    }
  }
  else
  {
    status = ERROR;
    
    goto end;
  }

  /* calculate blocks */
  ape_calculate_blocks_per_loop(audio, apex);
  
  /* set sample rate */
  audio_freq_set(audio->samplerate);
  
  /* set i2s transmit counter */
  audio_dma_counter_set(audio->tx_size);  

  /* move the file pointer to first frame */
  f_lseek(&audio->file, apex->firstframe);

  /* read data */
  if(f_read(&audio->file, audio->read_buf, APE_READBUF_SIZE, (UINT *)&bytes_left) != FR_OK)
  {
    status = ERROR;
    
    goto end;
  }
  
  /* take account of the little-endian 32-bit byte ordering */
  firstbyte = 3;
  currentframe = 0; 
    
  readptr = audio->read_buf;
  
  /* the main decoding loop - we decode the frames a small chunk at a time */
  while (currentframe < apex->totalframes)
  { 
    /* calculate how many blocks there are in this frame */
    if(currentframe == (apex->totalframes - 1))
    {
      nblocks = apex->finalframeblocks;
    }
    else
    {
      nblocks = apex->blocksperframe; 
    }    
    
    apex->currentframeblocks = nblocks;
    
    /* initialise the frame decoder */
    init_frame_decoder(apex, readptr, &firstbyte, &bytesconsumed);
    
    readptr += bytesconsumed;
    
    bytes_left -= bytesconsumed;   
    
    /* waiting for the completion of the previous frame data transmission */
    audio_wait_data_tx_end();
    
    /* decode the frame a chunk at a time */
    while(nblocks > 0)
    {
      /* get the number of decodes */
      decode_size = AUDIO_MIN(audio->tx_size, nblocks); 
      
      /* decode data */
      if(decode_chunk(apex, readptr, &firstbyte, &bytesconsumed, audio->decoded0, audio->decoded1, decode_size) != 0)
      {
        break;
      } 
      
      readptr += bytesconsumed;
      bytes_left -= bytesconsumed;
      
      /* play music */
      audio_data_convert_to_i2s(audio, audio->decoded0, audio->decoded1, decode_size);
      
      /* set volume */
      volume_value_set();
      
      audio->decode_cnt++;
      audio->decode_size += decode_size;
      audio->current_sec = audio->decode_size * audio->total_sec / audio->totalsamples;      
      
      /* display ape music information */
      music_info_display(audio);
      
      /* check button status */
      if(music_play_stop_check(audio) == MUSIC_PLAY_STOP)
      {
        goto end;
      }

      /* read new data into buffer */
      if(bytes_left < APE_READBUF_SIZE)
      {
        memmove(audio->read_buf, readptr, bytes_left);
        
        /* read data */
        ret = f_read(&audio->file, audio->read_buf + bytes_left, APE_READBUF_SIZE - bytes_left, &br);
        
        if((ret != FR_OK) || (br == 0))
        {
          status = ERROR;
          
          goto end;
        }
        
        if(br < APE_READBUF_SIZE - bytes_left)
        {
          memset(audio->read_buf + bytes_left + br, 0, APE_READBUF_SIZE - bytes_left - br);  
        }
        
        bytes_left += br;  
      }
      
      readptr = audio->read_buf;

      /* decrement the block count */
      nblocks -= decode_size;
    }
    
    currentframe++; 
  } 

  end:
  
  /* clear i2s data buffer */
  memset(music_output_buffer, 0, sizeof(music_output_buffer));
  
  /* close file */
  f_close(&audio->file);
  
  /* free memory */
  at32_free(filterbuf64); 
  at32_free(filterbuf256); 
  at32_free(filterbuf1280); 
  at32_free(audio->decoded0);
  at32_free(audio->decoded1);
  at32_free(audio->read_buf);  
  at32_free(apex);   
  
  return status;  
}
