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

/** @addtogroup 437_DMAMUX_genertor_exint DMAMUX_genertor_exint
  * @{
  */

gpio_init_type  gpio_init_struct = {0};
dma_init_type dma_init_struct;
dmamux_gen_init_type  dmamux_gen_init_struct;
exint_init_type exint_init_struct;
#define BUFFER_SIZE                      16
uint16_t src_buffer[BUFFER_SIZE] = {0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
                                    0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010};

uint16_t  dst_buffer[BUFFER_SIZE];

/**
  * @brief  exint1 interrupt handler
  * @param  none
  * @retval none
  */
void EXINT1_IRQHandler(void)
{
  if(exint_interrupt_flag_get(EXINT_LINE_1) != RESET)
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
  /* initial system clock */
  system_clock_config();

  /* at board initial */
  at32_board_init();

  /* enable dma2/gpioa clock */
  crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

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
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_select = EXINT_LINE_1;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  /* exint line1 interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EXINT1_IRQn, 1, 0);

  /* dma2 channel4 configuration */
  dma_reset(DMA2_CHANNEL4);
  dma_init_struct.buffer_size = BUFFER_SIZE;
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr = (uint32_t)dst_buffer;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)src_buffer;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = TRUE;
  dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(DMA2_CHANNEL4, &dma_init_struct);

  /* genertor1 configuration */
  dmamux_generator_default_para_init(&dmamux_gen_init_struct);
  dmamux_gen_init_struct.gen_polarity = DMAMUX_GEN_POLARITY_RISING;
  dmamux_gen_init_struct.gen_request_number = 4;
  dmamux_gen_init_struct.gen_signal_sel = DMAMUX_GEN_ID_EXINT1;
  dmamux_gen_init_struct.gen_enable = TRUE;
  dmamux_generator_config(DMA2MUX_GENERATOR1, &dmamux_gen_init_struct);

  /* enable transfer full data interrupt */
  dma_interrupt_enable(DMA2_CHANNEL4, DMA_FDT_INT, TRUE);

  /* dma2 channel4 interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(DMA2_Channel4_IRQn, 1, 0);

  /* dmamux function enable */
  dmamux_enable(DMA2, TRUE);
  dmamux_init(DMA2MUX_CHANNEL4, DMAMUX_DMAREQ_ID_REQ_G1);

  /* enable dma channe4 */
  dma_channel_enable(DMA2_CHANNEL4, TRUE);

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
