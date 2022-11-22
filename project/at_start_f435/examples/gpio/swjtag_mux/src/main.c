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

/* includes */
#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_GPIO_swjtag_muxe GPIO_swjtag_mux
  * @{
  */

void swj_dp_config(void);
void gpio_pins_toggle(gpio_type* gpio_x, uint16_t gpio_pin);

/**
  * @brief  configure the gpio of swj-dp.
  * @param  none
  * @retval none
  */
void swj_dp_config(void)
{
  gpio_init_type gpio_init_struct;

  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure pa13 (jtms/swdat) as general output push-pull */
  gpio_init_struct.gpio_pins = GPIO_PINS_13;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure pa14 (jtck/swclk) as mux usart2 tx */
  gpio_init_struct.gpio_pins = GPIO_PINS_14;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE14, GPIO_MUX_8);

  /* configure usart2 param */
  usart_init(USART2, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART2, TRUE);
  usart_enable(USART2, TRUE);
}

/**
  * @brief  toggles the specified gpio pin
  * @param  gpiox: where x can be (a..g depending on device used) to select the gpio peripheral
  * @param  gpio_pin: specifies the pins to be toggled.
  * @retval none
  */
void gpio_pins_toggle(gpio_type* gpio_x, uint16_t gpio_pin)
{
  gpio_x->odt ^= gpio_pin;
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  static uint8_t data = 0;
  system_clock_config();
  at32_board_init();
  swj_dp_config();
  while(1)
  {
    /* pa13 pin toggle */
    gpio_pins_toggle(GPIOA, GPIO_PINS_13);
    delay_ms(500);
    /* pa14 pin send usart char */
    while(usart_flag_get(USART2, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART2, data++);
  }
}

/**
  * @}
  */

/**
  * @}
  */
