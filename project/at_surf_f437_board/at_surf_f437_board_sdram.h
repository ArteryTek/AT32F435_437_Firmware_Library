/**
  **************************************************************************
  * @file     at_surf_f437_board_sdram.h
  * @brief    sdram application libray header file
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
#ifndef __AT_SURF_F437_BOARD_SDRAM_H
#define __AT_SURF_F437_BOARD_SDRAM_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

/**
  * @brief the sdram io definition
  */
#define SDRAM_D0_GPIO_PORT               GPIOD
#define SDRAM_D0_GPIO_CLK                CRM_GPIOD_PERIPH_CLOCK
#define SDRAM_D0_GPIO_PIN                GPIO_PINS_14
#define SDRAM_D0_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE14
#define SDRAM_D0_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D1_GPIO_PORT               GPIOD
#define SDRAM_D1_GPIO_CLK                CRM_GPIOD_PERIPH_CLOCK
#define SDRAM_D1_GPIO_PIN                GPIO_PINS_15
#define SDRAM_D1_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE15
#define SDRAM_D1_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D2_GPIO_PORT               GPIOD
#define SDRAM_D2_GPIO_CLK                CRM_GPIOD_PERIPH_CLOCK
#define SDRAM_D2_GPIO_PIN                GPIO_PINS_0
#define SDRAM_D2_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE0
#define SDRAM_D2_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D3_GPIO_PORT               GPIOD
#define SDRAM_D3_GPIO_CLK                CRM_GPIOD_PERIPH_CLOCK
#define SDRAM_D3_GPIO_PIN                GPIO_PINS_1
#define SDRAM_D3_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE1
#define SDRAM_D3_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D4_GPIO_PORT               GPIOE
#define SDRAM_D4_GPIO_CLK                CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_D4_GPIO_PIN                GPIO_PINS_7
#define SDRAM_D4_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE7
#define SDRAM_D4_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D5_GPIO_PORT               GPIOE
#define SDRAM_D5_GPIO_CLK                CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_D5_GPIO_PIN                GPIO_PINS_8
#define SDRAM_D5_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE8
#define SDRAM_D5_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D6_GPIO_PORT               GPIOE
#define SDRAM_D6_GPIO_CLK                CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_D6_GPIO_PIN                GPIO_PINS_9
#define SDRAM_D6_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE9
#define SDRAM_D6_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D7_GPIO_PORT               GPIOE
#define SDRAM_D7_GPIO_CLK                CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_D7_GPIO_PIN                GPIO_PINS_10
#define SDRAM_D7_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE10
#define SDRAM_D7_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D8_GPIO_PORT               GPIOE
#define SDRAM_D8_GPIO_CLK                CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_D8_GPIO_PIN                GPIO_PINS_11
#define SDRAM_D8_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE11
#define SDRAM_D8_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D9_GPIO_PORT               GPIOE
#define SDRAM_D9_GPIO_CLK                CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_D9_GPIO_PIN                GPIO_PINS_12
#define SDRAM_D9_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE12
#define SDRAM_D9_GPIO_MUX                GPIO_MUX_12

#define SDRAM_D10_GPIO_PORT              GPIOE
#define SDRAM_D10_GPIO_CLK               CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_D10_GPIO_PIN               GPIO_PINS_13
#define SDRAM_D10_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE13
#define SDRAM_D10_GPIO_MUX               GPIO_MUX_12

#define SDRAM_D11_GPIO_PORT              GPIOE
#define SDRAM_D11_GPIO_CLK               CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_D11_GPIO_PIN               GPIO_PINS_14
#define SDRAM_D11_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE14
#define SDRAM_D11_GPIO_MUX               GPIO_MUX_12

#define SDRAM_D12_GPIO_PORT              GPIOE
#define SDRAM_D12_GPIO_CLK               CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_D12_GPIO_PIN               GPIO_PINS_15
#define SDRAM_D12_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE15
#define SDRAM_D12_GPIO_MUX               GPIO_MUX_12

#define SDRAM_D13_GPIO_PORT              GPIOD
#define SDRAM_D13_GPIO_CLK               CRM_GPIOD_PERIPH_CLOCK
#define SDRAM_D13_GPIO_PIN               GPIO_PINS_8
#define SDRAM_D13_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE8
#define SDRAM_D13_GPIO_MUX               GPIO_MUX_12

#define SDRAM_D14_GPIO_PORT              GPIOD
#define SDRAM_D14_GPIO_CLK               CRM_GPIOD_PERIPH_CLOCK
#define SDRAM_D14_GPIO_PIN               GPIO_PINS_9
#define SDRAM_D14_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE9
#define SDRAM_D14_GPIO_MUX               GPIO_MUX_12

#define SDRAM_D15_GPIO_PORT              GPIOD
#define SDRAM_D15_GPIO_CLK               CRM_GPIOD_PERIPH_CLOCK
#define SDRAM_D15_GPIO_PIN               GPIO_PINS_10
#define SDRAM_D15_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE10
#define SDRAM_D15_GPIO_MUX               GPIO_MUX_12

#define SDRAM_A0_GPIO_PORT               GPIOF
#define SDRAM_A0_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A0_GPIO_PIN                GPIO_PINS_0
#define SDRAM_A0_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE0
#define SDRAM_A0_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A1_GPIO_PORT               GPIOF
#define SDRAM_A1_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A1_GPIO_PIN                GPIO_PINS_1
#define SDRAM_A1_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE1
#define SDRAM_A1_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A2_GPIO_PORT               GPIOF
#define SDRAM_A2_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A2_GPIO_PIN                GPIO_PINS_2
#define SDRAM_A2_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE2
#define SDRAM_A2_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A3_GPIO_PORT               GPIOF
#define SDRAM_A3_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A3_GPIO_PIN                GPIO_PINS_3
#define SDRAM_A3_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE3
#define SDRAM_A3_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A4_GPIO_PORT               GPIOF
#define SDRAM_A4_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A4_GPIO_PIN                GPIO_PINS_4
#define SDRAM_A4_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE4
#define SDRAM_A4_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A5_GPIO_PORT               GPIOF
#define SDRAM_A5_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A5_GPIO_PIN                GPIO_PINS_5
#define SDRAM_A5_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE5
#define SDRAM_A5_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A6_GPIO_PORT               GPIOF
#define SDRAM_A6_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A6_GPIO_PIN                GPIO_PINS_12
#define SDRAM_A6_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE12
#define SDRAM_A6_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A7_GPIO_PORT               GPIOF
#define SDRAM_A7_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A7_GPIO_PIN                GPIO_PINS_13
#define SDRAM_A7_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE13
#define SDRAM_A7_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A8_GPIO_PORT               GPIOF
#define SDRAM_A8_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A8_GPIO_PIN                GPIO_PINS_14
#define SDRAM_A8_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE14
#define SDRAM_A8_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A9_GPIO_PORT               GPIOF
#define SDRAM_A9_GPIO_CLK                CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_A9_GPIO_PIN                GPIO_PINS_15
#define SDRAM_A9_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE15
#define SDRAM_A9_GPIO_MUX                GPIO_MUX_12

#define SDRAM_A10_GPIO_PORT              GPIOG
#define SDRAM_A10_GPIO_CLK               CRM_GPIOG_PERIPH_CLOCK
#define SDRAM_A10_GPIO_PIN               GPIO_PINS_0
#define SDRAM_A10_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE0
#define SDRAM_A10_GPIO_MUX               GPIO_MUX_12

#define SDRAM_A11_GPIO_PORT              GPIOG
#define SDRAM_A11_GPIO_CLK               CRM_GPIOG_PERIPH_CLOCK
#define SDRAM_A11_GPIO_PIN               GPIO_PINS_1
#define SDRAM_A11_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE1
#define SDRAM_A11_GPIO_MUX               GPIO_MUX_12

#define SDRAM_A12_GPIO_PORT              GPIOG
#define SDRAM_A12_GPIO_CLK               CRM_GPIOG_PERIPH_CLOCK
#define SDRAM_A12_GPIO_PIN               GPIO_PINS_2
#define SDRAM_A12_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE2
#define SDRAM_A12_GPIO_MUX               GPIO_MUX_12

#define SDRAM_CKE_GPIO_PORT              GPIOC
#define SDRAM_CKE_GPIO_CLK               CRM_GPIOC_PERIPH_CLOCK
#define SDRAM_CKE_GPIO_PIN               GPIO_PINS_3
#define SDRAM_CKE_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE3
#define SDRAM_CKE_GPIO_MUX               GPIO_MUX_12

