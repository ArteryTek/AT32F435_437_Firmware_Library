/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
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

#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"
#include "usb_conf.h"
#include "usb_core.h"
#include "usbd_int.h"
#include "msc_class.h"
#include "msc_desc.h"
#include "flash_fat16.h"



/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_USB_device_virtual_msc_iap USB_device_virtual_msc_iap
  * @{
  */

/* usb global struct define */
otg_core_type otg_core_struct;
void (*pftarget)(void);
void usb_clock48m_select(usb_clk48_s clk_s);
void usb_gpio_config(void);
void usb_low_power_wakeup_config(void);
void button_exint_init(button_type button);
void button_isr(void);
void jump_to_app(uint32_t address);
void usb_re_connect(void);


/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  system_clock_config();

  flash_fat16_init();

  at32_board_init();

  if(flash_fat16_get_upgrade_flag() == IAP_SUCCESS &&
    (at32_button_press() == NO_BUTTON))
  {
    jump_to_app(flash_iap.flash_app_addr);
  }

  /* usb gpio config */
  usb_gpio_config();

#ifdef USB_LOW_POWER_WAKUP
  usb_low_power_wakeup_config();
#endif

  /* enable otgfs clock */
  crm_periph_clock_enable(OTG_CLOCK, TRUE);

  /* select usb 48m clcok source */
  usb_clock48m_select(USB_CLK_HEXT);

  /* enable otgfs irq */
  nvic_irq_enable(OTG_IRQ, 0, 0);

  /* init usb */
  usbd_init(&otg_core_struct,
            USB_FULL_SPEED_CORE_ID,
            USB_ID,
            &msc_class_handler,
            &msc_desc_handler);

  while(1)
  {
    flash_fat16_loop_status();
  }
}

/**
  * @brief  usb 48M clock select
  * @param  clk_s:USB_CLK_HICK, USB_CLK_HEXT
  * @retval none
  */
void usb_clock48m_select(usb_clk48_s clk_s)
{
  if(clk_s == USB_CLK_HICK)
  {
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);

    /* enable the acc calibration ready interrupt */
    crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

    /* update the c1\c2\c3 value */
    acc_write_c1(7980);
    acc_write_c2(8000);
    acc_write_c3(8020);
#if (USB_ID == 0)
    acc_sof_select(ACC_SOF_OTG1);
#else
    acc_sof_select(ACC_SOF_OTG2);
#endif
    /* open acc calibration */
    acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
  }
  else
  {
    switch(system_core_clock)
    {
      /* 48MHz */
      case 48000000:
        crm_usb_clock_div_set(CRM_USB_DIV_1);
        break;

      /* 72MHz */
      case 72000000:
        crm_usb_clock_div_set(CRM_USB_DIV_1_5);
        break;

      /* 96MHz */
      case 96000000:
        crm_usb_clock_div_set(CRM_USB_DIV_2);
        break;

      /* 120MHz */
      case 120000000:
        crm_usb_clock_div_set(CRM_USB_DIV_2_5);
        break;

      /* 144MHz */
      case 144000000:
        crm_usb_clock_div_set(CRM_USB_DIV_3);
        break;

      /* 168MHz */
      case 168000000:
        crm_usb_clock_div_set(CRM_USB_DIV_3_5);
        break;

      /* 192MHz */
      case 192000000:
        crm_usb_clock_div_set(CRM_USB_DIV_4);
        break;

      /* 216MHz */
      case 216000000:
        crm_usb_clock_div_set(CRM_USB_DIV_4_5);
        break;

      /* 240MHz */
      case 240000000:
        crm_usb_clock_div_set(CRM_USB_DIV_5);
        break;

      /* 264MHz */
      case 264000000:
        crm_usb_clock_div_set(CRM_USB_DIV_5_5);
        break;

      /* 288MHz */
      case 288000000:
        crm_usb_clock_div_set(CRM_USB_DIV_6);
        break;

      default:
        break;

    }
  }
}

/**
  * @brief  this function config gpio.
  * @param  none
  * @retval none
  */
void usb_gpio_config(void)
{
  gpio_init_type gpio_init_struct;

  crm_periph_clock_enable(OTG_PIN_GPIO_CLOCK, TRUE);
  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

  /* dp and dm */
  gpio_init_struct.gpio_pins = OTG_PIN_DP | OTG_PIN_DM;
  gpio_init(OTG_PIN_GPIO, &gpio_init_struct);

  gpio_pin_mux_config(OTG_PIN_GPIO, OTG_PIN_DP_SOURCE, OTG_PIN_MUX);
  gpio_pin_mux_config(OTG_PIN_GPIO, OTG_PIN_DM_SOURCE, OTG_PIN_MUX);

#ifdef USB_SOF_OUTPUT_ENABLE
  crm_periph_clock_enable(OTG_PIN_SOF_GPIO_CLOCK, TRUE);
  gpio_init_struct.gpio_pins = OTG_PIN_SOF;
  gpio_init(OTG_PIN_SOF_GPIO, &gpio_init_struct);
  gpio_pin_mux_config(OTG_PIN_SOF_GPIO, OTG_PIN_SOF_SOURCE, OTG_PIN_MUX);
#endif

  /* otgfs use vbus pin */
#ifndef USB_VBUS_IGNORE
  gpio_init_struct.gpio_pins = OTG_PIN_VBUS;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_pin_mux_config(OTG_PIN_GPIO, OTG_PIN_VBUS_SOURCE, OTG_PIN_MUX);
  gpio_init(OTG_PIN_GPIO, &gpio_init_struct);
#endif


}
#ifdef USB_LOW_POWER_WAKUP
/**
  * @brief  usb low power wakeup interrupt config
  * @param  none
  * @retval none
  */
