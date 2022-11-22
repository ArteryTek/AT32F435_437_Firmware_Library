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

/** @addtogroup 435_EDMA_two_dimension_mode EDMA_two_dimension_mode
  * @{
  */

#define BUFFER_SIZE                      32

const uint32_t src_const_buffer[BUFFER_SIZE]= {0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
                                               0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
                                               0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
                                               0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
                                               0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
                                               0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
                                               0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
                                               0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80};
uint32_t dst_buffer[BUFFER_SIZE];
edma_init_type edma_init_struct;
__IO uint32_t data_counter_end = 0x01; /* this variable should not be initialized to 0 */
error_status transfer_status = ERROR;
uint16_t counter = 0;


/**
  * @brief  this function handles edma stream1 handler.
  * @param  none
  * @retval none
  */
void EDMA_Stream1_IRQHandler(void)
{
  if(edma_flag_get(EDMA_FDT1_FLAG) != RESET)
  {
    data_counter_end = 0;
    edma_flag_clear(EDMA_FDT1_FLAG);
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  int index;
  system_clock_config();

  at32_board_init();

  uart_print_init(115200);

  /* enable dma1 clock */
  crm_periph_clock_enable(CRM_EDMA_PERIPH_CLOCK, TRUE);

  /* edma stream1 configuration */
  edma_default_para_init(&edma_init_struct);
  edma_init_struct.direction = EDMA_DIR_MEMORY_TO_MEMORY;
  edma_init_struct.buffer_size = (uint32_t)BUFFER_SIZE;
  edma_init_struct.peripheral_data_width = EDMA_PERIPHERAL_DATA_WIDTH_WORD;
  edma_init_struct.peripheral_base_addr = (uint32_t)src_const_buffer;
  edma_init_struct.peripheral_inc_enable = TRUE;
  edma_init_struct.memory_data_width = EDMA_MEMORY_DATA_WIDTH_WORD;
  edma_init_struct.memory0_base_addr = (uint32_t)dst_buffer;
  edma_init_struct.memory_inc_enable = TRUE;
  edma_init_struct.peripheral_burst_mode = EDMA_PERIPHERAL_SINGLE;
  edma_init_struct.memory_burst_mode = EDMA_MEMORY_SINGLE;
  edma_init_struct.fifo_mode_enable = TRUE;
  edma_init_struct.fifo_threshold = EDMA_FIFO_THRESHOLD_FULL;
  edma_init_struct.priority = EDMA_PRIORITY_HIGH;
  edma_init_struct.loop_mode_enable = FALSE;
  edma_init(EDMA_STREAM1, &edma_init_struct);

  /* enable transfer full data intterrupt */
  edma_interrupt_enable(EDMA_STREAM1, EDMA_FDT_INT, TRUE);

  /* edma stream1 interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EDMA_Stream1_IRQn, 1, 0);

  /* two dimension mode init */
  /* source stride = 0x10, destination stride = 0x8, xcnt = 0x2, ycnt = 0x8 */
  edma_2d_init(EDMA_STREAM1_2D, 0x10, 0x8, 0x2, 0x8);
  edma_2d_enable(EDMA_STREAM1_2D, TRUE);

  edma_stream_enable(EDMA_STREAM1, TRUE);

  /* wait the end of transmission */
  while(data_counter_end != 0)
  {
  }

  printf("source buffer:\r\n");
  for(counter = 0; counter < BUFFER_SIZE; counter ++)
  {
    printf("%x  ",src_const_buffer[counter]);
    index ++;
    if(index == 4)
    {
      printf("\r\n");
      index = 0;
    }
  }

  printf("destination buffer:\r\n");
  for(counter = 0; counter < BUFFER_SIZE/2; counter ++)
  {
    printf("%x  ",dst_buffer[counter]);
    index ++;
    if(index == 2)
    {
      printf("\r\n");
      index = 0;
    }
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
