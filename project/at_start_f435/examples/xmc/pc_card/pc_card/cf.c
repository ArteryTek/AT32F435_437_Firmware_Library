/**
  **************************************************************************
  * @file     cf.c
  * @brief    xmc pc card program
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

/* Includes ------------------------------------------------------------------*/
#include "cf.h"
#include "at32f435_437.h"
#include "at32f435_437_board.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup XMC_CF_CARD
  * @{
  */

edma_init_type EDMA_InitStructure;
u16 DMA1_MEM_LEN;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup PCCARD_Private_Constants PCCARD Private Constants
  * @{
  */

#define PCCARD_TIMEOUT_READ_ID      (uint32_t)0x0000FFFF
#define PCCARD_TIMEOUT_SECTOR       (uint32_t)0x0000FFFF
#define PCCARD_TIMEOUT_STATUS       (uint32_t)0x01000000

#define PCCARD_STATUS_OK            (uint8_t)0x58
#define PCCARD_STATUS_WRITE_OK      (uint8_t)0x50
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/** @defgroup PCCARD_Exported_Functions PCCARD Exported Functions
  * @{
  */

/** @defgroup PCCARD_Exported_Functions_Group1 Initialization and de-initialization functions
  * @brief    Initialization and Configuration functions
  *
  @verbatim
  ==============================================================================
          ##### PCCARD Initialization and de-initialization functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to initialize/de-initialize
    the PCCARD memory

@endverbatim
  * @{
  */
  /**
  * @brief  DMA configuration
  * @param  EDMA_CHx: pointer to a EDMA_Channel_Type structure
  * @param  cpar_src: the Memory 1 addr
  * @param  cmar_dst: the Memory 2 addr
  * @param  cndtr: transfer size
  * @param  Transfer_Type: 
  *     @arg Read_Transfer: from Memory 1 to Memory 2
  *     @arg Write_Transfer: from Memory 2 to Memory 1
  * @param  Transfer_Width: BYTE or HALFWORD
  * @retval None
  */
void MYDMA_Config(edma_stream_type* DMAy_Streamx, uint32_t cpar_src, uint32_t cmar_dst, uint32_t cndtr, uint8_t Transfer_Type, uint8_t Transfer_Width)
{
  edma_init_type  EDMA_InitStruct;
  crm_periph_clock_enable(CRM_EDMA_PERIPH_CLOCK, TRUE);

  edma_reset(DMAy_Streamx);

  if(Transfer_Type == Read_Transfer)
  {
    EDMA_InitStruct.peripheral_inc_enable = FALSE;
    EDMA_InitStruct.memory_inc_enable = TRUE;
  }
  else
  {
    EDMA_InitStruct.peripheral_inc_enable = TRUE;
    EDMA_InitStruct.memory_inc_enable = FALSE;
  }

  if(Transfer_Width == Enable_8_bit_Transfer)
  {
    EDMA_InitStruct.peripheral_data_width = EDMA_PERIPHERAL_DATA_WIDTH_BYTE;
    EDMA_InitStruct.memory_data_width = EDMA_MEMORY_DATA_WIDTH_BYTE;
  }
  else
  {
    EDMA_InitStruct.peripheral_data_width = EDMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    EDMA_InitStruct.memory_data_width = EDMA_MEMORY_DATA_WIDTH_HALFWORD;
  }

  EDMA_InitStruct.direction = EDMA_DIR_MEMORY_TO_MEMORY;
  EDMA_InitStruct.buffer_size = cndtr;
  EDMA_InitStruct.peripheral_base_addr = cpar_src;

  EDMA_InitStruct.peripheral_burst_mode = EDMA_PERIPHERAL_SINGLE;
  EDMA_InitStruct.memory0_base_addr = cmar_dst;

  EDMA_InitStruct.memory_burst_mode = EDMA_MEMORY_SINGLE;
  EDMA_InitStruct.loop_mode_enable = FALSE;
  EDMA_InitStruct.fifo_mode_enable = TRUE;
  EDMA_InitStruct.fifo_threshold = EDMA_FIFO_THRESHOLD_FULL;
  EDMA_InitStruct.priority = EDMA_PRIORITY_MEDIUM;
  edma_init(DMAy_Streamx,&EDMA_InitStruct);
 
  
  edma_stream_enable(DMAy_Streamx, TRUE);
}
/**
  * @brief  XMC InitCtrl
  * @param  Device: pointer to a XMC_Bank4_Type structure
  * @param  Init: pointer to a XMC_PCCARDInitType structure
  * @param  Set_reg_8_bit: choose 8/16bit
  * @retval STATUS_OK or STATUS_ERROR
  */
StatusType XMC_PCCARD_InitCtrl(xmc_bank4_type *Device, xmc_pccard_init_type *Init, uint8_t Set_reg_8_bit)
{
  if(Set_reg_8_bit == 1)
  {
   Device->bk4ctrl_bit.extmdbw = XMC_MEM_WIDTH_8;
  }
  else
  {
   Device->bk4ctrl_bit.extmdbw = XMC_MEM_WIDTH_16;
  }
   
  Device->bk4ctrl_bit.tar = Init->delay_time_ar;
  Device->bk4ctrl_bit.tcr = Init->delay_time_cr;

  return STATUS_OK;

}
/**
  * @brief  XMC Enable Wait Feature
  * @param  Device: pointer to a XMC_Bank4_Type structure
  * @param  Init: pointer to a XMC_PCCARDInitType structure
  * @retval STATUS_OK or STATUS_ERROR
  */
StatusType XMC_Enable_Wait_Feature(xmc_bank4_type *Device, xmc_pccard_init_type *Init)
{
 
 Device->bk4ctrl_bit.nwen = Init->enable_wait;

  return STATUS_OK;
}
/**
  * @brief  Set 16bit_Feature
  * @param  Device: pointer to a XMC_Bank4_Type structure
  * @param  Init: pointer to a XMC_PCCARDInitType structure
  * @param  Set_reg_8_bit: choose 8/16bit
  * @retval HAL status
  */
StatusType XMC_Enable_16bit_Feature(xmc_bank4_type *Device, xmc_pccard_init_type *Init, uint8_t Set_reg_8_bit)
{
  if(Set_reg_8_bit == 1)
  {
   Device->bk4ctrl_bit.extmdbw = XMC_MEM_WIDTH_8;
  }
  else
  {
   Device->bk4ctrl_bit.extmdbw = XMC_MEM_WIDTH_16;
  }

  return STATUS_OK;
}
/**
  * @brief  Init CommonSpace Timing
  * @param  Device: pointer to a XMC_Bank4_Type structure
  * @param  Timing: pointer to a Common space timing structure
  * @retval HAL status
  */
StatusType XMC_PCCARD_InitCommonSpaceTiming(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing)
{
 Device->bk4tmgmem_bit.cmdhizt = Timing->mem_hiz_time;
 Device->bk4tmgmem_bit.cmht = Timing->mem_hold_time;
 Device->bk4tmgmem_bit.cmst = Timing->mem_setup_time;
 Device->bk4tmgmem_bit.cmwt = Timing->mem_waite_time;

  return STATUS_OK;
}
/**
  * @brief  Init AttributeSpace Timing
  * @param  Device: pointer to a XMC_Bank4_Type structure
  * @param  Timing: pointer to a Attribute space timing structure
  * @retval HAL status
  */
StatusType XMC_PCCARD_InitAttributeSpaceTiming(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing)
{
  Device->bk4tmgatt_bit.amdhizt = Timing->mem_hiz_time;
  Device->bk4tmgatt_bit.amht = Timing->mem_hold_time;
  Device->bk4tmgatt_bit.amst = Timing->mem_setup_time;
  Device->bk4tmgatt_bit.amwt = Timing->mem_waite_time;
  return STATUS_OK;
}
/*****************************************************************/
/**
  * @brief  Init IOSpace Timing
  * @param  Device: pointer to a XMC_Bank4_Type structure
  * @param  Timing: pointer to a IO space timing structure
  * @retval HAL status
  */
StatusType XMC_PCCARD_InitIOSpaceTiming(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing)
{
 
  Device->bk4tmgio_bit.iohizt = Timing->mem_hiz_time;
  Device->bk4tmgio_bit.ioht = Timing->mem_hold_time;
  Device->bk4tmgio_bit.iost = Timing->mem_setup_time;
  Device->bk4tmgio_bit.iowt = Timing->mem_waite_time;

  return STATUS_OK;
}
/*****************************************************************/
/**
  * @brief  Perform the PCCARD memory Initialization sequence
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  ComSpaceTiming: Common space timing structure
  * @param  AttSpaceTiming: Attribute space timing structure
  * @param  IOSpaceTiming: IO space timing structure
  * @retval HAL status
  */
StatusType PCCARD_Init(PCCARD_HandleType *hpccard, xmc_nand_pccard_timinginit_type *ComSpaceTiming, xmc_nand_pccard_timinginit_type *AttSpaceTiming, xmc_nand_pccard_timinginit_type *IOSpaceTiming)
{
  gpio_init_type  GPIO_InitStructure;
  uint8_t Set_reg_8_bit;

  /* Check the PCCARD controller state */
  if(hpccard == NULL)
  {
    return STATUS_ERROR;
  }

 if(hpccard->CF.Enable_8_bit_mode == TRUE)
  Set_reg_8_bit = 1;
 else
	Set_reg_8_bit = 0;


  if(hpccard->State == PCCARD_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hpccard->Lock = UNLOCKED;

  }

  /* Initialize the PCCARD state */
  hpccard->State = PCCARD_STATE_BUSY;


  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK,TRUE);
  crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK,TRUE);
  /* address line:PF0 PF1 PF2 PF3 PF4 PF5 PF12 PF13 PF14 PF15 PD5 PG1 PG2 PG3 PG4 PG5 PD11 PD12 PD13 PE3 PE4 PE5 PE6 PE2 PG13 PG14 */
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE2, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE3, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE15, GPIO_MUX_12);
  
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_0|GPIO_PINS_1|GPIO_PINS_2|GPIO_PINS_3|GPIO_PINS_4|GPIO_PINS_5|GPIO_PINS_12|GPIO_PINS_13|GPIO_PINS_14|GPIO_PINS_15; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOF, &GPIO_InitStructure);
  
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE5, GPIO_MUX_10);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_11|GPIO_PINS_12|GPIO_PINS_13|GPIO_PINS_5; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &GPIO_InitStructure);

  /* data line:   PB14 PC6 PC11 PC12 PE7 PA3 PA4 PA5 PE11 PE12 PE13 PE14 PE15 PB12 PD9 PD10 */
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE14, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE12, GPIO_MUX_14);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_14|GPIO_PINS_12; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE6, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE11, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE12, GPIO_MUX_14);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_6|GPIO_PINS_11|GPIO_PINS_12; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE3, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE4, GPIO_MUX_14);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE5, GPIO_MUX_14);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_2|GPIO_PINS_3|GPIO_PINS_4|GPIO_PINS_5; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE7, GPIO_MUX_12); 
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE15, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_11|GPIO_PINS_12|GPIO_PINS_13|GPIO_PINS_14|GPIO_PINS_15|GPIO_PINS_7; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOE, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_9|GPIO_PINS_10; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &GPIO_InitStructure);
  /* XMC_NCE4_1/XMC_NCE4_2:PG10/PG11 XMC_NOE:PD4 XMC_NWE:PC2 XMC_NBL0:PE0 XMC_NBL1:PE1 XMC_NADV:PB7  XMC_NWAIT:PD6 */
  /* XMC_NIORD:PF6  XMC_NIOWR:PF8   XMC_NREG:PF7   XMC_INTR:PF10  XMC_CD:PF9  */
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE6, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_4|GPIO_PINS_6; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOD, &GPIO_InitStructure);
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE2, GPIO_MUX_14);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_2; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOC, &GPIO_InitStructure);

  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_10|GPIO_PINS_11; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOG, &GPIO_InitStructure);
  
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE6, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE7, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_default_para_init(&GPIO_InitStructure);
  GPIO_InitStructure.gpio_pins = GPIO_PINS_9|GPIO_PINS_6|GPIO_PINS_7|GPIO_PINS_8|GPIO_PINS_10; 
  GPIO_InitStructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  GPIO_InitStructure.gpio_mode = GPIO_MODE_MUX;
  GPIO_InitStructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  GPIO_InitStructure.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOF, &GPIO_InitStructure);

  /* Initialize PCCARD control Interface */
  XMC_PCCARD_InitCtrl(hpccard->Instance, &(hpccard->Init), Set_reg_8_bit);

  /* Init PCCARD common space timing Interface */
  XMC_PCCARD_InitCommonSpaceTiming(hpccard->Instance, ComSpaceTiming);

  /* Init PCCARD attribute space timing Interface */
  XMC_PCCARD_InitAttributeSpaceTiming(hpccard->Instance, AttSpaceTiming);

  /* Init PCCARD IO space timing Interface */
  XMC_PCCARD_InitIOSpaceTiming(hpccard->Instance, IOSpaceTiming);

  /* Enable the PCCARD device */
  xmc_pccard_enable(TRUE);

  /* Update the PCCARD state */
  hpccard->State = PCCARD_STATE_READY;

  return STATUS_OK;

}
/**
  * @brief  Enable Wait Feature
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval STATUS_OK or STATUS_ERROR
  */
