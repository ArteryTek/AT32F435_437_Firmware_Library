/**
  **************************************************************************
  * @file     at_surf_f437_board_avi.c
  * @brief    avi display.
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
#include "jpeglib.h"

avi_info_type avix;
uint8_t AVI_VIDS_FLAG_TBL[2][4] = {"00dc", "01dc"}; /* video encoding flag string */
uint8_t AVI_AUDS_FLAG_TBL[2][4] = {"00wb", "01wb"}; /* audio encoding flag string */

/* this struct contains the JPEG decompression parameters */
struct jpeg_decompress_struct cinfo;

/* this struct represents a JPEG error handler */
struct jpeg_error_mgr jerr;

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

  for(i = 0; i < rgb888_size; i  += 3)
  {
    rgb565[count++] = (((rgb888[i + 2] & 0xF8) << 8) | ((rgb888[i + 1] & 0xFC) << 3) | (rgb888[i] >> 3)); ;
  }

  return count;
}

/**
  * @brief  avi srarch id
  * @param  buf: input buffer
  *         size: buffer size
  *         id: the id to be searched must be 4 bytes in length
  * @retval the location of the 'id'.
  */
uint16_t avi_srarch_id(uint8_t* buf, uint16_t size, uint8_t *id)
{
  uint16_t i;

  size -= 4;

  for(i = 0; i < size; i++)
  {
    if(buf[i] == id[0])
     if(buf[i+1] == id[1])
       if(buf[i+2] == id[2])
         if(buf[i+3] == id[3])return i;
  }
  return 0;
}

/**
  * @brief  get stream information
  * @param  buf: stream start (must start with 01wb/00wb/01dc/00dc)
  * @retval avi_status_type: decode result.
  */
avi_status_type avi_stream_info_get(uint8_t* buf)
{
  /* get flow type */
  avix.StreamID = MAKEWORD(buf+2);

  /* get flow size */
  avix.StreamSize = MAKEDWORD(buf+4);

  /* odd plus 1 (avix.StreamSize, must be even) */
  if(avix.StreamSize%2)avix.StreamSize++;
  if(avix.StreamID == AVI_VIDS_FLAG||avix.StreamID == AVI_AUDS_FLAG)return AVI_OK;
  return AVI_STREAM_ERR;
}

/**
  * @brief  avi decoding initialization
  * @param  buf: input buffer
  *         size: buffer size
  * @retval avi_status_type: avi decode result.
  */
