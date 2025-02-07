/**
  **************************************************************************
  * @file     at32f435_437_i2c.h
  * @brief    at32f435_437 i2c header file
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
#ifndef __LIBJPEG_PORT_H
#define __LIBJPEG_PORT_H


/* includes ------------------------------------------------------------------*/
#include "ff.h" 
#include "string.h"
#include "stdio.h"
#include "at32f435_437.h"
#include "at_surf_f437_board_malloc.h"


#define FILE            FIL

size_t read_file (FIL  *file, uint8_t *buf, uint32_t sizeofbuf);
size_t write_file (FIL  *file, uint8_t *buf, uint32_t sizeofbuf) ;

#define JFREAD(file,buf,sizeofbuf)  \
   read_file (file,buf,sizeofbuf)

#define JFWRITE(file,buf,sizeofbuf)  \
   write_file (file,buf,sizeofbuf)

#endif