StatusType Enable_Wait_Feature(PCCARD_HandleType *hpccard)
{
  /* Check the PCCARD controller state */
  if(hpccard == NULL)
  {
    return STATUS_ERROR;
  }

  if(hpccard->State == PCCARD_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hpccard->Lock = UNLOCKED;
  }

  /* Initialize the PCCARD state */
  hpccard->State = PCCARD_STATE_BUSY;

  XMC_Enable_Wait_Feature(hpccard->Instance, &(hpccard->Init));

  hpccard->State = PCCARD_STATE_READY;

  return STATUS_OK;
}

/**
  * @brief  Perform the PCCARD memory De-initialization sequence
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval HAL status
  */
StatusType  PCCARD_DeInit(PCCARD_HandleType *hpccard)
{

  /* Configure the PCCARD registers with their reset values */
  xmc_pccard_reset();

  /* Update the PCCARD controller state */
  hpccard->State = PCCARD_STATE_RESET;
  return STATUS_OK;
}


/**
  * @}
  */

/** @defgroup PCCARD_Exported_Functions_Group2 Input Output and memory functions
  * @brief    Input Output and memory control functions
  *
  @verbatim
  ==============================================================================
                ##### PCCARD Input Output and memory functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to use and control the PCCARD memory

@endverbatim
  * @{
  */
/**
  * @brief  Identify
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Identify(PCCARD_HandleType *hpccard)
{
  uint8_t Reg;

  Reg = hpccard->CF.CFAddr.Drv;

  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;
  if(!CF_SendCommand(hpccard, ATA_IDENTIFY_DRIVE_CMD))
  {
    return FALSE;
  }

  return TRUE;
}


/**
  * @brief  Read Compact Flash's ID.
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  CompactFlash_ID: Compact flash ID structure.
  * @retval HAL status
  *
  */
