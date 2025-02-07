/**
  **************************************************************************
  * @file     at_surf_f437_board_video.h
  * @brief    header file for at-surf board.
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
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#ifndef __AT32_SURF_F437_BOARD_VIDEO_H
#define __AT32_SURF_F437_BOARD_VIDEO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at_surf_f437_board_video_avi.h"
#include "at_surf_f437_board_audio.h"
#include "at_surf_f437_board_video.h"
#include "at_surf_f437_board_key.h"
#include "at_surf_f437_board_sd_card.h"
#include "at_surf_f437_board_lcd.h"
#include "at_surf_f437_board_variable_resistor.h"
#include "at_surf_f437_board_key.h"
#include "at_surf_f437_board_malloc.h"
#include "ff.h"
#include "string.h"
#include "stdio.h"

/**
  * @brief get min value
  */
#define AUDIO_MIN(x,y)  ((x)<(y)? (x):(y))

/**
  * @brief video buffer size
  */
#define READ_BUF_SIZE                    (1024 * 50)
#define OUT_BUF_SIZE                     (1024 * 8)
#define LCD_BUF_SIZE                     (320 * 240 * 2)
/**
  * @brief video file type
  */
typedef enum
{
  VIDEO_ERROR,
  VIDEO_AVI,
} video_file_type;

/**
  * @brief define key type
  */
typedef enum
{
  VIDEO_NO_KEY                           = 0x00,
  VIDEO_KEY_PREVIOUS                     = 0x01,
  VIDEO_KEY_NEXT                         = 0x02,
  VIDEO_KEY_PAUSE                        = 0x03
} video_key_type;

/**
  * @brief define video play type
  */
typedef enum
{
  VIDEO_PLAY_CONTINUE                    = 0x00,
  VIDEO_PLAY_STOP                        = 0x01
} video_play_type;

/**
  * @brief video type
  */
typedef struct
{
  FIL file;
  FILINFO file_info;

  video_key_type key;

  video_file_type type;
  uint32_t video_number;
  uint32_t video_id;
  volatile uint32_t video_frame;

  uint32_t sec_per_frame;
  uint32_t decode_cnt;
  uint32_t total_sec;
  uint32_t current_sec;
  uint32_t last_sec;
  uint32_t last_vol;

  uint32_t line_counter;

  uint32_t channel;
  uint32_t samplerate;

  uint32_t image_width;
  uint32_t image_height;
  uint32_t image_line_cnt;

  uint8_t *read_buf;
  uint16_t *out_buf;
  uint16_t *lcd_buf;
  uint32_t read_buf_size;
  uint32_t out_buf_size;
  uint32_t lcd_buf_size;
  uint32_t lcd_refresh_flag;
}video_type;


extern video_type video_info;
extern int16_t music_output_buffer[DMA_BUFFER_SIZE];

video_key_type video_key_get(void);
video_play_type video_play_stop_check(video_type *video);
uint16_t volume_value_get(void);
void volume_value_set(void);
void audio_freq_set(uint32_t freq);
void video_play(video_type *video);
error_status video_play_mjpeg(video_type *video, uint8_t *pname);

#ifdef __cplusplus
}
#endif

#endif
