/**
  **************************************************************************
  * @file     at_surf_f437_board_avi.h
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

#ifndef __AT32_SURF_F437_BOARD_VIDEO_AVI_H
#define __AT32_SURF_F437_BOARD_VIDEO_AVI_H

#include "at32f435_437.h"
#include "ff.h"
#include "cmsis_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief avi information
  */
#define AVI_RIFF_ID                      0x46464952
#define AVI_AVI_ID                       0x20495641
#define AVI_LIST_ID                      0x5453494C
#define AVI_HDRL_ID                      0x6C726468 /* information block flag */
#define AVI_MOVI_ID                      0x69766F6D /* data block flag */
#define AVI_STRL_ID                      0x6C727473 /* strl flag */

#define AVI_AVIH_ID                      0x68697661
#define AVI_STRH_ID                      0x68727473
#define AVI_STRF_ID                      0x66727473
#define AVI_STRD_ID                      0x64727473

#define AVI_VIDS_STREAM                  0x73646976 /* video streaming */
#define AVI_AUDS_STREAM                  0x73647561 /* audio stream */

#define AVI_VIDS_FLAG                    0x6463     /* video stream flag */
#define AVI_AUDS_FLAG                    0x7762     /* avi information structure */
#define AVI_FORMAT_MJPG                  0x47504A4D

/**
  * @brief avi error type
  */
typedef enum {
  AVI_OK = 0,                            /* 0, successful */
  AVI_RIFF_ERR,                          /* 1. RIFF ID read failed */
  AVI_AVI_ERR,                           /* 2. AVI ID read failed */
  AVI_LIST_ERR,                          /* 3. LIST ID read failed */
  AVI_HDRL_ERR,                          /* 4. HDRL ID read failed */
  AVI_AVIH_ERR,                          /* 5. AVIH ID read failed */
  AVI_STRL_ERR,                          /* 6. STRL ID read failed */
  AVI_STRH_ERR,                          /* 7. STRH ID read failed */
  AVI_STRF_ERR,                          /* 8. STRF ID read failed */
  AVI_MOVI_ERR,                          /* 9. MOVI ID read failed */
  AVI_FORMAT_ERR,                        /* 10. Format error */
  AVI_STREAM_ERR,                        /* 11. Flow error */
}avi_status_type;

/**
  * @brief avi information structure
  */
typedef __PACKED_STRUCT
{
  uint32_t SecPerFrame;                  /* video frame interval time (unit: us) */
  uint32_t TotalFrame;                   /* total number of frames in the file */
  uint32_t Width;                        /* image width */
  uint32_t Height;                       /* image high */
  uint32_t SampleRate;                   /* audio sampling rate */
  uint16_t Channels;                     /* the number of channels */
  uint16_t AudioBufSize;                 /* audio buffer size */
  uint16_t AudioType;                    /* audio type: 0x0001=PCM; 0x0050=MP2; 0x0055=MP3; 0x2000=AC3; */
  uint16_t StreamID;                     /* stream type ID, StreamID=='dc'==0x6463 /StreamID=='wb'==0x7762 */
  uint32_t StreamSize;                   /* the stream size must be even. if an odd number is read, add 1 to make it even */
  uint8_t* VideoFLAG;                    /* video frame flag, VideoFLAG="00dc"/"01dc" */
  uint8_t* AudioFLAG;                    /* audio frame flag, AudioFLAG="00wb"/"01wb" */
}avi_info_type;

/**
  * @brief avi header information
  */
typedef struct
{
  uint32_t RiffID;                       /* RiffID=='RIFF'==0x61766968 */
  uint32_t FileSize;                     /* avi file size (excluding the first 8 bytes, RIFFID, and filesize) */
  uint32_t AviID;                        /* AviID=='AVI '==0x41564920 */
}avi_header_type;

/**
  * @brief avi frame information
  */
typedef struct
{
  uint32_t FrameID;                      /* frame ID, FrameID=='RIFF'==0x61766968 */
  uint32_t FrameSize;                    /* frame size */
}avi_frame_type;

/**
  * @brief list information
  */
typedef struct
{
  uint32_t ListID;                       /* listID=='LIST'==0x4c495354 */
  uint32_t BlockSize;                    /* block size (excluding the first 8 bytes, ListID, and BlockSize) */
  uint32_t ListType;                     /* list sub block type: hdrl (information block)/movi (data block)/idxl (index block, optional, not mandatory) */
}avi_list_type;

/**
  * @brief avih information
  */
typedef struct
{
  uint32_t BlockID;                      /* block flag: avih==0x61766968 */
  uint32_t BlockSize;                    /* block size (excluding the first 8 bytes, i.e. BlockID and BlockSize are not included) */
  uint32_t SecPerFrame;                  /* video frame interval time (unit: us) */
  uint32_t MaxByteSec;                   /* maximum data transfer rate, bytes per second */
  uint32_t PaddingGranularity;           /* granularity of data filling */
  uint32_t Flags;                        /* global tags of avi files, such as whether they contain index blocks, etc */
  uint32_t TotalFrame;                   /* total number of frames in the file */
  uint32_t InitFrames;                   /* specify the initial frame rate for interactive formats (non interactive formats should be specified as 0) */
  uint32_t Streams;                      /* the number of types of data streams included is usually 2 */
  uint32_t RefBufSize;                   /* suggest reading the cache size of this file (which should be able to accommodate the maximum block). the default size */
  uint32_t Width;                        /* image width */
  uint32_t Height;                       /* image high */
  uint32_t Reserved[4];                  /* reserved */
}avih_header_type;

