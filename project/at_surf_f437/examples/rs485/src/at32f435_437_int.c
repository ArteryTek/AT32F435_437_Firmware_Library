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
#include "at_surf_f437_board_rs485.h"

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
  * @brief  this function handles usart interrupt request.
  * @param  none
  * @retval none
  */
void RS485_USART_IRQHandler(void)
{
  if(usart_interrupt_flag_get(RS485_USARTx, USART_RDBF_FLAG) != RESET)
  {
    /* read one byte from the receive data register */
    rs485_rx_buffer[rs485_rx_counter++] = usart_data_receive(RS485_USARTx);
  }

  if(usart_interrupt_flag_get(RS485_USARTx, USART_TDBE_FLAG) != RESET)
  {
    if(rs485_tx_counter < rs485_tx_number)
    {
      /* write one byte to the transmit data register */
      usart_data_transmit(RS485_USARTx, rs485_tx_buffer[rs485_tx_counter++]);
    }
    else
    {
      /* disable the rs485 transmit interrupt */
      usart_interrupt_enable(RS485_USARTx, USART_TDBE_INT, FALSE);
    }
  }

  if(usart_interrupt_flag_get(RS485_USARTx, USART_IDLEF_FLAG) != RESET)
  {
    /* clear usart idle flag */
    usart_data_receive(RS485_USARTx);

    /* initializes receive conuter */
    rs485_rx_number = rs485_rx_counter;
    rs485_rx_counter = 0;
  }
}

