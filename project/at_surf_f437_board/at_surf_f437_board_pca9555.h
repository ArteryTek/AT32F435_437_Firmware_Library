/**
  **************************************************************************
  * @file     at_surf_f437_board_pca9555.h
  * @brief    pca9555 application libray header file
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
#ifndef __AT_SURF_F437_BOARD_PCA9555_H
#define __AT_SURF_F437_BOARD_PCA9555_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "i2c_application.h"

/**
  * @brief pca9555 i2c reg address
  */
#define PCA_REG_INPUT                    0x00
#define PCA_REG_OUTPUT                   0x02
#define PCA_REG_POLARITY                 0x04
#define PCA_REG_CTRL                     0x06

/**
  * @brief pca9555 io definition
  */
#define PCA_IO0_PINS_0                   0x0001 /*!< io 0 pins number 0 */
#define PCA_IO0_PINS_1                   0x0002 /*!< io 0 pins number 1 */
#define PCA_IO0_PINS_2                   0x0004 /*!< io 0 pins number 2 */
#define PCA_IO0_PINS_3                   0x0008 /*!< io 0 pins number 3 */
#define PCA_IO0_PINS_4                   0x0010 /*!< io 0 pins number 4 */
#define PCA_IO0_PINS_5                   0x0020 /*!< io 0 pins number 5 */
#define PCA_IO0_PINS_6                   0x0040 /*!< io 0 pins number 6 */
#define PCA_IO0_PINS_7                   0x0080 /*!< io 0 pins number 7 */
#define PCA_IO1_PINS_0                   0x0100 /*!< io 1 pins number 0 */
#define PCA_IO1_PINS_1                   0x0200 /*!< io 1 pins number 1 */
#define PCA_IO1_PINS_2                   0x0400 /*!< io 1 pins number 2 */
#define PCA_IO1_PINS_3                   0x0800 /*!< io 1 pins number 3 */
#define PCA_IO1_PINS_4                   0x1000 /*!< io 1 pins number 4 */
#define PCA_IO1_PINS_5                   0x2000 /*!< io 1 pins number 5 */
#define PCA_IO1_PINS_6                   0x4000 /*!< io 1 pins number 6 */
#define PCA_IO1_PINS_7                   0x8000 /*!< io 1 pins number 7 */
#define PCA_IO_PINS_ALL                  0xFFFF /*!< io all pins */

/**
  * @brief pca9555 io mode type
  */
typedef enum
{
  PCA_IO_OUTPUT                          = 0x00, /*!< input mode */
  PCA_IO_INPUT                           = 0x01  /*!< output mode */
} pca9555_io_mode_type;

/**
  * @brief pca9555 io reg union
  */
typedef union
{
  __IO uint16_t  io;
  struct
  {
    __IO uint16_t io00                   : 1; /* [0] */
    __IO uint16_t io01                   : 1; /* [1] */
    __IO uint16_t io02                   : 1; /* [2] */
    __IO uint16_t io03                   : 1; /* [3] */
    __IO uint16_t io04                   : 1; /* [4] */
    __IO uint16_t io05                   : 1; /* [5] */
    __IO uint16_t io06                   : 1; /* [6] */
    __IO uint16_t io07                   : 1; /* [7] */
    __IO uint16_t io10                   : 1; /* [8] */
    __IO uint16_t io11                   : 1; /* [9] */
    __IO uint16_t io12                   : 1; /* [10] */
    __IO uint16_t io13                   : 1; /* [11] */
    __IO uint16_t io14                   : 1; /* [12] */
    __IO uint16_t io15                   : 1; /* [13] */
    __IO uint16_t io16                   : 1; /* [14] */
    __IO uint16_t io17                   : 1; /* [15] */
  } io_bit;
} pca9555_io_type;

/**
  * @brief pca9555 pin definition
  */
#define PCA_I2C_ADDRESS                  0x40

#define PCA_I2C_TIMEOUT                  0xFFFFFFF