BOOL PCCARD_Read_ID(PCCARD_HandleType *hpccard)
{
  uint8_t CardInfo[PCCARD_SECTOR_SIZE * sizeof(uint8_t)] = {0};

  memset(CardInfo, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));
  PCCARD_Read_STATUS_REG(hpccard);

  if(PCCARD_Identify(hpccard))
  {
    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
        {
          /* Read CF ID bytes */

          Read_Sector(hpccard, CardInfo, PCCARD_SECTOR_SIZE);

          break;
        }
        else if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
        {
          printf("\r\nCommand Pass, but ERR bit is high");
          printf("\r\nCF Status Reg=%#x", PCCARD_Read_STATUS_REG(hpccard));
          printf("\r\nCF Error  Reg=%#x", PCCARD_Read_ERROR_REG(hpccard));
          return FALSE;
        }
      }
    }
  }

  while(1)
  {
    if(TaskFileRegIsValid(hpccard))
    {
      if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
      {
        break;
      }
      else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        printf("\r\nSend Command Failed");
        printf("\r\nCF Status Reg=%#x", PCCARD_Read_STATUS_REG(hpccard));
        printf("\r\nCF Error  Reg=%#x", PCCARD_Read_ERROR_REG(hpccard));
        return FALSE;
      }
    }
  }

#if 1

  printf("\r\n===============================================================\n");
  printf("\r\n\nIdentify Drive Information : ");
  printf("\r\n%s%#x", "Signature : ", ((uint16_t)CardInfo[0] | (uint16_t)CardInfo[1] << 8));
  printf("\r\n%s%u", "Default number of cylinders : ", ((uint16_t)CardInfo[2] | (uint16_t)CardInfo[3] << 8));
  printf("\r\n%s%u", "Default number of heads : ", ((uint16_t)CardInfo[6] | (uint16_t)CardInfo[7] << 8));
  printf("\r\n%s%u", "Number of unformatted bytes per tract : ", ((uint16_t)CardInfo[8] | (uint16_t)CardInfo[9] << 8));
  printf("\r\n%s%u", "Number of unformatted bytes per sector : ", ((uint16_t)CardInfo[10] | (uint16_t)CardInfo[11] << 8));
  printf("\r\n%s%u", "Default number of sectors per track : ", ((uint16_t)CardInfo[12] | (uint16_t)CardInfo[13] << 8));
  printf("\r\n%s%u", "Number of sectors per card : ", (uint32_t)((uint16_t)CardInfo[16] | (uint16_t)CardInfo[17] << 8) | ((uint32_t)((uint16_t)CardInfo[14] | (uint16_t)CardInfo[15] << 8) << 16));
  printf("\r\n%s%u", "Vendor Unique : ", ((uint16_t)CardInfo[18] | (uint16_t)CardInfo[19] << 8));
  printf("\r\n%s%#x", "Buffer type : ", ((uint16_t)CardInfo[40] | (uint16_t)CardInfo[41] << 8));
  printf("\r\n%s%u", "Buffer size in 512 byte increments : ", ((uint16_t)CardInfo[42] | (uint16_t)CardInfo[43] << 8));
  printf("\r\n%s%u", "#of ECC bytes passed on Read/Write Long Commands : ", ((uint16_t)CardInfo[44] | (uint16_t)CardInfo[45] << 8));
  printf("\r\n%s%u", "Maximum number of sectors on Read/Write Multiple command : ", ((uint16_t)CardInfo[94] | (uint16_t)CardInfo[95] << 8));
  printf("\r\n%s%u", "Double Word not supported : ", ((uint16_t)CardInfo[96] | (uint16_t)CardInfo[97] << 8));
  printf("\r\n%s%u", "Capabilities : ", ((uint16_t)CardInfo[98] | (uint16_t)CardInfo[99] << 8));
  printf("\r\n%s%u", "PIO data transfer cycle timing mode : ", ((uint16_t)CardInfo[102] | (uint16_t)CardInfo[103] << 8));
  printf("\r\n%s%u", "DMA data transfer cycle timing mode: ", ((uint16_t)CardInfo[104] | (uint16_t)CardInfo[105] << 8));
  printf("\r\n%s%u", "Translation parameters are valid : ", ((uint16_t)CardInfo[106] | (uint16_t)CardInfo[107] << 8));
  printf("\r\n%s%u", "Current numbers of cylinders : ", ((uint16_t)CardInfo[108] | (uint16_t)CardInfo[109] << 8));
  printf("\r\n%s%u", "Current numbers of heads : ", ((uint16_t)CardInfo[110] | (uint16_t)CardInfo[111] << 8));
  printf("\r\n%s%u", "Current sectors per track : ", ((uint16_t)CardInfo[112] | (uint16_t)CardInfo[113] << 8));
  printf("\r\n%s%u", "Current capacity in sectors : ", (uint32_t)((uint16_t)CardInfo[116] | (uint16_t)CardInfo[117] << 8) | ((uint32_t)((uint16_t)CardInfo[114] | (uint16_t)CardInfo[115] << 8) << 16));
  printf("\r\n%s%#x", "Multiple sector setting : ", ((uint16_t)CardInfo[118] | (uint16_t)CardInfo[119] << 8));
  printf("\r\n%s%u", "Total number of sectors addressable in LBA Mode : ", (uint32_t)((uint16_t)CardInfo[120] | (uint16_t)CardInfo[121] << 8) | ((uint32_t)((uint16_t)CardInfo[122] | (uint16_t)CardInfo[123] << 8) << 16));
  printf("\r\n%s%#x", "Security status : ",  ((uint16_t)CardInfo[256] | (uint16_t)CardInfo[257] << 8));
  printf("\r\n%s%#x", "Power requirement description : ",  ((uint16_t)CardInfo[320] | (uint16_t)CardInfo[321] << 8));
  printf("\r\n===============================================================");

#endif

  /* signature of CF storage */
  if(((uint16_t)CardInfo[0] | (uint16_t)CardInfo[1] << 8) == 0x848A)
  {
    hpccard->CF.CFCardInfo.Default_Cylinder = ((uint16_t)CardInfo[2] | (uint16_t)CardInfo[3] << 8);
    hpccard->CF.CFCardInfo.Default_Head = ((uint16_t)CardInfo[6] | (uint16_t)CardInfo[7] << 8);
    hpccard->CF.CFCardInfo.Default_Sector = ((uint16_t)CardInfo[12] | (uint16_t)CardInfo[13] << 8);
    hpccard->CF.CFCardInfo.Current_Cylinder = ((uint16_t)CardInfo[108] | (uint16_t)CardInfo[109] << 8);
    hpccard->CF.CFCardInfo.Current_Head = ((uint16_t)CardInfo[110] | (uint16_t)CardInfo[111] << 8);
    hpccard->CF.CFCardInfo.Current_Sector = ((uint16_t)CardInfo[112] | (uint16_t)CardInfo[113] << 8);

    hpccard->CF.CFCardInfo.Total_Sector = (uint32_t)((uint16_t)CardInfo[16] | (uint16_t)CardInfo[17] << 8) | ((uint32_t)((uint16_t)CardInfo[14] | (uint16_t)CardInfo[15] << 8) << 16);
    hpccard->CF.CFCardInfo.Total_LBA_Sector = (uint32_t)((uint16_t)CardInfo[120] | (uint16_t)CardInfo[121] << 8) | ((uint32_t)((uint16_t)CardInfo[122] | (uint16_t)CardInfo[123] << 8) << 16);
    hpccard->CF.CFCardInfo.Max_Mutiple_Sector = ((uint16_t)CardInfo[94] | (uint16_t)CardInfo[95] << 8);
    hpccard->CF.CFCardInfo.Mutiple_Sector_Setting = ((uint16_t)CardInfo[118] | (uint16_t)CardInfo[119] << 8);
  }
  else
  {

    return FALSE;
  }

  return TRUE;
}

