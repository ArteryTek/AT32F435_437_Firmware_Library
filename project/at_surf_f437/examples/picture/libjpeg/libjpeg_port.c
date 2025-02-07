/**
  **************************************************************************
  * @file     libjpeg_port.c
  * @brief    contains all the functions for the i2c firmware library
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

#include "libjpeg_port.h"

size_t read_file (FIL  *file, uint8_t *buf, uint32_t sizeofbuf)
{
	size_t BytesReadfile ; 

	
  f_read (file, buf , sizeofbuf, &BytesReadfile); 
	return BytesReadfile;    
}


size_t write_file (FIL  *file, uint8_t *buf, uint32_t sizeofbuf)
{
  size_t BytesWritefile ;  
	
  f_write (file, buf , sizeofbuf, &BytesWritefile);
  return BytesWritefile; 
}
