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
#include "i2c_application.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_I2C_communication_int I2C_communication_int
  * @{
  */

#define I2C_TIMEOUT                      0xFFFFFFF

//#define I2Cx_CLKCTRL                   0xB170FFFF   //10K
//#define I2Cx_CLKCTRL                   0xC0E06969   //50K
#define I2Cx_CLKCTRL                     0x80504C4E   //100K
//#define I2Cx_CLKCTRL                   0x30F03C6B   //200K

#define I2Cx_ADDRESS                     0xA0

#define I2Cx_PORT                        I2C1
#define I2Cx_CLK                         CRM_I2C1_PERIPH_CLOCK

#define I2Cx_SCL_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK
#define I2Cx_SCL_GPIO_PIN                GPIO_PINS_6
#define I2Cx_SCL_GPIO_PinsSource         GPIO_PINS_SOURCE6
#define I2Cx_SCL_GPIO_PORT               GPIOB
#define I2Cx_SCL_GPIO_MUX                GPIO_MUX_4

#define I2Cx_SDA_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK
#define I2Cx_SDA_GPIO_PIN                GPIO_PINS_7
#define I2Cx_SDA_GPIO_PinsSource         GPIO_PINS_SOURCE7
#define I2Cx_SDA_GPIO_PORT               GPIOB
#define I2Cx_SDA_GPIO_MUX                GPIO_MUX_4

#define I2Cx_EVT_IRQn                    I2C1_EVT_IRQn
#define I2Cx_ERR_IRQn                    I2C1_ERR_IRQn

#define BUF_SIZE                         8
#define MASTER_BOARD

uint8_t tx_buf[BUF_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t rx_buf[BUF_SIZE] = {0};

i2c_handle_type hi2cx;

/**
  * @brief  error handler program
  * @param  i2c_status
  * @retval none
  */
void error_handler(uint32_t error_code)
{
  while(1)
  {
    at32_led_toggle(LED2);
    delay_ms(500);
  }
}

/**
  * @brief  compare whether the valus of buffer 1 and buffer 2 are equal.
  * @param  buffer1: buffer 1 address.
            buffer2: buffer 2 address.
  * @retval 0: equal.
  *         1: unequal.
  */
uint32_t buffer_compare(uint8_t* buffer1, uint8_t* buffer2, uint32_t len)
{
  uint32_t i;

  for(i = 0; i < len; i++)
  {
    if(buffer1[i] != buffer2[i])
    {
      return 1;
    }
  }

  return 0;
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  i2c_status_type i2c_status;

  /* initial system clock */
  system_clock_config();

  /* config nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* at board initial */
  at32_board_init();

  hi2cx.i2cx = I2Cx_PORT;

  /* i2c config */
  i2c_config(&hi2cx);

  while(1)
  {

  #if defined (MASTER_BOARD)

    /* wait for key USER_BUTTON press before starting the communication */
    while(at32_button_press() != USER_BUTTON)
    {
    }

    /* start the request reception process */
    if((i2c_status = i2c_master_transmit_int(&hi2cx, I2Cx_ADDRESS, tx_buf, BUF_SIZE, I2C_TIMEOUT)) != I2C_OK)
    {
      error_handler(i2c_status);
    }

    /* wait for the communication to end */
    if(i2c_wait_end(&hi2cx, I2C_TIMEOUT) != I2C_OK)
    {
      error_handler(i2c_status);
    }

    delay_ms(10);

    /* start the request reception process */
    if((i2c_status = i2c_master_receive_int(&hi2cx, I2Cx_ADDRESS, rx_buf, BUF_SIZE, I2C_TIMEOUT)) != I2C_OK)
    {
      error_handler(i2c_status);
    }

    /* wait for the communication to end */
    if(i2c_wait_end(&hi2cx, I2C_TIMEOUT) != I2C_OK)
    {
      error_handler(i2c_status);
    }

    if(buffer_compare(tx_buf, rx_buf, BUF_SIZE) == 0)
    {
      at32_led_on(LED3);
    }
    else
    {
      error_handler(i2c_status);
    }

  #else

    /* wait for key USER_BUTTON press before starting the communication */
    while(at32_button_press() != USER_BUTTON)
    {
    }

    /* start the transmission process */
    if((i2c_status = i2c_slave_receive_int(&hi2cx, rx_buf, BUF_SIZE, I2C_TIMEOUT)) != I2C_OK)
    {
      error_handler(i2c_status);
    }

    /* wait for the communication to end */
    if(i2c_wait_end(&hi2cx, I2C_TIMEOUT) != I2C_OK)
    {
      error_handler(i2c_status);
    }

    if((i2c_status = i2c_slave_transmit_int(&hi2cx, tx_buf, BUF_SIZE, I2C_TIMEOUT)) != I2C_OK)
    {
      error_handler(i2c_status);
    }

    /* wait for the communication to end */
    if(i2c_wait_end(&hi2cx, I2C_TIMEOUT) != I2C_OK)
    {
      error_handler(i2c_status);
    }

    if(buffer_compare(tx_buf, rx_buf, BUF_SIZE) == 0)
    {
      at32_led_on(LED3);
    }
    else
    {
      error_handler(i2c_status);
    }
  #endif

  }
}

/**
  * @brief  initializes peripherals used by the i2c.
  * @param  none
  * @retval none
  */
void i2c_lowlevel_init(i2c_handle_type* hi2c)
{
  gpio_init_type gpio_init_structure;

  if(hi2c->i2cx == I2Cx_PORT)
  {
    /* i2c periph clock enable */
    crm_periph_clock_enable(I2Cx_CLK, TRUE);
    crm_periph_clock_enable(I2Cx_SCL_GPIO_CLK, TRUE);
    crm_periph_clock_enable(I2Cx_SDA_GPIO_CLK, TRUE);

    /* gpio configuration */
    gpio_pin_mux_config(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_GPIO_PinsSource, I2Cx_SCL_GPIO_MUX);

    gpio_pin_mux_config(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_GPIO_PinsSource, I2Cx_SDA_GPIO_MUX);

    /* configure i2c pins: scl */
    gpio_init_structure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_structure.gpio_mode           = GPIO_MODE_MUX;
    gpio_init_structure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_structure.gpio_pull           = GPIO_PULL_NONE;

    gpio_init_structure.gpio_pins           = I2Cx_SCL_GPIO_PIN;
    gpio_init(I2Cx_SCL_GPIO_PORT, &gpio_init_structure);

    /* configure i2c pins: sda */
    gpio_init_structure.gpio_pins           = I2Cx_SDA_GPIO_PIN;
    gpio_init(I2Cx_SDA_GPIO_PORT, &gpio_init_structure);

    /* configure and enable i2c interrupt */
    nvic_irq_enable(I2Cx_EVT_IRQn, 0, 0);
    nvic_irq_enable(I2Cx_ERR_IRQn, 0, 0);

    /* config i2c */
    i2c_init(hi2c->i2cx, 0x0F, I2Cx_CLKCTRL);

    i2c_own_address1_set(hi2c->i2cx, I2C_ADDRESS_MODE_7BIT, I2Cx_ADDRESS);
  }
}

/**
  * @}
  */

/**
  * @}
  */
