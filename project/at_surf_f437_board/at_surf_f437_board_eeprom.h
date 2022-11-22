/**
  **************************************************************************
  * @file     at_surf_f437_board_eeprom.h
  * @brief    eeprom application libray header file
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT_SURF_F437_BOARD_EEPROM_H
#define __AT_SURF_F437_BOARD_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "i2c_application.h"

/**
  * @brief eeprom operation mode type
  */
typedef enum
{
  EE_MODE_POLL = 0,
  EE_MODE_INT,
  EE_MODE_DMA,
} eeprom_i2c_mode_type;

/**
  * @brief eeprom definition
  */
#define EE_PAGE_SIZE                     0x08
#define EE_I2C_ADDRESS                   0xA0

#define EE_I2C_TIMEOUT                   0xFFFFFFF

#define EE_I2C_CLKCTRL_200K              0x30F03C6B   //200K
#define EE_I2C_CLKCTRL_400K              0x10F03D6B   //400K

#define EE_I2C                           I2C2
#define EE_I2C_CLK                       CRM_I2C2_PERIPH_CLOCK
#define EE_I2C_DMA                       DMA1
#define EE_I2C_DMA_CLK                   CRM_DMA1_PERIPH_CLOCK

#define EE_I2C_SCL_GPIO_CLK              CRM_GPIOH_PERIPH_CLOCK
#define EE_I2C_SCL_GPIO_PIN              GPIO_PINS_2
#define EE_I2C_SCL_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE2
#define EE_I2C_SCL_GPIO_PORT             GPIOH
#define EE_I2C_SCL_GPIO_MUX              GPIO_MUX_4

#define EE_I2C_SDA_GPIO_CLK              CRM_GPIOH_PERIPH_CLOCK
#define EE_I2C_SDA_GPIO_PIN              GPIO_PINS_3
#define EE_I2C_SDA_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE3
#define EE_I2C_SDA_GPIO_PORT             GPIOH
#define EE_I2C_SDA_GPIO_MUX              GPIO_MUX_4

#define EE_I2C_EVT_IRQn                  I2C2_EVT_IRQn
#define EE_I2C_ERR_IRQn                  I2C2_ERR_IRQn
#define EE_I2C_EVT_IRQHandler            I2C2_EVT_IRQHandler
#define EE_I2C_ERR_IRQHandler            I2C2_ERR_IRQHandler

#define EE_I2C_DMA_TX_CHANNEL            DMA1_CHANNEL1
#define EE_I2C_DMA_TX_DMAMUX_CHANNEL     DMA1MUX_CHANNEL1
#define EE_I2C_DMA_TX_DMAREQ             DMAMUX_DMAREQ_ID_I2C2_TX
#define EE_I2C_DMA_TX_IRQn               DMA1_Channel1_IRQn
#define EE_I2C_DMA_TX_IRQHandler         DMA1_Channel1_IRQHandler

#define EE_I2C_DMA_RX_CHANNEL            DMA1_CHANNEL2
#define EE_I2C_DMA_RX_DMAMUX_CHANNEL     DMA1MUX_CHANNEL2
#define EE_I2C_DMA_RX_DMAREQ             DMAMUX_DMAREQ_ID_I2C2_RX
#define EE_I2C_DMA_RX_IRQn               DMA1_Channel2_IRQn
#define EE_I2C_DMA_RX_IRQHandler         DMA1_Channel2_IRQHandler

extern i2c_handle_type hi2c_ee;

/**
  * @brief eeprom exported functions
  */
void eeprom_init(uint32_t i2c_speed);
i2c_status_type eeprom_data_read(i2c_handle_type* hi2c, eeprom_i2c_mode_type mode, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t number, uint32_t timeout);
i2c_status_type eeprom_data_write(i2c_handle_type* hi2c, eeprom_i2c_mode_type mode, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t number, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif

