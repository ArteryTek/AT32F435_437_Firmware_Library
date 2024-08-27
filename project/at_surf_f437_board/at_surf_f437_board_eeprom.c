/**
  **************************************************************************
  * @file     at_surf_f437_board_eeprom.c
  * @brief    the driver library of the eeprom
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

#include "at_surf_f437_board_eeprom.h"

i2c_handle_type hi2c_ee;

/**
  * @brief  check if the eeprom is ready.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: eeprom address.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type eeprom_ready_check(i2c_handle_type* hi2c, uint16_t address, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->error_code = I2C_OK;

  /* wait for the busy falg to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSYF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_transmit_set(hi2c->i2cx, address, 0, I2C_SOFT_STOP_MODE, I2C_GEN_START_WRITE);

  /* wait for the tdis falg to be set */
  if(i2c_wait_flag(hi2c, I2C_TDC_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    /* wait for the stop falg to be set */
    if(i2c_wait_flag(hi2c, I2C_STOPF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
    {
      return I2C_ERR_STEP_2;
    }

    /* clear stop flag */
    i2c_flag_clear(hi2c->i2cx, I2C_STOPF_FLAG);

    /* reset ctrl2 register */
    i2c_reset_ctrl2_register(hi2c);

    return I2C_ERR_STEP_3;
  }

  /* wait for the stop falg to be set */
  i2c_stop_generate(hi2c->i2cx);

  /* wait for the stop falg to be set */
  if(i2c_wait_flag(hi2c, I2C_STOPF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_FLAG);

  /* reset ctrl2 register */
  i2c_reset_ctrl2_register(hi2c);

  return I2C_OK;
}

/**
  * @brief  wait for the eeprom to be ready.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: eeprom address.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type eeprom_ready_wait(i2c_handle_type* hi2c, uint16_t address, uint32_t timeout)
{
  uint32_t number = 2000;

  while(number)
  {
    /* check if the eeprom is ready, there will be an acknowledge if it is ready */
    if(eeprom_ready_check(hi2c, address, timeout) == I2C_OK)
    {
      return I2C_OK;
    }

    /* cumulative transfers */
    number++;
  }

  return I2C_ERR_TIMEOUT;
}

/**
  * @brief  write a single page to eeprom.
  * @param  hi2c: the handle points to the operation information.
  * @param  mode: i2c transfer mode.
  *         - EE_MODE_POLL: transmits data through polling mode.
  *         - EE_MODE_INT: transmits data through interrupt mode.
  *         - EE_MODE_DMA: transmits data through dma mode.
  * @param  address: eeprom address.
  * @param  mem_address: eeprom memory address.
  * @param  pdata: data buffer.
  * @param  number: the number of data to be transferred.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type eeprom_page_write(i2c_handle_type* hi2c, eeprom_i2c_mode_type mode, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t number, uint32_t timeout)
{
  i2c_status_type i2c_status = I2C_OK;

  if(mode == EE_MODE_POLL)
  {
    i2c_status = i2c_memory_write     (hi2c, I2C_MEM_ADDR_WIDIH_8, address, mem_address, pdata, number, timeout);
  }
  else if(mode == EE_MODE_INT)
  {
    i2c_status = i2c_memory_write_int (hi2c, I2C_MEM_ADDR_WIDIH_8, address, mem_address, pdata, number, timeout);

    /* wait for the communication to end */
    if(i2c_wait_end(hi2c, EE_I2C_TIMEOUT) != I2C_OK)
    {
      i2c_status = I2C_ERR_TIMEOUT;
    }
  }
  else if(mode == EE_MODE_DMA)
  {
    i2c_status = i2c_memory_write_dma (hi2c, I2C_MEM_ADDR_WIDIH_8, address, mem_address, pdata, number, timeout);

    /* wait for the communication to end */
    if(i2c_wait_end(hi2c, EE_I2C_TIMEOUT) != I2C_OK)
    {
      i2c_status = I2C_ERR_TIMEOUT;
    }
  }

  if(i2c_status != I2C_OK)
  {
    return i2c_status;
  }

  /* wait for eeprom to be ready */
  if(eeprom_ready_wait(hi2c, address, timeout) != I2C_OK)
  {
    return i2c_status;
  }

  return I2C_OK;
}

