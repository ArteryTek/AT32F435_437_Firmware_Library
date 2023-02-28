/**
  ******************************************************************************
  * @file     cf.h
  * @brief    header file for the cf configuration.
  ******************************************************************************
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
#ifndef __CF_H
#define __CF_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "at32f435_437.h"
/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup XMC_CF_CARD
  * @{
  */
#ifdef __cplusplus
extern "C" {
#endif


    /* Exported typedef ----------------------------------------------------------*/
  /** @defgroup PCCARD_Exported_Constant PCCARD Exported Constant
    * @{
    */
#define FALSE 	0
#define TRUE	1
#define XMC_BASE             			((uint32_t)0xA8000000) /*!< XMC base address */
#define XMC_BANK4_            			(XMC_BASE + 0x0000000)  /*!< XMC Bank4 base address */
#define PCCARD_DEVICE_ADDRESS           XMC_BANK4_  /* 0x9000 0000 */
#define PCCARD_COMMON_SPACE_ADDRESS     PCCARD_DEVICE_ADDRESS                  /* Common space size to @0x93FF FFFF    */
#define PCCARD_ATTRIBUTE_SPACE_ADDRESS  ((uint32_t)(XMC_BANK4_ + 0x4000000))  /* Attribute space size to @0x9BFF FFFF */
#define PCCARD_IO_SPACE_ADDRESS         ((uint32_t)(XMC_BANK4_ + 0x6000000))  /* IO space size to @0x9FFF FFFF        */

// In XMC document page 545/1128, the XMC is not supported True IDE mode. //
#define PCCARD_IO_SPACE_PRIMARY_ADDR    ((uint32_t)(XMC_BANK4_ + PCCARD_IO_SPACE_ADDRESS + 0x1F0))  /* IO space size to @0x9FFF FFFF        */
#define PCCARD_IO_SPACE_SECONDARY_ADDR 	((uint32_t)(XMC_BANK4_ + PCCARD_IO_SPACE_ADDRESS + 0x170))  /* IO space size to @0x9FFF FFFF        */

  /* Compact Flash-ATA registers description */
#define ATA_DATA                        ((uint8_t)0x00)    /* Data register */
#define ATA_ERROR_REG                   ((uint8_t)0x01)    /* Error register */
#define ATA_FEATURE_REG                 ((uint8_t)0x01)    /* Feature register */
#define ATA_SECTOR_COUNT                ((uint8_t)0x02)    /* Sector Count register */
#define ATA_SECTOR_NUMBER               ((uint8_t)0x03)    /* Sector Number register */
#define ATA_CYLINDER_LOW                ((uint8_t)0x04)    /* Cylinder low register */
#define ATA_CYLINDER_HIGH               ((uint8_t)0x05)    /* Cylinder high register */
#define ATA_CARD_HEAD                   ((uint8_t)0x06)    /* Card/Head register */
#define ATA_STATUS_CMD                  ((uint8_t)0x07)    /* Status(read)/Command(write) register */
#define ATA_STATUS_CMD_ALTERNATE        ((uint8_t)0x0E)    /* Alternate Status(read)/Command(write) register */
#define ATA_COMMON_DATA_AREA            ((uint16_t)0x0400) /* Start of data area (for Common access only!) */

// CFA spec. page 19
// The configuration of the CompactFlash Card will be controlled using the standard
// PCMCIA configuration registers starting at address 200h in the Attribute Memory space
  /* attribute memory space register description */
#define	ATTRIBUTE_MEM_BASE							0
#define	ATTRIBUTE_MEM_CONFIG_BASE				0x200
#define	CONFIG_OPTION_REG								ATTRIBUTE_MEM_CONFIG_BASE + 0x00
#define	CARD_CONFIG_STATUS_REG							ATTRIBUTE_MEM_CONFIG_BASE + 0x02
#define	PIN_REPLACE_REG									ATTRIBUTE_MEM_CONFIG_BASE + 0x04
#define	SOCKET_COPY_REG									ATTRIBUTE_MEM_CONFIG_BASE + 0x06
// #define ATA_CARD_CONFIGURATION          ((uint16_t)0x0202) /* Card Configuration and Status Register */

  /* configuration option register */
#define	CONF0_BIT						0x01
#define	CONF1_BIT						0x02
#define	CONF2_BIT						0x04
#define	CONF3_BIT						0x08
#define	CONF4_BIT						0x10
#define	CONF5_BIT						0x20
#define	LEVLREQ_BIT				0x40
#define	SRESET_BIT					0x80

#define	CF_MEM_MAP_MODE			0x00
#define	CF_IO_16_MODE				 0x01
#define	CF_IO_PRICH_MODE		0x02	// the XMC is not supported True IDE mode.
#define	CF_IO_SECCH_MODE		0x03	// the XMC is not supported True IDE mode.

  /* Compact Flash-ATA commands */
#define	ATA_CHECK_POWER_MODE_CMD 					((uint8_t)0xE5)
#define ATA_EXECUTE_DRIVE_DIAG_CMD   	   	((uint8_t)0x90)
#define ATA_ERASE_SECTOR_CMD         	   	((uint8_t)0xC0)
#define	ATA_FORMAT_TRACK_CMD							((uint8_t)0x50)
#define ATA_IDENTIFY_DRIVE_CMD         	 	((uint8_t)0xEC)
#define	ATA_IDLE_CMD											((uint8_t)0xE3)
#define	ATA_IDLE_IMMEDIATE_CMD						((uint8_t)0xE1)
#define	ATA_INIT_DRIVE_PARA_CMD						((uint8_t)0x91)
#define	ATA_READ_BUFFER_CMD								((uint8_t)0xE4)
#define	ATA_READ_LONG_SECTOR_CMD					((uint8_t)0x22)
#define	ATA_READ_MULTIPLE_CMD							((uint8_t)0xC4)
#define ATA_READ_SECTOR_CMD           	 	((uint8_t)0x20)
#define	ATA_READ_VERIFY_SECTOR_CMD				((uint8_t)0x40)
#define	ATA_RECALIBRATE_CMD								((uint8_t)0x10)
#define	ATA_REQUEST_SENSE_CMD							((uint8_t)0x03)
#define	ATA_SECURITY_DISABLE_PASSWORD_CMD	((uint8_t)0xF6)
#define	ATA_SECURITY_EREASE_PREPARE_CMD		((uint8_t)0xF3)
#define	ATA_SECURITY_ERASE_UNIT_CMD				((uint8_t)0xF4)
#define	ATA_SECURITY_FREEZE_LOCK_CMD			((uint8_t)0xF5)
#define	ATA_SECURITY_SET_PASSWORD_CMD			((uint8_t)0xF1)
#define	ATA_SECURITY_UNLOCK_CMD						((uint8_t)0xF2)
#define	ATA_SEEK_CMD											((uint8_t)0x70)
#define	ATA_SET_FEATURE_CMD								((uint8_t)0xEF)
#define	ATA_SET_MULTIPLE_MODE_CMD					((uint8_t)0xC6)
#define	ATA_SET_SLEEP_MODE_CMD						((uint8_t)0xE6)
#define	ATA_STANDBY_CMD										((uint8_t)0xE2)
#define	ATA_STANDBY_IMMEDIATE_CMD					((uint8_t)0xE0)
#define	ATA_TRANSLATE_SECTOR_CMD					((uint8_t)0x87)
#define	ATA_WEAR_LEVEL_CMD								((uint8_t)0xF5)
#define	ATA_WRITE_BUFFER_CMD							((uint8_t)0xE8)
#define	ATA_WRITE_LONG_SECTOR_CMD					((uint8_t)0x32)
#define	ATA_WRITE_MULTIPLE_CMD						((uint8_t)0xC5)
#define	ATA_WRITE_MULTIPLE_WO_ERASE_CMD		((uint8_t)0xCD)
#define ATA_WRITE_SECTOR_CMD            	((uint8_t)0x30)
#define	ATA_WRITE_SECTOR_WO_ERASE_CMD			((uint8_t)0x38)
#define	ATA_WRITE_VERIFY_CMD							((uint8_t)0x3C)

  /* feature command */
#define	ENABLE_8BIT_MODE							0x01
#define	ENABLE_PWR_LV1								0x0a
#define	DISABLE_READ_LOOK_AHEAD				0x55
#define	DISABLE_POR										0x66
#define	NOP														0x69
#define	DISABLE_8BIT_MODE							0x81
#define	DISABLE_PWR_LV1								0x8a
#define	SET_HOST_CUR_SOURCE_CAP				0x9a
#define	APPLY_4BYTE_DATA_RW_LONG_CMD	0xbb
#define	ENABLE_POR										0xcc

  /* status register */
#define	BUSY_BIT				0x80
#define	RDY_BIT					0x40
#define	DWF_BIT					0x20
#define	DSC_BIT					0x10
#define DRQ_BIT					0x08
#define	CORR_BIT				0x04
#define	ERR_BIT					0x01

  /* error register */
#define	BBK_BIT					0x80
#define	UNC_BIT					0x40
#define	IDNF_BIT				0x10
#define	ABRT_BIT				0x04
#define	AMNF_BIT				0x01

  /* Compact Flash status */
#define PCCARD_TIMEOUT_ERROR            ((uint8_t)0x60)
#define PCCARD_BUSY                     ((uint8_t)0x80)
#define PCCARD_PROGR                    ((uint8_t)0x01)
#define PCCARD_READY                    ((uint8_t)0x40)

//#define PCCARD_SECTOR_SIZE              ((uint32_t)255)    	/* In half words */
#define PCCARD_SECTOR_SIZE             	((uint32_t)512)
#define PCCARD_LONG_SECTOR_SIZE        	((uint32_t)516)
#define PCCARD_25_SECTORS_SIZES			((uint32_t)(512*25))
  /* CF CIS information */
#define	CISTPL_DEVICE					0x01
#define	CISTPL_DEVICE_OC				0x1c
#define	CISTPL_JEDEC_C					0x18
#define	CISTPL_MANFID					0x20
#define	CISTPL_VERS_1					0x15
#define	CISTPL_FUNCID					0x21
#define	CISTPL_FUNCE					0x22
#define	CISTPL_CONFIG					0x1a
#define	CISTPL_CFTABLE_ENTRY			0x1b
#define	CISTPL_NO_LINK					0x14
#define	CISTPL_END						0xff
#define	TPL_LINK_LOC					16
#define MAX_CIS_LEN						0x200

  /* Compact Flash redefinition */

#define CF_DEVICE_ADDRESS           PCCARD_DEVICE_ADDRESS
#define CF_ATTRIBUTE_SPACE_ADDRESS  PCCARD_ATTRIBUTE_SPACE_ADDRESS
#define CF_COMMON_SPACE_ADDRESS     PCCARD_COMMON_SPACE_ADDRESS
#define CF_IO_SPACE_ADDRESS         PCCARD_IO_SPACE_ADDRESS
#define CF_IO_SPACE_PRIMARY_ADDR    PCCARD_IO_SPACE_PRIMARY_ADDR

#define CF_TIMEOUT_ERROR            PCCARD_TIMEOUT_ERROR
#define CF_BUSY                     PCCARD_BUSY
#define CF_PROGR                    PCCARD_PROGR
#define CF_READY                    PCCARD_READY

#define CF_SECTOR_SIZE              PCCARD_SECTOR_SIZE
#define Read_Transfer				1
#define Write_Transfer				0
#define Enable_8_bit_Transfer				1
#define Enable_16_bit_Transfer				0

#define CF_print( cond, _x_) \
	if ( cond ) { \
		printf _x_ ; \
	}

  /* For print message level */
#define ERR_REG_INFO	0
#define STS_REG_INFO	0
#define PRT_INIT_INFO	1
#define PRT_CMD_INFO	0
#define PRT_ERR_INFO	0

  /**
    * @}
    */

  /* Exported typedef ----------------------------------------------------------*/
  /** @defgroup PCCARD_Exported_Types PCCARD Exported Types
    * @{
    */

  typedef unsigned char	BOOL;
  typedef enum
  {
    UNLOCKED = 0x00,
    LOCKED   = 0x01
  } LockType;
  typedef enum
  {
    STATUS_OK       = 0x00,
    STATUS_ERROR    = 0x01,
  } StatusType;

  /**
    * @brief  HAL PCCARD State structures definition
    */
  typedef enum
  {
    PCCARD_STATE_RESET     = 0x00,    /*!< PCCARD peripheral not yet initialized or disabled */
    PCCARD_STATE_READY     = 0x01,    /*!< PCCARD peripheral ready                           */
    PCCARD_STATE_BUSY      = 0x02,    /*!< PCCARD peripheral busy                            */
    PCCARD_STATE_ERROR     = 0x04     /*!< PCCARD peripheral error                           */
  } PCCARD_StateType;


  /* CF type */
  typedef	struct
  {
    uint16_t Default_Cylinder;
    uint8_t  Default_Head;
    uint16_t Default_Sector;
    uint16_t Current_Cylinder;
    uint8_t  Current_Head;
    uint16_t Current_Sector;
    uint32_t Total_Sector;
    uint32_t Total_LBA_Sector;
    uint16_t Max_Mutiple_Sector;
    uint16_t Mutiple_Sector_Setting;
  } CFCardInfoStruct;

  /* CF address struct */
  typedef struct
  {
    uint16_t	Sector_Count;
    uint16_t	Cylinder;
    uint8_t		Head;
    uint8_t		Sector;
    uint8_t		Drv;
    uint32_t	SectorSize;
  } CFCardAddrStruct;

  /* CF card structure */
  typedef	struct
  {
    uint32_t IOAddr;
    BOOL DMAEnable;
    BOOL Enable_8_bit_mode;
    CFCardInfoStruct CFCardInfo;
    CFCardAddrStruct CFAddr;
    uint16_t	Protocol;

    /* Drive Name */
    uint32_t Drive;

  } CFCardStruct;

  /* ATA-Command verification result */
  typedef	struct
  {
    BOOL ATA_CHECK_POWER_MODE_PASS;
    BOOL ATA_EXECUTE_DRIVE_DIAG_PASS;
    BOOL ATA_ERASE_SECTOR_PASS;
    BOOL ATA_FORMAT_TRACK_PASS;
    BOOL ATA_IDENTIFY_DRIVE_PASS;
    BOOL ATA_IDLE_PASS;
    BOOL ATA_IDLE_IMMEDIATE_PASS;
    BOOL ATA_INIT_DRIVE_PARA_PASS;
    BOOL ATA_READ_BUFFER_PASS;
    BOOL ATA_READ_LONG_SECTOR_PASS;
    BOOL ATA_READ_MULTIPLE_PASS;
    BOOL ATA_READ_SECTOR_PASS;
    BOOL ATA_READ_VERIFY_SECTOR_PASS;
    BOOL ATA_RECALIBRATE_PASS;
    BOOL ATA_REQUEST_SENSE_PASS;
    BOOL ATA_SECURITY_DISABLE_PASSWORD_PASS;
    BOOL ATA_SECURITY_EREASE_PREPARE_PASS;
    BOOL ATA_SECURITY_ERASE_UNIT_PASS;
    BOOL ATA_SECURITY_FREEZE_LOCK_PASS;
    BOOL ATA_SECURITY_SET_PASSWORD_PASS;
    BOOL ATA_SECURITY_UNLOCK_PASS;
    BOOL ATA_SEEK_PASS;
    BOOL ATA_SET_FEATURE_PASS;
    BOOL ATA_SET_MULTIPLE_MODE_PASS;
    BOOL ATA_SET_SLEEP_MODE_PASS;
    BOOL ATA_STANDBY_PASS;
    BOOL ATA_STANDBY_IMMEDIATE_PASS;
    BOOL ATA_TRANSLATE_SECTOR_PASS;
    BOOL ATA_WEAR_LEVEL_PASS;
    BOOL ATA_WRITE_BUFFER_PASS;
    BOOL ATA_WRITE_LONG_SECTOR_PASS;
    BOOL ATA_WRITE_MULTIPLE_PASS;
    BOOL ATA_WRITE_MULTIPLE_WO_ERASE_PASS;
    BOOL ATA_WRITE_SECTOR_PASS;
    BOOL ATA_WRITE_SECTOR_WO_ERASE_PASS;
    BOOL ATA_WRITE_VERIFY_PASS;
    BOOL ACESS_25_SECTORS_PASS;
    BOOL CIS_READ_PASS;
    BOOL ATTRIBUTE_RW_PASS;

  } verification_result_struct;

  /**
    * @brief  XMC_PCCARD handle Structure definition
    */
  typedef struct
  {
    xmc_bank4_type           *Instance;              /*!< Register base address for PCCARD device          */

    xmc_pccard_init_type       Init;                   /*!< PCCARD device control configuration parameters   */

    __IO PCCARD_StateType 	State;                  /*!< PCCARD device access state                       */

    LockType              	Lock;                   /*!< PCCARD Lock                                      */

    CFCardStruct					CF;

  } PCCARD_HandleType;
  /**
    * @}
    */

  /* Exported constants --------------------------------------------------------*/
  /* Exported macro ------------------------------------------------------------*/
  /** @defgroup PCCARD_Exported_Macros PCCARD Exported Macros
    * @{
    */

  /** @brief Reset PCCARD handle state
    * @param  __HANDLE__: specifies the PCCARD handle.
    * @retval None
    */
#define __PCCARD_Reset_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = PCCARD_STATE_RESET)
  /**
    * @}
    */

  /* Exported functions --------------------------------------------------------*/
  /** @addtogroup PCCARD_Exported_Functions PCCARD Exported Functions
    * @{
    */

  /** @addtogroup PCCARD_Exported_Functions_Group1 Initialization and de-initialization functions
    * @{
    */
  /* Initialization/de-initialization functions  **********************************/
  StatusType  PCCARD_Init(PCCARD_HandleType *hpccard, xmc_nand_pccard_timinginit_type *ComSpaceTiming, xmc_nand_pccard_timinginit_type *AttSpaceTiming, xmc_nand_pccard_timinginit_type *IOSpaceTiming);
  StatusType  PCCARD_DeInit(PCCARD_HandleType *hpccard);
  StatusType  Enable_Wait_Feature(PCCARD_HandleType *hpccard);
  /**
    * @}
    */

  /** @addtogroup PCCARD_Exported_Functions_Group2 Input Output and memory functions
    * @{
    */
  /* IO operation functions  *****************************************************/

  BOOL PCCARD_Read_ID(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Identify(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Write_Sector(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count);
  BOOL PCCARD_Read_Sector(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count);
  BOOL PCCARD_Erase_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address, uint32_t Sector_Count);
  BOOL PCCARD_Diagnostic(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Reset(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Check_Power_Mode(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Idle(PCCARD_HandleType *hpccard, uint32_t Sector_Count);
  BOOL PCCARD_Idle_Immediate(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Set_Sleep_Mode(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Standby(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Standby_Immediate(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Reuqest_Sense(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Read_Buffer(PCCARD_HandleType *hpccard, uint8_t *pBuffer);
  BOOL PCCARD_Write_Buffer(PCCARD_HandleType *hpccard, uint8_t *pBuffer);
  BOOL PCCARD_Recalibrate(PCCARD_HandleType *hpccard);
  BOOL PCCARD_Init_Drive_Para(PCCARD_HandleType *hpccard, uint16_t Sector_Count, uint8_t Head_Count);
  BOOL PCCARD_Seek(PCCARD_HandleType *hpccard, uint8_t Head_Count, uint16_t Cylinder_Count);
  BOOL PCCARD_Translate_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address);
  BOOL PCCARD_Read_Long_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address, uint8_t *pBuffer);
  BOOL PCCARD_Write_Long_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address, uint8_t *pBuffer);
  BOOL PCCARD_Write_Sector_WO_ERASE(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count);
  BOOL PCCARD_Format_Track(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count);
  BOOL PCCARD_Read_Verify_Sector(PCCARD_HandleType *hpccard, uint32_t Sector_Address, uint16_t Sector_Count);
  BOOL PCCARD_Write_Verify(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count);
  BOOL PCCARD_Set_Multiple_Mode(PCCARD_HandleType *hpccard, uint32_t Sector_Count);
  BOOL PCCARD_Set_Features(PCCARD_HandleType *hpccard, uint32_t Sector_Count, uint8_t Feature);
  BOOL PCCARD_Write_Multiple(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count);
  BOOL PCCARD_Read_Multiple(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count);
  BOOL PCCARD_Write_Multiple_WO_ERASE(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Sector_Address, uint32_t Sector_Count);

  BOOL ReadCFCardCISInformation(PCCARD_HandleType *hpccard);
  BOOL CF_SendCommand(PCCARD_HandleType *hpccard, uint8_t Cmd);
  BOOL TaskFileRegIsValid(PCCARD_HandleType *hpccard);
  BOOL Vaild_Init_Drive_Para(PCCARD_HandleType *hpccard, uint16_t Sector_Count, uint8_t Head_Count);

  void Translate_CHSAddr(PCCARD_HandleType *hpccard, uint32_t Sector_Address, uint16_t Sector_Count, uint16_t Sector_Limit);
  void PCCARD_IRQHandler(PCCARD_HandleType *hpccard);
  void PCCARD_ITCallback(PCCARD_HandleType *hpccard);
  void show_verification_result(verification_result_struct *result);
  uint8_t *Read_Sector(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Transfer_Size);
  uint8_t *Write_Sector(PCCARD_HandleType *hpccard, uint8_t *pBuffer, uint32_t Transfer_Size);

  /**
    * @}
    */

  /** @defgroup PCCARD_Exported_Functions_Group3 Peripheral State functions
    * @{
    */
  /* PCCARD State functions *******************************************************/
  PCCARD_StateType  PCCARD_GetState(PCCARD_HandleType *hpccard);
  uint8_t PCCARD_Read_ERROR_REG(PCCARD_HandleType *hpccard);
  uint8_t PCCARD_Read_STATUS_REG(PCCARD_HandleType *hpccard);


#define __XMC_PCCARD_GET_FLAG(__INSTANCE__, __FLAG__)  (((__INSTANCE__)->BK4STS &(__FLAG__)) == (__FLAG__))
#define __XMC_PCCARD_CLEAR_FLAG(__INSTANCE__, __FLAG__)  BIT_CLEAR((__INSTANCE__)->BK4STS, (__FLAG__))

  StatusType XMC_PCCARD_InitCtrl(xmc_bank4_type *Device, xmc_pccard_init_type *Init, uint8_t Set_reg_8_bit);
  StatusType XMC_Enable_Wait_Feature(xmc_bank4_type *Device, xmc_pccard_init_type *Init);
  StatusType XMC_Enable_16bit_Feature(xmc_bank4_type *Device, xmc_pccard_init_type *Init, uint8_t Set_reg_8_bit);
  StatusType XMC_PCCARD_InitCommonSpaceTiming(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing);
  StatusType XMC_PCCARD_InitAttributeSpaceTiming(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing);
  StatusType XMC_PCCARD_InitIOSpaceTiming(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing);


#define XMC_BKxCTRL_DEV_PCCARD        ((uint32_t)0x00000000)
#define BKxTMGMEM_CLEAR_MASK   ((uint32_t)(XMC_BK4TMGMEM_STP  | XMC_BK4TMGMEM_OP |\
                                      XMC_BK4TMGMEM_HLD | XMC_BK4TMGMEM_WRSTP))

#define BKxTMGATT_CLEAR_MASK   ((uint32_t)(XMC_BK4TMGATT_STP  | XMC_BK4TMGATT_OP |\
                                      XMC_BK4TMGATT_HLD | XMC_BK4TMGATT_WRSTP))

#define BK4TMGIO_CLEAR_MASK   ((uint32_t)(XMC_BK4TMGIO_STP    | XMC_BK4TMGIO_OP   | \
                                      XMC_BK4TMGIO_HLD   | XMC_BK4TMGIO_WRSTP))
#endif
  /**
    * @}
    */
 /**
    * @}
    */
#ifdef __cplusplus
}
#endif

 /**
    * @}
    */
 /**
    * @}
    */
	

