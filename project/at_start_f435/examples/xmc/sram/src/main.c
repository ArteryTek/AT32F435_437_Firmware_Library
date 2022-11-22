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
#include "xmc_sram.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_XMC_sram XMC_sram
  * @{
  */

#define buffer_size        (uint16_t)0x400
#define write_read_addr    (uint32_t)0x8000

uint16_t txbuffer[buffer_size];
uint16_t rxbuffer[buffer_size];
uint32_t writereadstatus = 0, number = 0;

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  uart_print_init(115200);

  /* sram interface config */
  sram_init();

  /* write data to xmc sram memory */
  /* fill the buffer to send */
  fill_buffer(txbuffer, buffer_size, 0x3212);
  sram_writebuffer(txbuffer, write_read_addr, buffer_size);

  /* read data from xmc sram memory */
  sram_readbuffer(rxbuffer, write_read_addr, buffer_size);

  /* read back sram memory and check content correctness */
  for(number = 0x00; (number < buffer_size) && (writereadstatus == 0); number++)
  {
    if(rxbuffer[number] != txbuffer[number])
    {
      writereadstatus = number + 1;
    }
  }

  if(writereadstatus == 0)
  {
    /* pass : printf "data is right" */
    printf("data is right\r\n");
  }
  else
  {
    /* fail: printf "data is error" */
    printf("data is error\r\n");
  }
  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */

