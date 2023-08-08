/**
  **************************************************************************
  * @file     usbh_msc_diskio.c
  * @brief    usb mass storage disk io
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
#include "ff.h"      /* Obtains integer types */
#include "diskio.h"    /* Declarations of disk functions */
#include "usb_core.h"
#include "usbh_msc_class.h"
/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_USB_host_two_otg_host_demo
  * @{
  */

extern otg_core_type otg_core_struct;

/* Definitions of physical drive number for each drive */
#define DEV_RAM    0  /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC    1  /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB    2  /* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
  BYTE pdrv    /* Physical drive nmuber to identify the drive */
)
{
  if(usbh_msc_is_ready(&otg_core_struct.host, pdrv) == MSC_OK)
  {
    return RES_OK;
  }

  return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
  BYTE pdrv        /* Physical drive nmuber to identify the drive */
)
{
  return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
  BYTE pdrv,    /* Physical drive nmuber to identify the drive */
  BYTE *buff,    /* Data buffer to store read data */
  LBA_t sector,  /* Start sector in LBA */
  UINT count    /* Number of sectors to read */
)
{
  usb_sts_type status;

  status = usbh_msc_read(&otg_core_struct.host, sector, count, buff, pdrv);

  if(status == USB_OK)
    return RES_OK;

  return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
  BYTE pdrv,      /* Physical drive nmuber to identify the drive */
  const BYTE *buff,  /* Data to be written */
  LBA_t sector,    /* Start sector in LBA */
  UINT count      /* Number of sectors to write */
)
{
  usb_sts_type status;

  status = usbh_msc_write(&otg_core_struct.host, sector, count, (uint8_t *)buff, pdrv);

  if(status == USB_OK)
    return RES_OK;

  return RES_ERROR;


}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
  BYTE pdrv,    /* Physical drive nmuber (0..) */
  BYTE cmd,    /* Control code */
  void *buff    /* Buffer to send/receive control data */
)
{
  usbh_core_type *host = (usbh_core_type *)&otg_core_struct.host;
  usbh_msc_type *pmsc = (usbh_msc_type *)host->class_handler->pdata;
  DRESULT res = RES_OK;
  switch(cmd)
  {
    case CTRL_SYNC:
      res = RES_OK;
      break;
    case GET_SECTOR_COUNT:
      *(DWORD*)buff = pmsc->l_unit_n[pdrv].capacity.blk_nbr;
      break;
    case GET_SECTOR_SIZE:
      *(DWORD*)buff = pmsc->l_unit_n[pdrv].capacity.blk_size;
      break;
    case GET_BLOCK_SIZE:
      *(DWORD*)buff = pmsc->l_unit_n[pdrv].capacity.blk_size;
      break;
    default:
      res = RES_PARERR;
  }
  return res;
}

/*-----------------------------------------------------------------------*/
/* get_fattime                                                    */
/*-----------------------------------------------------------------------*/
DWORD get_fattime (void)
{
 return 0;
}

/**
  * @}
  */

/**
  * @}
  */

