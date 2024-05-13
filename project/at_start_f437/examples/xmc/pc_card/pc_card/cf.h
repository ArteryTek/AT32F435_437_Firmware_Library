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

#include "stdint.h"
#include "at32f435_437.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XMC_BASE                         ((uint32_t)0xA8000000)
#define XMC_BANK4_                       (XMC_BASE + 0x0000000)
#define PCCARD_DEVICE_ADDRESS            XMC_BANK4_
#define PCCARD_COMMON_SPACE_ADDRESS      PCCARD_DEVICE_ADDRESS
#define PCCARD_ATTRIBUTE_SPACE_ADDRESS   ((uint32_t)(XMC_BANK4_ + 0x4000000))
#define PCCARD_IO_SPACE_ADDRESS          ((uint32_t)(XMC_BANK4_ + 0x6000000))

/* in xmc document page 545/1128, the xmc is not supported true ide mode. */
#define PCCARD_IO_SPACE_PRIMARY_ADDR     ((uint32_t)(XMC_BANK4_ + PCCARD_IO_SPACE_ADDRESS + 0x1F0)) 
#define PCCARD_IO_SPACE_SECONDARY_ADDR   ((uint32_t)(XMC_BANK4_ + PCCARD_IO_SPACE_ADDRESS + 0x170)) 

/* compact flash-ata registers description */
#define ATA_DATA                         ((uint8_t)0x00)
#define ATA_ERROR_REG                    ((uint8_t)0x01)
#define ATA_FEATURE_REG                  ((uint8_t)0x01)
#define ATA_sector_count                 ((uint8_t)0x02)
#define ATA_SECTOR_NUMBER                ((uint8_t)0x03)
#define ATA_CYLINDER_LOW                 ((uint8_t)0x04)
#define ATA_CYLINDER_HIGH                ((uint8_t)0x05)
#define ATA_CARD_HEAD                    ((uint8_t)0x06)
#define ATA_STATUS_CMD                   ((uint8_t)0x07)
#define ATA_STATUS_CMD_ALTERNATE         ((uint8_t)0x0E)
#define ATA_COMMON_DATA_AREA             ((uint16_t)0x0400)

/* attribute memory space register description */
#define ATTRIBUTE_MEM_BASE               0
#define ATTRIBUTE_MEM_CONFIG_BASE        0x200
#define CONFIG_OPTION_REG                ATTRIBUTE_MEM_CONFIG_BASE + 0x00
#define CARD_CONFIG_STATUS_REG           ATTRIBUTE_MEM_CONFIG_BASE + 0x02
#define PIN_REPLACE_REG                  ATTRIBUTE_MEM_CONFIG_BASE + 0x04
#define SOCKET_COPY_REG                  ATTRIBUTE_MEM_CONFIG_BASE + 0x06

/* configuration option register */
#define CONF0_BIT                        0x01
#define CONF1_BIT                        0x02
#define CONF2_BIT                        0x04
#define CONF3_BIT                        0x08
#define CONF4_BIT                        0x10
#define CONF5_BIT                        0x20
#define LEVLREQ_BIT                      0x40
#define SRESET_BIT                       0x80
                                         
#define CF_MEM_MAP_MODE                  0x00
#define CF_IO_16_MODE                    0x01
#define CF_IO_PRICH_MODE                 0x02
#define CF_IO_SECCH_MODE                 0x03

/* compact flash-ata commands */
#define ATA_CHECK_POWER_MODE_CMD         ((uint8_t)0xE5)
#define ATA_EXECUTE_DRIVE_DIAG_CMD       ((uint8_t)0x90)
#define ATA_ERASE_SECTOR_CMD             ((uint8_t)0xC0)
#define ATA_FORMAT_TRACK_CMD             ((uint8_t)0x50)
#define ATA_IDENTIFY_DRIVE_CMD           ((uint8_t)0xEC)
#define ATA_IDLE_CMD                     ((uint8_t)0xE3)
#define ATA_IDLE_IMMEDIATE_CMD           ((uint8_t)0xE1)
#define ATA_INIT_DRIVE_PARA_CMD          ((uint8_t)0x91)
#define ATA_READ_BUFFER_CMD              ((uint8_t)0xE4)
#define ATA_READ_LONG_SECTOR_CMD         ((uint8_t)0x22)
#define ATA_READ_MULTIPLE_CMD            ((uint8_t)0xC4)
#define ATA_READ_SECTOR_CMD              ((uint8_t)0x20)
#define ATA_READ_VERIFY_SECTOR_CMD       ((uint8_t)0x40)
#define ATA_RECALIBRATE_CMD              ((uint8_t)0x10)
#define ATA_REQUEST_SENSE_CMD            ((uint8_t)0x03)
#define ATA_SECURITY_DISABLE_PASSWORD_CMD ((uint8_t)0xF6)
#define ATA_SECURITY_EREASE_PREPARE_CMD  ((uint8_t)0xF3)
#define ATA_SECURITY_ERASE_UNIT_CMD      ((uint8_t)0xF4)
#define ATA_SECURITY_FREEZE_LOCK_CMD     ((uint8_t)0xF5)
#define ATA_SECURITY_SET_PASSWORD_CMD    ((uint8_t)0xF1)
#define ATA_SECURITY_UNLOCK_CMD          ((uint8_t)0xF2)
#define ATA_SEEK_CMD                     ((uint8_t)0x70)
#define ATA_SET_FEATURE_CMD              ((uint8_t)0xEF)
#define ATA_SET_MULTIPLE_MODE_CMD        ((uint8_t)0xC6)
#define ATA_SET_SLEEP_MODE_CMD           ((uint8_t)0xE6)
#define ATA_STANDBY_CMD                  ((uint8_t)0xE2)
#define ATA_STANDBY_IMMEDIATE_CMD        ((uint8_t)0xE0)
#define ATA_TRANSLATE_SECTOR_CMD         ((uint8_t)0x87)
#define ATA_WEAR_LEVEL_CMD               ((uint8_t)0xF5)
#define ATA_WRITE_BUFFER_CMD             ((uint8_t)0xE8)
#define ATA_WRITE_LONG_SECTOR_CMD        ((uint8_t)0x32)
#define ATA_WRITE_MULTIPLE_CMD           ((uint8_t)0xC5)
#define ATA_WRITE_MULTIPLE_WO_ERASE_CMD  ((uint8_t)0xCD)
#define ATA_WRITE_SECTOR_CMD             ((uint8_t)0x30)
#define ATA_WRITE_SECTOR_WO_ERASE_CMD    ((uint8_t)0x38)
#define ATA_WRITE_VERIFY_CMD             ((uint8_t)0x3C)

/* feature command */
#define ENABLE_8BIT_MODE                 0x01
#define ENABLE_PWR_LV1                   0x0a
#define DISABLE_READ_LOOK_AHEAD          0x55
#define DISABLE_POR                      0x66
#define NOP                              0x69
#define DISABLE_8BIT_MODE                0x81
#define DISABLE_PWR_LV1                  0x8a
#define SET_HOST_CUR_SOURCE_CAP          0x9a
#define APPLY_4BYTE_DATA_RW_LONG_CMD     0xbb
#define ENABLE_POR                       0xcc

/* status register */
#define BUSY_BIT                         0x80
#define RDY_BIT                          0x40
#define DWF_BIT                          0x20
#define DSC_BIT                          0x10
#define DRQ_BIT                          0x08
#define CORR_BIT                         0x04
#define ERR_BIT                          0x01

/* error register */
#define BBK_BIT                          0x80
#define UNC_BIT                          0x40
#define IDNF_BIT                         0x10
#define ABRT_BIT                         0x04
#define AMNF_BIT                         0x01

/* compact flash status */
#define PCCARD_TIMEOUT_ERROR             ((uint8_t)0x60)
#define PCCARD_BUSY                      ((uint8_t)0x80)
#define PCCARD_PROGR                     ((uint8_t)0x01)
#define PCCARD_READY                     ((uint8_t)0x40)
#define PCCARD_SECTOR_SIZE               ((uint32_t)512)
#define PCCARD_LONG_SECTOR_SIZE          ((uint32_t)516)
#define PCCARD_25_SECTORS_SIZES          ((uint32_t)(512*25))

