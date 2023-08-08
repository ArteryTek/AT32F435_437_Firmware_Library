/**
  **************************************************************************
  * @file     FlashDev.c
  * @brief    device description for new device flash
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

#include "FlashOS.H"        // FlashOS Structures

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup qspi_algorithm_for_keil
  * @{
  */
#ifdef QSPI1_SEL
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "AT32F435/437 QSPI1 ESMT32M",   // Device Name
   EXTSPI,                     // Device Type
   0x90000000,                 // Device Start Address
   16*1024*1024,               // Device Size in Bytes
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout mSec
   1000,                       // Erase Sector Timeout mSec

// Specify Size and Address of Sectors
   4096, 0x000000,           // Sector Size (1 Sectors)
   SECTOR_END
};
#else
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "AT32F435/437 QSPI2 ESMT32M",   // Device Name
   EXTSPI,                     // Device Type
   0xB0000000,                 // Device Start Address
   16*1024*1024,               // Device Size in Bytes
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout mSec
   1000,                       // Erase Sector Timeout mSec

// Specify Size and Address of Sectors
   4096, 0x000000,           // Sector Size (1 Sectors)
   SECTOR_END
};
#endif
/**
  * @}
  */

/**
  * @}
  */


