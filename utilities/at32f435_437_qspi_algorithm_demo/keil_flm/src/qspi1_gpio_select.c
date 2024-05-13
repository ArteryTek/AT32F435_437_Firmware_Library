/**
  **************************************************************************
  * @file     qspi1_gpio_select.c
  * @brief    device algorithm for new device flash
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

#include "at32f435_437.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup QSPI_flash_algorithm_for_keil
  * @{
  */

//<<< Use Configuration Wizard in Context Menu >>>
// <h>QSPI1 GPIO select
//   <i>Note: qspi1 gpio select
void qspi_gpio_slect(void)
{
  // <o>  QSPI1 CS pin
  //  <i> choose which  pin as qspi1 cs signal
  //  <0=> PB6
  //  <1=> PB9
  //  <2=> PB10
  //  <3=> PC11
  //  <4=> PG6
  #define QSPI_CS_PIN_SELECT     4
  #if (QSPI_CS_PIN_SELECT == 0)
    #define GPIO_CS_PORT_VALUE  GPIOB
    #define GPIO_CS_CLOCK_VALUE  1U
    #define GPIO_CS_PIN_VALUE    6U
    #define GPIO_CS_MUX_VALUE    10U
  #elif (QSPI_CS_PIN_SELECT == 1)
    #define GPIO_CS_PORT_VALUE  GPIOB
    #define GPIO_CS_CLOCK_VALUE  1U
    #define GPIO_CS_PIN_VALUE    9U
    #define GPIO_CS_MUX_VALUE    10U
  #elif (QSPI_CS_PIN_SELECT == 2)
    #define GPIO_CS_PORT_VALUE  GPIOB
    #define GPIO_CS_CLOCK_VALUE  1U
    #define GPIO_CS_PIN_VALUE    10U
    #define GPIO_CS_MUX_VALUE    9U
  #elif (QSPI_CS_PIN_SELECT == 3)
    #define GPIO_CS_PORT_VALUE  GPIOC
    #define GPIO_CS_CLOCK_VALUE  2U
    #define GPIO_CS_PIN_VALUE    11U
    #define GPIO_CS_MUX_VALUE    9U
  #elif (QSPI_CS_PIN_SELECT == 4)
    #define GPIO_CS_PORT_VALUE  GPIOG
    #define GPIO_CS_CLOCK_VALUE  6U
    #define GPIO_CS_PIN_VALUE    6U
    #define GPIO_CS_MUX_VALUE    10U
  #endif
  // <o>  QSPI1 SCK pin
  //  <i> choose which  pin as qspi1 sck signal
  //  <0=> PB1
  //  <1=> PB2
  //  <2=> PD3
  //  <3=> PF10
  #define QSPI_SCK_PIN_SELECT     3
  #if (QSPI_SCK_PIN_SELECT == 0)
    #define GPIO_SCK_PORT_VALUE  GPIOB
    #define GPIO_SCK_CLOCK_VALUE  1U
    #define GPIO_SCK_PIN_VALUE    1U
    #define GPIO_SCK_MUX_VALUE    9U
  #elif (QSPI_SCK_PIN_SELECT == 1)
    #define GPIO_SCK_PORT_VALUE  GPIOB
    #define GPIO_SCK_CLOCK_VALUE  1U
    #define GPIO_SCK_PIN_VALUE    2U
    #define GPIO_SCK_MUX_VALUE    9U
  #elif (QSPI_SCK_PIN_SELECT == 2)
    #define GPIO_SCK_PORT_VALUE  GPIOD
    #define GPIO_SCK_CLOCK_VALUE  3U
    #define GPIO_SCK_PIN_VALUE    3U
    #define GPIO_SCK_MUX_VALUE    9U
  #elif (QSPI_SCK_PIN_SELECT == 3)
    #define GPIO_SCK_PORT_VALUE  GPIOF
    #define GPIO_SCK_CLOCK_VALUE  5U
    #define GPIO_SCK_PIN_VALUE    10U
    #define GPIO_SCK_MUX_VALUE    9U
  #endif
  // <o>  QSPI1 IO0 pin
  //  <i> choose which  pin as qspi1 io0 signal
  //  <0=> PA6
  //  <1=> PB0
  //  <2=> PB11
  //  <3=> PC9
  //  <4=> PF8
  //  <5=> PH2
  #define QSPI_IO0_PIN_SELECT     4
  #if (QSPI_IO0_PIN_SELECT == 0)
    #define GPIO_IO0_PORT_VALUE  GPIOA
    #define GPIO_IO0_CLOCK_VALUE  0U
    #define GPIO_IO0_PIN_VALUE    6U
    #define GPIO_IO0_MUX_VALUE    10U
  #elif (QSPI_IO0_PIN_SELECT == 1)
    #define GPIO_IO0_PORT_VALUE  GPIOB
    #define GPIO_IO0_CLOCK_VALUE  1U
    #define GPIO_IO0_PIN_VALUE    0U
    #define GPIO_IO0_MUX_VALUE    10U
  #elif (QSPI_IO0_PIN_SELECT == 2)
    #define GPIO_IO0_PORT_VALUE  GPIOB
    #define GPIO_IO0_CLOCK_VALUE  1U
    #define GPIO_IO0_PIN_VALUE    11U
    #define GPIO_IO0_MUX_VALUE    10U
  #elif (QSPI_IO0_PIN_SELECT == 3)
    #define GPIO_IO0_PORT_VALUE  GPIOC
    #define GPIO_IO0_CLOCK_VALUE  2U
    #define GPIO_IO0_PIN_VALUE    9U
    #define GPIO_IO0_MUX_VALUE    9U
  #elif (QSPI_IO0_PIN_SELECT == 4)
    #define GPIO_IO0_PORT_VALUE  GPIOF
    #define GPIO_IO0_CLOCK_VALUE  5U
    #define GPIO_IO0_PIN_VALUE    8U
    #define GPIO_IO0_MUX_VALUE    10U
  #elif (QSPI_IO0_PIN_SELECT == 5)
    #define GPIO_IO0_PORT_VALUE  GPIOH
    #define GPIO_IO0_CLOCK_VALUE  7U
    #define GPIO_IO0_PIN_VALUE    2U
    #define GPIO_IO0_MUX_VALUE    10U
  #endif
  // <o>  QSPI1 IO1 pin
  //  <i> choose which  pin as qspi1 io1 signal
  //  <0=> PA7
  //  <1=> PB10
  //  <2=> PF9
  //  <3=> PH3
  #define QSPI_IO1_PIN_SELECT     2
  #if (QSPI_IO1_PIN_SELECT == 0)
    #define GPIO_IO1_PORT_VALUE  GPIOA
    #define GPIO_IO1_CLOCK_VALUE  0U
    #define GPIO_IO1_PIN_VALUE    7U
    #define GPIO_IO1_MUX_VALUE    10U
  #elif (QSPI_IO1_PIN_SELECT == 1)
    #define GPIO_IO1_PORT_VALUE  GPIOB
    #define GPIO_IO1_CLOCK_VALUE  1U
    #define GPIO_IO1_PIN_VALUE    10U
    #define GPIO_IO1_MUX_VALUE    10U
  #elif (QSPI_IO1_PIN_SELECT == 2)
    #define GPIO_IO1_PORT_VALUE  GPIOF
    #define GPIO_IO1_CLOCK_VALUE  5U
    #define GPIO_IO1_PIN_VALUE    9U
    #define GPIO_IO1_MUX_VALUE    10U
  #elif (QSPI_IO1_PIN_SELECT == 3)
    #define GPIO_IO1_PORT_VALUE  GPIOH
    #define GPIO_IO1_CLOCK_VALUE  7U
    #define GPIO_IO1_PIN_VALUE    3U
    #define GPIO_IO1_MUX_VALUE    10U
  #endif
  // <o>  QSPI1 IO2 pin
  //  <i> choose which  pin as qspi1 io2 signal
  //  <0=> PA15
  //  <1=> PC4
  //  <2=> PC8
  //  <3=> PE2
  //  <4=> PF7
  //  <5=> PG9
  #define QSPI_IO2_PIN_SELECT     4
  #if (QSPI_IO2_PIN_SELECT == 0)
    #define GPIO_IO2_PORT_VALUE  GPIOA
    #define GPIO_IO2_CLOCK_VALUE  0U
    #define GPIO_IO2_PIN_VALUE    15U
    #define GPIO_IO2_MUX_VALUE    10U
  #elif (QSPI_IO2_PIN_SELECT == 1)
    #define GPIO_IO2_PORT_VALUE  GPIOC
    #define GPIO_IO2_CLOCK_VALUE  2U
    #define GPIO_IO2_PIN_VALUE    4U
    #define GPIO_IO2_MUX_VALUE    10U
  #elif (QSPI_IO2_PIN_SELECT == 2)
    #define GPIO_IO2_PORT_VALUE  GPIOC
    #define GPIO_IO2_CLOCK_VALUE  2U
    #define GPIO_IO2_PIN_VALUE    8U
    #define GPIO_IO2_MUX_VALUE    9U
  #elif (QSPI_IO2_PIN_SELECT == 3)
    #define GPIO_IO2_PORT_VALUE  GPIOE
    #define GPIO_IO2_CLOCK_VALUE  4U
    #define GPIO_IO2_PIN_VALUE    2U
    #define GPIO_IO2_MUX_VALUE    9U
  #elif (QSPI_IO2_PIN_SELECT == 4)
    #define GPIO_IO2_PORT_VALUE  GPIOF
    #define GPIO_IO2_CLOCK_VALUE  5U
    #define GPIO_IO2_PIN_VALUE    7U
    #define GPIO_IO2_MUX_VALUE    9U
  #elif (QSPI_IO2_PIN_SELECT == 5)
    #define GPIO_IO2_PORT_VALUE  GPIOG
    #define GPIO_IO2_CLOCK_VALUE  6U
    #define GPIO_IO2_PIN_VALUE    9U
    #define GPIO_IO2_MUX_VALUE    9U
  #endif
  // <o>  QSPI1 IO3 pin
  //  <i> choose which  pin as qspi1 io3 signal
  //  <0=> PA1
  //  <1=> PB3
  //  <2=> PD13
  //  <3=> PF6
  //  <4=> PG14
  //  <5=> PC5
  #define QSPI_IO3_PIN_SELECT     5
  #if (QSPI_IO3_PIN_SELECT == 0)
    #define GPIO_IO3_PORT_VALUE  GPIOA
    #define GPIO_IO3_CLOCK_VALUE  0U
    #define GPIO_IO3_PIN_VALUE    1U
    #define GPIO_IO3_MUX_VALUE    9U
  #elif (QSPI_IO3_PIN_SELECT == 1)
    #define GPIO_IO3_PORT_VALUE  GPIOB
    #define GPIO_IO3_CLOCK_VALUE  1U
    #define GPIO_IO3_PIN_VALUE    3U
    #define GPIO_IO3_MUX_VALUE    10U
  #elif (QSPI_IO3_PIN_SELECT == 2)
    #define GPIO_IO3_PORT_VALUE  GPIOD
    #define GPIO_IO3_CLOCK_VALUE  3U
    #define GPIO_IO3_PIN_VALUE    13U
    #define GPIO_IO3_MUX_VALUE    9U
  #elif (QSPI_IO3_PIN_SELECT == 3)
    #define GPIO_IO3_PORT_VALUE  GPIOF
    #define GPIO_IO3_CLOCK_VALUE  5U
    #define GPIO_IO3_PIN_VALUE    6U
    #define GPIO_IO3_MUX_VALUE    9U
  #elif (QSPI_IO3_PIN_SELECT == 4)
    #define GPIO_IO3_PORT_VALUE  GPIOG
    #define GPIO_IO3_CLOCK_VALUE  6U
    #define GPIO_IO3_PIN_VALUE    14U
    #define GPIO_IO3_MUX_VALUE    9U
  #elif (QSPI_IO3_PIN_SELECT == 5)
    #define GPIO_IO3_PORT_VALUE  GPIOC
    #define GPIO_IO3_CLOCK_VALUE  2U
    #define GPIO_IO3_PIN_VALUE    5U
    #define GPIO_IO3_MUX_VALUE    10U
  #endif 
  
  /* enable the qspi clock */
  CRM->ahben3 |= (uint32_t)(1<<1);
  
  /* enable the gpio pin clock */
  CRM->ahben1 |= (uint32_t)(1<<GPIO_CS_CLOCK_VALUE);
  CRM->ahben1 |= (uint32_t)(1<<GPIO_SCK_CLOCK_VALUE);
  CRM->ahben1 |= (uint32_t)(1<<GPIO_IO0_CLOCK_VALUE);
  CRM->ahben1 |= (uint32_t)(1<<GPIO_IO1_CLOCK_VALUE);
  CRM->ahben1 |= (uint32_t)(1<<GPIO_IO2_CLOCK_VALUE);
  CRM->ahben1 |= (uint32_t)(1<<GPIO_IO3_CLOCK_VALUE);
  
  /* configure the gpio */  
  GPIO_CS_PORT_VALUE->cfgr |= (uint32_t)(2<<(2 * GPIO_CS_PIN_VALUE));
  GPIO_CS_PORT_VALUE->odrvr |= (uint32_t)(1<<(2 * GPIO_CS_PIN_VALUE));
#if (GPIO_CS_PIN_VALUE < 8)
  GPIO_CS_PORT_VALUE->muxl |= (uint32_t)(GPIO_CS_MUX_VALUE<<(4 * GPIO_CS_PIN_VALUE));
#else
  GPIO_CS_PORT_VALUE->muxh |= (uint32_t)(GPIO_CS_MUX_VALUE<<(4 *(GPIO_CS_PIN_VALUE - 8)));
#endif
  
  GPIO_SCK_PORT_VALUE->cfgr |= (uint32_t)(2<<(2 * GPIO_SCK_PIN_VALUE));
  GPIO_SCK_PORT_VALUE->odrvr |= (uint32_t)(1<<(2 * GPIO_SCK_PIN_VALUE));
#if (GPIO_SCK_PIN_VALUE < 8)
  GPIO_SCK_PORT_VALUE->muxl |= (uint32_t)(GPIO_SCK_MUX_VALUE<<(4 * GPIO_SCK_PIN_VALUE));
#else
  GPIO_SCK_PORT_VALUE->muxh |= (uint32_t)(GPIO_SCK_MUX_VALUE<<(4 *(GPIO_SCK_PIN_VALUE - 8)));
#endif
  
  GPIO_IO0_PORT_VALUE->cfgr |= (uint32_t)(2<<(2 * GPIO_IO0_PIN_VALUE));
  GPIO_IO0_PORT_VALUE->odrvr |= (uint32_t)(1<<(2 * GPIO_IO0_PIN_VALUE));
#if (GPIO_IO0_PIN_VALUE < 8)
  GPIO_IO0_PORT_VALUE->muxl |= (uint32_t)(GPIO_IO0_MUX_VALUE<<(4 * GPIO_IO0_PIN_VALUE));
#else
  GPIO_IO0_PORT_VALUE->muxh |= (uint32_t)(GPIO_IO0_MUX_VALUE<<(4 *(GPIO_IO0_PIN_VALUE - 8)));
#endif
  
  GPIO_IO1_PORT_VALUE->cfgr |= (uint32_t)(2<<(2 * GPIO_IO1_PIN_VALUE));
  GPIO_IO1_PORT_VALUE->odrvr |= (uint32_t)(1<<(2 * GPIO_IO1_PIN_VALUE));
#if (GPIO_IO1_PIN_VALUE < 8)
  GPIO_IO1_PORT_VALUE->muxl |= (uint32_t)(GPIO_IO1_MUX_VALUE<<(4 * GPIO_IO1_PIN_VALUE));
#else
  GPIO_IO1_PORT_VALUE->muxh |= (uint32_t)(GPIO_IO1_MUX_VALUE<<(4 *(GPIO_IO1_PIN_VALUE - 8)));
#endif
  
  GPIO_IO2_PORT_VALUE->cfgr |= (uint32_t)(2<<(2 * GPIO_IO2_PIN_VALUE));
  GPIO_IO2_PORT_VALUE->odrvr |= (uint32_t)(1<<(2 * GPIO_IO2_PIN_VALUE));
#if (GPIO_IO2_PIN_VALUE < 8)
  GPIO_IO2_PORT_VALUE->muxl |= (uint32_t)(GPIO_IO2_MUX_VALUE<<(4 * GPIO_IO2_PIN_VALUE));
#else
  GPIO_IO2_PORT_VALUE->muxh |= (uint32_t)(GPIO_IO2_MUX_VALUE<<(4 *(GPIO_IO2_PIN_VALUE - 8)));
#endif
  
  GPIO_IO3_PORT_VALUE->cfgr |= (uint32_t)(2<<(2 * GPIO_IO3_PIN_VALUE));
  GPIO_IO3_PORT_VALUE->odrvr |= (uint32_t)(1<<(2 * GPIO_IO3_PIN_VALUE));
#if (GPIO_IO3_PIN_VALUE < 8)
  GPIO_IO3_PORT_VALUE->muxl |= (uint32_t)(GPIO_IO3_MUX_VALUE<<(4 * GPIO_IO3_PIN_VALUE));
#else
  GPIO_IO3_PORT_VALUE->muxh |= (uint32_t)(GPIO_IO3_MUX_VALUE<<(4 *(GPIO_IO3_PIN_VALUE - 8)));
#endif
}
// </h>
//<<< end of configuration section >>>

/**
  * @}
  */

/**
  * @}
  */
