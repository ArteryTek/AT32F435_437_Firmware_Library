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
#include "FLAC/stream_decoder.h"

#define FLAC_MAX_DECODE_SIZE             1152

FLAC__StreamDecoder *flac_decoder = 0;

/**
  * @brief  calculate the number of decoding each time.
  * @param  audio: audio information structure.
  *         frame: flac file information.
  * @retval none.
  */
void flac_calculate_blocks_per_loop(audio_type *audio, const FLAC__Frame *frame)
{
  uint16_t i;
  
  audio->tx_size = frame->header.blocksize / 4;
  
  for(i = FLAC_MAX_DECODE_SIZE; i > 250; i --)
  {
    if((frame->header.blocksize % i) == 0)
    {
      audio->tx_size = i;
      break;
    }
  }
}

/**
  * @brief  this is the metadata callback function. it is called after each frame of
  *         audio data has been completely decoded. the purpose of this callback
  *         is to output he decoded information
  * @param  decoder: 
  *         metadata: 
  *         client_data: 
  * @retval none. 
  */
void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
  audio_type *audio = client_data;
  
  /* print some stats */
  if(metadata->type == FLAC__METADATA_TYPE_STREAMINFO) 
  {
    /* save information */
    audio->bps = metadata->data.stream_info.bits_per_sample;
    audio->channel = metadata->data.stream_info.channels;      
    audio->samplerate = metadata->data.stream_info.sample_rate;
    audio->totalsamples = metadata->data.stream_info.total_samples;
    audio->total_sec = audio->totalsamples / audio->samplerate;
    
    /* set sample rate */
    audio_freq_set(audio->samplerate);
  }
}

/**
  * @brief  this is the error callback function. it is called whenever a decoding
  *         error occurs. 
  * @param  decoder.
  *         status.
  *         client_data.
  * @retval none. 
  */
void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
  (void)decoder, (void)client_data;
 
  /* printf("Got error callback: %s\n", FLAC__StreamDecoderErrorStatusString[status]); */
}

/**
  * @brief  this is the input callback. the purpose of this callback is to (re)fill
  *         the stream buffer which is to be decoded. in this example, an entire file
  *         has been mapped into memory, when this callback is called a second
  *         time, we are finished decoding.
  * @param  decoder: 
  *         buffer: 
  *         bytes: 
  * @retval FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM: to stop decoding.
  *         FLAC__STREAM_DECODER_READ_STATUS_ABORT: to stop decoding andsignal an error.
  *         FLAC__STREAM_DECODER_READ_STATUS_CONTINUE: continue normall. 
  */
FLAC__StreamDecoderReadStatus read_callback( const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data )
{
  size_t bytes_read;  
  audio_type *audio = client_data;
  
  if( f_eof( &audio->file ) )
  {
    return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
  }

  if( *bytes > 0 )
  {
    f_read( &audio->file, buffer, *bytes, &bytes_read );

    if( bytes_read == 0 )
    {
      if(f_eof( &audio->file ))
      {  
        return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM; 
      }
      else
      {  
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT; 
      }
    }
    else
    {
      *bytes = ( unsigned )bytes_read;
      
      return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
    }
  }
  else
  {
    return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
  }
}

/**
  * @brief  this is the output callback function. It is called after each frame of
  *         audio data has been completely decoded. the purpose of this callback
  *         is to output (or play) the decoded PCM audio.
  * @param  decoder: 
  *         frame: 
  *         buffer: 
  *         client_data: 
  * @retval FLAC__STREAM_DECODER_WRITE_STATUS_ABORT: to stop decoding.
  *         FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE: continue normall. 
  */
FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
  uint32_t i, frame_mum; 
  int32_t *left_ch, *right_ch;
  audio_type *audio = client_data;
  
  if(buffer[0] == NULL) 
  {
    /* error: buffer[0] is null */
    return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
  }
  
  if(buffer[1] == NULL) 
  {
    /* error: buffer[1] is null */
    return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
  }

  /* set volume */
  volume_value_set();

  /* calculate blocks */
  flac_calculate_blocks_per_loop(audio, frame);
  
  /* set i2s transmit counter */
  audio_dma_counter_set(audio->tx_size);  
  
  left_ch  = (FLAC__int32 *)buffer[0];
  right_ch = (FLAC__int32 *)buffer[1];
  frame_mum = frame->header.blocksize / audio->tx_size;
  
  /* write decoded pcm samples */
  for(i = 0; i < frame_mum; i++)
  {
    /* play music */
    audio_data_convert_to_i2s(audio, left_ch, right_ch, audio->tx_size);
    
    left_ch  += audio->tx_size;
    right_ch += audio->tx_size;
  }
  
  /* check button status */
  if(music_play_stop_check(audio) == MUSIC_PLAY_STOP)
  {
    return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
  }

  audio->decode_cnt++;   
  
  audio->current_sec = frame->header.number.sample_number / audio->samplerate;
  
  /* display flac music information */
  music_info_display(audio);
 
  return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

/**
  * @brief  play flac format music files.
  * @param  audio: audio information structure.
  *         pname: music file path.
  * @retval ERROR: did not play successfully.
  *         SUCCESS: play completed successfully .
  */
error_status flac_song_play(audio_type *audio, uint8_t *pname)
{   
  FLAC__StreamDecoderInitStatus init_status;
  error_status status = SUCCESS;

  /* init audio struct */  
  music_play_init(audio);
  
  /* open the specified file */
  if(f_open(&audio->file,(const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;
    
    goto end;
  }   
  
  /* get file size */
  audio->file_size = f_size(&audio->file);    
  
  /* flac decoder malloc memory */
  if((flac_decoder = FLAC__stream_decoder_new()) == NULL) 
  {
    status = ERROR;
    
    goto end;
  }

  FLAC__stream_decoder_set_md5_checking(flac_decoder, true);

  /* flac decoder init */
  init_status = FLAC__stream_decoder_init_file(flac_decoder, (const char *)pname, write_callback, metadata_callback, error_callback, /*client_data=*/audio);
  
  /* waiting for the completion of the previous frame data transmission */
  audio_wait_data_tx_end();  
  
  if(init_status == FLAC__STREAM_DECODER_INIT_STATUS_OK) 
  {
    /* decoder music file */
    FLAC__stream_decoder_process_until_end_of_stream(flac_decoder);
  }
  else
  {
    status = ERROR;
  }
  
  /* flac decoder delete */
  FLAC__stream_decoder_delete(flac_decoder);
  
  end:
  
  /* clear i2s data buffer */
  memset(music_output_buffer, 0, sizeof(music_output_buffer));
  
  /* close file */
  f_close(&audio->file);

  return status;  
}
