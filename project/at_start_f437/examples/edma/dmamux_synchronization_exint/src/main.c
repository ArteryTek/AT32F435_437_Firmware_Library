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

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_EDMA_dmamux_synchronization_exint EDMA_dmamux_synchronization_exint
  * @{
  */

gpio_init_type  gpio_init_struct = {0};
edma_init_type edma_init_struct;
edmamux_sync_init_type  edmamux_sync_init_struct;
exint_init_type exint_init_struct;
#define BUFFER_SIZE                      16
uint16_t src_buffer[BUFFER_SIZE] = {0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
                                    0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010};

uint16_t  dst_buffer[BUFFER_SIZE];

/**
  * @brief  this function handles edma stream4 handler.
  * @param  none
  * @retval none
  */
void EDMA_Stream4_IRQHandler(void)
{
  if(edma_flag_get(EDMA_FDT4_FLAG) != RESET)
  {
    /* turn led2/led3/led4 on */
    at32_led_on(LED2);
    at32_led_on(LED3);
    at32_led_on(LED4);
    edma_flag_clear(EDMA_FDT4_FLAG);
  }
}

/**
  * @brief  exint1 interrupt handler
  * @param  none
  * @retval none
  */
void EXINT1_IRQHandler(void)
{
  if(exint_flag_get(EXINT_LINE_1) != RESET)
  {
    at32_led_toggle(LED2);
    exint_flag_clear(EXINT_LINE_1);
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

  /* enable edma/gpioa/tmr1 clock */
  crm_periph_clock_enable(CRM_EDMA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

  /* config pa1 for input mode */
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  scfg_exint_line_config(SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE1);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_1;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  /* exint line1 interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EXINT1_IRQn, 1, 0);

  /* edma stream4 configuration */
  edma_default_para_init(&edma_init_struct);
  edma_init_struct.direction = EDMA_DIR_PERIPHERAL_TO_MEMORY;
  edma_init_struct.buffer_size = (uint32_t)16;
  edma_init_struct.peripheral_data_width = EDMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  edma_init_struct.peripheral_base_addr = (uint32_t)src_buffer;
  edma_init_struct.peripheral_inc_enable = TRUE;
  edma_init_struct.memory_data_width = EDMA_MEMORY_DATA_WIDTH_HALFWORD;
  edma_init_struct.memory0_base_addr = (uint32_t)dst_buffer;
  edma_init_struct.memory_inc_enable = TRUE;
  edma_init_struct.peripheral_burst_mode = EDMA_PERIPHERAL_SINGLE;
  edma_init_struct.memory_burst_mode = EDMA_MEMORY_SINGLE;
  edma_init_struct.fifo_mode_enable = FALSE;
  edma_init_struct.fifo_threshold = EDMA_FIFO_THRESHOLD_1QUARTER;
  edma_init_struct.priority = EDMA_PRIORITY_HIGH;
  edma_init_struct.loop_mode_enable = FALSE;
  edma_init(EDMA_STREAM4, &edma_init_struct);

  /* edmamux synchronization configuration */
  edmamux_sync_default_para_init(&edmamux_sync_init_struct);
  edmamux_sync_init_struct.sync_enable = TRUE;
  edmamux_sync_init_struct.sync_event_enable = TRUE;
  edmamux_sync_init_struct.sync_polarity = EDMAMUX_SYNC_POLARITY_RISING;
  edmamux_sync_init_struct.sync_request_number = 4;
  edmamux_sync_init_struct.sync_signal_sel = EDMAMUX_SYNC_ID_EXINT1;
  edmamux_sync_config(EDMAMUX_CHANNEL4, &edmamux_sync_init_struct);

  /* enable transfer full data intterrupt */
  edma_interrupt_enable(EDMA_STREAM4, EDMA_FDT_INT, TRUE);

  /* edma stream4 interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EDMA_Stream4_IRQn, 1, 0);

  /* dmamux function enable */
  edmamux_enable(TRUE);
  edmamux_init(EDMAMUX_CHANNEL4, EDMAMUX_DMAREQ_ID_TMR1_OVERFLOW);

  /* enable edma stream4 */
  edma_stream_enable(EDMA_STREAM4, TRUE);

  /* tmr1 configuration */
  tmr_base_init(TMR1, 5000, 0);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

  /* enable tmr1 overflow edam request */
  tmr_dma_request_enable(TMR1, TMR_OVERFLOW_DMA_REQUEST, TRUE);

  /* enable tmr1 */
  tmr_counter_enable(TMR1, TRUE);

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
