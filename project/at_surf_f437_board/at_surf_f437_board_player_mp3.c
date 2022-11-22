/**
  **************************************************************************
  * @file     at_surf_f437_board_player_mp3.c
  * @brief    mp3 file decode.
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
#include "decoder.h"

#define MP3_READBUF_SIZE                 (1152 * 20)

struct mad_decoder mp3_decoder;

/**
  * @brief  this is the input callback. the purpose of this callback is to (re)fill
  *         the stream buffer which is to be decoded. in this example, an entire file
  *         has been mapped into memory, so we just call mad_stream_buffer() with the
  *         address and length of the mapping. when this callback is called a second
  *         time, we are finished decoding.
  * @param  data: 
  *         header: 
  *         pcm: 
  * @retval MAD_FLOW_STOP: to stop decoding.
  *         MAD_FLOW_BREAK: to stop decoding andsignal an error.
  *         MAD_FLOW_CONTINUE: continue normall. 
  */
enum mad_flow input(void *data, struct mad_stream *stream)
{
  UINT br; 
  FRESULT ret; 
  uint8_t *read_ptr;  
  uint32_t bytes_left = 0;
  
  audio_type *audio = data;
  
  read_ptr = audio->read_buf;
  
  bytes_left = stream->bufend - stream->next_frame;

  /* read new data into buffer */
  if(bytes_left < MP3_READBUF_SIZE)
  {
    memmove(read_ptr, read_ptr + MP3_READBUF_SIZE - bytes_left, bytes_left);
    
    /* read data */
    ret = f_read(&audio->file, read_ptr + bytes_left, MP3_READBUF_SIZE - bytes_left, &br);

    if((ret != FR_OK) || (br == 0))
    {
      return MAD_FLOW_STOP;
    }

    if(br < MP3_READBUF_SIZE - bytes_left)
    {
      memset(read_ptr + bytes_left + br, 0, MP3_READBUF_SIZE - bytes_left - br);

      bytes_left = bytes_left + br;      
    }
    else
    {
      bytes_left = MP3_READBUF_SIZE;  
    }
  }
  
  mad_stream_buffer(stream, read_ptr, bytes_left);

  return MAD_FLOW_CONTINUE;
}

/**
  * @brief  the following utility routine performs simple rounding, clipping, and
  *         scaling of MAD's high-resolution samples down to 16 bits. it does not
  *         perform any dithering or noise shaping, which would be recommended to
  *         obtain any exceptional audio quality. it is therefore not recommended to
  *         use this routine if high-quality output is desired.
  * @param  sample: 
  * @retval 
  */
int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
  {
    sample = MAD_F_ONE - 1;  
  }
  else if (sample < -MAD_F_ONE)
  {
    sample = -MAD_F_ONE;  
  }

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

/**
  * @brief  this is the output callback function. It is called after each frame of
  *         MPEG audio data has been completely decoded. the purpose of this callback
  *         is to output (or play) the decoded PCM audio.
  * @param  data: 
  *         header: 
  *         pcm: 
  * @retval MAD_FLOW_STOP: to stop decoding.
  *         MAD_FLOW_BREAK: to stop decoding andsignal an error.
  *         MAD_FLOW_CONTINUE: continue normall. 
  */
enum mad_flow output(void *data, struct mad_header const *header, struct mad_pcm *pcm)
{
  uint32_t nchannels, nsamples;
  int32_t sample;  
  mad_fixed_t const *left_ch, *right_ch;
  uint32_t i = 0;
  audio_type *audio = data;
  
  /* pcm->samplerate contains the sampling frequency */
  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];

  audio->bps = header->bitrate;
  audio->channel = pcm->channels;
  audio->samplerate = header->samplerate;
  audio->tx_size = pcm->length;
  
  /* set sample rate */
  audio_freq_set(mp3_decoder.sync->frame.header.samplerate);
 
  /* set i2s transmit counter */
  audio_dma_counter_set(audio->tx_size);  

  while (nsamples--) 
  {
    /* output sample(s) in 16-bit signed little-endian pcm */
    sample = scale(*left_ch++);

    music_output_buffer[i++] = sample & 0xFFFF;
    
    if(nchannels == 2) 
    {
      sample = scale(*right_ch++);
      
      music_output_buffer[i++] = sample & 0xFFFF;
    }
    else
    {
      i++;
    }
  }

  /* waiting for the completion of the previous frame data transmission */
  audio_wait_data_tx_end();
  
  /* set volume */
  volume_value_set();
  
  /* check button status */
  if(music_play_stop_check(audio) == MUSIC_PLAY_STOP)
  {
    return MAD_FLOW_STOP;
  }

  audio->decode_size += mp3_decoder.sync->stream.next_frame - mp3_decoder.sync->stream.this_frame;
  
  /* calculate total play time */
  audio->total_sec = audio->file_size * 8.0 / mp3_decoder.sync->frame.header.bitrate;
  
  /* calculate the current play time */
  audio->current_sec = audio->decode_size * 8.0 / mp3_decoder.sync->frame.header.bitrate;
  
  music_info_display(&audio_info);
  
  audio->decode_cnt++;
  
  return MAD_FLOW_CONTINUE;
}

/**
  * @brief  this is the error callback function. it is called whenever a decoding
  *         error occurs. The error is indicated by stream->error; the list of
  *         possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
  *         header file.
  * @param  start: decode input buffer.
  *         length: buffer length.
  * @retval MAD_FLOW_STOP: to stop decoding.
  *         MAD_FLOW_BREAK: to stop decoding andsignal an error.
  *         MAD_FLOW_CONTINUE: continue normall. 
  */
enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
  audio_type *audio = data;
  
  audio->error_cnt++;  
  
  return MAD_FLOW_CONTINUE;
}

/**
  * @brief  play music files.
  * @param  audio: audio information structure.
  *         pname: music file path.
  * @retval ERROR: did not play successfully.
  *         SUCCESS: play completed successfully .
  */
error_status mp3_song_play(audio_type *audio, uint8_t *pname)
{   
  error_status status = SUCCESS;
  
  /* init audio struct */  
  music_play_init(audio);
  
  /* malloc memory */
  audio->read_buf = at32_malloc(MP3_READBUF_SIZE);
  
  /* open the specified file */
  if(f_open(&audio->file,(const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;
    
    goto end;
  }    
  
  /* get file size */
  audio->file_size = f_size(&audio->file);
  
  /* waiting for the completion of the previous frame data transmission */
  audio_wait_data_tx_end();
  
  /* configure input, output, and error functions */
  mad_decoder_init(&mp3_decoder, audio,
       input, 0 /* header */, 0 /* filter */, output,
       error, 0 /* message */);

  /* start decoding */
  if(mad_decoder_run(&mp3_decoder, MAD_DECODER_MODE_SYNC) != 0)
  {
    status =  ERROR;  
  }
  
  /* release the decoder */
  mad_decoder_finish(&mp3_decoder);

  end:
  
  /* clear i2s data buffer */
  memset(music_output_buffer, 0, sizeof(music_output_buffer));  
  
  /* close file */
  f_close(&audio->file);  
  
  /* free memory */
  at32_free(audio->read_buf);  
  
  return status;    
}