/**
  * @brief  Read sector from PCCARD memory
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to destination read buffer
  * @param  Sector_Address: Sector address to read
  * @retval HAL status
  */
BOOL PCCARD_Read_Sector(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(Sector_Count == 0)
  {
    Sector_Count = 256;
  }

  while(Count < Sector_Count)
  {

    Translate_CHSAddr(hpccard, Sector_Address, Count, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = (uint8_t)hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_READ_SECTOR_CMD))
    {
      while(1)
      {
        if(TaskFileRegIsValid(hpccard))
        {
          if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            pBuffer = Read_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

            Sector_Address += 1;
            Count += 1;

            if (Count == Sector_Count)
            {
              break;
            }

            if (Count <= (Sector_Count / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector)
              if (Count % hpccard->CF.CFAddr.Sector_Count == 0)
              {
                break;
              }
          }
          else if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
										printf("Error Code=%X\n", PCCARD_Read_STATUS_REG(hpccard));
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}


/**
  * @brief  Write sector to PCCARD memory
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to source write buffer
  * @param  Sector_Address: Sector address to write
  * @retval HAL status
  */
BOOL PCCARD_Write_Sector(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(Sector_Count == 0)
  {
    Sector_Count = 256;
  }

  while(Count < Sector_Count)
  {
    // Set the parameters to write a sector //
    Translate_CHSAddr(hpccard, Sector_Address, Count, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    //*(__IO uint16_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = hpccard->CF.CFAddr.Cylinder;
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = (uint8_t)hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_WRITE_SECTOR_CMD))
    {
      while(1)
      {
        if(TaskFileRegIsValid(hpccard))
        {
          if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            pBuffer = Write_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

            Sector_Address += 1;
            Count += 1;

            if (Count == Sector_Count)
            {
              break;
            }

            if (Count <= (Sector_Count / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector)
              if (Count % hpccard->CF.CFAddr.Sector_Count == 0)
              {
                break;
              }
          }
          else if (PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
										printf("Error Code=%X\n", PCCARD_Read_STATUS_REG(hpccard));
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}


/**
  * @brief  Erase sector from PCCARD memory
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Address: Sector address to erase
  * @retval HAL status
  */
BOOL PCCARD_Erase_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address, uint32_t Sector_Count)
{
  uint8_t Reg;
  uint16_t Count = 0;

  while (Count < Sector_Count)
  {

    Translate_CHSAddr(hpccard, Sector_Address, 0, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_ERASE_SECTOR_CMD))
    {
      Count += hpccard->CF.CFAddr.Sector_Count;
      Sector_Address += hpccard->CF.CFAddr.Sector_Count;
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}
/**
  * @brief  Diagnostic
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Diagnostic(PCCARD_HandleType *hpccard)
{
  uint8_t ERROR_REG;

  /* Set the parameters */
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(!CF_SendCommand(hpccard, ATA_EXECUTE_DRIVE_DIAG_CMD))
  {
    return FALSE;
  }

  ERROR_REG = PCCARD_Read_ERROR_REG(hpccard);

  if((ERROR_REG == 0x01) || (ERROR_REG == 0x02) || (ERROR_REG == 0x03) || (ERROR_REG == 0x04) || (ERROR_REG == 0x05) || (ERROR_REG == 0x80))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  Check Power Mode
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Check_Power_Mode(PCCARD_HandleType *hpccard)
{
  uint8_t SECTOR_COUNT_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = 0x80;		
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(!CF_SendCommand(hpccard, ATA_CHECK_POWER_MODE_CMD))
  {
    return FALSE;
  }

  SECTOR_COUNT_REG =  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT);

  if(SECTOR_COUNT_REG == 0xFF || SECTOR_COUNT_REG == 0x00)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  Set idle 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Count: the sector count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Idle(PCCARD_HandleType *hpccard, uint32_t Sector_Count)
{
  uint8_t SECTOR_COUNT_REG;

  hpccard->CF.CFAddr.Sector_Count = Sector_Count;

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(!CF_SendCommand(hpccard, ATA_IDLE_CMD))
  {
    return FALSE;
  }


  /* Check Power Mode */
  if(!PCCARD_Check_Power_Mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  SECTOR_COUNT_REG =  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT);

  if(SECTOR_COUNT_REG == 0xFF)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  Idle immediate 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Idle_Immediate(PCCARD_HandleType *hpccard)
{
  uint8_t SECTOR_COUNT_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = 0x80;		
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(!CF_SendCommand(hpccard, ATA_IDLE_IMMEDIATE_CMD))
  {
    return FALSE;
  }

  /* Check Power Mode */
  if(!PCCARD_Check_Power_Mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  SECTOR_COUNT_REG =  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT);

  if(SECTOR_COUNT_REG == 0xFF)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  Set sleep mode 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Set_Sleep_Mode(PCCARD_HandleType *hpccard)
{
  uint8_t SECTOR_COUNT_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = 0x80;		
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(!CF_SendCommand(hpccard, ATA_SET_SLEEP_MODE_CMD))
  {
    return FALSE;
  }

  /* Check Power Mode */
  if(!PCCARD_Check_Power_Mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  SECTOR_COUNT_REG =  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT);

  if(SECTOR_COUNT_REG == 0x00)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  Set Standby 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Standby(PCCARD_HandleType *hpccard)
{
  uint8_t SECTOR_COUNT_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = 0x80;		
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(!CF_SendCommand(hpccard, ATA_STANDBY_CMD))
  {
    return FALSE;
  }

  /* Check Power Mode */
  if(!PCCARD_Check_Power_Mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  SECTOR_COUNT_REG =  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT);

  if(SECTOR_COUNT_REG == 0x00)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  Standby Immediate 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Standby_Immediate(PCCARD_HandleType *hpccard)
{
  uint8_t SECTOR_COUNT_REG;
  /* set sector_count register 0x80, after check_power_mode */
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = 0x80;		
  /* the register will be change to 0x00 or 0xFF */

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(!CF_SendCommand(hpccard, ATA_STANDBY_IMMEDIATE_CMD))
  {
    return FALSE;
  }

  /* Check Power Mode */
  if(!PCCARD_Check_Power_Mode(hpccard))
  {
    printf("Compact Flash Check Power Mode Failed \n");
    return FALSE;
  }

  SECTOR_COUNT_REG =  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT);

  if(SECTOR_COUNT_REG == 0x00)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  Reuqest Sense the pccard 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Reuqest_Sense(PCCARD_HandleType *hpccard)
{
  uint8_t Reg, ERROR_REG;

  Reg = hpccard->CF.CFAddr.Drv | 0xA0;

  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

  if(!CF_SendCommand(hpccard, ATA_REQUEST_SENSE_CMD))
  {
    return FALSE;
  }

  ERROR_REG = PCCARD_Read_ERROR_REG(hpccard);

  if((ERROR_REG == 0x2F) || (ERROR_REG == 0x21))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  Recalibrate the pccard 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Recalibrate(PCCARD_HandleType *hpccard)
{
  uint8_t Reg, ERROR_REG;

  Reg = hpccard->CF.CFAddr.Drv | 0xA0;

  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

  if(!CF_SendCommand(hpccard, ATA_RECALIBRATE_CMD))
  {
    return FALSE;
  }

  ERROR_REG = PCCARD_Read_ERROR_REG(hpccard);

  if((0x00 == ERROR_REG))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  Init drive para 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Count: the sector count 
  * @param  Head_Count: the head count.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Init_Drive_Para(PCCARD_HandleType *hpccard, uint16_t Sector_Count, uint8_t Head_Count)
{
  uint8_t Reg;

  hpccard->CF.CFAddr.Head = (uint8_t)Head_Count;
  hpccard->CF.CFAddr.Sector_Count = (uint8_t)Sector_Count;

  Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv;

  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

  if(!CF_SendCommand(hpccard, ATA_INIT_DRIVE_PARA_CMD))
  {
    return FALSE;
  }

  return TRUE;
}
/**
  * @brief  seek pccard 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Head_Count: the head count.
  * @param  Cylinder_Count: the cylinder count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Seek(PCCARD_HandleType *hpccard, uint8_t Head_Count, uint16_t Cylinder_Count)
{
  uint8_t Reg, ERROR_REG;

  hpccard->CF.CFAddr.Head = (uint8_t)Head_Count;
  hpccard->CF.CFAddr.Cylinder = (uint16_t)Cylinder_Count;

  Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);

  /* This command should return false */
  if(CF_SendCommand(hpccard, ATA_SEEK_CMD)) 
  {
    return FALSE;
  }

  ERROR_REG = PCCARD_Read_ERROR_REG(hpccard);

  if((ERROR_REG != 0x00))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  format track 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @param  Sector_Address: the sector addr 
  * @param  Sector_Count: the sector count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Format_Track(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count)
{
  uint16_t Count = 0;
  uint8_t  Reg;

  if(Sector_Count == 0)
  {
    Sector_Count = 256;
  }

  while(Count < Sector_Count)
  {
    /* Set the parameters to write a sector */
    Translate_CHSAddr(hpccard, Sector_Address, Count, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_FORMAT_TRACK_CMD))
    {
      while(1)
      {
        if(TaskFileRegIsValid(hpccard))
        {
          if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
           // Write_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

            Sector_Address += 1;
            Count += 1;

            if (Count == Sector_Count)
            {
              break;
            }

            if (Count <= (Sector_Count / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector)
              if (Count % hpccard->CF.CFAddr.Sector_Count == 0)
              {
                break;
              }
          }
          else if (PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}
/**
  * @brief  read Verify Sector
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Address: the sector addr 
  * @param  Sector_Count: the sector count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Read_Verify_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address, uint16_t Sector_Count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(Sector_Count == 0)
  {
    Sector_Count = 256;
  }

  while(Count < Sector_Count)
  {

    Translate_CHSAddr(hpccard, Sector_Address, Count, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_READ_VERIFY_SECTOR_CMD))
    {
      while(1)
      {
        if(TaskFileRegIsValid(hpccard))
        {
          if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT))
          {
            if(PCCARD_Read_STATUS_REG(hpccard) == DRQ_BIT)  //  DRQ_BIT should not be assert in this command
            {
              return FALSE;
            }
            else
            {
              Sector_Address += 1;
              Count += 1;

              if (Count == Sector_Count)
              {
                break;
              }

              if (Count <= (Sector_Count / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector)
                if (Count % hpccard->CF.CFAddr.Sector_Count == 0)
                {
                  break;
                }
            }
          }
          else if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}
/**
  * @brief  write Verify 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @param  Sector_Address: the sector addr 
  * @param  Sector_Count: the sector count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Write_Verify(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count)
{
  uint16_t Count = 0;
  uint8_t  Reg;

  if(Sector_Count == 0)
  {
    Sector_Count = 256;
  }

  while(Count < Sector_Count)
  {
    // Set the parameters to write a sector //
    Translate_CHSAddr(hpccard, Sector_Address, Count, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_WRITE_VERIFY_CMD))
    {
      while(1)
      {
        if(TaskFileRegIsValid(hpccard))
        {
          if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            Write_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

            Sector_Address += 1;
            Count += 1;

            if (Count == Sector_Count)
            {
              break;
            }

            if (Count <= (Sector_Count / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector)
              if (Count % hpccard->CF.CFAddr.Sector_Count == 0)
              {
                break;
              }
          }
          else if (PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}
/**
  * @brief  set multiple mode 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Count: the sector count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Set_Multiple_Mode(PCCARD_HandleType *hpccard, uint32_t Sector_Count)
{
  hpccard->CF.CFAddr.Sector_Count = Sector_Count;

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(!CF_SendCommand(hpccard, ATA_SET_MULTIPLE_MODE_CMD))
  {
    return FALSE;
  }

  return TRUE;
}
/**
  * @brief  set features 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Count: the sector count 
  * @param  Feature: Feature val
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Set_Features(PCCARD_HandleType *hpccard, uint32_t Sector_Count, uint8_t Feature)
{
  hpccard->CF.CFAddr.Sector_Count = Sector_Count;

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_FEATURE_REG)   = Feature;

  if(!CF_SendCommand(hpccard, ATA_SET_FEATURE_CMD))
  {
    return FALSE;
  }

  return TRUE;
}
/**
  * @brief  read buffer 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Read_Buffer(PCCARD_HandleType *hpccard, uint8_t *pBuffer)
{
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(CF_SendCommand(hpccard, ATA_READ_BUFFER_CMD))
  {
    if (TaskFileRegIsValid(hpccard))
    {
      if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
      {
        Read_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);
      }
      else if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
      {
        return FALSE;
      }
    }
  }

  while(1)
  {
    if(TaskFileRegIsValid(hpccard))
    {
      if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
      {
        break;
      }
      else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        return FALSE;
      }
    }
  }

  return TRUE;
}
/**
  * @brief  write buffer 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Write_Buffer(PCCARD_HandleType *hpccard, uint8_t *pBuffer)
{
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = hpccard->CF.CFAddr.Drv;

  if(CF_SendCommand(hpccard, ATA_WRITE_BUFFER_CMD))
  {
    if (TaskFileRegIsValid(hpccard))
    {
      if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
      {
        Write_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);
      }
      else if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
      {
        return FALSE;
      }
    }
  }

  while(1)
  {
    if(TaskFileRegIsValid(hpccard))
    {
      if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT))
      {
        break;
      }
      else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        return FALSE;
      }
    }
  }

  return TRUE;
}
/**
  * @brief  Translate sector 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Address: the sector addr 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Translate_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address)
{
  uint32_t Total_Cylinder;
  uint8_t Reg;

  Total_Cylinder = (uint32_t)(hpccard->CF.CFCardInfo.Default_Head * hpccard->CF.CFCardInfo.Default_Sector); // The total sectors in one cylinder

  hpccard->CF.CFAddr.Cylinder  = (uint16_t)(Sector_Address / Total_Cylinder);
  hpccard->CF.CFAddr.Head      = (uint8_t)((Sector_Address % Total_Cylinder) / hpccard->CF.CFCardInfo.Default_Sector);
  hpccard->CF.CFAddr.Sector    = (uint8_t)((Sector_Address % Total_Cylinder) % hpccard->CF.CFCardInfo.Default_Sector) + 1;

  /* Set the parameters to write a sector */
  Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

  if(!CF_SendCommand(hpccard, ATA_TRANSLATE_SECTOR_CMD))
  {
    return FALSE;
  }

  return TRUE;
}
/**
  * @brief  write sector WO_ERASE
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @param  Sector_Address: the sector addr 
  * @param  Sector_Count: the sector count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Write_Sector_WO_ERASE(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count)
{
  uint16_t Count = 0;
  uint8_t  Reg;

  if(Sector_Count == 0)
  {
    Sector_Count = 256;
  }

  while(Count < Sector_Count)
  {
    /* Set the parameters to write a sector */
    Translate_CHSAddr(hpccard, Sector_Address, Count, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_WRITE_SECTOR_WO_ERASE_CMD))
    {
      while(1)
      {
        if(TaskFileRegIsValid(hpccard))
        {
          if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            Write_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

            Sector_Address += 1;
            Count += 1;

            if (Count == Sector_Count)
            {
              break;
            }

            if (Count <= (Sector_Count / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector)
              if (Count % hpccard->CF.CFAddr.Sector_Count == 0)
              {
                break;
              }
          }
          else if (PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}
/**
  * @brief  write Multiple
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @param  Sector_Address: the sector addr 
  * @param  Sector_Count: the sector count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Write_Multiple(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count)
{
  uint16_t Count = 0;
  uint8_t  Reg;

  if(Sector_Count == 0)
  {
    Sector_Count = 256;
  }

  while(Count < Sector_Count)
  {
    /* Set the parameters to write a sector */
    Translate_CHSAddr(hpccard, Sector_Address, Count, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_WRITE_MULTIPLE_CMD))
    {
      while(1)
      {
        if(TaskFileRegIsValid(hpccard))
        {
          if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            Write_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

            Sector_Address += 1;
            Count += 1;

            if (Count == Sector_Count)
            {
              break;
            }

            if (Count <= (Sector_Count / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector)
              if (Count % hpccard->CF.CFAddr.Sector_Count == 0)
              {
                break;
              }
          }
          else if (PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}
/**
  * @brief  read Multiple 
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @param  Sector_Address: the sector addr 
  * @param  Sector_Count: the sector count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Read_Multiple(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(Sector_Count == 0)
  {
    Sector_Count = 256;
  }

  while(Count < Sector_Count)
  {

    Translate_CHSAddr(hpccard, Sector_Address, Count, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_READ_MULTIPLE_CMD))
    {
      while(1)
      {
        if(TaskFileRegIsValid(hpccard))
        {
          if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            Read_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

            Sector_Address += 1;
            Count += 1;

            if (Count == Sector_Count)
            {
              break;
            }

            if (Count <= (Sector_Count / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector)
              if (Count % hpccard->CF.CFAddr.Sector_Count == 0)
              {
                break;
              }
          }
          else if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}
/**
  * @brief  write Multiple WO_ERASE
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @param  Sector_Address: the sector addr 
  * @param  Sector_Count: the sector count 
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Write_Multiple_WO_ERASE(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count)
{
  uint16_t Count = 0;
  uint8_t Reg;

  if(Sector_Count == 0)
  {
    Sector_Count = 256;
  }

  while(Count < Sector_Count)
  {
    /* Set the parameters to write a sector */
    Translate_CHSAddr(hpccard, Sector_Address, Count, Sector_Count);
    Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
    *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

    if(CF_SendCommand(hpccard, ATA_WRITE_MULTIPLE_WO_ERASE_CMD))
    {
      while(1)
      {
        if(TaskFileRegIsValid(hpccard))
        {
          if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
          {
            Write_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

            Sector_Address += 1;
            Count += 1;

            if (Count == Sector_Count)
            {
              break;
            }

            if (Count <= (Sector_Count / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector)
              if (Count % hpccard->CF.CFAddr.Sector_Count == 0)
              {
                break;
              }
          }
          else if (PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
          {
            return FALSE;
          }
        }
      }
    }

    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT))
        {
          break;
        }
        else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
        {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}
/**
  * @brief  read long sector
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Address: the sector addr 
  * @param  pBuffer: pointer to the data buffer.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Read_Long_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address, uint8_t *pBuffer)
{
  uint32_t index = 0, Total_Cylinder;
  uint8_t Reg;
  BOOL command_vaild = TRUE;

  Total_Cylinder = (uint32_t)(hpccard->CF.CFCardInfo.Default_Head * hpccard->CF.CFCardInfo.Default_Sector); // The total sectors in one cylinder

  hpccard->CF.CFAddr.Cylinder  = (uint16_t)(Sector_Address / Total_Cylinder);
  hpccard->CF.CFAddr.Head      = (uint8_t)((Sector_Address % Total_Cylinder) / hpccard->CF.CFCardInfo.Default_Sector);
  hpccard->CF.CFAddr.Sector    = (uint8_t)((Sector_Address % Total_Cylinder) % hpccard->CF.CFCardInfo.Default_Sector) + 1;
  hpccard->CF.CFAddr.Sector_Count = 1;	//  some CF storage card implement the read long sector command as read sector commend
  //  set sector count to 1, after read 512 byte data, if the DRQ_BIT still high, means the
  //  read long sector command work well, because it shoult return 516 byte data (ECC).


  Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

  if(CF_SendCommand(hpccard, ATA_READ_LONG_SECTOR_CMD))
  {
    if(TaskFileRegIsValid(hpccard))
    {
      if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
      {
        Read_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

        if(PCCARD_Read_STATUS_REG(hpccard) & DRQ_BIT)
        {
          for(index = 0; index < 4; index++)
          {
            *(uint8_t *)pBuffer++ = *(uint8_t *)(hpccard->CF.IOAddr);
          }
        }
        else
        {
          command_vaild = FALSE;
        }

        if(PCCARD_Read_STATUS_REG(hpccard) & DRQ_BIT)
        {
          command_vaild = FALSE;
        }

        else if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
        {
          return FALSE;

        }
      }
    }
  }

  while(1)
  {
    if(TaskFileRegIsValid(hpccard))
    {
      if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
      {
        break;
      }
      else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        return FALSE;
      }
    }
  }

  if(command_vaild == TRUE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  write long sector
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Address: the sector addr 
  * @param  pBuffer: pointer to the data buffer.
  * @retval TRUE or FALSE.
  */
BOOL PCCARD_Write_Long_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address, uint8_t *pBuffer)
{
  uint32_t index = 0, Total_Cylinder;
  uint8_t Reg;
  BOOL command_vaild = TRUE;

  Total_Cylinder = (uint32_t)(hpccard->CF.CFCardInfo.Default_Head * hpccard->CF.CFCardInfo.Default_Sector); // The total sectors in one cylinder

  hpccard->CF.CFAddr.Cylinder  = (uint16_t)(Sector_Address / Total_Cylinder);
  hpccard->CF.CFAddr.Head      = (uint8_t)((Sector_Address % Total_Cylinder) / hpccard->CF.CFCardInfo.Default_Sector);
  hpccard->CF.CFAddr.Sector    = (uint8_t)((Sector_Address % Total_Cylinder) % hpccard->CF.CFCardInfo.Default_Sector) + 1;
  hpccard->CF.CFAddr.Sector_Count = 1;	//  some CF storage card implement the read long sector command as read sector commend
  //  set sector count to 1, after read 512 byte data, if the DRQ_BIT still high, means the
  //  read long sector command is work well, because it shoult return 516 byte data (ECC).


  Reg = hpccard->CF.CFAddr.Head | hpccard->CF.CFAddr.Drv | 0xA0;

  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_HIGH) = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder >> 8);
  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_CYLINDER_LOW)  = (uint8_t)(0xFF & hpccard->CF.CFAddr.Cylinder);
  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_NUMBER) = hpccard->CF.CFAddr.Sector;
  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_SECTOR_COUNT)  = hpccard->CF.CFAddr.Sector_Count;
  *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_CARD_HEAD)     = Reg;

  if(CF_SendCommand(hpccard, ATA_WRITE_LONG_SECTOR_CMD))
  {
    if(TaskFileRegIsValid(hpccard))
    {
      if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
      {
        Write_Sector(hpccard, pBuffer, PCCARD_SECTOR_SIZE);

        if(PCCARD_Read_STATUS_REG(hpccard) & DRQ_BIT)
        {
          for(index = 0; index < 4; index++)
          {
            *(uint8_t *)(hpccard->CF.IOAddr) = *(uint8_t *)pBuffer++;
          }
        }
        else
        {
          command_vaild = FALSE;
        }

        if(PCCARD_Read_STATUS_REG(hpccard) & DRQ_BIT)
        {
          command_vaild = FALSE;
        }

        else if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
        {
          return FALSE;
        }
      }
    }
  }

  while(1)
  {
    if(TaskFileRegIsValid(hpccard))
    {
      if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT))
      {
        break;
      }
      else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        return FALSE;
      }
    }
  }

  if(command_vaild == TRUE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/**
  * @brief  read sector
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @param  Transfer_Size: the data length.
  * @retval pointer to the data buffer after read.
  */
uint8_t *Read_Sector(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Transfer_Size)
{
  uint32_t index = 0;

  XMC_Enable_16bit_Feature(hpccard->Instance, &(hpccard->Init), hpccard->CF.Enable_8_bit_mode);

  if(hpccard->CF.DMAEnable == TRUE)
  {
    if(hpccard->CF.Enable_8_bit_mode)
    {
      MYDMA_Config(EDMA_STREAM1, (uint32_t)hpccard->CF.IOAddr, (uint32_t)pBuffer, Transfer_Size, Read_Transfer, Enable_8_bit_Transfer);
    }
    else
    {
      MYDMA_Config(EDMA_STREAM1, (uint32_t)hpccard->CF.IOAddr, (uint32_t)pBuffer, (Transfer_Size / 2), Read_Transfer, Enable_16_bit_Transfer);
    }

    while(1)
    {
      if(edma_flag_get(EDMA_FDT1_FLAG) != RESET)
      {
        edma_flag_clear(EDMA_FDT1_FLAG);
        break;
      }
    }

    pBuffer += 512;
  }
  else
  {
    if(hpccard->CF.Enable_8_bit_mode)
    {
      for(; index < (Transfer_Size) ; index++)
      {
        *(uint8_t *)pBuffer++ = *(uint8_t *)(hpccard->CF.IOAddr);
      }
    }
    else
    {
      for(; index < (Transfer_Size / 2); index++)
      {
        *(uint16_t *)pBuffer = *(uint16_t *)(hpccard->CF.IOAddr);
       /* offset to next 16 bit */
        pBuffer += 2;  
      }
    }
  }

  XMC_Enable_16bit_Feature(hpccard->Instance, &(hpccard->Init), 1);
  return pBuffer;
}
/**
  * @brief  write sector
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  pBuffer: pointer to the data buffer.
  * @param  Transfer_Size: the data length.
  * @retval pointer to the data buffer after write.
  */
uint8_t *Write_Sector(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Transfer_Size)
{
  uint32_t index = 0;

  XMC_Enable_16bit_Feature(hpccard->Instance, &(hpccard->Init), hpccard->CF.Enable_8_bit_mode);

  if(hpccard->CF.DMAEnable == TRUE)
  {
    if(hpccard->CF.Enable_8_bit_mode)
    {
      MYDMA_Config(EDMA_STREAM1, (uint32_t)pBuffer, (uint32_t)hpccard->CF.IOAddr, Transfer_Size, Write_Transfer, Enable_8_bit_Transfer);
    }
    else
    {
      MYDMA_Config(EDMA_STREAM1, (uint32_t)pBuffer, (uint32_t)hpccard->CF.IOAddr, (Transfer_Size / 2), Write_Transfer, Enable_16_bit_Transfer);
    }

    while(1)
    {
      if(edma_flag_get(EDMA_FDT1_FLAG) != RESET)	
      {
        edma_flag_clear(EDMA_FDT1_FLAG);
        break;
      }
    }

    pBuffer += 512;
  }
  else
  {
    if(hpccard->CF.Enable_8_bit_mode)
    {
      for(; index < (Transfer_Size); index++)
      {
        *(uint8_t *)(hpccard->CF.IOAddr) = *(uint8_t *)pBuffer++;
      }
    }
    else
    {
      for(; index < (Transfer_Size / 2); index++)
      {
        *(uint16_t *)(hpccard->CF.IOAddr) = *(uint16_t *)pBuffer;
        /* offset to next 16 bit */
        pBuffer += 2;  // offset to next 16 bit
      }
    }
  }

  XMC_Enable_16bit_Feature(hpccard->Instance, &(hpccard->Init), 1);
  return pBuffer;
}

/**
  * @brief  Reset the PCCARD memory
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval HAL status
  */
BOOL PCCARD_Reset(PCCARD_HandleType *hpccard)
{
  /* Provide an SW reset and Read and verify the:
   - CF Configuration Option Register at address 0x98000200 --> 0x80
   - Card Configuration and Status Register at address 0x98000202 --> 0x00
   - Pin Replacement Register  at address 0x98000204 --> 0x0C
   - Socket and Copy Register at address 0x98000206 --> 0x00
  */

  uint8_t config_option_reg;

  /* Set up the Reset bit in config option register */
  *(__IO uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | CONFIG_OPTION_REG) = 0x80;
  delay_ms(100);
  /* Clear the Reset bit in config option register */
  *(__IO uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | CONFIG_OPTION_REG) = 0x00;
  delay_ms(100);
  /* Set the PC card I/O configurations */
  *(__IO uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | CONFIG_OPTION_REG) = hpccard->CF.Protocol;
  delay_ms(100);

  config_option_reg = *(__IO uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | CONFIG_OPTION_REG);

  if(config_option_reg == hpccard->CF.Protocol)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
	* Translate_CHSAddr:
	*	translate DISK sector number to C.H.S address.
	*	input	: CFCard
	*			  Start (Start sector number)
	*			  Sector_Count (sector count which read from CF card)
	*			  Sector_Limit (maximum sector number)
	*	output	: None
	*/
void Translate_CHSAddr(PCCARD_HandleType *hpccard, uint32_t Start, uint16_t Sector_Count, uint16_t Sector_Limit)
{
  uint32_t Total_Cylinder;
  uint16_t Prev_Count;

  Prev_Count = (Sector_Limit / hpccard->CF.CFCardInfo.Default_Sector) * hpccard->CF.CFCardInfo.Default_Sector;

  Total_Cylinder = (uint32_t)(hpccard->CF.CFCardInfo.Default_Head * hpccard->CF.CFCardInfo.Default_Sector); // The total sectors in one cylinder

  hpccard->CF.CFAddr.Cylinder  = (uint16_t)(Start / Total_Cylinder);
  hpccard->CF.CFAddr.Head      = (uint8_t)((Start % Total_Cylinder) / hpccard->CF.CFCardInfo.Default_Sector);
  hpccard->CF.CFAddr.Sector    = (uint8_t)((Start % Total_Cylinder) % hpccard->CF.CFCardInfo.Default_Sector) + 1;

  if ((Prev_Count <= Sector_Count) && (Sector_Count <= Sector_Limit))
  {
    hpccard->CF.CFAddr.Sector_Count = (uint8_t)(Sector_Limit % hpccard->CF.CFCardInfo.Default_Sector);
  }
  else
  {
    hpccard->CF.CFAddr.Sector_Count = (uint8_t)hpccard->CF.CFCardInfo.Default_Sector;
  }
}

/**
  * @brief  Read CIS Information.
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL ReadCFCardCISInformation(PCCARD_HandleType *hpccard)
{
  uint8_t CIS_1, CIS_2, CIS_3;

  CIS_1 = *(uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | 0x0);
  CIS_2 = *(uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | 0x2);
  CIS_3 = *(uint8_t *)(PCCARD_ATTRIBUTE_SPACE_ADDRESS | 0x4);

  if(CIS_1 == 0x1 && CIS_2 == 0x4 && CIS_3 == 0xdf)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  Send command.
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Cmd: the command value
  * @retval TRUE or FALSE.
  */
BOOL CF_SendCommand(PCCARD_HandleType *hpccard, uint8_t Cmd)
{
  uint8_t Reg;

  do
  {
    *(__IO uint8_t  *)(hpccard->CF.IOAddr | ATA_STATUS_CMD) = Cmd;

    while(!TaskFileRegIsValid(hpccard)) ;

    Reg = PCCARD_Read_STATUS_REG(hpccard);

    if ((Reg & ERR_BIT) != 0)
    {
      return FALSE;
    }
  }
  while((Reg & RDY_BIT) == 0);

  return TRUE;
}
/**
  * @brief  Check the PCCARD TaskFileReg valid.
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval TRUE or FALSE.
  */
BOOL TaskFileRegIsValid(PCCARD_HandleType *hpccard)
{
  uint8_t status = 0;

  status = PCCARD_Read_STATUS_REG(hpccard);

  if(status & BUSY_BIT)
  {
    return FALSE;
  }

  return TRUE;
}
/**
  * @brief  Check the PCCARD info valid.
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @param  Sector_Count: sector count
  * @param  Head_Count:head count
  * @retval TRUE or FALSE.
  */
BOOL Vaild_Init_Drive_Para(PCCARD_HandleType *hpccard, uint16_t Sector_Count, uint8_t Head_Count)
{
  uint8_t CardInfo[PCCARD_SECTOR_SIZE * sizeof(uint8_t)] = {0};

  memset(CardInfo, 0, PCCARD_SECTOR_SIZE * sizeof(uint8_t));

  if(PCCARD_Identify(hpccard))
  {
    while(1)
    {
      if(TaskFileRegIsValid(hpccard))
      {
        if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | DRQ_BIT))
        {
          Read_Sector(hpccard, CardInfo, PCCARD_SECTOR_SIZE);
          break;
        }
        else if(PCCARD_Read_STATUS_REG(hpccard) == (RDY_BIT | DSC_BIT | ERR_BIT))
        {
          printf("\r\nCommand Pass, but ERR bit is high");
          printf("\r\nCF Status Reg=%#x", PCCARD_Read_STATUS_REG(hpccard));
          printf("\r\nCF Error  Reg=%#x", PCCARD_Read_ERROR_REG(hpccard));
          return FALSE;
        }
      }
    }
  }

  while(1)
  {
    if(TaskFileRegIsValid(hpccard))
    {
      if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x50)
      {
        break;
      }
      else if((PCCARD_Read_STATUS_REG(hpccard) & (RDY_BIT | DSC_BIT | ERR_BIT)) == 0x51)
      {
        printf("\r\nSend Command Failed");
        printf("\r\nCF Status Reg=%#x", PCCARD_Read_STATUS_REG(hpccard));
        printf("\r\nCF Error  Reg=%#x", PCCARD_Read_ERROR_REG(hpccard));
        return FALSE;
      }
    }
  }

  if (((uint16_t)CardInfo[0] | (uint16_t)CardInfo[1] << 8) == 0x848A && ((uint16_t)CardInfo[110] | \
      (uint16_t)CardInfo[111] << 8) ==  Head_Count && ((uint16_t)CardInfo[112] | (uint16_t)CardInfo[113] << 8) ==  Sector_Count )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }

}
/**
  * @brief  printf the verification result.
  * @param  result: pointer to a verification_result structure that contains
  *                various kinds of verification.
  * @retval None.
  */
void show_verification_result(verification_result_struct *result)
{
  printf("\r\n");
  printf("\r\n----------------------------------------------------------------------");
  printf("\r\n%s    %-6s   ", "ATTRIBUTE_RW_PASS      ", ((result->ATTRIBUTE_RW_PASS) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "READ_ATTRIBUTE_CIS     ", ((result->CIS_READ_PASS) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "IDENTIFY_DRIVE         ", ((result->ATA_IDENTIFY_DRIVE_PASS) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "EXECUTE_DRIVE_DIAG     ", ((result->ATA_EXECUTE_DRIVE_DIAG_PASS) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "WRITE_SECTOR           ", ((result->ATA_WRITE_SECTOR_PASS) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "READ_SECTOR            ", ((result->ATA_READ_SECTOR_PASS) ? " PASS" : "FAILED *****"));
  printf("\r\n%s    %-6s   ", "ACESS_25_SECTORS       ", ((result->ACESS_25_SECTORS_PASS) ? " PASS" : "FAILED *****"));
  printf("\r\n----------------------------------------------------------------------");
  printf("\r\n\r\n");
}


/**
  * @brief  This function handles PCCARD device interrupt request.
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval HAL status
  */
void PCCARD_IRQHandler(PCCARD_HandleType *hpccard)
{
}


/**
  * @}
  */

/** @defgroup PCCARD_Exported_Functions_Group3 Peripheral State functions
 *  @brief   Peripheral State functions
 *
@verbatim
  ==============================================================================
                   ##### PCCARD Peripheral State functions #####
  ==============================================================================
  [..]
    This subsection permits to get in run-time the status of the PCCARD controller
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  return the PCCARD controller state
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval HAL state
  */
PCCARD_StateType PCCARD_GetState(PCCARD_HandleType *hpccard)
{
  return hpccard->State;
}
/**
  * @brief  Get the PCCARD error reg value.
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval reg: error reg value.
  */
uint8_t PCCARD_Read_ERROR_REG(PCCARD_HandleType *hpccard)
{
  uint8_t Reg = 0;

  /* Read ERROR Reg operation */
  Reg =  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_ERROR_REG);
  CF_print(ERR_REG_INFO, ("\r\nCF card ERROR Register value is %#x", Reg));
  return Reg;
}
/**
  * @brief  Get the PCCARD status reg value.
  * @param  hpccard: pointer to a PCCARD_HandleType structure that contains
  *                the configuration information for PCCARD module.
  * @retval reg: status reg value.
  */
uint8_t PCCARD_Read_STATUS_REG(PCCARD_HandleType *hpccard)
{
  uint8_t Reg = 0;

  /* Read STATUS Reg operation */
  Reg =  *(__IO uint8_t *)(hpccard->CF.IOAddr | ATA_STATUS_CMD);
  CF_print(STS_REG_INFO, ("\r\nCF card STATUS Register value is %#x", Reg));

  return Reg;
}


void XMC_IRQHandler(void)
{
  __nop();
}

/**
  * @}
  */

/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */












