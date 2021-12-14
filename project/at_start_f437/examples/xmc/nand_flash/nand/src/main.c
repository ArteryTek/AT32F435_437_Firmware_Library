/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.2
  * @date     2021-11-26
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

#include "xmc_nand.h"
#include "at32f435_437_clock.h"
#include "at32f435_437_clock.h"
#include "stdio.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */
  
/** @addtogroup 437_XMC_nand XMC_nand
  * @{
  */
  
crm_clocks_freq_type crm_clocks_freq_struct = {0};
nand_id_type nand_id_struct = {0};
nand_address_type write_read_addr_struct;
uint8_t txbuffer[BUFFER_SIZE], rxbuffer[BUFFER_SIZE + NAND_SPARE_AREA_SIZE];
uint32_t pagenumber = BUFFER_SIZE / NAND_PAGE_SIZE, writereadstatus = 0, status= 0;
uint32_t j = 0;

void fill_buffer(uint8_t *pbuffer, uint16_t bufferlenght, uint32_t offset);

/* suport printf function, usemicrolib is unnecessary */
#if (__ARMCC_VERSION > 6000000)
  __asm (".global __use_no_semihosting\n\t");
  void _sys_exit(int x)
  {
    x = x;
  }
  /* __use_no_semihosting was requested, but _ttywrch was */
  void _ttywrch(int ch)
  {
    ch = ch;
  }
  FILE __stdout;
#else
 #ifdef __CC_ARM
  #pragma import(__use_no_semihosting)
  struct __FILE
  {
    int handle;
  };
  FILE __stdout;
  void _sys_exit(int x)
  {
    x = x;
  }
 #endif
#endif

#if defined ( __GNUC__ ) && !defined (__clang__)
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/**
  * @brief  retargets the c library printf function to the usart.
  * @param  none
  * @retval none
  */
PUTCHAR_PROTOTYPE
{
  while(usart_flag_get(USART1, USART_TDBE_FLAG) == RESET);
  usart_data_transmit(USART1, ch);
  return ch;
}

__IO uint32_t time_cnt = 0;

/**
  * @brief  initialize uart1
  * @param  baudrate: uart baudrate
  * @retval none
  */
void uart_print_init(uint32_t baudrate)
{
  gpio_init_type gpio_init_struct;

  /* enable the uart1 and gpio clock */
  crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the uart1 tx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9, GPIO_MUX_7);
  
  /* configure uart param */
  usart_init(USART1, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART1, TRUE);
  usart_enable(USART1, TRUE);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* configure the system clock */
  system_clock_config();

  /* led initalization */
  at32_board_init();    
  
  /* usart1 initalization */
  uart_print_init(115200);
  
  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);
  
  /* xmc initialization */
  nand_init();
  
  
  /* nand reset command */
  nand_reset();
  delay_us(10);
  
  /* nand read id command */
  nand_read_id(&nand_id_struct);

  /* verify the nand id */
  /* nand support:samsung:k9gag08u0e        hynix:h27u1g8f2ctr
                  id     :0xecd58472        id   :0xadf1801d
  */  
  if((nand_id_struct.maker_id == NAND_AT_MakerID) && (nand_id_struct.device_id == NAND_AT_DeviceID))    
  {
    /* nand memory address to write to */
    write_read_addr_struct.zone  = 0x00;
    write_read_addr_struct.block = 0x00;
    write_read_addr_struct.page  = 0x00;
    write_read_addr_struct.byte  = 0x00;
    
    /* erase the nand first block */
    status = nand_erase_block(write_read_addr_struct);

    /* fill the buffer to send */
    fill_buffer(txbuffer, BUFFER_SIZE , 0x66);
    
    /* write data to xmc nand memory */
    status = nand_write_small_page(txbuffer, write_read_addr_struct, pagenumber);

    /* read back the written data */
    status = nand_read_small_page (rxbuffer, write_read_addr_struct, pagenumber);
   
    /* verify the written data */
    for(j = 0; j < BUFFER_SIZE; j++)
    {
      if(txbuffer[j] != rxbuffer[j])
      {
        writereadstatus++;
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
  }
  
  /* printf to indicate that whether there is an device error (id not correct) */
  else
  {
    printf("the id is error\r\n");
  }

  while(1)
  {
  }
}

/**
  * @brief  fill the global buffer
  * @param  pbuffer: pointer on the buffer to fill
  * @param  buffersize: size of the buffer to fill
  * @param  offset: first value to fill on the buffer
  */
void fill_buffer(uint8_t *pbuffer, uint16_t bufferlenght, uint32_t offset)
{
  uint16_t indextmp = 0;

  /* put in global buffer same values */
  for(indextmp = 0; indextmp < bufferlenght; indextmp++ )
  {
    pbuffer[indextmp] = indextmp + offset;
  }
}


/**
  * @}
  */ 

/**
  * @}
  */ 


