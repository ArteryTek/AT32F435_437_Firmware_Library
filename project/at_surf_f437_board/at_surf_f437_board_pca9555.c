/**
  **************************************************************************
  * @file     at_surf_f437_board_pca9555.c
  * @brief    the driver library of the pca9555
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

#include "at_surf_f437_board_pca9555.h"

pca9555_io_type pca_io;
pca9555_io_type pca_io_mode;
i2c_status_type pca_i2c_status;
volatile uint16_t pca9555_int_flag = 0;

i2c_handle_type hi2c_pca;

/**
  * @brief  read io input status.
  * @param  para: io status.
  * @retval function execution result.
  *         - ERROR.
  *         - SUCCESS.
  */
error_status pca9555_io_input_read(void)
{
  pca_i2c_status = i2c_memory_read_dma(&hi2c_pca, I2C_MEM_ADDR_WIDIH_8, PCA_I2C_ADDRESS, PCA_REG_INPUT, (uint8_t *)&pca_io.io, 2, PCA_I2C_TIMEOUT);

  /* write data to memory device */
  if(pca_i2c_status != I2C_OK)
  {
    return ERROR;
  }

  if(i2c_wait_end(&hi2c_pca, PCA_I2C_TIMEOUT) != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  read io input status.
  * @param  none.
  * @retval function execution result.
  *         - ERROR.
  *         - SUCCESS.
  */
void pca9555_io_scan(void)
{
  if(pca9555_int_flag)
  {
    exint_interrupt_enable(PCA_INT_EXINT_LINE, FALSE);

    pca9555_int_flag = 0;

    exint_interrupt_enable(PCA_INT_EXINT_LINE, TRUE);

    pca9555_io_input_read();
  }
}

/**
  * @brief  when the io state changes, the interrupt pin generates a falling edge
  * @param  none.
  * @retval none.
  */
void pca9555_irq_handler(void)
{
  if(exint_flag_get(PCA_INT_EXINT_LINE) != RESET)
  {
    pca9555_int_flag = 1;

    exint_flag_clear(PCA_INT_EXINT_LINE);
  }
}

/**
  * @brief  config the selected pin as input mode.
  * @param  pins: gpio pin number
  *         parameter can be any combination of gpio_pin_x, gpio_pin_x as following values:
  *         - PCA_IO0_PINS_0
  *         - PCA_IO0_PINS_1
  *         - PCA_IO0_PINS_2
  *         - PCA_IO0_PINS_3
  *         - PCA_IO0_PINS_4
  *         - PCA_IO0_PINS_5
  *         - PCA_IO0_PINS_6
  *         - PCA_IO0_PINS_7
  *         - PCA_IO1_PINS_0
  *         - PCA_IO1_PINS_1
  *         - PCA_IO1_PINS_2
  *         - PCA_IO1_PINS_3
  *         - PCA_IO1_PINS_4
  *         - PCA_IO1_PINS_5
  *         - PCA_IO1_PINS_6
  *         - PCA_IO1_PINS_7
  *         - PCA_IO_PINS_ALL
  * @retval function execution result.
  *         - ERROR.
  *         - SUCCESS.
  */
error_status pca9555_input_mode_config(uint16_t pins)
{
  pca_io_mode.io |= pins;

  pca_i2c_status = i2c_memory_write_dma(&hi2c_pca, I2C_MEM_ADDR_WIDIH_8, PCA_I2C_ADDRESS, PCA_REG_CTRL, (uint8_t *)&pca_io_mode.io, 2, PCA_I2C_TIMEOUT);

  /* write data to memory device */
  if(pca_i2c_status != I2C_OK)
  {
    return ERROR;
  }

  if(i2c_wait_end(&hi2c_pca, PCA_I2C_TIMEOUT) != I2C_OK)
  {
    return ERROR;
  }

  pca9555_io_input_read();

  return SUCCESS;
}

/**
  * @brief  config the selected pin as input mode.
  * @param  pins: gpio pin number
  *         parameter can be any combination of gpio_pin_x, gpio_pin_x as following values:
  *         - PCA_IO0_PINS_0
  *         - PCA_IO0_PINS_1
  *         - PCA_IO0_PINS_2
  *         - PCA_IO0_PINS_3
  *         - PCA_IO0_PINS_4
  *         - PCA_IO0_PINS_5
  *         - PCA_IO0_PINS_6
  *         - PCA_IO0_PINS_7
  *         - PCA_IO1_PINS_0
  *         - PCA_IO1_PINS_1
  *         - PCA_IO1_PINS_2
  *         - PCA_IO1_PINS_3
  *         - PCA_IO1_PINS_4
  *         - PCA_IO1_PINS_5
  *         - PCA_IO1_PINS_6
  *         - PCA_IO1_PINS_7
  *         - PCA_IO_PINS_ALL
  * @retval function execution result.
  *         - ERROR.
  *         - SUCCESS.
  */
error_status pca9555_out_mode_config(uint16_t pins)
{
  pca_io_mode.io &= ~pins;

  pca_i2c_status = i2c_memory_write_dma(&hi2c_pca, I2C_MEM_ADDR_WIDIH_8, PCA_I2C_ADDRESS, PCA_REG_CTRL, (uint8_t *)&pca_io_mode.io, 2, PCA_I2C_TIMEOUT);

  /* write data to memory device */
  if(pca_i2c_status != I2C_OK)
  {
    return ERROR;
  }

  if(i2c_wait_end(&hi2c_pca, PCA_I2C_TIMEOUT) != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  read the specified input port pin.
  * @param  pins: gpio pin number
  *         this parameter can be one of the following values:
  *         - PCA_IO0_PINS_0
  *         - PCA_IO0_PINS_1
  *         - PCA_IO0_PINS_2
  *         - PCA_IO0_PINS_3
  *         - PCA_IO0_PINS_4
  *         - PCA_IO0_PINS_5
  *         - PCA_IO0_PINS_6
  *         - PCA_IO0_PINS_7
  *         - PCA_IO1_PINS_0
  *         - PCA_IO1_PINS_1
  *         - PCA_IO1_PINS_2
  *         - PCA_IO1_PINS_3
  *         - PCA_IO1_PINS_4
  *         - PCA_IO1_PINS_5
  *         - PCA_IO1_PINS_6
  *         - PCA_IO1_PINS_7
  * @retval flag_status (SET or RESET)
  */
flag_status pca9555_input_data_bit_read(uint16_t pins)
{
  if(pins != (pins & pca_io.io))
  {
    return RESET;
  }
  else
  {
    return SET;
  }
}

/**
  * @brief  read the specified gpio input data port.
  * @retval gpio input data port value.
  */
uint16_t pca9555_input_data_read(void)
{
  return ((uint16_t)(pca_io.io));
}

/**
  * @brief  reset the selected data port bits.
  * @param  pins: gpio pin number
  *         parameter can be any combination of gpio_pin_x, gpio_pin_x as following values:
  *         - PCA_IO0_PINS_0
  *         - PCA_IO0_PINS_1
  *         - PCA_IO0_PINS_2
  *         - PCA_IO0_PINS_3
  *         - PCA_IO0_PINS_4
  *         - PCA_IO0_PINS_5
  *         - PCA_IO0_PINS_6
  *         - PCA_IO0_PINS_7
  *         - PCA_IO1_PINS_0
  *         - PCA_IO1_PINS_1
  *         - PCA_IO1_PINS_2
  *         - PCA_IO1_PINS_3
  *         - PCA_IO1_PINS_4
  *         - PCA_IO1_PINS_5
  *         - PCA_IO1_PINS_6
  *         - PCA_IO1_PINS_7
  *         - PCA_IO_PINS_ALL
  * @retval function execution result.
  *         - ERROR.
  *         - SUCCESS.
  */
error_status pca9555_bits_reset(uint16_t pins)
{
  pca_io.io &= ~pins;

  pca_i2c_status = i2c_memory_write_dma(&hi2c_pca, I2C_MEM_ADDR_WIDIH_8, PCA_I2C_ADDRESS, PCA_REG_OUTPUT, (uint8_t *)&pca_io.io, 2, PCA_I2C_TIMEOUT);

  /* write data to memory device */
  if(pca_i2c_status != I2C_OK)
  {
    return ERROR;
  }

  if(i2c_wait_end(&hi2c_pca, PCA_I2C_TIMEOUT) != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  set the selected data port bits.
  * @param  pins: gpio pin number
  *         parameter can be any combination of gpio_pin_x, gpio_pin_x as following values:
  *         - PCA_IO0_PINS_0
  *         - PCA_IO0_PINS_1
  *         - PCA_IO0_PINS_2
  *         - PCA_IO0_PINS_3
  *         - PCA_IO0_PINS_4
  *         - PCA_IO0_PINS_5
  *         - PCA_IO0_PINS_6
  *         - PCA_IO0_PINS_7
  *         - PCA_IO1_PINS_0
  *         - PCA_IO1_PINS_1
  *         - PCA_IO1_PINS_2
  *         - PCA_IO1_PINS_3
  *         - PCA_IO1_PINS_4
  *         - PCA_IO1_PINS_5
  *         - PCA_IO1_PINS_6
  *         - PCA_IO1_PINS_7
  *         - PCA_IO_PINS_ALL
  * @retval function execution result.
  *         - ERROR.
  *         - SUCCESS.
  */
error_status pca9555_bits_set(uint16_t pins)
{
  pca_io.io |= pins;

  pca_i2c_status = i2c_memory_write_dma(&hi2c_pca, I2C_MEM_ADDR_WIDIH_8, PCA_I2C_ADDRESS, PCA_REG_OUTPUT, (uint8_t *)&pca_io.io, 2, PCA_I2C_TIMEOUT);

  /* write data to memory device */
  if(pca_i2c_status != I2C_OK)
  {
    return ERROR;
  }

  if(i2c_wait_end(&hi2c_pca, PCA_I2C_TIMEOUT) != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  eeprom i2c peripheral initialization.
  * @param  i2c_speed: i2c speed.
  * @retval none.
  */
void pca9555_i2c_lowlevel_init(i2c_handle_type* hi2c, uint32_t i2c_speed)
{
  gpio_init_type gpio_init_structure;

  /* i2c periph clock enable */
  crm_periph_clock_enable(PCA_I2C_CLK, TRUE);
  crm_periph_clock_enable(PCA_I2C_SCL_GPIO_CLK, TRUE);
  crm_periph_clock_enable(PCA_I2C_SDA_GPIO_CLK, TRUE);

  /* gpio configuration */
  gpio_pin_mux_config(PCA_I2C_SCL_GPIO_PORT, PCA_I2C_SCL_GPIO_PINS_SOURCE, PCA_I2C_SCL_GPIO_MUX);

  gpio_pin_mux_config(PCA_I2C_SDA_GPIO_PORT, PCA_I2C_SDA_GPIO_PINS_SOURCE, PCA_I2C_SDA_GPIO_MUX);

  /* configure i2c pins: scl */
  gpio_init_structure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_structure.gpio_mode           = GPIO_MODE_MUX;
  gpio_init_structure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
  gpio_init_structure.gpio_pull           = GPIO_PULL_UP;

  gpio_init_structure.gpio_pins           = PCA_I2C_SCL_GPIO_PIN;
  gpio_init(PCA_I2C_SCL_GPIO_PORT, &gpio_init_structure);

  /* configure i2c pins: sda */
  gpio_init_structure.gpio_pins           = PCA_I2C_SDA_GPIO_PIN;
  gpio_init(PCA_I2C_SDA_GPIO_PORT, &gpio_init_structure);

  /* configure and enable i2c interrupt */
  nvic_irq_enable(PCA_I2C_EVT_IRQn, 0, 0);
  nvic_irq_enable(PCA_I2C_ERR_IRQn, 0, 0);

  /* configure and enable i2c dma channel interrupt */
  nvic_irq_enable(PCA_I2C_DMA_TX_IRQn, 0, 0);
  nvic_irq_enable(PCA_I2C_DMA_RX_IRQn, 0, 0);

  /* i2c dma tx and rx channels configuration */
  /* enable the dma clock */
  crm_periph_clock_enable(PCA_I2C_DMA_CLK, TRUE);

  /* i2c dma channel configuration */
  hi2c->dma_tx_channel = PCA_I2C_DMA_TX_CHANNEL;
  hi2c->dma_rx_channel = PCA_I2C_DMA_RX_CHANNEL;

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

  dmamux_init(PCA_I2C_DMA_TX_DMAMUX_CHANNEL, PCA_I2C_DMA_TX_DMAREQ);
  dmamux_init(PCA_I2C_DMA_RX_DMAMUX_CHANNEL, PCA_I2C_DMA_RX_DMAREQ);

  dmamux_enable(PCA_I2C_DMA, TRUE);

  /* config i2c */
  i2c_init(hi2c->i2cx, 0x0F, i2c_speed);
}

/**
  * @brief  initializes pca9555.
  * @param  i2c_speed: i2c speed.
  * @retval none
  */
void pca9555_init(uint32_t i2c_speed)
{
  static uint8_t init_status = 0;
  gpio_init_type gpio_init_struct;
  exint_init_type exint_init_struct;

  /* prevent reinitialization */
  if(init_status == 0)
  {
    init_status = 1;

    hi2c_pca.i2cx = PCA_I2C;

    /* reset i2c peripheral */
    i2c_reset(hi2c_pca.i2cx);

    /* i2c peripheral initialization */
    pca9555_i2c_lowlevel_init(&hi2c_pca, i2c_speed);

    /* i2c peripheral enable */
    i2c_enable(hi2c_pca.i2cx, TRUE);

    /* enable gpio clock */
    crm_periph_clock_enable(PCA_INT_GPIO_CLK, TRUE);
    crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

    gpio_default_para_init(&gpio_init_struct);

    /* configure the int pin */
    gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;

    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_pins = PCA_INT_GPIO_PIN;
    gpio_init(PCA_INT_GPIO_PORT, &gpio_init_struct);

    /* configure the exint line */
    scfg_exint_line_config(PCA_INT_SOURCE_PORT, PCA_INT_SOURCE_PIN);

    exint_default_para_init(&exint_init_struct);
    exint_init_struct.line_enable = TRUE;
    exint_init_struct.line_mode = EXINT_LINE_INTERRUPT;
    exint_init_struct.line_select = PCA_INT_EXINT_LINE;
    exint_init_struct.line_polarity = EXINT_TRIGGER_FALLING_EDGE;
    exint_init(&exint_init_struct);

    /* enable the exint interrupt */
    nvic_irq_enable(PCA_INT_EXINT_IRQn, 1, 0);

    /* config all io modes as input */
    pca9555_input_mode_config(PCA_IO_PINS_ALL);

    /* read io input status */
    pca9555_io_input_read();
  }
}