/**
  * @brief  write data to eeprom.
  * @param  hi2c: the handle points to the operation information.
  * @param  mode: i2c transfer mode.
  *         - EE_MODE_POLL: transmits data through polling mode.
  *         - EE_MODE_INT: transmits data through interrupt mode.
  *         - EE_MODE_DMA: transmits data through dma mode.
  * @param  address: eeprom address.
  * @param  mem_address: eeprom memory address.
  * @param  pdata: data buffer.
  * @param  number: the number of data to be transferred.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type eeprom_data_write(i2c_handle_type* hi2c, eeprom_i2c_mode_type mode, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t number, uint32_t timeout)
{
  i2c_status_type i2c_status = I2C_OK;

  uint16_t page_number = 0;
  uint16_t page_last = 0;
  uint16_t single_write = 0;
  uint16_t write_address = 0;

  if(number == 0)
  {
    return I2C_OK;
  }

  /* calculate the number of data transferred for the first time, and the subsequent transfer address will be aligned */
  single_write = mem_address / 8 * 8 + 8 - mem_address;
  write_address = mem_address;

  if(number < single_write)
  {
    single_write = number;
  }

  /* transfer data */
  if((i2c_status = eeprom_page_write(hi2c, mode, address, write_address, pdata, single_write, timeout)) != I2C_OK)
  {
    return i2c_status;
  }

  /* subtract the number of data transmitted for the first time */
  number -= single_write;

  /* address increase */
  write_address += single_write;

  /* pointer increase */
  pdata += single_write;

  /* if the remaining quantity is 0, it means the transfer is complete */
  if(number == 0)
  {
    return I2C_OK;
  }

  /* calculate the number of bytes in less than one page */
  page_last = number % EE_PAGE_SIZE;

  /* calculate the number of full pages */
  page_number =  number / EE_PAGE_SIZE;

  /* transfer an integer number of pages of data */
  while(page_number > 0)
  {
    /* transfer data */
    if((i2c_status = eeprom_page_write(hi2c, mode, address, write_address, pdata, EE_PAGE_SIZE, timeout)) != I2C_OK)
    {
      return i2c_status;
    }

    /* page number subtract*/
    page_number--;

    /* address increase */
    write_address += EE_PAGE_SIZE;

    /* pointer increase */
    pdata += EE_PAGE_SIZE;

  }

  /* transferring less than one page of data */
  if(page_last)
  {
    /* transfer data */
    if((i2c_status = eeprom_page_write(hi2c, mode, address, write_address, pdata, page_last, timeout)) != I2C_OK)
    {
      return i2c_status;
    }
  }

  return i2c_status;
}

/**
  * @brief  read data from eeprom.
  * @param  hi2c: the handle points to the operation information.
  * @param  mode: i2c transfer mode.
  *         - EE_MODE_POLL: transmits data through polling mode.
  *         - EE_MODE_INT: transmits data through interrupt mode.
  *         - EE_MODE_DMA: transmits data through dma mode.
  * @param  address: eeprom address.
  * @param  mem_address: eeprom memory address.
  * @param  pdata: data buffer.
  * @param  number: the number of data to be transferred.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type eeprom_data_read(i2c_handle_type* hi2c, eeprom_i2c_mode_type mode, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t number, uint32_t timeout)
{
  i2c_status_type i2c_status = I2C_OK;

  if(mode == EE_MODE_POLL)
  {
    i2c_status = i2c_memory_read(hi2c, I2C_MEM_ADDR_WIDIH_8, address, mem_address, pdata, number, timeout);
  }
  else if(mode == EE_MODE_INT)
  {
    i2c_status = i2c_memory_read_int(hi2c, I2C_MEM_ADDR_WIDIH_8, address, mem_address, pdata, number, timeout);

    /* wait for the communication to end */
    if(i2c_wait_end(hi2c, EE_I2C_TIMEOUT) != I2C_OK)
    {
      i2c_status = I2C_ERR_TIMEOUT;
    }
  }
  else if(mode == EE_MODE_DMA)
  {
    i2c_status = i2c_memory_read_dma(hi2c, I2C_MEM_ADDR_WIDIH_8, address, mem_address, pdata, number, timeout);

    /* wait for the communication to end */
    if(i2c_wait_end(hi2c, EE_I2C_TIMEOUT) != I2C_OK)
    {
      i2c_status = I2C_ERR_TIMEOUT;
    }
  }

  return i2c_status;
}

/**
  * @brief  eeprom i2c peripheral initialization.
  * @param  i2c_speed: i2c speed.
  * @retval none.
  */
