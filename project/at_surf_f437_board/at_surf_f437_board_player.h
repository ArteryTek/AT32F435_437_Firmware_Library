/**
  **************************************************************************
  * @file     at_surf_f437_board_player.c
  * @version  v2.0.7
  * @date     2022-04-02
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
#include "ff.h"
#include "string.h"
#include "stdio.h"

typedef enum
{
  MUSIC_ERROR,
  MUSIC_MP3,
  MUSIC_WAV,
  MUSIC_FLAC,
  MUSIC_APE
}music_format_type;

typedef struct
{
  uint32_t decode_cnt;
  uint32_t read_cnt;
  uint32_t error_cnt;
  uint32_t file_size;
  uint32_t decode_size;
  uint32_t total_time;
  uint32_t current_time;
  music_format_type music_type;
  key_type key;
}audio_type;

#define MP3_READBUF_SIZE                 (1152 * 8)
#define MP3_DECODE_BUFFER_SIZE           DMA_BUFFER_SIZE


extern uint8_t music_read_buffer[MP3_READBUF_SIZE];
extern int16_t music_output_buffer[MP3_DECODE_BUFFER_SIZE];

extern FILINFO file_music_info;
extern FIL file_music;
extern uint8_t name_buf[FF_LFN_BUF + 1];

extern audio_type audio_info;

extern int16_t music_output_buffer[MP3_DECODE_BUFFER_SIZE];

extern void Convert_Mono(short *buffer);
extern void Convert_Stereo(short *buffer);
extern volatile uint32_t i2s_dma_tx_end;

void music_play(void);
uint16_t volume_value_get(void);
void volume_value_set(void);

error_status mp3_song_play(uint8_t *pname);
error_status wav_song_play(uint8_t *pname);
error_status buf_cmp(uint8_t* buffer1, uint8_t* buffer2, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
