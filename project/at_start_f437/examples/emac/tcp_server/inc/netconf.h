/**
  **************************************************************************
  * @file     netconf.h
  * @version  v2.0.2
  * @date     2021-11-26
  * @brief    This file contains all the functions prototypes for the netconf.c 
  *           file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETCONF_H
#define __NETCONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup AT32F437_periph_examples
  * @{
  */
  
/** @addtogroup 437_EMAC_tcp_server
  * @{
  */
  
/** @addtogroup network_configuration_prototype
  * @{
  */
/* Includes ------------------------------------------------------------------*/
void tcpip_stack_init(void);
void lwip_pkt_handle(void);
void lwip_periodic_handle(volatile uint32_t localtime);


#ifdef __cplusplus
}
#endif

/**
  * @}
  */
  
/**
  * @}
  */
#endif /* __NETCONF_H */
/**
  * @}
  */


