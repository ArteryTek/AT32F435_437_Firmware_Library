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

#include "xmc_ecc.h"
#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"
#include "stdio.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */
  
/** @addtogroup 437_XMC_nand_ecc XMC_nand_ecc
  * @{
  */
  
crm_clocks_freq_type crm_clocks_freq_struct = {0};
nand_id_type nand_id_struct;
nand_address_type write_read_addr_struct;
uint8_t tx_buffer[BUFFER_SIZE], rx_buffer[BUFFER_SIZE];
uint32_t page_number = 1, status = 0;
uint32_t j = 0, ecc_value_write = 0, ecc_value_read = 0, ecc_value_write_last = 0;

void fill_buffer(uint8_t *pbuffer, uint16_t bufferlenght, uint32_t offset);
void nand_ecc_correction(uint8_t *pbuffer, uint32_t tx_ecc_value, uint32_t rx_ecc_value);

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
  * @brief  Fill the global buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferSize: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  */
void fill_buffer(uint8_t *pbuffer, uint16_t bufferlenght, uint32_t offset)
{
  uint16_t indextmp = 0;

  /* Put in global buffer same values */
  for(indextmp = 0; indextmp < bufferlenght; indextmp++ )
  {
    pbuffer[indextmp] = indextmp + offset;
  }
}

/**
  * @brief  ecc correct 1 bit error
  * @param  pbuffer: pointer on the buffer to correct 
  * @param  tx_ecc_value: ecc value for transmiting to nand flash
  * @param  rx_ecc_value: ecc value for receiving from nand flash
  */
void nand_ecc_correction(uint8_t *pbuffer,uint32_t tx_ecc_value ,uint32_t rx_ecc_value)
{
  uint32_t ecc_value, position ,byte_position;
  uint8_t i,compare_data;
  
  /* check ecc value */
  if(tx_ecc_value!=rx_ecc_value)
  {
    /* 2048 byte -- 28bit ecc valid value; 8192 byte -- 32bit ecc valid value */
#ifdef H27U1G8F2CTR
    ecc_value =(tx_ecc_value^rx_ecc_value)&0x0FFFFFFF;
#elif defined K9GAG08U0E
    ecc_value =(tx_ecc_value^rx_ecc_value)&0xFFFFFFFF;
#endif    
    /* 1 bit error correction */
#ifdef H27U1G8F2CTR
    for(i=0;i<(28/2);i++)
#elif defined K9GAG08U0E
    for(i=0;i<(32/2);i++)
#endif
    {
      compare_data = (ecc_value>>(i*2))&0x3;
      
      /* find position */
      if(compare_data==0x2)
      {
        position |= (1<<i);
      }
      
      /* more than 1 bit erroc */
      else if(compare_data!=0x1)
      {
        return;
      }
    }
    
    /* correct receive value */
    byte_position = position/8;
    pbuffer[byte_position] ^= 1 << (position % 8);
  }
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

    /* write data to xmc nand memory */
    /* fill the buffer to send */
    fill_buffer(tx_buffer, BUFFER_SIZE , 0x0);
 
    /* change the regular to make ecc value isn't 0 */
    tx_buffer[10]=0x03;
    
     /* calculate ecc value while transmiting */
    xmc_nand_ecc_enable(XMC_BANK2_NAND, TRUE);
    status = nand_write_small_page(tx_buffer, write_read_addr_struct, page_number);
    
    /* save the right ecc_value (in this case ,we suppose it as the true value) */
    ecc_value_write_last = ecc_value_write;

    /* change the 85 data like there is a 1 bit error happened */
    status = nand_erase_block(write_read_addr_struct);
    tx_buffer[10]=0x23;    
    status = nand_write_small_page(tx_buffer, write_read_addr_struct, page_number); 
    
    /* calculate ecc value while transmiting */
    xmc_nand_ecc_enable(XMC_BANK2_NAND, TRUE);
    
    /* read back the written data */
    status = nand_read_small_page (rx_buffer, write_read_addr_struct, page_number);
    
    /* ecc check and correct 1 bit error */
    nand_ecc_correction(rx_buffer ,ecc_value_write_last ,ecc_value_read);
   
    if(rx_buffer[10]==0x03)
    {
      /* pass : printf "ecc is work" */
      printf("ecc is work\r\n");  
    }
    else
    {
      /* fail: printf "ecc is not work" */
      printf("ecc is not work\r\n");   
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
  * @}
  */ 

/**
  * @}
  */ 
