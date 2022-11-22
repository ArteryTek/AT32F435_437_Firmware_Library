/**
  **************************************************************************
  * @file     at_surf_f437_board_can.c
  * @brief    the driver library of the can
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

#include "at_surf_f437_board_can.h"

can_rx_message_type can1_rx_message = {0,}, can2_rx_message = {0,};
can_tx_message_type can1_tx_message = {0,}, can2_tx_message = {0,};
uint8_t can1_rx_flag = 0, can2_rx_flag = 0;

/**
  * @brief  initialize can data.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1, CAN2.
  * @retval none
  */
void can_data_init(can_type *can_x)
{
  if(can_x == CAN1)
  {
    can1_tx_message.standard_id = 0x400;
    can1_tx_message.extended_id = 0;
    can1_tx_message.id_type = CAN_ID_STANDARD;
    can1_tx_message.frame_type = CAN_TFT_DATA;
    can1_tx_message.dlc = 8;
    can1_tx_message.data[0] = 0x11;
    can1_tx_message.data[1] = 0x22;
    can1_tx_message.data[2] = 0x33;
    can1_tx_message.data[3] = 0x44;
    can1_tx_message.data[4] = 0x55;
    can1_tx_message.data[5] = 0x66;
    can1_tx_message.data[6] = 0x77;
    can1_tx_message.data[7] = 0x88;
  }
  else if(can_x == CAN2)
  {
    can2_tx_message.standard_id = 0x666;
    can2_tx_message.extended_id = 0;
    can2_tx_message.id_type = CAN_ID_STANDARD;
    can2_tx_message.frame_type = CAN_TFT_DATA;
    can2_tx_message.dlc = 8;
    can2_tx_message.data[0] = 0x01;
    can2_tx_message.data[1] = 0x02;
    can2_tx_message.data[2] = 0x03;
    can2_tx_message.data[3] = 0x04;
    can2_tx_message.data[4] = 0x05;
    can2_tx_message.data[5] = 0x06;
    can2_tx_message.data[6] = 0x07;
    can2_tx_message.data[7] = 0x08;
  }
}

/**
  * @brief  initialize can gpio.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1, CAN2.
  * @retval none
  */
void can_gpio_config(can_type *can_x)
{
  gpio_init_type gpio_init_struct;

  gpio_default_para_init(&gpio_init_struct);

  if(can_x == CAN1)
  {
    /* enable the gpio clock */
    crm_periph_clock_enable(CAN1_TX_GPIO_CLK, TRUE);
    crm_periph_clock_enable(CAN1_RX_GPIO_CLK, TRUE);

    /* configure the can tx, rx pin */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

    gpio_init_struct.gpio_pins = CAN1_TX_GPIO_PIN;
    gpio_init(CAN1_TX_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.gpio_pins = CAN1_RX_GPIO_PIN;
    gpio_init(CAN1_RX_GPIO_PORT, &gpio_init_struct);

    gpio_pin_mux_config(CAN1_TX_GPIO_PORT, CAN1_TX_GPIO_PINS_SOURCE, CAN1_TX_GPIO_MUX);
    gpio_pin_mux_config(CAN1_RX_GPIO_PORT, CAN1_RX_GPIO_PINS_SOURCE, CAN1_RX_GPIO_MUX);
  }
  else if(can_x == CAN2)
  {
    /* enable the gpio clock */
    crm_periph_clock_enable(CAN2_TX_GPIO_CLK, TRUE);
    crm_periph_clock_enable(CAN2_RX_GPIO_CLK, TRUE);

    /* configure the can tx, rx pin */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

    gpio_init_struct.gpio_pins = CAN2_TX_GPIO_PIN;
    gpio_init(CAN2_TX_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.gpio_pins = CAN2_RX_GPIO_PIN;
    gpio_init(CAN2_RX_GPIO_PORT, &gpio_init_struct);

    gpio_pin_mux_config(CAN2_TX_GPIO_PORT, CAN2_TX_GPIO_PINS_SOURCE, CAN2_TX_GPIO_MUX);
    gpio_pin_mux_config(CAN2_RX_GPIO_PORT, CAN2_RX_GPIO_PINS_SOURCE, CAN2_RX_GPIO_MUX);
  }
}

/**
  * @brief  initialize can.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1, CAN2.
  * @retval none
  */
void can_init(can_type *can_x)
{
  can_base_type can_base_struct;
  can_baudrate_type can_baudrate_struct;
  can_filter_init_type can_filter_init_struct;

  /* config the can gpio */
  can_gpio_config(can_x);

  /* enable the can clock */
  if(can_x == CAN1)
  {
    crm_periph_clock_enable(CRM_CAN1_PERIPH_CLOCK, TRUE);
  }
  else if(can_x == CAN2)
  {
    crm_periph_clock_enable(CRM_CAN2_PERIPH_CLOCK, TRUE);
  }

  /* can base init */
  can_default_para_init(&can_base_struct);
  can_base_struct.mode_selection = CAN_MODE_COMMUNICATE;
  can_base_struct.ttc_enable = FALSE;
  can_base_struct.aebo_enable = TRUE;
  can_base_struct.aed_enable = TRUE;
  can_base_struct.prsf_enable = FALSE;
  can_base_struct.mdrsel_selection = CAN_DISCARDING_FIRST_RECEIVED;
  can_base_struct.mmssr_selection = CAN_SENDING_BY_ID;
  can_base_init(can_x, &can_base_struct);

  /* can baudrate, set boudrate = pclk/(baudrate_div *(3 + bts1_size + bts2_size)) */
  can_baudrate_struct.baudrate_div = 12;
  can_baudrate_struct.rsaw_size = CAN_RSAW_1TQ;
  can_baudrate_struct.bts1_size = CAN_BTS1_8TQ;
  can_baudrate_struct.bts2_size = CAN_BTS2_3TQ;
  can_baudrate_set(can_x, &can_baudrate_struct);

  /* can filter init */
  can_filter_init_struct.filter_activate_enable = TRUE;
  can_filter_init_struct.filter_mode = CAN_FILTER_MODE_ID_MASK;
  can_filter_init_struct.filter_fifo = CAN_FILTER_FIFO0;
  can_filter_init_struct.filter_bit = CAN_FILTER_32BIT;
  can_filter_init_struct.filter_id_high = 0;
  can_filter_init_struct.filter_id_low = 0;
  can_filter_init_struct.filter_mask_high = 0;
  can_filter_init_struct.filter_mask_low = 0;

  if(can_x == CAN1)
  {
    can_filter_init_struct.filter_number = 0;
    can_filter_init(can_x, &can_filter_init_struct);
  }
  else if(can_x == CAN2)
  {
    can_filter_init_struct.filter_number = 1;
    can_filter_init(can_x, &can_filter_init_struct);
  }

  /* can interrupt config */
  can_interrupt_enable(can_x, CAN_RF0MIEN_INT, TRUE);
  can_interrupt_enable(can_x, CAN_ETRIEN_INT, TRUE);
  can_interrupt_enable(can_x, CAN_EOIEN_INT, TRUE);

  if(can_x == CAN1)
  {
    nvic_irq_enable(CAN1_SE_IRQn, 0x00, 0x00);
    nvic_irq_enable(CAN1_RX0_IRQn, 0x00, 0x00);
  }
  else if(can_x == CAN2)
  {
    nvic_irq_enable(CAN2_SE_IRQn, 0x00, 0x00);
    nvic_irq_enable(CAN2_RX0_IRQn, 0x00, 0x00);
  }
}

/**
  * @brief  can transmit data
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1, CAN2.
  *         tx_message_struct: transmitmessage
  * @retval none
  */
void can_transmit_data(can_type *can_x, can_tx_message_type tx_message_struct)
{
  uint32_t timeout = 0;
  uint8_t transmit_mailbox;

  transmit_mailbox = can_message_transmit(can_x, &tx_message_struct);

  while(can_transmit_status_get(can_x, (can_tx_mailbox_num_type)transmit_mailbox) != CAN_TX_STATUS_SUCCESSFUL)
  {
    timeout++;

    if(timeout > 0xFFFF)
    {
      break;
    }
  }
}
