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

#include "at_surf_f437_board_key.h"
#include "at_surf_f437_board_sd_card.h"
#include "at_surf_f437_board_lcd.h"
#include "at_surf_f437_board_key.h"
#include "at_surf_f437_board_joystick.h"
#include "at_surf_f437_board_malloc.h"
#include "at_surf_f437_board_delay.h"
#include "ff.h"
#include "string.h"
#include "stdio.h"
#include "cmsis_compiler.h"

/**
  * @brief memory size
  */
#define READ_BUF_SIZE                    (1024 * 50)
#define OUT_BUF_SIZE                     (1024 * 8)

/**
  * @brief rgb888 to rgb565
  */
#define RGB888_TO_RGB565(r, g, b)        (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

/**
  * @brief bmp para
  */
#define BI_RGB                           0
#define BI_RLE8                          1
#define BI_RLE4                          2
#define BI_BITFIELDS                     3

#define LCD_MAX_LOG_COLORS               256
#define MAX_NUM_LWZ_BITS                 12

/**
  * @brief gif file eoc
  */
#define GIF_INTRO_TERMINATOR             ';'  /* 0x3B */
#define GIF_INTRO_EXTENSION              '!'  /* 0x21 */
#define GIF_INTRO_IMAGE                  ','  /* 0x2C */

#define GIF_COMMENT                      0xFE
#define GIF_APPLICATION                  0xFF
#define GIF_PLAINTEXT                    0x01
#define GIF_GRAPHICCTL                   0xF9

/**
  * @brief picture file type
  */
typedef enum
{
  PICTURE_ERROR,
  PICTURE_BMP,
  PICTURE_JPEG,
  PICTURE_GIF,
  PICTURE_PNG
} picture_file_type;

/**
  * @brief decoder type
  */
typedef enum
{
  DECODER_BMP                            = 0x00,
  DECODER_LIBJPEG                        = 0x01,
  DECODER_TJPGD                          = 0x02,
  DECODER_GIF                            = 0x03,
  DECODER_PNG                            = 0x04
} decoder_type;

/**
  * @brief bmp type
  */
typedef __PACKED_STRUCT
{
  uint16_t bfType;
  uint32_t bfSize;
  uint16_t bfReserved1;
  uint16_t bfReserved2;
  uint32_t bfOffBits;
  uint32_t biSize;
  int32_t  biWidth;
  int32_t  biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int32_t  biXPelsPerMeter;
  int32_t  biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
}bmp_header_type;

/**
  * @brief lzw info type
  */
typedef struct
{
  uint8_t aBuffer[258];                     /* input buffer for data block */
  int16_t aCode  [(1 << MAX_NUM_LWZ_BITS)]; /* this array stores the lzw codes for the compressed strings */
  uint8_t aPrefix[(1 << MAX_NUM_LWZ_BITS)]; /* prefix character of the lzw code. */
  uint8_t aDecompBuffer[3000];              /* decompression buffer. the higher the compression, the more bytes are needed in the buffer. */
  uint8_t *sp;                              /* pointer into the decompression buffer */
  int32_t CurBit;
  int32_t LastBit;
  int32_t GetDone;
  int32_t LastByte;
  int32_t ReturnClear;
  int32_t CodeSize;
  int32_t SetCodeSize;
  int32_t MaxCode;
  int32_t MaxCodeSize;
  int32_t ClearCode;
  int32_t EndCode;
  int32_t FirstCode;
  int32_t OldCode;
}lzw_info_type;

/**
  * @brief logical screen description block
  */
typedef __PACKED_STRUCT
{
  uint16_t width;                        /* gif width */
  uint16_t height;                       /* gif height */
  uint8_t  flag;                         /* identifier 1:3:1:3=global color table flag (1): color depth (3): classification flag (1): global color table size (3) */
  uint8_t  bkcindex;                     /* index of background color in the global color table (only valid when the global color table exists) */
  uint8_t  pixratio;                     /* pixel aspect ratio */
}LogicalScreenDescriptor;

/**
  * @brief image description block
  */
typedef __PACKED_STRUCT
{
  uint16_t xoff;                         /* x-direction offset */
  uint16_t yoff;                         /* y-direction offset */
  uint16_t width;                        /* width */
  uint16_t height;                       /* height */
  uint8_t flag;                          /* identifier 1:1:1:2:3=local color table flag (1): interwoven flag (1): reserved (2): local color table size (3) */
}ImageScreenDescriptor;

/**
  * @brief image description
  */
typedef __PACKED_STRUCT
{
  LogicalScreenDescriptor gifLSD;        /* logical screen description block */
  ImageScreenDescriptor   gifISD;        /* fast image description */
  uint16_t colortbl[256];                /* currently using color chart */
  uint16_t bkpcolortbl[256];             /* backup color chart when there is a local color chart, use it */
  uint16_t numcolors;                    /* color chart size */
  uint16_t delay;                        /* delay time */
  lzw_info_type *lzw;                    /* lzw information */
}gif89a_type;

/**
  * @brief picture type
  */
typedef struct
{
  FIL file;
  FILINFO file_info;
  uint32_t file_size;
  picture_file_type file_type;

  uint32_t picture_number;
  uint32_t picture_id;
  uint32_t display_xpos;
  uint32_t display_ypos;
  uint32_t image_width;
  uint32_t image_height;
  uint32_t image_line_cnt;
  float fps;

  bmp_header_type bmp;
  gif89a_type *gif89a;

  uint8_t *read_buf;
  uint16_t *out_buf;
  uint32_t read_buf_size;
  uint32_t out_buf_size;
}picture_type;

extern picture_type picture_info;

void picture_display(picture_type *picture);

uint32_t rgb888_to_rgb565(uint8_t *rgb888, uint32_t rgb888_size, uint16_t *rgb565);
uint32_t rgb555_to_rgb565(uint8_t *rgb555, uint32_t rgb555_size, uint16_t *rgb565);
uint32_t rgba888_to_rgb565(uint8_t *rgb888, uint32_t rgb888_size, uint16_t *rgb565);

error_status jpeg_libjpeg_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos);
error_status jpeg_tjpgd_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos);
error_status bmp_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos);
error_status gif_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height);
error_status png_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos);

#ifdef __cplusplus
}
#endif

#endif