/* cf cis information */
#define CISTPL_DEVICE                    0x01
#define CISTPL_DEVICE_OC                 0x1c
#define CISTPL_JEDEC_C                   0x18
#define CISTPL_MANFID                    0x20
#define CISTPL_VERS_1                    0x15
#define CISTPL_FUNCID                    0x21
#define CISTPL_FUNCE                     0x22
#define CISTPL_CONFIG                    0x1a
#define CISTPL_CFTABLE_ENTRY             0x1b
#define CISTPL_NO_LINK                   0x14
#define CISTPL_END                       0xff
#define TPL_LINK_LOC                     16
#define MAX_CIS_LEN                      0x200

/* compact flash redefinition */
#define CF_DEVICE_ADDRESS                PCCARD_DEVICE_ADDRESS
#define CF_ATTRIBUTE_SPACE_ADDRESS       PCCARD_ATTRIBUTE_SPACE_ADDRESS
#define CF_COMMON_SPACE_ADDRESS          PCCARD_COMMON_SPACE_ADDRESS
#define CF_IO_SPACE_ADDRESS              PCCARD_IO_SPACE_ADDRESS
#define CF_IO_SPACE_PRIMARY_ADDR         PCCARD_IO_SPACE_PRIMARY_ADDR
                                         
#define CF_TIMEOUT_ERROR                 PCCARD_TIMEOUT_ERROR
#define CF_BUSY                          PCCARD_BUSY
#define CF_PROGR                         PCCARD_PROGR
#define CF_READY                         PCCARD_READY
                                         
#define CF_SECTOR_SIZE                   PCCARD_SECTOR_SIZE
#define READ_TRANSFER                    1
#define WRITE_TRANSFER                   0
#define ENABLE_8_BIT_TRANSFER            1
#define ENABLE_16_BIT_TRANSFER           0

#define CF_print( cond, _x_)             if ( cond ) { printf _x_ ; }

/* for print message level */
#define ERR_REG_INFO                     0
#define STS_REG_INFO                     0
#define PRT_INIT_INFO                    1
#define PRT_CMD_INFO                     0
#define PRT_ERR_INFO                     0

typedef unsigned char  BOOL;
typedef enum
{
  unlocked                               = 0x00,
  locked                                 = 0x01
}lock_type;

typedef enum
{
  status_ok                              = 0x00,
  status_error                           = 0x01,
}status_type;

/**
  * @brief pccard state structures definition
  */
typedef enum
{
  pccard_state_reset                     = 0x00,
  pccard_state_ready                     = 0x01,
  pccard_state_busy                      = 0x02,
  pccard_state_error                     = 0x04 
}pccard_state_type;

/* cf type */
typedef  struct
{
  uint16_t default_cylinder;
  uint8_t  default_head;
  uint16_t default_sector;
  uint16_t current_cylinder;
  uint8_t  current_head;
  uint16_t current_sector;
  uint32_t total_sector;
  uint32_t total_lba_sector;
  uint16_t max_mutiple_sector;
  uint16_t mutiple_sector_setting;
}cfcard_info_struct;

/* cf address struct */
typedef struct
{
  uint16_t sector_count;
  uint16_t cylinder;
  uint8_t  head;
  uint8_t  sector;
  uint8_t  drv;
  uint32_t sectorsize;
}cfcard_addr_struct;

/* cf card structure */
typedef  struct
{
  uint32_t           io_addr;
  BOOL               dma_enable;
  BOOL               enable_8_bit_mode;
  cfcard_info_struct cfcard_info;
  cfcard_addr_struct cf_addr;
  uint16_t           protocol;
  uint32_t           drive;
}cfcard_struct;

