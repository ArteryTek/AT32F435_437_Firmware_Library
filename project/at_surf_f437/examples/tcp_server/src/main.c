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

#include "at32f435_437.h"
#include "at32f435_437_clock.h"
#include "at_surf_f437_board_delay.h"
#include "at_surf_f437_board_emac.h"
#include "at_surf_f437_board_tcp_server.h"
#include "at_surf_f437_board_lcd.h"
#include "netconf.h"

extern uint8_t local_ip[];
extern uint8_t local_gw[];
extern uint8_t local_mask[];

volatile uint32_t local_time = 0;

/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  /* initial system clock */
  system_clock_config();

  /* initial the nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* initialize delay */
  delay_init();

  /* initialize lcd */
  lcd_init(LCD_DISPLAY_VERTICAL);

  /* display information */
  lcd_string_show(10, 20, 200, 24, 24, (uint8_t *)"TCP Server Test");

  /* initialize pca9555 io expansion chip */
  pca9555_init(PCA_I2C_CLKCTRL_400K);

  /* initialize emac */
  if(emac_system_init() == SUCCESS)
  {
    lcd_string_show(10, 60, 200, 24, 24, (uint8_t *)"emac init ok");
  }
  else
  {
    lcd_string_show(10, 60, 200, 24, 24, (uint8_t *)"emac init fail");
    while(1);
  }

  /* initialize tcpip stack */
  tcpip_stack_init();

  /* initialize tcp server */
  tcp_server_init();

  /* display ip */
  lcd_string_show(10, 90, 300, 24, 24,  (uint8_t *)"ip  :    .   .   .   ");
  lcd_num_show(82,  90, 200, 24, 24, local_ip[0], 3);
  lcd_num_show(130, 90, 200, 24, 24, local_ip[1], 3);
  lcd_num_show(178, 90, 200, 24, 24, local_ip[2], 3);
  lcd_num_show(226, 90, 200, 24, 24, local_ip[3], 3);

  /* display gateway */
  lcd_string_show(10, 120, 300, 24, 24, (uint8_t *)"gw  :    .   .   .   ");
  lcd_num_show(82,  120, 200, 24, 24, local_gw[0], 3);
  lcd_num_show(130, 120, 200, 24, 24, local_gw[1], 3);
  lcd_num_show(178, 120, 200, 24, 24, local_gw[2], 3);
  lcd_num_show(226, 120, 200, 24, 24, local_gw[3], 3);

  /* display mask */
  lcd_string_show(10, 150, 300, 24, 24, (uint8_t *)"mask:    .   .   .   ");
  lcd_num_show(82,  150, 200, 24, 24, local_mask[0], 3);
  lcd_num_show(130, 150, 200, 24, 24, local_mask[1], 3);
  lcd_num_show(178, 150, 200, 24, 24, local_mask[2], 3);
  lcd_num_show(226, 150, 200, 24, 24, local_mask[3], 3);

  /* display tcp server port */
  lcd_string_show(10, 180, 300, 24, 24, (uint8_t *)"port:    ");
  lcd_num_show(82,  180, 200, 24, 24, TCP_LOCAL_PORT, 1);

  while(1)
  {
    /* lwip receive handle */
    lwip_rx_loop_handler();
    
    /*timeout handle*/
    lwip_periodic_handle(local_time);
  }
}

