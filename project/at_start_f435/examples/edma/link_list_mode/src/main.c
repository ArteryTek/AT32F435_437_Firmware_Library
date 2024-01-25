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

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_EDMA_link_list_mode EDMA_link_list_mode
  * @{
  */

#define LIST_COUNT                       3
#define BUFFER_SIZE                      35

/* link list descriptors struct type */
typedef struct
{
  uint32_t ctrl_dtcnt;
  uint32_t paddr;
  uint32_t m0addr;
  uint32_t llp;
} ll_descriptors_type;

/* tx data buffer */
const int8_t tx_buffer[LIST_COUNT][BUFFER_SIZE] ={
  {'T','h','i','s',' ','i','s',' ','E','D','M','A',' '},
  {'L','i','n','k',' ','L','i','s','t',' ','M','o','d','e',' ','t','e','s','t','!'},
  {'\r','\n','T','h','i','s',' ','t','e','s','t',' ','c','a','s','e',' ','u','s','e',' ','U','s','a','r','t','1','_','T','X','!','\r','\n'}
};

/* link list descriptor array, start address must be aligned by 16 bytes */
__ALIGNED(16) ll_descriptors_type edma_ll_descriptors_tx[LIST_COUNT];

/* declared private functions */
void uart_init(uint32_t bound);
void edma_ll_descriptors_init(void);

/**
  * @brief  edma link list descriptor init function.
  * @param  none
  * @retval none
  */
void edma_ll_descriptors_init(void)
{
  uint32_t index = 0;

  for(index = 0; index < LIST_COUNT; index ++)
  {
    edma_ll_descriptors_tx[index].ctrl_dtcnt = 0x20090000 | sizeof(tx_buffer[index]);
    edma_ll_descriptors_tx[index].paddr = (uint32_t)(&USART1->dt);
    edma_ll_descriptors_tx[index].m0addr = (uint32_t)(&tx_buffer[index][0]);
    if(index == (LIST_COUNT - 1))
    {
      edma_ll_descriptors_tx[index].llp = 0;
    }
    else
    {
      edma_ll_descriptors_tx[index].llp = (uint32_t)(&edma_ll_descriptors_tx[index + 1]);
    }
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  /* usart1 configuration */
  uart_init(115200);

  /* enable dma1 clock */
  crm_periph_clock_enable(CRM_EDMA_PERIPH_CLOCK, TRUE);

  /* edma link list descriptor init */
  edma_ll_descriptors_init();

  /* edma stream1 link list mode configuration */
  edma_link_list_enable(EDMA_STREAM1_LL, TRUE);
  edma_link_list_init(EDMA_STREAM1_LL, (uint32_t)edma_ll_descriptors_tx);

  /* edmamux configuration */
  edmamux_enable(TRUE);
  edmamux_init(EDMAMUX_CHANNEL1, EDMAMUX_DMAREQ_ID_USART1_TX);

  /* enable transfer full data interrupt */
  edma_interrupt_enable(EDMA_STREAM1, EDMA_FDT_INT, TRUE);

  /* edma stream1 interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EDMA_Stream1_IRQn, 1, 0);

  /* enable edma stream1 */
  edma_stream_enable(EDMA_STREAM1, TRUE);

  while(1)
  {
  }
}

/**
  * @brief  initialize print usart
  * @param  bound: uart baudrate
  * @retval none
  */
void uart_init(uint32_t bound)
{
  gpio_init_type gpio_init_struct;

  /* enable the uart clock */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure uart tx gpio */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);
  /* configure uart rx gpio */
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9, GPIO_MUX_7);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE10, GPIO_MUX_7);

  /* configure usart */
  usart_init(USART1, bound, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_parity_selection_config(USART1, USART_PARITY_NONE);
  usart_transmitter_enable(USART1, TRUE);
  usart_receiver_enable(USART1, TRUE);
  usart_dma_transmitter_enable(USART1, TRUE);
  usart_enable(USART1, TRUE);
}

/**
  * @brief  this function handles edma stream1 handler.
  * @param  none
  * @retval none
  */
void EDMA_Stream1_IRQHandler(void)
{
  if(edma_interrupt_flag_get(EDMA_FDT1_FLAG) != RESET)
  {
    /* led2/3/4 on  */
    at32_led_on(LED2);
    at32_led_on(LED3);
    at32_led_on(LED4);
    edma_flag_clear(EDMA_FDT1_FLAG);
  }
}

/**
  * @}
  */

/**
  * @}
  */