avi_status_type avi_init(uint8_t *buf, uint16_t size)
{
  uint16_t offset;
  uint8_t *tbuf;
  avi_status_type res = AVI_OK;
  avi_header_type *aviheader;
  avi_list_type *listheader;
  avih_header_type *avihheader;
  strh_header_type *strhheader;

  strf_bmp_header_type *bmpheader;
  strf_wav_header_type *wavheader;

  tbuf = buf;
  aviheader = (avi_header_type*)buf;
  if(aviheader->RiffID != AVI_RIFF_ID)return AVI_RIFF_ERR;
  if(aviheader->AviID != AVI_AVI_ID)return AVI_AVI_ERR;
  buf += sizeof(avi_header_type);

  listheader = (avi_list_type*)(buf);
  if(listheader->ListID != AVI_LIST_ID)return AVI_LIST_ERR;
  if(listheader->ListType != AVI_HDRL_ID)return AVI_HDRL_ERR;
  buf += sizeof(avi_list_type);
  avihheader = (avih_header_type*)(buf);
  if(avihheader->BlockID != AVI_AVIH_ID)return AVI_AVIH_ERR;

  /* get the frame interval time */
  avix.SecPerFrame = avihheader->SecPerFrame;

  /* get the total number of frames */
  avix.TotalFrame = avihheader->TotalFrame;
  buf += avihheader->BlockSize+8;

  listheader = (avi_list_type*)(buf);
  if(listheader->ListID != AVI_LIST_ID)return AVI_LIST_ERR;
  if(listheader->ListType != AVI_STRL_ID)return AVI_STRL_ERR;
  strhheader = (strh_header_type*)(buf+12);
  if(strhheader->BlockID != AVI_STRH_ID)return AVI_STRH_ERR;

   /* video frame at the beginning */
  if(strhheader->StreamType == AVI_VIDS_STREAM)
  {
    /* non MJPG video stream, not supported */
    if(strhheader->Handler != AVI_FORMAT_MJPG)return AVI_FORMAT_ERR;

    /* video stream flag "00dc" */
    avix.VideoFLAG = (uint8_t*)AVI_VIDS_FLAG_TBL[0];

    /* audio stream flag "01wb" */
    avix.AudioFLAG = (uint8_t*)AVI_AUDS_FLAG_TBL[1];

    bmpheader = (strf_bmp_header_type*)(buf+12+strhheader->BlockSize+8);
    if(bmpheader->BlockID != AVI_STRF_ID)return AVI_STRF_ERR;
    avix.Width = bmpheader->bmiHeader.Width;
    avix.Height = bmpheader->bmiHeader.Height;
    buf += listheader->BlockSize+8;
    listheader = (avi_list_type*)(buf);

    /* it is a video file without audio frames */
    if(listheader->ListID != AVI_LIST_ID)
    {
      /* audio sampling rate */
      avix.SampleRate = 0;
      /* number of audio channels */
      avix.Channels = 0;
      /* audio format */
      avix.AudioType = 0;

    }else
    {
      if(listheader->ListType != AVI_STRL_ID)return AVI_STRL_ERR;
      strhheader = (strh_header_type*)(buf+12);
      if(strhheader->BlockID != AVI_STRH_ID)return AVI_STRH_ERR;
      if(strhheader->StreamType != AVI_AUDS_STREAM)return AVI_FORMAT_ERR;
      wavheader = (strf_wav_header_type*)(buf+12+strhheader->BlockSize+8);
      if(wavheader->BlockID != AVI_STRF_ID)return AVI_STRF_ERR;
      /* audio sampling rate */
      avix.SampleRate = wavheader->SampleRate;
      /* number of audio channels */
      avix.Channels = wavheader->Channels;
      /* audio format */
      avix.AudioType = wavheader->FormatTag;
    }
  }
  /* audio frame at the beginning */
  else if(strhheader->StreamType == AVI_AUDS_STREAM)
  {
    /* video stream flag "01dc" */
    avix.VideoFLAG = (uint8_t*)AVI_VIDS_FLAG_TBL[1];
    /* audio stream flag "00wb" */
    avix.AudioFLAG = (uint8_t*)AVI_AUDS_FLAG_TBL[0];
    wavheader = (strf_wav_header_type*)(buf+12+strhheader->BlockSize+8);
    if(wavheader->BlockID != AVI_STRF_ID)return AVI_STRF_ERR;
    /* audio sampling rate */
    avix.SampleRate = wavheader->SampleRate;
    /* number of audio channels */
    avix.Channels = wavheader->Channels;
    /* audio format */
    avix.AudioType = wavheader->FormatTag;

    buf += listheader->BlockSize+8;
    listheader = (avi_list_type*)(buf);
    if(listheader->ListID != AVI_LIST_ID)return AVI_LIST_ERR;
    if(listheader->ListType != AVI_STRL_ID)return AVI_STRL_ERR;
    strhheader = (strh_header_type*)(buf+12);
    if(strhheader->BlockID != AVI_STRH_ID)return AVI_STRH_ERR;
    if(strhheader->StreamType != AVI_VIDS_STREAM)return AVI_FORMAT_ERR;
    bmpheader = (strf_bmp_header_type*)(buf+12+strhheader->BlockSize+8);
    if(bmpheader->BlockID != AVI_STRF_ID)return AVI_STRF_ERR;
    if(bmpheader->bmiHeader.Compression != AVI_FORMAT_MJPG)return AVI_FORMAT_ERR;
    avix.Width = bmpheader->bmiHeader.Width;
    avix.Height = bmpheader->bmiHeader.Height;
  }

  /* find movi ID */
  offset = avi_srarch_id(tbuf, size, (uint8_t *)"movi");

  if(offset == 0)return AVI_MOVI_ERR;
  /* search only when there is an audio stream available */
  if(avix.SampleRate)
  {
    tbuf += offset;
    /* find audio stream flag */
    offset = avi_srarch_id(tbuf, size, avix.AudioFLAG);
    if(offset == 0)return AVI_STREAM_ERR;
    tbuf += offset+4;
    /* get the buf size of the audio stream */
    avix.AudioBufSize = *((uint16_t*)tbuf);
  }

  return res;
}

/**
  * @brief  lcd dma init.
  * @param  video: video information structure.
  * @retval none
  */
void lcd_dma_init(video_type *video)
{
  dma_init_type dma_init_struct;

  /* dma1 channel6: lcd refresh */
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = 1;
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_MEMORY;
  dma_init_struct.memory_base_addr = XMC_LCD_DATA;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = FALSE;
  dma_init_struct.peripheral_base_addr = (uint32_t)video->lcd_buf;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = TRUE;
  dma_init_struct.priority = DMA_PRIORITY_LOW;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(LCD_DMA_TX_CHANNEL, &dma_init_struct);

  dma_interrupt_enable(LCD_DMA_TX_CHANNEL, DMA_FDT_INT, TRUE);
  nvic_irq_enable(LCD_DMA_TX_IRQn, 10, 0);
}

/**
  * @brief  lcd refresh dma interrupt handles.
  * @param  none.
  * @retval none
  */
void LCD_DMA_TX_IRQHandler(void)
{
  if(dma_flag_get(LCD_DMA_TX_FDT_FLAG))
  {
    dma_flag_clear(LCD_DMA_TX_FDT_FLAG);

    dma_channel_enable(LCD_DMA_TX_CHANNEL, FALSE);

    video_info.lcd_refresh_flag++;

    if(video_info.lcd_refresh_flag == 1)
    {
      dma_channel_enable(LCD_DMA_TX_CHANNEL, FALSE);

      LCD_DMA_TX_CHANNEL->paddr = (uint32_t)&video_info.lcd_buf[65535];

      dma_data_number_set(LCD_DMA_TX_CHANNEL, 320*240 - 65535);

      dma_channel_enable(LCD_DMA_TX_CHANNEL, TRUE);
    }
  }
}

/**
  * @brief  lcd data copy dma init.
  * @param  video: video information structure.
  * @retval none
  */
void lcd_dma_buf_copy_init(video_type *video)
{
  dma_init_type dma_init_struct;

  /* dma1 channel6: lcd refresh */
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = 1;
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_MEMORY;
  dma_init_struct.memory_base_addr = 0;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = 0;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = TRUE;
  dma_init_struct.priority = DMA_PRIORITY_LOW;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(LCD_DMA_BUF_CHANNEL, &dma_init_struct);
}

/**
  * @brief  lcd data copy config.
  * @param  dst_buf: destnation buf.
  *         src_buf: source buf.
  *         cnt: copy number.
  * @retval none
  */
void lcd_dma_buf_copy(uint16_t *dst_buf, uint16_t *src_buf, uint16_t cnt)
{
  LCD_DMA_BUF_CHANNEL->ctrl_bit.chen = 0;

  LCD_DMA_BUF_CHANNEL->maddr = (uint32_t)dst_buf;
  LCD_DMA_BUF_CHANNEL->paddr = (uint32_t)src_buf;
  LCD_DMA_BUF_CHANNEL->dtcnt = cnt;

  LCD_DMA_BUF_CHANNEL->ctrl_bit.chen = 1;
}

/**
  * @brief  refresh lcd data through dma.
  * @param  video: video information structure.
  * @retval none
  */
void lcd_buf_refresh(video_type *video)
{
  #ifdef LCD_DMA_REFRESH
  lcd_windows_set(0, 0, 320 - 1, 240 - 1);

  /* dma refresh */
  dma_channel_enable(LCD_DMA_TX_CHANNEL, FALSE);

  LCD_DMA_TX_CHANNEL->paddr = (uint32_t)video->lcd_buf;
  LCD_DMA_TX_CHANNEL->dtcnt = 65535;

  dma_channel_enable(LCD_DMA_TX_CHANNEL, TRUE);

  video_info.lcd_refresh_flag = 0;

  #else
  uint32_t x;

  lcd_windows_set(0, 0, 320 - 1, 240 - 1);

  video_info.lcd_refresh_flag = 2;

  /* software refresh  */
  for(x = 0; x < 320 * 240; x++)
  {
    lcd_data_16bit_write(video->lcd_buf[x]);
  }
  #endif
}

/**
  * @brief  draw point callback function.
  * @param  video: video information structure.
  *         cinfo: decoder information structure.
  *         buf: data.
  *         data_len: data len.
  * @retval none.
  */
void libjpeg_display_function(j_decompress_ptr cinfo, video_type *video, uint8_t* buf, uint32_t data_len)
{
  uint32_t width;

  rgb888_to_rgb565(buf, data_len, video->out_buf);

  if(cinfo->image_width > LCD_W)
  {
    width = LCD_W;
  }
  else
  {
    width = cinfo->image_width;
  }

  #ifdef LCD_DMA_REFRESH
  lcd_dma_buf_copy(&video->lcd_buf[video->line_counter * width], video->out_buf, width);
  #else
  copy_buff(&video->lcd_buf[line_counter * width], video->out_buf, width);
  #endif

  video->line_counter++;
}