#define PCA_I2C_CLKCTRL_100K             0x70F03C6B   //100K
#define PCA_I2C_CLKCTRL_200K             0x30F03C6B   //200K
#define PCA_I2C_CLKCTRL_400K             0x10F03D6B   //400K

#define PCA_I2C                          I2C2
#define PCA_I2C_CLK                      CRM_I2C2_PERIPH_CLOCK
#define PCA_I2C_DMA                      DMA1
#define PCA_I2C_DMA_CLK                  CRM_DMA1_PERIPH_CLOCK

#define PCA_I2C_SCL_GPIO_CLK             CRM_GPIOH_PERIPH_CLOCK
#define PCA_I2C_SCL_GPIO_PIN             GPIO_PINS_2
#define PCA_I2C_SCL_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE2
#define PCA_I2C_SCL_GPIO_PORT            GPIOH
#define PCA_I2C_SCL_GPIO_MUX             GPIO_MUX_4

#define PCA_I2C_SDA_GPIO_CLK             CRM_GPIOH_PERIPH_CLOCK
#define PCA_I2C_SDA_GPIO_PIN             GPIO_PINS_3
#define PCA_I2C_SDA_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE3
#define PCA_I2C_SDA_GPIO_PORT            GPIOH
#define PCA_I2C_SDA_GPIO_MUX             GPIO_MUX_4

#define PCA_I2C_EVT_IRQn                 I2C2_EVT_IRQn
#define PCA_I2C_ERR_IRQn                 I2C2_ERR_IRQn
#define PCA_I2C_EVT_IRQHandler           I2C2_EVT_IRQHandler
#define PCA_I2C_ERR_IRQHandler           I2C2_ERR_IRQHandler

#define PCA_I2C_DMA_TX_CHANNEL           DMA1_CHANNEL1
#define PCA_I2C_DMA_TX_DMAMUX_CHANNEL    DMA1MUX_CHANNEL1
#define PCA_I2C_DMA_TX_DMAREQ            DMAMUX_DMAREQ_ID_I2C2_TX
#define PCA_I2C_DMA_TX_IRQn              DMA1_Channel1_IRQn
#define PCA_I2C_DMA_TX_IRQHandler        DMA1_Channel1_IRQHandler

#define PCA_I2C_DMA_RX_CHANNEL           DMA1_CHANNEL2
#define PCA_I2C_DMA_RX_DMAMUX_CHANNEL    DMA1MUX_CHANNEL2
#define PCA_I2C_DMA_RX_DMAREQ            DMAMUX_DMAREQ_ID_I2C2_RX
#define PCA_I2C_DMA_RX_IRQn              DMA1_Channel2_IRQn
#define PCA_I2C_DMA_RX_IRQHandler        DMA1_Channel2_IRQHandler

#define PCA_INT_GPIO_PORT                GPIOG
#define PCA_INT_GPIO_CLK                 CRM_GPIOG_PERIPH_CLOCK
#define PCA_INT_GPIO_PIN                 GPIO_PINS_3

#define PCA_INT_SOURCE_PORT              SCFG_PORT_SOURCE_GPIOG
#define PCA_INT_SOURCE_PIN               SCFG_PINS_SOURCE3
#define PCA_INT_EXINT_LINE               EXINT_LINE_3
#define PCA_INT_EXINT_IRQn               EXINT3_IRQn

#define PCA_INT_READ()                   gpio_input_data_bit_read(PCA_INT_GPIO_PORT, PCA_INT_GPIO_PIN)

extern i2c_handle_type hi2c_pca;

/**
  * @brief pca9555 exported functions
  */
void         pca9555_init(uint32_t i2c_speed);
void         pca9555_irq_handler(void);
void         pca9555_io_scan(void);

error_status pca9555_input_mode_config(uint16_t pins);
error_status pca9555_out_mode_config(uint16_t pins);

flag_status  pca9555_input_data_bit_read(uint16_t pins);
uint16_t     pca9555_input_data_read(void);
error_status pca9555_bits_reset(uint16_t pins);
error_status pca9555_bits_set(uint16_t pins);

#ifdef __cplusplus
}
#endif

#endif

