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

/** @addtogroup 435_EDMA_continuous_mode EDMA_continuous_mode
  * @{
  */

gpio_init_type  gpio_init_struct = {0};
tmr_output_config_type  tmr_output_struct;
edma_init_type edma_init_struct;

uint32_t duty_buffer[4] = {10,10,10,10};

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  /* turn led2/led3/led4 on */
  at32_led_on(LED2);
  at32_led_on(LED3);
  at32_led_on(LED4);

  /* enable tmr1/gpioa clock */
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* enable dma1 clock */
  crm_periph_clock_enable(CRM_EDMA_PERIPH_CLOCK, TRUE);

  /* timer1 output pin Configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_11;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_1);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9, GPIO_MUX_1);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE10, GPIO_MUX_1);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE11, GPIO_MUX_1);

  /* i2s2_rx:edma stream1 configuration */
  edma_default_para_init(&edma_init_struct);
  edma_init_struct.direction = EDMA_DIR_MEMORY_TO_PERIPHERAL;
  edma_init_struct.buffer_size = (uint32_t)4;
  edma_init_struct.peripheral_data_width = EDMA_PERIPHERAL_DATA_WIDTH_WORD;
  edma_init_struct.peripheral_base_addr = (uint32_t)&(TMR1->c1dt);
  edma_init_struct.peripheral_inc_enable = TRUE;
  edma_init_struct.memory_data_width = EDMA_MEMORY_DATA_WIDTH_WORD;
  edma_init_struct.memory0_base_addr = (uint32_t)duty_buffer;
  edma_init_struct.memory_inc_enable = TRUE;
  edma_init_struct.peripheral_burst_mode = EDMA_PERIPHERAL_BURST_4;
  edma_init_struct.memory_burst_mode = EDMA_MEMORY_BURST_4;
  edma_init_struct.fifo_mode_enable = TRUE;
  edma_init_struct.fifo_threshold = EDMA_FIFO_THRESHOLD_FULL;
  edma_init_struct.priority = EDMA_PRIORITY_HIGH;
  edma_init_struct.loop_mode_enable = TRUE;
  edma_init(EDMA_STREAM1, &edma_init_struct);

  /* edmamux init */
  edmamux_enable(TRUE);
  edmamux_init(EDMAMUX_CHANNEL1, EDMAMUX_DMAREQ_ID_TMR1_OVERFLOW);

  /* enable stream */
  edma_stream_enable(EDMA_STREAM1, TRUE);

  /* tmr1 configuration */
  tmr_base_init(TMR1, 500, 0);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

  /* channel 1, 2, 3 and 4 configuration in output mode */
  tmr_output_default_para_init(&tmr_output_struct);
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_B;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
  tmr_output_struct.oc_idle_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_idle_state = FALSE;

  /* channel 1 */
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, duty_buffer[0]);

  /* channel 2 */
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_2, &tmr_output_struct);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_2, duty_buffer[1]);

  /* channel 3 */
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_3, &tmr_output_struct);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_3, duty_buffer[2]);

  /* channel 4 */
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_4, &tmr_output_struct);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, duty_buffer[3]);

  /* tmr1 output enable */
  tmr_output_enable(TMR1, TRUE);

  /* enable tmr1 overflow edam request */
  tmr_dma_request_enable(TMR1, TMR_OVERFLOW_DMA_REQUEST, TRUE);

  /* overflow interrupt enable */
  tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);

  /* tmr1 overflow interrupt nvic init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 1, 0);

  /* enable tmr1 */
  tmr_counter_enable(TMR1, TRUE);

  while(1)
  {
  }
}

/**
  * @brief  this function handles timer1 overflow handler.
  * @param  none
  * @retval none
  */
void TMR1_OVF_TMR10_IRQHandler(void)
{
  if(tmr_interrupt_flag_get(TMR1, TMR_OVF_FLAG) == SET)
  {
    /* change duty_buffer data value */
    duty_buffer[0] += 10;
    duty_buffer[1] += 10;
    duty_buffer[2] += 10;
    duty_buffer[3] += 10;

    if(duty_buffer[0] >= 500)
    {
      duty_buffer[0] = 10;
      duty_buffer[1] = 10;
      duty_buffer[2] = 10;
      duty_buffer[3] = 10;
    }

    /* clear tmr1 overflow flag */
    tmr_flag_clear(TMR1, TMR_OVF_FLAG);
  }
}


/**
  * @}
  */

/**
  * @}
  */
