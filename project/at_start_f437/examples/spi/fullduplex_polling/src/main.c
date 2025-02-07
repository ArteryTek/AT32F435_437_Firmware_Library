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

/** @addtogroup 437_SPI_fullduplex_polling SPI_fullduplex_polling
  * @{
  */

#define SPI3_AS_MASTER_CS_HIGH   gpio_bits_set(GPIOA, GPIO_PINS_4)
#define SPI3_AS_MASTER_CS_LOW    gpio_bits_reset(GPIOA, GPIO_PINS_4)

#define SPI2_AS_MASTER_CS_HIGH   gpio_bits_set(GPIOD, GPIO_PINS_0)
#define SPI2_AS_MASTER_CS_LOW    gpio_bits_reset(GPIOD, GPIO_PINS_0)
#define BUFFER_SIZE              32

spi_init_type spi_init_struct;
uint8_t spi3_buffer_tx[BUFFER_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                       0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
                                       0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                       0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20};
uint8_t spi2_buffer_tx[BUFFER_SIZE] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
                                       0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60,
                                       0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
                                       0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70};
uint8_t spi3_buffer_rx[BUFFER_SIZE], spi2_buffer_rx[BUFFER_SIZE];
uint32_t tx_index = 0, rx_index = 0;
volatile error_status transfer_status1 = ERROR, transfer_status2 = ERROR;
volatile error_status transfer_status3 = ERROR, transfer_status4 = ERROR;

/**
  * @brief  buffer compare function.
  * @param  pbuffer1, pbuffer2: buffers to be compared.
  * @param  buffer_length: buffer's length
  * @retval the result of compare
  */
error_status buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t bufferlength)
{
  while(bufferlength--)
  {
    if(*pbuffer1 != *pbuffer2)
    {
      return ERROR;
    }

    pbuffer1++;
    pbuffer2++;
  }
  return SUCCESS;
}

/**
  * @brief  spi configuration.
  * @param  none
  * @retval none
  */
static void spi_config(void)
{
  /* master spi initialization */
  crm_periph_clock_enable(CRM_SPI3_PERIPH_CLOCK, TRUE);
  spi_default_para_init(&spi_init_struct);
  
  /* dual line unidirectional full-duplex mode */
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(SPI3, &spi_init_struct);
  
  spi_enable(SPI3, TRUE);
  
  /* slave spi initialization */
  crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
  
  /* dual line unidirectional full-duplex mode */
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_SLAVE;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_HARDWARE_MODE;
  spi_init(SPI2, &spi_init_struct);

  spi_enable(SPI2, TRUE);
}

/**
  * @brief  gpio configuration.
  * @param  spi1_mode : the mode of spi3
  * @param  spi2_mode : the mode of spi2
  * @retval none
  */
