/**
  **************************************************************************
  * @file     iap.h
  * @brief    iap header file
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

#ifndef __IAP_H__
#define __IAP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f435_437_board.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USART_iap_app_led3_toggle
  * @{
  */

/** @defgroup IAP_app_led3_address_definition
  * @{
  */

/* app starting address */
#define APP_START_ADDR          0x08004000

/* the previous sector of app starting address is iap upgrade flag */
#define IAP_UPGRADE_FLAG_ADDR    (APP_START_ADDR - 0x1000)

/**
  * @}
  */

/** @defgroup IAP_app_led3_flag_definition
  * @{
  */

/* when app received cmd 0x5aa5 from pc-tool, will set up the flag,
indicates that an app upgrade will follow, see iap application note for more details */
#define IAP_UPGRADE_FLAG         0x41544B38

/**
  * @}
  */

/** @defgroup IAP_app_led3_exported_functions
  * @{
  */

void iap_command_handle(void);
void iap_init(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
