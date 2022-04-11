/**
  **************************************************************************
  * @file     at_surf_f437_board_player_mp3.c
  * @version  v2.0.7
  * @date     2022-04-02
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

struct mad_decoder decoder;

/**
  * @brief  this is a private message structure. a generic pointer to this structure
  *         is passed to each of the callback functions. put here any data you need
  *         to access from within the callbacks.
  */
struct buffer {
  uint8_t *start;
  uint32_t length;
};

/**
  * @brief  display wave music information.
  * @param  decode: mp3 file information.
  *         info: paly status information.
  * @retval none.
  */
void mp3_info_display(struct mad_decoder *decode, audio_type *info)
{
  uint8_t buf[30];

  /* display mp3 file information */
  if(info->decode_cnt == 0)
  {
    sprintf((char *)buf, "%d bp/s", (uint32_t)decode->sync->frame.header.bitrate);
    lcd_string_show(10, 240, 300, 24, 24, buf);

    sprintf((char *)buf, "MPEG layer %d", decode->sync->frame.header.layer);
    lcd_string_show(10, 270, 300, 24, 24, buf);

    sprintf((char *)buf, "%d Hz sample rate", decode->sync->frame.header.samplerate);
    lcd_string_show(10, 300, 300, 24, 24, buf);

    sprintf((char *)buf, "%d Chanel", decode->sync->synth.pcm.channels);
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
  struct buffer *buffer = data;

  audio_info.read_cnt++;

  read_ptr = (uint8_t *)buffer->start;

  bytes_left = stream->bufend - stream->next_frame;

  /* read new data into cache */
  if(bytes_left < MP3_READBUF_SIZE)
  {
    memmove(read_ptr, read_ptr + MP3_READBUF_SIZE - bytes_left, bytes_left);

    /* read data */
    ret = f_read(&file_music, read_ptr + bytes_left, MP3_READBUF_SIZE - bytes_left, &br);

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

  /* pcm->samplerate contains the sampling frequency */
  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];

  while (nsamples--)
  {
    /* output sample(s) in 16-bit signed little-endian PCM */
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

  while(i2s_dma_tx_end == 0);

  i2s_dma_tx_end = 0;

  /* set volume */
  volume_value_set();

  /* check button status */
  audio_info.key = key_press();

  if(audio_info.key != NO_KEY)
  {
    return MAD_FLOW_STOP;
  }

  audio_info.decode_size += decoder.sync->stream.next_frame - decoder.sync->stream.this_frame;

  /* calculate total play time */
  audio_info.total_time = audio_info.file_size * 8.0 / decoder.sync->frame.header.bitrate;

  /* calculate the current play time */
  audio_info.current_time = audio_info.decode_size * 8.0 / decoder.sync->frame.header.bitrate;

  mp3_info_display(&decoder, &audio_info);

  audio_info.decode_cnt++;

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
  /*
  struct buffer *buffer = data;

  printf("decoding error 0x%04x (%s) at byte offset %u\n",
	       stream->error, mad_stream_errorstr(stream),
	       stream->this_frame - buffer->start);
  */

  audio_info.error_cnt++;

  return MAD_FLOW_CONTINUE;
}

/**
  * @brief  this is the function called by main() above to perform all the decoding.
            it instantiates a decoder object and configures it with the input,
            output, and error callback functions above.
  * @param  start: decode input buffer.
  *         length: buffer length.
  * @retval MAD_FLOW_STOP: to stop decoding.
  *         MAD_FLOW_BREAK: to stop decoding andsignal an error.
  */
int32_t decode(uint8_t *start, uint32_t length)
{
  struct buffer buffer;

  int32_t result;

  /* initialize our private message structure */
  buffer.start  = start;
  buffer.length = length;

  /* configure input, output, and error functions */
  mad_decoder_init(&decoder, &buffer,
		   input, 0 /* header */, 0 /* filter */, output,
		   error, 0 /* message */);

  /* start decoding */
  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

  /* release the decoder */
  mad_decoder_finish(&decoder);

  /* clear i2s data buffer */
  memset(music_output_buffer, 0, sizeof(music_output_buffer));

  return result;
}

/**
  * @brief  play music files.
  * @param  pname: music file path.
  * @retval ERROR: did not play successfully.
  *         SUCCESS: play completed successfully .
  */
error_status mp3_song_play(uint8_t *pname)
{
  int32_t ret = 0;

  audio_info.key = NO_KEY;
  audio_info.decode_cnt = 0;
  audio_info.read_cnt = 0;
  audio_info.error_cnt = 0;
  audio_info.file_size = 0;
  audio_info.decode_size = 0;
  audio_info.total_time = 0;
  audio_info.current_time = 0;

  /* open the specified file */
  if(f_open(&file_music,(const TCHAR*)pname, FA_READ) != FR_OK)
  {
    return ERROR;
  }

  /* get file size */
  audio_info.file_size = f_size(&file_music);

  /* wait for i2s to transmit a frame of data */
  i2s_dma_tx_end = 0;

  while(i2s_dma_tx_end == 0);

  i2s_dma_tx_end = 0;

  ret = decode((uint8_t *)music_read_buffer, MP3_READBUF_SIZE);

  /* close file */
  f_close(&file_music);

  if(ret == 0)
  {
    return SUCCESS;
  }
  else
  {
    return ERROR;
  }
}


