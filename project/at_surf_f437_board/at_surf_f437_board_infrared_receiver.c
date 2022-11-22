/**
  **************************************************************************
  * @file     at_surf_f437_board_infrared.c
  * @brief    the driver library of the infrared
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

#include "at_surf_f437_board_infrared_receiver.h"

infrared_receiver_fifo_type ir_fifo;

/**
  * @brief  infrared receiver fifo init.
  * @param  none.
  * @retval none.
  */
void infrared_receiver_fifo_init(void)
{
  ir_fifo.write = 0;
  ir_fifo.read = 0;
}

/**
  * @brief  put infrared receiver key.
  * @param  key val.
  * @retval none.
  */
void infrared_receiver_key_put(uint16_t key)
{
  ir_fifo.buf[ir_fifo.write] = key;

  ir_fifo.write++;

  if(ir_fifo.write == IR_FIFO_SIZE)
  {
    ir_fifo.write = 0;
  }
}

/**
  * @brief  get infrared receiver key.
  * @param  the pointer of key value.
  * @retval error_status.
  */
error_status infrared_receiver_key_get(uint16_t *val)
{
  if(ir_fifo.read != ir_fifo.write)
  {
    *val = ir_fifo.buf[ir_fifo.read];
    ir_fifo.read++;
    if(ir_fifo.read == IR_FIFO_SIZE)
    {
      ir_fifo.read = 0;
    }
    return SUCCESS;
  }

  return ERROR;
}

/**
  * @brief  infrared receiver data deal.
  * @param  none.
  * @retval none.
  */
void infrared_receiver_key_deal(uint32_t value)
{
  infrared_receiver_data_type ir;

  ir.value = value;

  /* address received correctly */
  if(ir.byte.addr0 == (uint8_t)(~ir.byte.addr1))
  {
    /* command received correctly */
    if(ir.byte.cmd0 == (uint8_t)(~ir.byte.cmd1))
    {
      infrared_receiver_key_put((uint16_t)(ir.byte.addr0 << 8) | (uint16_t)ir.byte.cmd0);
    }
  }
}

/**
  * @brief  infrared receiver init.
  * @param  none.
  * @retval none.
  */
void infrared_receiver_init(void)
{
  gpio_init_type gpio_init_struct;
  tmr_input_config_type  tmr_input_config_struct;
  crm_clocks_freq_type   crm_clock_freq;

  /* init infrared receiver fifo */
  infrared_receiver_fifo_init();

  /* enable tmr3/gpioa clock */
  crm_periph_clock_enable(IR_TMR_CLK, TRUE);
  crm_periph_clock_enable(IR_GPIO_CLK, TRUE);

  /* timer input pin Configuration */
  gpio_init_struct.gpio_pins = IR_GPIO_PIN;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(IR_GPIO_PORT, &gpio_init_struct);

  gpio_pin_mux_config(IR_GPIO_PORT, IR_GPIO_PINS_SOURCE, IR_GPIO_MUX);

  /* get the system frequency value */
  crm_clocks_freq_get(&crm_clock_freq);

  /* tmr counter mode configuration */
  tmr_base_init(IR_TMRx, 6000, crm_clock_freq.apb1_freq / 1000000 * 2);
  tmr_cnt_dir_set(IR_TMRx, TMR_COUNT_UP);

  /* configure tmr channel to get clock signal */
  tmr_input_config_struct.input_channel_select = IR_TMR_CHANNEL;
  tmr_input_config_struct.input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;
  tmr_input_config_struct.input_polarity_select = TMR_INPUT_RISING_EDGE;
  tmr_input_channel_init(IR_TMRx, &tmr_input_config_struct, TMR_CHANNEL_INPUT_DIV_1);

  /* enable capture interrupt */
  tmr_interrupt_enable(IR_TMRx, IR_TMR_CHANNEL_INT, TRUE);

  /* disable overflow interrupt */
  tmr_interrupt_enable(IR_TMRx, TMR_OVF_INT, FALSE);

  /* interrupt nvic init */
  nvic_irq_enable(IR_TMR_IRQn, 1, 0);

  /* enable tmr */
  tmr_counter_enable(IR_TMRx, TRUE);
}

/**
  * @brief  infrared receiver data deal.
  * @param  none.
  * @retval none.
  */
void infrared_receiver_data_deal(void)
{
  uint32_t value;
  static uint32_t ir_data = 0;

  /* input capture event occurs */
  if(tmr_flag_get(IR_TMRx, IR_TMR_CHANNEL_FLAG) != RESET)
  {
    /* clear input capture flag */
    tmr_flag_clear(IR_TMRx, IR_TMR_CHANNEL_FLAG);

    /* clear overflow flag */
    tmr_flag_clear(IR_TMRx, TMR_OVF_FLAG);

    /* enable overflow interrupt */
    tmr_interrupt_enable(IR_TMRx, TMR_OVF_INT, TRUE);

    /* rising edge capture */
    if(IR_GPIO_READ())
    {
      /* clear the counter */
      tmr_counter_value_set(IR_TMRx, 0);

      /* configure capture mode as falling edge capture */
      tmr_output_channel_polarity_set(IR_TMRx, IR_TMR_CHANNEL, TMR_POLARITY_ACTIVE_LOW);
    }
    /* falling edge capture */
    else
    {
      /* get capture counter */
      value = tmr_channel_value_get(IR_TMRx, IR_TMR_CHANNEL);

      /* configure capture mode as rising edge capture */
      tmr_output_channel_polarity_set(IR_TMRx, IR_TMR_CHANNEL, TMR_POLARITY_ACTIVE_HIGH);

      /* received start code */
      if((value > 4200) && (value < 4700))
      {
        ir_data = 0;
      }
      /* received repeat code */
      else if((value > 2000) && (value < 2600))
      {
        /* analysis data */
        infrared_receiver_key_deal(ir_data);
      }
      /* received bit 0 */
      else if((value > 300) && (value < 800))
      {
        ir_data <<= 1;
      }
      /* received bit 1 */
      else if((value > 1400) && (value < 1800))
      {
        ir_data <<= 1;
        ir_data |= 1;
      }
    }
  }

  /* overflow event occurs, 6ms , receive complete */
  else if(tmr_flag_get(IR_TMRx, TMR_OVF_FLAG) != RESET)
  {
    /* clear overflow flag */
    tmr_flag_clear(IR_TMRx, TMR_OVF_FLAG);

    /* disable overflow interrupt */
    tmr_interrupt_enable(IR_TMRx, TMR_OVF_INT, FALSE);

    /* configure capture mode as rising edge capture */
    tmr_output_channel_polarity_set(IR_TMRx, IR_TMR_CHANNEL, TMR_POLARITY_ACTIVE_HIGH);

    /* analysis data */
    infrared_receiver_key_deal(ir_data);
  }
}