/**
  * @brief  display .MJPEG image through libjpeg.
  * @param  video: video information structure.
  * @retval ERROR: did not display successfully.
  *         SUCCESS: display completed successfully .
  */
error_status mjpeg_libjpeg_decode(video_type *video)
{
  error_status status = SUCCESS;

  /* output row buffer */
  JSAMPROW buffer[2] = {0};

  buffer[0] = video->read_buf;

  /* step 1: allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr);

  /* initialize the JPEG decompression object */
  jpeg_create_decompress(&cinfo);

  /* prepare for input from a stdio stream */
  jpeg_stdio_src(&cinfo, &video->file);

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

  video->image_width  = cinfo.image_width ;
  video->image_height = cinfo.image_height;

  video->line_counter = 0;

  while(cinfo.output_scanline < cinfo.output_height)
  {
    /* image decoding */
    jpeg_read_scanlines(&cinfo, buffer, 1);

    libjpeg_display_function(&cinfo, video, buffer[0], cinfo.image_width *3);
  }

  end:

  /* step 6: finish decompression */
  jpeg_finish_decompress(&cinfo);

  /* step 7: release jpeg decompression object */
  jpeg_destroy_decompress(&cinfo);

  return status;
}

/**
  * @brief  init frame timer.
  * @param  us: frame interval
  * @retval none
  */
void frame_base_timer_init(uint32_t us)
{
  crm_clocks_freq_type crm_clocks_freq_struct = {0};

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  /* enable tmr1 clock */
  crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);

  /* enable tmr1 */
  tmr_counter_enable(TMR2, FALSE);
  tmr_flag_clear(TMR2, TMR_OVF_FLAG);

  /* tmr1 configuration */
  /* time base configuration */
  tmr_base_init(TMR2, us / 10 - 1, (crm_clocks_freq_struct.apb1_freq * 2 / 100000) - 1);
  tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);

  tmr_flag_clear(TMR2, TMR_OVF_FLAG);
  /* overflow interrupt enable */
  tmr_interrupt_enable(TMR2, TMR_OVF_INT, TRUE);

  /* tmr1 interrupt nvic init */
  nvic_irq_enable(TMR2_GLOBAL_IRQn, 1, 0);

  TMR2->cval = 0;

  /* enable tmr1 */
  tmr_counter_enable(TMR2, TRUE);
}

/**
  * @brief  this function handles timer2 overflow handler.
  * @param  none
  * @retval none
  */
void TMR2_GLOBAL_IRQHandler(void)
{
  if(tmr_interrupt_flag_get(TMR2, TMR_OVF_FLAG) == SET)
  {
    video_info.video_frame = 1;
    /* add user code... */
    tmr_flag_clear(TMR2, TMR_OVF_FLAG);
  }
}

/**
  * @brief  display music information.
  * @param  audio: audio information structure.
  * @retval none.
  */
void video_info_display(video_type *video)
{
  uint8_t buf[30];

  /* display the number of video files */
  sprintf((char *)buf, "%02dx%02d", avix.Width, avix.Height);
  lcd_string_show(95, 270, 200, 24, 24, buf);

  /* show volume title */
  sprintf((char *)buf, "volume:");
  lcd_string_show(10, 325, 200, 24, 24, buf);

  /* show play progress */
  sprintf((char *)buf, "video:%d fps  %02d:%02d", video->sec_per_frame, video->total_sec / 60, video->total_sec % 60);
  lcd_string_show(10, 355, 300, 24, 24, buf);

  /* display wav file information */
  sprintf((char *)buf, "audio:%d  ch   %d hz", video->channel, video->samplerate);
  lcd_string_show(10, 380, 300, 24, 24, buf);
}

/**
  * @brief  display music information.
  * @param  audio: audio information structure.
  * @retval none.
  */
void video_time_display(video_type *video)
{
  uint8_t buf[30];
  uint16_t vol;

  vol = volume_value_get();

  if(video->last_vol != vol)
  {
    video->last_vol = vol;

    /* show volume */
    sprintf((char *)buf, "%d ", video->last_vol);

    lcd_string_show(100, 325, 200, 24, 24, buf);
  }

  if(video->last_sec != video->current_sec)
  {
    video->last_sec = video->current_sec;

    /* show play progress */
    sprintf((char *)buf, "%02d:%02d", video->last_sec / 60, video->last_sec % 60);

    lcd_string_show(180, 325, 100, 24, 24, buf);
  }
}

