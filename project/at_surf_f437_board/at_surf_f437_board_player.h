/**
  **************************************************************************
  * @file     at_surf_f437_board_player.c
  * @brief    header file for at-start board.
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

#ifndef __AT32_SURF_F437_BOARD_PALYER_H
#define __AT32_SURF_F437_BOARD_PALYER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at_surf_f437_board_audio.h"
#include "at_surf_f437_board_key.h"
#include "at_surf_f437_board_sd_card.h"
#include "at_surf_f437_board_lcd.h"
#include "at_surf_f437_board_variable_resistor.h"
#include "at_surf_f437_board_key.h"
#include "at_surf_f437_board_joystick.h"
#include "at_surf_f437_board_malloc.h"
#include "ff.h" 
#include "string.h"
#include "stdio.h"

/**
  * @brief get min value
  */
#define AUDIO_MIN(x,y)  ((x)<(y)? (x):(y))

/**
  * @brief music file type
  */
typedef enum
{
  MUSIC_ERROR,
  MUSIC_MP3,
  MUSIC_WAV,
  MUSIC_FLAC,
  MUSIC_APE
} music_file_type;

/**
  * @brief define key type
  */
typedef enum
{
  MUSIC_NO_KEY                           = 0x00,  
  MUSIC_KEY_PREVIOUS                     = 0x01,
  MUSIC_KEY_NEXT                         = 0x02,
  MUSIC_KEY_PAUSE                        = 0x03
} music_key_type;

/**
  * @brief define key type
  */
typedef enum
{
  MUSIC_PLAY_CONTINUE                    = 0x00,  
  MUSIC_PLAY_STOP                        = 0x01
} music_play_type;

/**
  * @brief audio type
  */
typedef struct
{
  FIL file;
  FILINFO file_info;
  
  music_key_type key;   
  
  music_file_type music_type;
  uint32_t music_number;
  uint32_t music_id;
  
  uint32_t decode_cnt;  
  uint32_t decode_size;
  uint32_t file_size;  
  uint32_t total_sec;
  uint32_t current_sec;
  uint32_t error_cnt;

  uint32_t bps;
  uint32_t channel;
  uint32_t samplerate;
  uint32_t totalsamples;

  uint16_t tx_size;
  uint8_t *read_buf;
  int32_t *decoded0;
  int32_t *decoded1;
}audio_type;  

extern audio_type audio_info; 
extern int16_t music_output_buffer[DMA_BUFFER_SIZE];

music_key_type music_key_get(void);
music_play_type music_play_stop_check(audio_type *audio);
uint16_t volume_value_get(void);
void volume_value_set(void);
void audio_freq_set(uint32_t freq);
void audio_dma_counter_set(uint16_t counter);
void audio_data_convert_to_i2s(audio_type *audio, int32_t *decode_left, int32_t *decode_right, uint32_t sample);
void music_play(audio_type *audio);
void music_play_init(audio_type *audio);
void music_info_display(audio_type *audio);
error_status mp3_song_play(audio_type *audio, uint8_t *pname);
error_status wav_song_play(audio_type *audio, uint8_t *pname);
error_status flac_song_play(audio_type *audio, uint8_t *pname);
error_status ape_song_play(audio_type *audio, uint8_t *pname);
error_status buf_cmp(uint8_t* buffer1, uint8_t* buffer2, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