#define SDRAM_CLK_GPIO_PORT              GPIOG
#define SDRAM_CLK_GPIO_CLK               CRM_GPIOG_PERIPH_CLOCK
#define SDRAM_CLK_GPIO_PIN               GPIO_PINS_8
#define SDRAM_CLK_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE8
#define SDRAM_CLK_GPIO_MUX               GPIO_MUX_12

#define SDRAM_CS_GPIO_PORT               GPIOC
#define SDRAM_CS_GPIO_CLK                CRM_GPIOC_PERIPH_CLOCK
#define SDRAM_CS_GPIO_PIN                GPIO_PINS_2
#define SDRAM_CS_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE2
#define SDRAM_CS_GPIO_MUX                GPIO_MUX_12

#define SDRAM_WE_GPIO_PORT               GPIOC
#define SDRAM_WE_GPIO_CLK                CRM_GPIOC_PERIPH_CLOCK
#define SDRAM_WE_GPIO_PIN                GPIO_PINS_0
#define SDRAM_WE_GPIO_PINS_SOURCE        GPIO_PINS_SOURCE0
#define SDRAM_WE_GPIO_MUX                GPIO_MUX_12

#define SDRAM_CAS_GPIO_PORT              GPIOG
#define SDRAM_CAS_GPIO_CLK               CRM_GPIOG_PERIPH_CLOCK
#define SDRAM_CAS_GPIO_PIN               GPIO_PINS_15
#define SDRAM_CAS_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE15
#define SDRAM_CAS_GPIO_MUX               GPIO_MUX_12

#define SDRAM_RAS_GPIO_PORT              GPIOF
#define SDRAM_RAS_GPIO_CLK               CRM_GPIOF_PERIPH_CLOCK
#define SDRAM_RAS_GPIO_PIN               GPIO_PINS_11
#define SDRAM_RAS_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE11
#define SDRAM_RAS_GPIO_MUX               GPIO_MUX_12

#define SDRAM_BA0_GPIO_PORT              GPIOG
#define SDRAM_BA0_GPIO_CLK               CRM_GPIOG_PERIPH_CLOCK
#define SDRAM_BA0_GPIO_PIN               GPIO_PINS_4
#define SDRAM_BA0_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE4
#define SDRAM_BA0_GPIO_MUX               GPIO_MUX_12

#define SDRAM_BA1_GPIO_PORT              GPIOG
#define SDRAM_BA1_GPIO_CLK               CRM_GPIOG_PERIPH_CLOCK
#define SDRAM_BA1_GPIO_PIN               GPIO_PINS_5
#define SDRAM_BA1_GPIO_PINS_SOURCE       GPIO_PINS_SOURCE5
#define SDRAM_BA1_GPIO_MUX               GPIO_MUX_12

#define SDRAM_NBL0_GPIO_PORT             GPIOE
#define SDRAM_NBL0_GPIO_CLK              CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_NBL0_GPIO_PIN              GPIO_PINS_0
#define SDRAM_NBL0_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE0
#define SDRAM_NBL0_GPIO_MUX              GPIO_MUX_12

#define SDRAM_NBL1_GPIO_PORT             GPIOE
#define SDRAM_NBL1_GPIO_CLK              CRM_GPIOE_PERIPH_CLOCK
#define SDRAM_NBL1_GPIO_PIN              GPIO_PINS_1
#define SDRAM_NBL1_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE1
#define SDRAM_NBL1_GPIO_MUX              GPIO_MUX_12


#define SDRAM_BURST_LEN_1                ((uint16_t)0x0000)
#define SDRAM_BURST_LEN_2                ((uint16_t)0x0001)
#define SDRAM_BURST_LEN_4                ((uint16_t)0x0002)
#define SDRAM_BURST_LEN_8                ((uint16_t)0x0004)
#define SDRAM_BURST_SEQUENTIAL           ((uint16_t)0x0000)
#define SDRAM_BURST_INTERLEAVED          ((uint16_t)0x0008)
#define SDRAM_CAS_LATENCY_1              ((uint16_t)0x0010)
#define SDRAM_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_WR_BURST_PROGRAMMED        ((uint16_t)0x0000)
#define SDRAM_WR_BURST_SINGLE            ((uint16_t)0x0200)

void sdram_init(void);
void sdram_data_write(uint32_t writeaddr, uint16_t* pbuffer, uint32_t numhalfwordtowrite);
void sdram_data_read(uint32_t readaddr, uint16_t* pbuffer, uint32_t numhalfwordtoread);

#ifdef __cplusplus
}
#endif

#endif