/**
  * @brief  display video through libjpeg.
  * @param  video: video information structure.
  *         pname: image file path.
  *         x_pos: the x-axis displays the starting point.
  *         y_pos: the y-axis displays the starting point.
  * @retval ERROR: did not display successfully.
  *         SUCCESS: display completed successfully .
  */
error_status video_play_mjpeg(video_type *video, uint8_t *pname)
{
  error_status status = SUCCESS;
  uint32_t file_addr, offset;
  uint8_t header_buf[8];
  UINT br;

  /* malloc memory */
  video->read_buf = at32_malloc(video->read_buf_size);
  video->out_buf  = at32_malloc(video->out_buf_size);
  video->lcd_buf  = at32_malloc(video->lcd_buf_size);

  lcd_dma_init(video);
  lcd_dma_buf_copy_init(video);

  /* open the specified file */
  if(f_open(&video->file, (const TCHAR*)pname, FA_READ) != FR_OK)
  {
    status = ERROR;

    goto end;
  }

  /* read avi header */
  if(f_read(&video->file, video->read_buf, video->read_buf_size, &br) != FR_OK)
  {
    status = ERROR;

    goto end;
  }

  /* decode avi header */
  if(avi_init(video->read_buf, video->read_buf_size) != AVI_OK)
  {
    status = ERROR;

    goto end;
  }

  /* srarch movi string */
  offset = avi_srarch_id(video->read_buf, video->read_buf_size, (uint8_t *)"movi");

  /* get stream information */
  avi_stream_info_get(video->read_buf + offset + 4);

  /* move the file pointer to the starting position of the data stream */
  f_lseek(&video->file, offset + 12);

  /* if there is audio information, initialize the sampling rate */
  if(avix.SampleRate)
  {
    /* set sample rate */
    audio_freq_set(avix.SampleRate);
  }

  video->channel = avix.Channels;
  video->samplerate = avix.SampleRate;
  video->sec_per_frame = 1000000 / avix.SecPerFrame;
  video->total_sec = avix.TotalFrame / video->sec_per_frame;

  video_info_display(video);

  video->video_frame = 0;

  frame_base_timer_init(avix.SecPerFrame);

  while(1)
  {
    /* video stream */
    if(avix.StreamID == AVI_VIDS_FLAG)
    {
      video->decode_cnt++;

      file_addr = video->file.fptr;

      GPIOB->odt_bit.odt9 = 1;
      /* video decode */
      if(avix.StreamSize)
      {
        mjpeg_libjpeg_decode(video);

        lcd_buf_refresh(video);
      }

      GPIOB->odt_bit.odt9 = 0;
      /* move the file pointer to the starting position of the next stream */
      f_lseek(&video->file, file_addr + avix.StreamSize);

      if(avix.SampleRate == 0)
      {
        /* waiting for video frame interval */
        while(video->video_frame == 0);
        video->video_frame = 0;
      }
    }
    /* audio stream */
    else
    {
      /* waiting for the completion of the previous frame data transmission */
      audio_wait_data_tx_end();

      /* read audio data */
      if(f_read(&video->file, music_output_buffer, avix.StreamSize, &br) != FR_OK)
      {
        status = ERROR;

        goto end;
      }

      /* set i2s transmit counter */
      audio_dma_counter_set(avix.StreamSize/4);
    }

    /* read the next frame stream information data */
    if(f_read(&video->file, header_buf, 8, &br) != FR_OK)
    {
      status = ERROR;

      goto end;
    }

    /* get the next frame stream flag */
    if(avi_stream_info_get(header_buf) != AVI_OK)
    {
      status = ERROR;

      goto end;
    }

    volume_value_set();

    video->current_sec = video->decode_cnt * video->total_sec / avix.TotalFrame;

    if(video_info.lcd_refresh_flag == 2)
    {
      video_time_display(video);
    }

    /* check button status */
    if(video_play_stop_check(video) == VIDEO_PLAY_STOP)
    {
      memset(music_output_buffer, 0, sizeof(music_output_buffer));

      goto end;
    }
  }

  end:

  while(video_info.lcd_refresh_flag < 2);

  /* close file */
  f_close(&video->file);

  /* free memory */
  at32_free(video->read_buf);
  at32_free(video->out_buf);
  at32_free(video->lcd_buf);

  return status;
}