/**
  * @brief strh header information
  */
typedef struct
{
  uint32_t BlockID;                      /* block flag: strh==0x73747268 */
  uint32_t BlockSize;                    /* block size (excluding the first 8 bytes, i.e. BlockID and BlockSize are not included) */
  uint32_t StreamType;                   /* types of data streams, vids(0x73646976): Video; auds(0x73647561): audio frequency */
  uint32_t Handler;                      /* the designated stream processor is a decoder for audio and video, such as MJPG/H264 */
  uint32_t Flags;                        /* tag: Allow this stream to output? Has the color palette changed? */
  uint16_t Priority;                     /* priority of streams (when there are multiple streams of the same type, the default stream has the highest priority) */
  uint16_t Language;                     /* language code for audio */
  uint32_t InitFrames;                   /* specify the initial frame rate for the interaction format */
  uint32_t Scale;                        /* data volume, the size of each frame in the video or the sampling size of the audio */
  uint32_t Rate;                         /* scale/Rate=number of samples per second */
  uint32_t Start;                        /* the position where the data stream starts playing, measured in Scale */
  uint32_t Length;                       /* the amount of data in the data stream, measured in Scale */
  uint32_t RefBufSize;                   /* suggested buffer size for use */
  uint32_t Quality;                      /* decompression quality parameter, the larger the value, the better the quality */
  uint32_t SampleSize;                   /* sample size of audio */
  struct                                 /* the rectangle occupied by the video frame */
  {
    short Left;
    short Top;
    short Right;
    short Bottom;
  }Frame;
}strh_header_type;

/**
  * @brief bmp header information
  */
typedef struct
{
  uint32_t BmpSize;                      /* bmp structure size, including (bmpsize) */
  long Width;                            /* image width */
  long Height;                           /* image high */
  uint16_t Planes;                       /* the number of planes must be 1 */
  uint16_t BitCount;                     /* pixel bits, 0x0018 represents 24 bits */
  uint32_t Compression;                  /* compression type, such as MJPG/H264, etc */
  uint32_t SizeImage;                    /* image size */
  long XpixPerMeter;                     /* horizontal resolution */
  long YpixPerMeter;                     /* vertical resolution */
  uint32_t ClrUsed;                      /* actually used the number of colors in the color palette, not in the compressed format */
  uint32_t ClrImportant;                 /* important colors */
}avi_bmp_header_type;

/**
  * @brief color table type
  */
typedef struct
{
  uint8_t rgbBlue;                       /* brightness of blue (value range 0-255) */
  uint8_t rgbGreen;                      /* the brightness of green (value range 0-255) */
  uint8_t rgbRed;                        /* the brightness of red (value range 0-255) */
  uint8_t rgbReserved;                   /* reserved, must be 0 */
}avi_rgb_type;

/**
  * @brief if it is a video stream, strf (stream format) use the strh_bmp_header block
  */
typedef struct
{
  uint32_t BlockID;                      /* block flag, strf==0x73747266 */
  uint32_t BlockSize;                    /* block size (excluding the first 8 bytes, i.e. BlockID and this BlockSize are not included) */
  avi_bmp_header_type bmiHeader;         /* bitmap header */
  avi_rgb_type bmColors[1];              /* color table */
}strf_bmp_header_type;

/**
  * @brief if it is an audio stream, strf (stream format) makes the strh_wav_header block
  */
typedef struct
{
  uint32_t BlockID;                      /* block flag, strf==0x73747266 */
  uint32_t BlockSize;                    /* block size (excluding the first 8 bytes, i.e. BlockID and this BlockSize are not included) */
  uint16_t FormatTag;                    /* format symbol: 0x0001=PCM, 0x0055=MP3 */
  uint16_t Channels;                     /* the number of channels, usually 2, represents stereo sound */
  uint32_t SampleRate;                   /* audio sampling rate */
  uint32_t BaudRate;                     /* baud rate */
  uint16_t BlockAlign;                   /* data block alignment flag */
  uint16_t Size;                         /* the size of the structure */
}strf_wav_header_type;

#define MAKEWORD(ptr)   (uint16_t)(((uint16_t)*((uint8_t*)(ptr))<<8)|(uint16_t)*(uint8_t*)((ptr)+1))
#define MAKEDWORD(ptr)  (uint32_t)(((uint16_t)*(uint8_t*)(ptr)|(((uint16_t)*(uint8_t*)(ptr+1))<<8)|\
                         (((uint16_t)*(uint8_t*)(ptr+2))<<16)|(((uint16_t)*(uint8_t*)(ptr+3))<<24)))

#define LCD_DMA_TX_CHANNEL               DMA1_CHANNEL6
#define LCD_DMA_TX_IRQn                  DMA1_Channel6_IRQn
#define LCD_DMA_TX_IRQHandler            DMA1_Channel6_IRQHandler
#define LCD_DMA_TX_FDT_FLAG              DMA1_FDT6_FLAG

#define LCD_DMA_BUF_CHANNEL              DMA1_CHANNEL7

#define LCD_DMA_REFRESH

extern avi_info_type avix;
void copy_buff(uint16_t *dest, uint16_t *src, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