void usb_low_power_wakeup_config(void)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
  exint_default_para_init(&exint_init_struct);

  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPT;
  exint_init_struct.line_select = OTG_WKUP_EXINT_LINE;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  nvic_irq_enable(OTG_WKUP_IRQ, 0, 0);
}

/**
  * @brief  this function handles otgfs wakup interrupt.
  * @param  none
  * @retval none
  */
void OTG_WKUP_HANDLER(void)
{
  exint_flag_clear(OTG_WKUP_EXINT_LINE);
}

#endif


/**
  * @brief  usb re-connect
  * @param  none
  * @retval none
  */
void usb_reset()
{
  delay_ms(1000);
  usbd_disconnect(&otg_core_struct.dev);
  delay_ms(1000);
  usbd_connect(&otg_core_struct.dev);
}

/**
  * @brief  loop status
  * @param  none
  * @retval none
  */
void flash_fat16_loop_status(void)
{
  static uint8_t status = UPGRADE_READY;
  if(status == flash_iap.msc_up_status)
  {
    return;
  }
  else
  {
    status = flash_iap.msc_up_status;
  }

  switch(flash_iap.msc_up_status)
  {
    case UPGRADE_READY:
      flash_fat16_set_name((const uint8_t *)"Ready", 5);
      break;
    case UPGRAGE_ONGOING:
      break;
    case UPGRADE_SUCCESS:
      flash_fat16_set_name((const uint8_t *)"Success", 7);
      flash_iap.msc_up_status = UPGRADE_DONE;
      usb_reset();
      break;
    case UPGRADE_FAILED:
      flash_fat16_set_name((const uint8_t *)"Failed", 6);
      flash_iap.msc_up_status = UPGRADE_DONE;
      usb_reset();
      break;
    case UPGRADE_LARGE:
      flash_fat16_set_name((const uint8_t *)"Large", 5);
      flash_iap.msc_up_status = UPGRADE_DONE;
      usb_reset();
      break;
    case UPGRADE_UNKNOWN:
      flash_fat16_set_name((const uint8_t *)"Unkown", 6);
      break;
    case UPGRADE_DONE:
      break;
    case UPGRADE_JUMP:
      flash_fat16_set_name((const uint8_t *)"Jump", 6);
      delay_ms(1000);
      jump_to_app(flash_iap.write_addr);
      break;
    default:
      break;
  }

}


/**
  * @brief  jump to app
  * @param  none
  * @retval none
  */
void jump_to_app(uint32_t address)
{
  uint32_t stkptr, jumpaddr;
  stkptr = *(uint32_t *)address;
  jumpaddr = *(uint32_t *)(address + sizeof(uint32_t));

  /* disable nvic irq and periph clock, clear pending */
  nvic_irq_disable(OTGFS1_IRQn);
  nvic_irq_disable(OTGFS2_IRQn);

  __NVIC_ClearPendingIRQ(OTGFS1_IRQn);
  __NVIC_ClearPendingIRQ(OTGFS2_IRQn);

  crm_periph_clock_enable(CRM_OTGFS1_PERIPH_CLOCK, FALSE);
  crm_periph_clock_enable(CRM_OTGFS2_PERIPH_CLOCK, FALSE);

  crm_periph_reset(CRM_OTGFS1_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_OTGFS1_PERIPH_RESET, FALSE);

  crm_periph_reset(CRM_OTGFS2_PERIPH_RESET, TRUE);
  crm_periph_reset(CRM_OTGFS2_PERIPH_RESET, FALSE);

  __set_MSP(stkptr);
  pftarget = (void (*) (void))jumpaddr;
  pftarget();
}

/**
  * @brief  this function handles otgfs interrupt.
  * @param  none
  * @retval none
  */
void OTG_IRQ_HANDLER(void)
{
  usbd_irq_handler(&otg_core_struct);
}

/**
  * @brief  usb delay millisecond function.
  * @param  ms: number of millisecond delay
  * @retval none
  */
void usb_delay_ms(uint32_t ms)
{
  /* user can define self delay function */
  delay_ms(ms);
}

/**
  * @brief  usb delay microsecond function.
  * @param  us: number of microsecond delay
  * @retval none
  */
void usb_delay_us(uint32_t us)
{
  delay_us(us);
}

/**
  * @}
  */

/**
  * @}
  */