void eeprom_i2c_lowlevel_init(i2c_handle_type* hi2c, uint32_t i2c_speed)
{
  gpio_init_type gpio_init_structure;

  /* i2c periph clock enable */
  crm_periph_clock_enable(EE_I2C_CLK, TRUE);
  crm_periph_clock_enable(EE_I2C_SCL_GPIO_CLK, TRUE);
  crm_periph_clock_enable(EE_I2C_SDA_GPIO_CLK, TRUE);

  /* gpio configuration */
  gpio_pin_mux_config(EE_I2C_SCL_GPIO_PORT, EE_I2C_SCL_GPIO_PINS_SOURCE, EE_I2C_SCL_GPIO_MUX);

  gpio_pin_mux_config(EE_I2C_SDA_GPIO_PORT, EE_I2C_SDA_GPIO_PINS_SOURCE, EE_I2C_SDA_GPIO_MUX);

  /* configure i2c pins: scl */
  gpio_init_structure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_structure.gpio_mode           = GPIO_MODE_MUX;
  gpio_init_structure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
  gpio_init_structure.gpio_pull           = GPIO_PULL_UP;

  gpio_init_structure.gpio_pins           = EE_I2C_SCL_GPIO_PIN;
  gpio_init(EE_I2C_SCL_GPIO_PORT, &gpio_init_structure);

  /* configure i2c pins: sda */
  gpio_init_structure.gpio_pins           = EE_I2C_SDA_GPIO_PIN;
  gpio_init(EE_I2C_SDA_GPIO_PORT, &gpio_init_structure);

  /* configure and enable i2c interrupt */
  nvic_irq_enable(EE_I2C_EVT_IRQn, 0, 0);
  nvic_irq_enable(EE_I2C_ERR_IRQn, 0, 0);

  /* configure and enable i2c dma channel interrupt */
  nvic_irq_enable(EE_I2C_DMA_TX_IRQn, 0, 0);
  nvic_irq_enable(EE_I2C_DMA_RX_IRQn, 0, 0);

  /* i2c dma tx and rx channels configuration */
  /* enable the dma clock */
  crm_periph_clock_enable(EE_I2C_DMA_CLK, TRUE);

  /* i2c dma channel configuration */
  hi2c->dma_tx_channel = EE_I2C_DMA_TX_CHANNEL;
  hi2c->dma_rx_channel = EE_I2C_DMA_RX_CHANNEL;

  dma_reset(hi2c->dma_tx_channel);
  dma_reset(hi2c->dma_rx_channel);

  hi2c->dma_init_struct.peripheral_base_addr    = (uint32_t)&hi2c->i2cx->txdt;
  hi2c->dma_init_struct.memory_base_addr        = 0;
  hi2c->dma_init_struct.direction               = DMA_DIR_MEMORY_TO_PERIPHERAL;
  hi2c->dma_init_struct.buffer_size             = 0xFFFF;
  hi2c->dma_init_struct.peripheral_inc_enable   = FALSE;
  hi2c->dma_init_struct.memory_inc_enable       = TRUE;
  hi2c->dma_init_struct.peripheral_data_width   = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  hi2c->dma_init_struct.memory_data_width       = DMA_MEMORY_DATA_WIDTH_BYTE;
  hi2c->dma_init_struct.loop_mode_enable        = FALSE;
  hi2c->dma_init_struct.priority                = DMA_PRIORITY_LOW;

  dma_init(hi2c->dma_tx_channel, &hi2c->dma_init_struct);
  dma_init(hi2c->dma_rx_channel, &hi2c->dma_init_struct);

  dmamux_init(EE_I2C_DMA_TX_DMAMUX_CHANNEL, EE_I2C_DMA_TX_DMAREQ);
  dmamux_init(EE_I2C_DMA_RX_DMAMUX_CHANNEL, EE_I2C_DMA_RX_DMAREQ);

  dmamux_enable(EE_I2C_DMA, TRUE);

  /* config i2c */
  i2c_init(hi2c->i2cx, 0x0F, i2c_speed);
}

/**
  * @brief  eeprom_init.
  * @param  i2c_speed: i2c speed.
  * @retval none.
  */
void eeprom_init(uint32_t i2c_speed)
{
  hi2c_ee.i2cx = EE_I2C;

  /* reset i2c peripheral */
  i2c_reset(hi2c_ee.i2cx);

  /* i2c peripheral initialization */
  eeprom_i2c_lowlevel_init(&hi2c_ee, i2c_speed);

  /* i2c peripheral enable */
  i2c_enable(hi2c_ee.i2cx, TRUE);
}
