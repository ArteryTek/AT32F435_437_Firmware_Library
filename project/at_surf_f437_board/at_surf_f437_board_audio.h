/**
  **************************************************************************
  * @file     at_surf_f437_board_audio.c
  * @version  v2.0.7
  * @date     2022-04-02
  * @brief    header file for at-start board. set of firmware functions to
  *           manage leds and push-button. initialize delay function.
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

#ifndef __AT32_SURF_F437_BOARD_AUDIO_H
#define __AT32_SURF_F437_BOARD_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at_surf_f437_board_pca9555.h"

/** @defgroup USB_device_audio_codec_reg_definition
  * @{
  */
#define WM8988_R0_LEFT_INPUT_VOLUME                0x00    /*Left channel PGA control*/
#define WM8988_R1_RIGHT_INPUT_VOLUME               0x01    /*Right channel PGA control*/
#define WM8988_R2_LOUT1_VOLUME                     0x02
#define WM8988_R3_ROUT1_VOLUME                     0x03
#define WM8988_R4_RESERVED                         0x04
#define WM8988_R5_ADC_DAC_CONTROL                  0x05
#define WM8988_R6_RESERVED                         0x06
#define WM8988_R7_AUDIO_INTERFACE                  0x07
#define WM8988_R8_SAMPLE_RATE                      0x08
#define WM8988_R9_RESERVED                         0x09
#define WM8988_R10_LEFT_DAC_VOLUME                 0x0A
#define WM8988_R11_RIGHT_DAC_VOLUME                0x0B
#define WM8988_R12_BASS_CONTROL                    0x0C
#define WM8988_R13_TREBLE_CONTROL                  0x0D
#define WM8988_R14_
#define WM8988_R15_RESET                           0x0F
#define WM8988_R16_3D_CONTROL                      0x10
#define WM8988_R17_ALC1                            0x11
#define WM8988_R18_ALC2                            0x12
#define WM8988_R19_ALC3                            0x13
#define WM8988_R20_NOISE_GATE                      0x14
#define WM8988_R21_LEFT_ADC_VOLUME                 0x15
#define WM8988_R22_RIGHT_ADC_VOLUME                0x16
#define WM8988_R23_ADDITIONAL_1_CONTROL            0x17
#define WM8988_R24_ADDITIONAL_2_CONTROL            0x18
#define WM8988_R25_PWR_1_MGMT                      0x19
#define WM8988_R26_PWR_2_MGMT                      0x1A
#define WM8988_R27_ADDITIONAL_3_CONTROL            0x1B

#define WM8988_R31_ADC_INPUT_MODE                  0x1F
#define WM8988_R32_ADC_L_SIGNAL_PATH               0x20
#define WM8988_R33_ADC_R_SIGNAL_PATH               0x21
#define WM8988_R34_LEFT_OUT_MIX_1                  0x22
#define WM8988_R35_LEFT_OUT_MIX_2                  0x23
#define WM8988_R36_RIGHT_OUT_MIX_1                 0x24
#define WM8988_R37_RIGHT_OUT_MIX_2                 0x25
#define WM8988_R38_RESERVED
#define WM8988_R39_RESERVED
#define WM8988_R40_LOUT2_VOLUME                    0x28
#define WM8988_R41_ROUT2_VOLUME                    0x29
#define WM8988_R42_RESERVED
#define WM8988_R43_LOW_POWER_PALYBACK              0x43

#define WM8988_I2C_ADDR_CSB_LOW                    0x34
#define WM8988_I2C_ADDR_CSB_HIGH                   0x36

/**
  * @}
  */

/** @defgroup USB_device_audio_codec_pin_definition
  * @{
  */
#define I2S_DMAx_CLK                     CRM_DMA1_PERIPH_CLOCK
#define I2S_SPIx_CLK                     CRM_SPI2_PERIPH_CLOCK

#define I2S_CK_GPIO_CLK                  CRM_GPIOB_PERIPH_CLOCK
#define I2S_CK_GPIO_PIN                  GPIO_PINS_13
#define I2S_CK_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE13
#define I2S_CK_GPIO_PORT                 GPIOB
#define I2S_CK_GPIO_MUX                  GPIO_MUX_5

#define I2S_WS_GPIO_CLK                  CRM_GPIOB_PERIPH_CLOCK
#define I2S_WS_GPIO_PIN                  GPIO_PINS_12
#define I2S_WS_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE12
#define I2S_WS_GPIO_PORT                 GPIOB
#define I2S_WS_GPIO_MUX                  GPIO_MUX_5

#define I2S_SD_OUT_GPIO_CLK              CRM_GPIOB_PERIPH_CLOCK
#define I2S_SD_OUT_GPIO_PIN              GPIO_PINS_15
#define I2S_SD_OUT_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE15
#define I2S_SD_OUT_GPIO_PORT             GPIOB
#define I2S_SD_OUT_GPIO_MUX              GPIO_MUX_5

#define I2S_SD_IN_GPIO_CLK               CRM_GPIOB_PERIPH_CLOCK
#define I2S_SD_IN_GPIO_PIN               GPIO_PINS_14
#define I2S_SD_IN_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE14
#define I2S_SD_IN_GPIO_PORT              GPIOB
#define I2S_SD_IN_GPIO_MUX               GPIO_MUX_6

#define SPK_TX_FIFO_SIZE                 (1024 * 4)
#define I2S_DT_ADDRESS                   (&(SPI2->dt))
/**
  * @}
  */

/** @defgroup USB_device_audio_codec_exported_functions
  * @{
  */
#define MIC_BUFFER_SIZE                  1024
#define SPK_BUFFER_SIZE                  4096
#define DMA_BUFFER_SIZE                  2304

typedef struct
{
  uint32_t audio_freq;
  uint32_t audio_bitw;
  //spk part
  uint16_t spk_buffer[SPK_BUFFER_SIZE];
  uint16_t *spk_roff;
  uint16_t *spk_woff;
  uint16_t *spk_rend;
  uint32_t spk_freq;
  uint32_t spk_wtotal;
  uint32_t spk_rtotal;
  uint16_t spk_threshold;
  uint16_t spk_calc;
  uint8_t  spk_stage;

  //mic part
  uint16_t mic_buffer[MIC_BUFFER_SIZE];
  uint16_t *mic_roff;
  uint16_t *mic_woff;
  uint16_t *mic_wend;
  uint32_t mic_wtotal;
  uint32_t mic_rtotal;
  uint32_t mic_delta;
  uint16_t mic_calc;
  uint16_t mic_adj_count;
  uint8_t  mic_adj_stage;
  uint8_t  mic_stage;

  uint8_t mic_mute;
  uint8_t spk_mute;

  uint16_t mic_volume;
  uint16_t spk_volume;

  uint32_t spk_tx_size;
  uint32_t mic_rx_size;

  uint32_t spk_enable;
  uint32_t mic_enable;

}audio_codec_type;


#define AUDIO_SUPPORT_SPK                1
#define AUDIO_SUPPORT_MIC                1
#define AUDIO_SUPPORT_FEEDBACK           1

#define AUDIO_SUPPORT_FREQ_16K           0
#define AUDIO_SUPPORT_FREQ_48K           1


#define AUDIO_SUPPORT_FREQ               (AUDIO_SUPPORT_FREQ_16K + \
                                          AUDIO_SUPPORT_FREQ_48K \
                                         )

#define AUDIO_FREQ_16K                   16000
#define AUDIO_FREQ_48K                   48000
#define AUDIO_BITW_16                    16

#define AUDIO_MIC_CHANEL_NUM            2
#define AUDIO_MIC_DEFAULT_BITW          AUDIO_BITW_16

#define AUDIO_SPK_CHANEL_NUM            2
#define AUDIO_SPK_DEFAULT_BITW          AUDIO_BITW_16


#define AUDIO_SUPPORT_MAX_FREQ           48
#define AUDIO_DEFAULT_FREQ               AUDIO_FREQ_48K
#define AUDIO_DEFAULT_BITW               AUDIO_BITW_16

#define AUDIO_PA_ON_PIN                 PCA_IO0_PINS_2


error_status audio_init(void);
error_status audio_codec_loop(void);
void audio_codec_modify_freq(uint32_t freq);

/**
  * @brief audio codec interface
  */
void audio_codec_spk_fifo_write(uint8_t *data, uint32_t len);
uint32_t audio_codec_mic_get_data(uint8_t *buffer);
uint8_t audio_codec_spk_feedback(uint8_t *feedback);
void audio_codec_spk_alt_setting(uint32_t alt_seting);
void audio_codec_mic_alt_setting(uint32_t alt_seting);
void audio_codec_set_mic_mute(uint8_t mute);
void audio_codec_set_spk_mute(uint8_t mute);
void audio_codec_set_mic_volume(uint16_t volume);
void audio_codec_set_spk_volume(uint16_t volume);
void audio_codec_set_mic_freq(uint32_t freq);
void audio_codec_set_spk_freq(uint32_t freq);

error_status audio_spk_volume_set(uint8_t left_volume, uint8_t right_volume);
#ifdef __cplusplus
}
#endif

#endif
