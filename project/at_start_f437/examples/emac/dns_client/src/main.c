/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
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

#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"
#include "at32_emac.h"
#include "netconf.h"
#include "ip_addr.h"
#include "dns.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_EMAC_dns_client EMAC_dns_client
  * @{
  */

#define DELAY                            100
#define FAST                             1
#define SLOW                             4

#define DOMAIN_NAME                      "www.arterytek.com"

uint8_t g_speed = FAST;
volatile uint32_t local_time = 0;

void dns_test(void);
void parsing_result(const char *name, ip_addr_t *ipaddr, void *arg);

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  error_status status;

  system_clock_config();

  at32_board_init();
  
  uart_print_init(115200);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  delay_init();

  status = emac_system_init();

  while(status == ERROR);

  tcpip_stack_init();

  #if LWIP_DNS
  dns_init();
  dns_test();
  #endif

  while(1)
  {
    /* lwip receive handle */
    lwip_rx_loop_handler();
    
    /*timeout handle*/
    lwip_periodic_handle(local_time);
  }
}

#if LWIP_DNS
void dns_test(void)
{
  ip_addr_t cipaddr_dns;

  dns_gethostbyname(DOMAIN_NAME, &cipaddr_dns, (dns_found_callback)parsing_result, NULL);
}

void parsing_result(const char *name, ip_addr_t *ipaddr, void *arg)
{
  uint8_t server_ip[4];

  server_ip[0] = ipaddr->addr>>24;
  server_ip[1] = ipaddr->addr>>16;
  server_ip[2] = ipaddr->addr>>8;
  server_ip[3] = ipaddr->addr;

  printf("%d.%d.%d.%d\n",server_ip[3], server_ip[2], server_ip[1], server_ip[0]);
}
#endif

/**
  * @}
  */

/**
  * @}
  */