static void gpio_config(uint16_t spi3_mode, uint16_t spi2_mode)
{
  gpio_init_type gpio_initstructure;
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  
  /* spi3 cs pin */
  gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull = GPIO_PULL_UP;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  if(spi3_mode == SPI_MODE_MASTER)
  {
    gpio_initstructure.gpio_mode = GPIO_MODE_OUTPUT;
  }
  else
  {
    gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE4, GPIO_MUX_6);
  }
  gpio_initstructure.gpio_pins = GPIO_PINS_4;
  gpio_init(GPIOA, &gpio_initstructure);

  /* spi3 sck pin */
  gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_initstructure.gpio_pins = GPIO_PINS_10;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE10, GPIO_MUX_6);

  /* spi3 miso pin */
  gpio_initstructure.gpio_pull = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins = GPIO_PINS_11;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE11, GPIO_MUX_6);

  /* spi3 mosi pin */
  gpio_initstructure.gpio_pull = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins = GPIO_PINS_12;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE12, GPIO_MUX_6);

  /* spi2 cs pin */
  gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull = GPIO_PULL_UP;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  if(spi2_mode == SPI_MODE_SLAVE)
  {
    gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
    gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE0, GPIO_MUX_7);
  }
  else
  {
    gpio_initstructure.gpio_mode = GPIO_MODE_OUTPUT;
  }
  gpio_initstructure.gpio_pins = GPIO_PINS_0;
  gpio_init(GPIOD, &gpio_initstructure);

  /* spi2 sck pin */
  gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
  gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_initstructure.gpio_pins = GPIO_PINS_1;
  gpio_init(GPIOD, &gpio_initstructure);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE1, GPIO_MUX_6);

  /* spi2 miso pin */
  gpio_initstructure.gpio_pull = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins = GPIO_PINS_2;
  gpio_init(GPIOC, &gpio_initstructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE2, GPIO_MUX_5);

  /* spi2 mosi pin */
  gpio_initstructure.gpio_pull = GPIO_PULL_UP;
  gpio_initstructure.gpio_pins = GPIO_PINS_4;
  gpio_init(GPIOD, &gpio_initstructure);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE4, GPIO_MUX_6);
  
  /* non communication time: master pull up CS pin release slave */
  if(spi3_mode == SPI_MODE_MASTER)
  {
    SPI3_AS_MASTER_CS_HIGH;
  }
  if(spi2_mode == SPI_MODE_MASTER)
  {
    SPI2_AS_MASTER_CS_HIGH;
  }
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index = 0;
  system_clock_config();
  at32_board_init();
  at32_led_on(LED4);
  gpio_config(SPI_MODE_MASTER, SPI_MODE_SLAVE);
  spi_config();
  
  /* start communication: master pull down CS pin select slave */
  SPI3_AS_MASTER_CS_LOW;

  /* transfer procedure:the "BUFFER_SIZE" data transfer */
  while(tx_index < BUFFER_SIZE)
  {
    /* slave and master transmit data fill */
    while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI2, spi2_buffer_tx[tx_index]);
    while(spi_i2s_flag_get(SPI3, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI3, spi3_buffer_tx[tx_index++]);
    
    /* slave and master receive data get */
    while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET);
    spi2_buffer_rx[rx_index] = spi_i2s_data_receive(SPI2);
    while(spi_i2s_flag_get(SPI3, SPI_I2S_RDBF_FLAG) == RESET);
    spi3_buffer_rx[rx_index++] = spi_i2s_data_receive(SPI3);
  }
  
  /* wait master and slave idle when communication end */
  while(spi_i2s_flag_get(SPI3, SPI_I2S_BF_FLAG) != RESET);
  while(spi_i2s_flag_get(SPI2, SPI_I2S_BF_FLAG) != RESET);
  
  /* end communication: master pull up CS pin release slave */
  SPI3_AS_MASTER_CS_HIGH;

  /* test result:the data check */
  transfer_status1 = buffer_compare(spi2_buffer_rx, spi3_buffer_tx, BUFFER_SIZE);
  transfer_status2 = buffer_compare(spi3_buffer_rx, spi2_buffer_tx, BUFFER_SIZE);

  
  spi_enable(SPI3, FALSE);
  spi_enable(SPI2, FALSE);
  
  /* master &slave mode switch */
  gpio_config(SPI_MODE_SLAVE, SPI_MODE_MASTER);
  spi_init_struct.master_slave_mode =SPI_MODE_SLAVE;
  spi_init_struct.cs_mode_selection = SPI_CS_HARDWARE_MODE;
  spi_init(SPI3, &spi_init_struct);

  spi_init_struct.master_slave_mode =SPI_MODE_MASTER;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(SPI2, &spi_init_struct);

  /* receive buffer clear */
  tx_index = 0;
  rx_index = 0;
  for(index = 0; index < BUFFER_SIZE; index++)
  {
    spi3_buffer_rx[index] = 0;
    spi2_buffer_rx[index] = 0;
  }

  spi_enable(SPI2, TRUE);
  spi_enable(SPI3, TRUE);
  
  /* start communication: master pull down CS pin select slave */
  SPI2_AS_MASTER_CS_LOW;

  /* transfer procedure:the "BUFFER_SIZE" data transfer */
  while(tx_index < BUFFER_SIZE)
  {
    /* slave and master transmit data fill */
    while(spi_i2s_flag_get(SPI3, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI3, spi3_buffer_tx[tx_index]);
    while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(SPI2, spi2_buffer_tx[tx_index++]);
    
    /* slave and master receive data get */
    while(spi_i2s_flag_get(SPI3, SPI_I2S_RDBF_FLAG) == RESET);
    spi3_buffer_rx[rx_index] = spi_i2s_data_receive(SPI3);
    while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET);
    spi2_buffer_rx[rx_index++] = spi_i2s_data_receive(SPI2);
  }
  
  /* wait master and slave idle when communication end */
  while(spi_i2s_flag_get(SPI2, SPI_I2S_BF_FLAG) != RESET);
  while(spi_i2s_flag_get(SPI3, SPI_I2S_BF_FLAG) != RESET);
  
  /* end communication: master pull up CS pin release slave */
  SPI2_AS_MASTER_CS_HIGH;

  /* test result:the data check */
  transfer_status3 = buffer_compare(spi2_buffer_rx, spi3_buffer_tx, BUFFER_SIZE);
  transfer_status4 = buffer_compare(spi3_buffer_rx, spi2_buffer_tx, BUFFER_SIZE);

  /* test result indicate:if success ,led2 lights */
  if((transfer_status1 == SUCCESS) && (transfer_status2 == SUCCESS) && \
     (transfer_status3 == SUCCESS) && (transfer_status4 == SUCCESS))
  {
    at32_led_on(LED2);
  }
  else
  {
    at32_led_on(LED3);
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
