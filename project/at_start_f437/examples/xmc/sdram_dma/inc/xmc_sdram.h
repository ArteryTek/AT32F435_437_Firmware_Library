/**
  ******************************************************************************
  * @file     xmc_sdram.h
  * @brief    header file for the sdram configuration.
  ******************************************************************************
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

#ifndef __XMC_SDRAM_H
#define __XMC_SDRAM_H

#include "at32f435_437_board.h"

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
void sdram_writebuffer(uint16_t* pbuffer, uint32_t writeaddr, uint32_t numhalfwordtowrite);
void sdram_readbuffer(uint16_t* pbuffer, uint32_t readaddr, uint32_t numhalfwordtoread);
void fill_buffer(uint16_t *pbuffer, uint16_t bufferlenght, uint32_t offset);
void sdram_copy_to_sram_dma(uint16_t *buffer, uint32_t length);
void sram_copy_to_sdram_dma(uint16_t *buffer, uint32_t length);

#endif