/* ata-command verification result */
typedef  struct
{
  BOOL ata_check_power_mode_pass;
  BOOL ata_execute_drive_diag_pass;
  BOOL ata_erase_sector_pass;
  BOOL ata_format_track_pass;
  BOOL ata_identify_drive_pass;
  BOOL ata_idle_pass;
  BOOL ata_idle_immediate_pass;
  BOOL ata_init_drive_para_pass;
  BOOL ata_read_buffer_pass;
  BOOL ata_read_long_sector_pass;
  BOOL ata_read_multiple_pass;
  BOOL ata_read_sector_pass;
  BOOL ata_read_verify_sector_pass;
  BOOL ata_recalibrate_pass;
  BOOL ata_request_sense_pass;
  BOOL ata_security_disable_password_pasS;
  BOOL ata_security_erease_prepare_pass;
  BOOL ata_security_erase_unit_pass;
  BOOL ata_security_freeze_lock_pass;
  BOOL ata_security_set_password_pass;
  BOOL ata_security_unlock_pass;
  BOOL ata_seek_pass;
  BOOL ata_set_feature_pass;
  BOOL ata_set_multiple_mode_pass;
  BOOL ata_set_sleep_mode_pass;
  BOOL ata_standby_pass;
  BOOL ata_standby_immediate_pass;
  BOOL ata_translate_sector_pass;
  BOOL ata_wear_level_pass;
  BOOL ata_write_buffer_pass;
  BOOL ata_write_long_sector_pass;
  BOOL ata_write_multiple_pass;
  BOOL ata_write_multiple_wo_erase_pass;
  BOOL ata_write_sector_pass;
  BOOL ata_write_sector_wo_erase_pass;
  BOOL ata_write_verify_pass;
  BOOL acess_25_sectors_pass;
  BOOL cis_read_pass;
  BOOL attribute_rw_pass;
}verification_result_struct;

/* xmc_pccard handle structure definition */
typedef struct
{
  xmc_bank4_type           *instance;
  xmc_pccard_init_type     init;
  __IO pccard_state_type   state;
  lock_type                lock;
  cfcard_struct            cf;
}pccard_handle_type;


#define __PCCARD_Reset_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->state = pccard_state_reset)
#define __XMC_PCCARD_GET_FLAG(__INSTANCE__, __FLAG__)  (((__INSTANCE__)->BK4STS &(__FLAG__)) == (__FLAG__))
#define __XMC_PCCARD_CLEAR_FLAG(__INSTANCE__, __FLAG__)  BIT_CLEAR((__INSTANCE__)->BK4STS, (__FLAG__))
#define XMC_BKxCTRL_DEV_PCCARD           ((uint32_t)0x00000000)
#define BKxTMGMEM_CLEAR_MASK             ((uint32_t)(XMC_BK4TMGMEM_STP | XMC_BK4TMGMEM_OP | XMC_BK4TMGMEM_HLD | XMC_BK4TMGMEM_WRSTP))
#define BKxTMGATT_CLEAR_MASK             ((uint32_t)(XMC_BK4TMGATT_STP | XMC_BK4TMGATT_OP | XMC_BK4TMGATT_HLD | XMC_BK4TMGATT_WRSTP))
#define BK4TMGIO_CLEAR_MASK              ((uint32_t)(XMC_BK4TMGIO_STP | XMC_BK4TMGIO_OP | XMC_BK4TMGIO_HLD | XMC_BK4TMGIO_WRSTP))

status_type pccard_init(pccard_handle_type *hpccard, xmc_nand_pccard_timinginit_type *comspacetiming, xmc_nand_pccard_timinginit_type *attspacetiming, xmc_nand_pccard_timinginit_type *iospacetiming);
status_type pccard_deinit(pccard_handle_type *hpccard);
status_type enable_wait_feature(pccard_handle_type *hpccard);
BOOL pccard_read_id(pccard_handle_type *hpccard);
BOOL pccard_identify(pccard_handle_type *hpccard);
BOOL pccard_write_sector(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count);
BOOL pccard_read_sector(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count);
BOOL pccard_erase_sector(pccard_handle_type *hpccard, uint32_t sector_address, uint32_t sector_count);
BOOL pccard_diagnostic(pccard_handle_type *hpccard);
BOOL pccard_reset(pccard_handle_type *hpccard);
BOOL pccard_check_power_mode(pccard_handle_type *hpccard);
BOOL pccard_idle(pccard_handle_type *hpccard, uint32_t sector_count);
BOOL pccard_idle_immediate(pccard_handle_type *hpccard);
BOOL pccard_set_sleep_mode(pccard_handle_type *hpccard);
BOOL pccard_standby(pccard_handle_type *hpccard);
BOOL pccard_standby_immediate(pccard_handle_type *hpccard);
BOOL pccard_reuqest_sense(pccard_handle_type *hpccard);
BOOL pccard_read_buffer(pccard_handle_type *hpccard, uint8_t *pbuffer);
BOOL pccard_write_buffer(pccard_handle_type *hpccard, uint8_t *pbuffer);
BOOL pccard_recalibrate(pccard_handle_type *hpccard);
BOOL pccard_init_drive_para(pccard_handle_type *hpccard, uint16_t sector_count, uint8_t Head_Count);
BOOL pccard_seek(pccard_handle_type *hpccard, uint8_t Head_Count, uint16_t Cylinder_Count);
BOOL pccard_translate_sector(pccard_handle_type *hpccard, uint32_t sector_address);
BOOL pccard_read_long_sector(pccard_handle_type *hpccard, uint32_t sector_address, uint8_t *pbuffer);
BOOL pccard_write_long_sector(pccard_handle_type *hpccard, uint32_t sector_address, uint8_t *pbuffer);
BOOL pccard_write_sector_wo_erase(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count);
BOOL pccard_format_track(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count);
BOOL pccard_read_verify_sector(pccard_handle_type *hpccard, uint32_t sector_address, uint16_t sector_count);
BOOL pccard_write_verify(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count);
BOOL pccard_set_multiple_mode(pccard_handle_type *hpccard, uint32_t sector_count);
BOOL pccard_set_features(pccard_handle_type *hpccard, uint32_t sector_count, uint8_t Feature);
BOOL pccard_write_multiple(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count);
BOOL pccard_read_multiple(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count);
BOOL pccard_write_multiple_wo_erase(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t sector_address, uint32_t sector_count);
BOOL read_cfcard_cis_information(pccard_handle_type *hpccard);
BOOL cf_send_command(pccard_handle_type *hpccard, uint8_t Cmd);
BOOL task_file_regis_valid(pccard_handle_type *hpccard);
BOOL vaild_init_drive_para(pccard_handle_type *hpccard, uint16_t sector_count, uint8_t head_count);
void translate_chsaddr(pccard_handle_type *hpccard, uint32_t sector_address, uint16_t sector_count, uint16_t Sector_Limit);
void pccard_irqhandler(pccard_handle_type *hpccard);
void pccard_int_callback(pccard_handle_type *hpccard);
void show_verification_result(verification_result_struct *result);
uint8_t *read_sector(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t transfer_size);
uint8_t *write_sector(pccard_handle_type *hpccard, uint8_t *pbuffer, uint32_t transfer_size);
pccard_state_type pccard_get_state(pccard_handle_type *hpccard);
uint8_t pccard_read_error_reg(pccard_handle_type *hpccard);
uint8_t pccard_read_status_reg(pccard_handle_type *hpccard);
status_type xmc_pccard_init_ctrl(xmc_bank4_type *Device, xmc_pccard_init_type *init, uint8_t Set_reg_8_bit);
status_type xmc_enable_wait_feature(xmc_bank4_type *Device, xmc_pccard_init_type *init);
status_type xmc_enable_16bit_feature(xmc_bank4_type *Device, xmc_pccard_init_type *init, uint8_t Set_reg_8_bit);
status_type xmc_pccard_init_common_space_timing(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing);
status_type xmc_pccard_init_attribute_space_timing(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing);
status_type xmc_pccard_init_iospace_timing(xmc_bank4_type *Device, xmc_nand_pccard_timinginit_type *Timing);

#endif

#ifdef __cplusplus
}
#endif

  

