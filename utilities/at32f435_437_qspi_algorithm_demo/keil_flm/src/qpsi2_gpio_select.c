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
// <h> QSPI2 GPIO select
//   <i>Note: qspi2 gpio select
void qspi_gpio_slect(void)
{
  // <o>  QSPI2 CS pin 
  //  <i> choose which  pin as qspi2 cs signal
  //  <0=> PB8
  //  <1=> PG8
  #define QSPI_CS_PIN_SELECT     1
  #if (QSPI_CS_PIN_SELECT == 0)
    #define GPIO_CS_PORT_VALUE  GPIOB
    #define GPIO_CS_CLOCK_VALUE  1U
    #define GPIO_CS_PIN_VALUE    8U
    #define GPIO_CS_MUX_VALUE    10U
  #elif (QSPI_CS_PIN_SELECT == 1)
    #define GPIO_CS_PORT_VALUE  GPIOG
    #define GPIO_CS_CLOCK_VALUE  6U
    #define GPIO_CS_PIN_VALUE    8U
    #define GPIO_CS_MUX_VALUE    5U
  #endif
  // <o>  QSPI2 SCK pin 
  //  <i> choose which  pin as qspi2 sck signal
  //  <0=> PB1
  //  <1=> PG13
  #define QSPI_SCK_PIN_SELECT     1
  #if (QSPI_SCK_PIN_SELECT == 0)
    #define GPIO_SCK_PORT_VALUE  GPIOB
    #define GPIO_SCK_CLOCK_VALUE  1U
    #define GPIO_SCK_PIN_VALUE    1U
    #define GPIO_SCK_MUX_VALUE    10U
  #elif (QSPI_SCK_PIN_SELECT == 1)
    #define GPIO_SCK_PORT_VALUE  GPIOG
    #define GPIO_SCK_CLOCK_VALUE  6U
    #define GPIO_SCK_PIN_VALUE    13U
    #define GPIO_SCK_MUX_VALUE    5U
  #endif
  // <o>  QSPI2 IO0 pin 
  //  <i> choose which  pin as qspi2 io0 signal
  //  <0=> PB0
  //  <1=> PE7
  //  <2=> PG14
  #define QSPI_IO0_PIN_SELECT     2
  #if (QSPI_IO0_PIN_SELECT == 0)
    #define GPIO_IO0_PORT_VALUE  GPIOB
    #define GPIO_IO0_CLOCK_VALUE  1U
    #define GPIO_IO0_PIN_VALUE    0U
    #define GPIO_IO0_MUX_VALUE    9U
  #elif (QSPI_IO0_PIN_SELECT == 1)
    #define GPIO_IO0_PORT_VALUE  GPIOE
    #define GPIO_IO0_CLOCK_VALUE  4U
    #define GPIO_IO0_PIN_VALUE    7U
    #define GPIO_IO0_MUX_VALUE    10U
  #elif (QSPI_IO0_PIN_SELECT == 2)
    #define GPIO_IO0_PORT_VALUE  GPIOG
    #define GPIO_IO0_CLOCK_VALUE  6U
    #define GPIO_IO0_PIN_VALUE    14U
    #define GPIO_IO0_MUX_VALUE    5U
  #endif
  // <o>  QSPI2 IO1 pin 
  //  <i> choose which  pin as qspi2 io1 signal
  //  <0=> PB7
  //  <1=> PC3
  //  <2=> PE8
  //  <3=> PG12
  #define QSPI_IO1_PIN_SELECT     3
  #if (QSPI_IO1_PIN_SELECT == 0)
    #define GPIO_IO1_PORT_VALUE  GPIOB
    #define GPIO_IO1_CLOCK_VALUE  1U
    #define GPIO_IO1_PIN_VALUE    7U
    #define GPIO_IO1_MUX_VALUE    9U
  #elif (QSPI_IO1_PIN_SELECT == 1)
    #define GPIO_IO1_PORT_VALUE  GPIOC
    #define GPIO_IO1_CLOCK_VALUE  2U
    #define GPIO_IO1_PIN_VALUE    3U
    #define GPIO_IO1_MUX_VALUE    9U
  #elif (QSPI_IO1_PIN_SELECT == 2)
    #define GPIO_IO1_PORT_VALUE  GPIOE
    #define GPIO_IO1_CLOCK_VALUE  4U
    #define GPIO_IO1_PIN_VALUE    8U
    #define GPIO_IO1_MUX_VALUE    10U
  #elif (QSPI_IO1_PIN_SELECT == 3)
    #define GPIO_IO1_PORT_VALUE  GPIOG
    #define GPIO_IO1_CLOCK_VALUE  6U
    #define GPIO_IO1_PIN_VALUE    12U
    #define GPIO_IO1_MUX_VALUE    5U
  #endif
  // <o>  QSPI2 IO2 pin 
  //  <i> choose which  pin as qspi2 io2 signal
  //  <0=> PA5
  //  <1=> PE9
  //  <2=> PG10
  #define QSPI_IO2_PIN_SELECT     2
  #if (QSPI_IO2_PIN_SELECT == 0)
    #define GPIO_IO2_PORT_VALUE  GPIOA
    #define GPIO_IO2_CLOCK_VALUE  0U
    #define GPIO_IO2_PIN_VALUE    5U
    #define GPIO_IO2_MUX_VALUE    9U
  #elif (QSPI_IO2_PIN_SELECT == 1)
    #define GPIO_IO2_PORT_VALUE  GPIOE
    #define GPIO_IO2_CLOCK_VALUE  4U
    #define GPIO_IO2_PIN_VALUE    9U
    #define GPIO_IO2_MUX_VALUE    10U
  #elif (QSPI_IO2_PIN_SELECT == 2)
    #define GPIO_IO2_PORT_VALUE  GPIOG
    #define GPIO_IO2_CLOCK_VALUE  6U
    #define GPIO_IO2_PIN_VALUE    10U
    #define GPIO_IO2_MUX_VALUE    5U
  #endif
  // <o>  QSPI2 IO3 pin 
  //  <i> choose which  pin as qspi2 io3 signal
  //  <0=> PA3
  //  <1=> PE10
  //  <2=> PG11
  #define QSPI_IO3_PIN_SELECT     2
  #if (QSPI_IO3_PIN_SELECT == 0)
    #define GPIO_IO3_PORT_VALUE  GPIOA
    #define GPIO_IO3_CLOCK_VALUE  0U
    #define GPIO_IO3_PIN_VALUE    3U
    #define GPIO_IO3_MUX_VALUE    9U
  #elif (QSPI_IO3_PIN_SELECT == 1)
    #define GPIO_IO3_PORT_VALUE  GPIOE
    #define GPIO_IO3_CLOCK_VALUE  4U
    #define GPIO_IO3_PIN_VALUE    10U
    #define GPIO_IO3_MUX_VALUE    10U
  #elif (QSPI_IO3_PIN_SELECT == 2)
    #define GPIO_IO3_PORT_VALUE  GPIOG
    #define GPIO_IO3_CLOCK_VALUE  6U
    #define GPIO_IO3_PIN_VALUE    11U
    #define GPIO_IO3_MUX_VALUE    5U
  #endif
  
  /* enable the qspi clock */
  CRM->ahben3 |= (uint32_t)(1<<14);
  
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
//</h>
//<<< end of configuration section >>>

/**
  * @}
  */

/**
  * @}
  */
