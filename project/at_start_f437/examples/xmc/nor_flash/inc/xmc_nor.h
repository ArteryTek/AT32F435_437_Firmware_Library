/**
  ******************************************************************************
  * @file     xmc_sram.h
  * @brief    header file for the nand configuration.
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

#ifndef __XMC_SRAM_H
#define __XMC_SRAM_H

#include "at32f435_437_board.h"

typedef struct
{
  uint16_t manufacturer_code;
  uint16_t device_code1;
  uint16_t device_code2;
  uint16_t device_code3;
}nor_idtype;

/* nor status */
typedef enum
{
  NOR_SUCCESS = 0,
  NOR_ONGOING,
  NOR_ERROR,
  NOR_TIMEOUT
}nor_status;

void nor_init(void);
void nor_readid(nor_idtype* nor_id);
nor_status nor_eraseblock(uint32_t blockaddr);
nor_status nor_erasechip(void);
nor_status nor_writehalfword(uint32_t writeaddr, uint16_t data);
nor_status nor_writebuffer(uint16_t* pbuffer, uint32_t writeaddr, uint32_t numhalfwordtowrite);
nor_status nor_programbuffer(uint16_t* pbuffer, uint32_t writeaddr, uint32_t numhalfwordtowrite);
uint16_t nor_readhalfword(uint32_t readaddr);
void nor_readbuffer(uint16_t* pbuffer, uint32_t readaddr, uint32_t numhalfwordtoread);
nor_status nor_returntoreadmode(void);
nor_status nor_reset(void);
nor_status nor_getstatus(uint32_t timeout);
void fill_buffer(uint16_t *pbuffer, uint16_t bufferlenght, uint32_t offset);

#endif
