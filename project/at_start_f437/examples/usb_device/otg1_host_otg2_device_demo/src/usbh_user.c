/**
  **************************************************************************
  * @file     usbh_user.c
  * @brief    usb user function
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
#include "usbh_user.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_USB_device_otg1_host_otg2_device_demo
  * @{
  */

static usb_sts_type usbh_user_init(void);
static usb_sts_type usbh_user_reset(void);
static usb_sts_type usbh_user_attached(void);
static usb_sts_type usbh_user_disconnect(void);
static usb_sts_type usbh_user_speed(uint8_t speed);
static usb_sts_type usbh_user_mfc_string(void *string);
static usb_sts_type usbh_user_product_string(void *string);
static usb_sts_type usbh_user_serial_string(void *string);
static usb_sts_type usbh_user_enumeration_done(void);
static usb_sts_type usbh_user_application(void);
static usb_sts_type usbh_user_active_vbus(void *uhost, confirm_state state);
static usb_sts_type usbh_user_not_support(void);


usbh_user_handler_type usbh_user_handle =
{
  usbh_user_init,
  usbh_user_reset,
  usbh_user_attached,
  usbh_user_disconnect,
  usbh_user_speed,
  usbh_user_mfc_string,
  usbh_user_product_string,
  usbh_user_serial_string,
  usbh_user_enumeration_done,
  usbh_user_application,
  usbh_user_active_vbus,
  usbh_user_not_support,
};

/**
  * @brief  usb host init user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_init(void)
{
  usb_sts_type status = USB_OK;

  return status;
}

/**
  * @brief  usb host reset user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_reset(void)
{
  usb_sts_type status = USB_OK;

  return status;
}

/**
  * @brief  usb host check device attached
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_attached(void)
{
  usb_sts_type status = USB_OK;
  USBH_DEBUG("USB Device Attached");
  return status;
}

/**
  * @brief  usb host discconet user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_disconnect(void)
{
  usb_sts_type status = USB_OK;
  USBH_DEBUG("Device Disconnect");
  return status;
}

/**
  * @brief  usb host speed user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_speed(uint8_t speed)
{
  usb_sts_type status = USB_OK;
  if(speed == USB_PRTSPD_FULL_SPEED)
  {
    USBH_DEBUG("This is a Full-Speed device");
  }
  else if(speed == USB_PRTSPD_LOW_SPEED)
  {
    USBH_DEBUG("This is a Low-Speed device");
  }
  return status;
}

/**
  * @brief  usb host manufacturer string user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_mfc_string(void *string)
{
  usb_sts_type status = USB_OK;
  USBH_DEBUG("Manufacturer: %s", (uint8_t *)string);
  return status;
}

/**
  * @brief  usb host product string user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_product_string(void *string)
{
  usb_sts_type status = USB_OK;
  USBH_DEBUG("Product: %s", (uint8_t *)string);
  return status;
}

/**
  * @brief  usb host serial string user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_serial_string(void *string)
{
  usb_sts_type status = USB_OK;
  USBH_DEBUG("Serial: %s", (uint8_t *)string);
  return status;
}

/**
  * @brief  usb host enumeration done user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_enumeration_done(void)
{
  usb_sts_type status = USB_OK;
  USBH_DEBUG("Enumeration done");
  return status;
}

/**
  * @brief  usb host application user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_application(void)
{
  usb_sts_type status = USB_OK;


  return status;
}

/**
  * @brief  usb host active vbus user handler
  * @param  uhost: to the structure of usbh_core_type
  * @param  state: vbus state
            TRUE: active vbus
            FALSE: deactive vbus
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_active_vbus(void *uhost, confirm_state state)
{
  usb_sts_type status = USB_OK;
  usbh_core_type *puhost = (usbh_core_type *)uhost;
#ifdef USBH_5V_POWER_SWITCH
  if(state == TRUE)
  {
    /* active vbus */
    if(puhost->usb_reg == OTG1_GLOBAL)
    {
      gpio_bits_reset(OTG_PIN_POWER_SWITCH_GPIO, OTG_PIN_POWER_SWITCH);
    }

  }
  else
  {
    /* deactive vubs */
    if(puhost->usb_reg == OTG1_GLOBAL)
    {
      gpio_bits_set(OTG_PIN_POWER_SWITCH_GPIO, OTG_PIN_POWER_SWITCH);
    }
  }
#endif
  return status;
}

/**
  * @brief  usb host not support user handler
  * @param  none
  * @retval usb_sts_type
  */
static usb_sts_type usbh_user_not_support(void)
{
  usb_sts_type status = USB_OK;

  return status;
}

/**
  * @}
  */

/**
  * @}
  */
