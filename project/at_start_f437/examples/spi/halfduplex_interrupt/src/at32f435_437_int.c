/**
  **************************************************************************
  * @file     at32f435_437_int.c
  * @brief    main interrupt service routines.
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

/* includes ------------------------------------------------------------------*/
#include "at32f435_437_int.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_SPI_halfduplex_interrupt
  * @{
  */

#define BUFFERSIZE 32

extern uint8_t spi3_buffer_tx[BUFFERSIZE];
extern uint8_t spi2_buffer_rx[BUFFERSIZE];
extern uint32_t tx_index;
extern uint32_t rx_index;

/**
  * @brief  this function handles nmi exception.
  * @param  none
  * @retval none
  */
void NMI_Handler(void)
{
}

/**
  * @brief  this function handles hard fault exception.
  * @param  none
  * @retval none
  */
void HardFault_Handler(void)
{
  /* go to infinite loop when hard fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles memory manage exception.
  * @param  none
  * @retval none
  */
void MemManage_Handler(void)
{
  /* go to infinite loop when memory manage exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles bus fault exception.
  * @param  none
  * @retval none
  */
void BusFault_Handler(void)
{
  /* go to infinite loop when bus fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles usage fault exception.
  * @param  none
  * @retval none
  */
void UsageFault_Handler(void)
{
  /* go to infinite loop when usage fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles svcall exception.
  * @param  none
  * @retval none
  */
void SVC_Handler(void)
{
}

/**
  * @brief  this function handles debug monitor exception.
  * @param  none
  * @retval none
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  this function handles pendsv_handler exception.
  * @param  none
  * @retval none
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  this function handles systick handler.
  * @param  none
  * @retval none
  */
void SysTick_Handler(void)
{
}

/**
  * @brief  This function handles the spi3 interrupt request.
  * @param  None
  * @retval None
  */
 void SPI3_I2S3EXT_IRQHandler(void)
{
  if(spi_i2s_interrupt_flag_get(SPI3, SPI_I2S_TDBE_FLAG) != RESET)
  {
    spi_i2s_data_transmit(SPI3, spi3_buffer_tx[tx_index++]);
    if(tx_index == BUFFERSIZE)
    {
      spi_i2s_interrupt_enable(SPI3, SPI_I2S_TDBE_INT, FALSE);
    }
  }
}

/**
  * @brief  This function handles the spi2 interrupt request.
  * @param  None
  * @retval None
  */
 void SPI2_I2S2EXT_IRQHandler(void)
{
  if(spi_i2s_interrupt_flag_get(SPI2, SPI_I2S_RDBF_FLAG) != RESET)
  {
    spi2_buffer_rx[rx_index++] = spi_i2s_data_receive(SPI2);
  }
}

/**
  * @}
  */

/**
  * @}
  */

