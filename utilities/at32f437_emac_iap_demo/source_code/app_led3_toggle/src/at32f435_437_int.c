/**
  **************************************************************************
  * @file     at32f435_437_int.c
  * @brief    main interrupt service routines.
  **************************************************************************
  */

/* includes ------------------------------------------------------------------*/
#include "at32f435_437_int.h"
#include "netconf.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup EMAC_iap_app_led3_toggle
  * @{
  */

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
  while (1)
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
  while (1)
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
  while (1)
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
  while (1)
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
  * @brief  this function handles timer6 overflow handler.
  * @param  none
  * @retval none
  */
void TMR6_DAC_GLOBAL_IRQHandler(void)
{
  if(tmr_interrupt_flag_get(TMR6, TMR_OVF_FLAG) != RESET)
  {
    /* Update the local_time by adding SYSTEMTICK_PERIOD_MS each SysTick interrupt */
    time_update();
    tmr_flag_clear(TMR6, TMR_OVF_FLAG);
  }
}

/**
  * @}
  */

/**
  * @}
  */
