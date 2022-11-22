/**
  **************************************************************************
  * @file     FlashPrg.c
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

#include "FlashOS.H"        // FlashOS Structures
#include "at32f435_437.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup QSPI_flash_algorithm_for_keil
  * @{
  */

/**
  * @brief  qspi xip mode enable or disable
  * @param  none
  * @retval none
  */
void custom_qspi_xip_enable(confirm_state new_state)
{
  if(new_state == TRUE)
  {
    QSPI1->ctrl_bit.xipsel = TRUE;
    while(QSPI1->xip_cmd_w3_bit.csts != RESET);
    while(QSPI1->ctrl_bit.xipidle == RESET);
  }
  else
  {
    while(QSPI1->fifosts_bit.txfifordy == RESET);
    while((QSPI1->ctrl_bit.abort != RESET) && (QSPI1->ctrl_bit.xipidle == RESET));
    QSPI1->ctrl_bit.xiprcmdf = TRUE;
    while(QSPI1->ctrl_bit.abort != RESET || QSPI1->ctrl_bit.xipidle == RESET);
    QSPI1->ctrl_bit.xipsel = FALSE;
  }
}

/**
  * @brief  qspi xip mode enable config
  * @param  none
  * @retval none
  */
void custom_xip_enable_config(void)
{
  uint32_t xc0_val = 0, xc1_val = 0, xc2_val = 0;
  qspi_xip_type *custom_xip_config;

  custom_xip_config->read_instruction_code = 0xEB;
  custom_xip_config->read_address_length = QSPI_XIP_ADDRLEN_3_BYTE;
  custom_xip_config->read_operation_mode = QSPI_OPERATE_MODE_144;
  custom_xip_config->read_second_dummy_cycle_num = 6;
  custom_xip_config->read_select_mode = QSPI_XIPR_SEL_MODED;
  custom_xip_config->read_time_counter = 0xF;
  custom_xip_config->read_data_counter = 32;

  /* config analyse xip_cmd_w0 register */
  xc0_val = (uint32_t)custom_xip_config->read_second_dummy_cycle_num;
  xc0_val |= (uint32_t)(custom_xip_config->read_operation_mode << 8);
  xc0_val |= (uint32_t)(custom_xip_config->read_address_length << 11);
  xc0_val |= (uint32_t)(custom_xip_config->read_instruction_code << 12);
  QSPI1->xip_cmd_w0 = xc0_val;

  /* config analyse xip_cmd_w1 register */
  xc1_val = (uint32_t)custom_xip_config->write_second_dummy_cycle_num;
  xc1_val |= (uint32_t)(custom_xip_config->write_operation_mode << 8);
  xc1_val |= (uint32_t)(custom_xip_config->write_address_length << 11);
  xc1_val |= (uint32_t)(custom_xip_config->write_instruction_code << 12);
  QSPI1->xip_cmd_w1 = xc1_val;

  /* config analyse xip_cmd_w2 register */
  xc2_val = (uint32_t)custom_xip_config->read_data_counter;
  xc2_val |= (uint32_t)(custom_xip_config->read_time_counter << 8);
  xc2_val |= (uint32_t)(custom_xip_config->read_select_mode << 15);
  xc2_val |= (uint32_t)(custom_xip_config->write_data_counter << 16);
  xc2_val |= (uint32_t)(custom_xip_config->write_time_counter << 24);
  xc2_val |= (uint32_t)(custom_xip_config->write_select_mode << 31);
  QSPI1->xip_cmd_w2 = xc2_val;

  custom_qspi_xip_enable(TRUE);
}

/**
  * @brief  esmt32m rstqpi command
  * @param  none
  * @retval none
  */
void custom_qspi_rstqpi(void)
{
  QSPI1->cmd_w0 = 0;
  QSPI1->cmd_w1 = 0x01000000;
  QSPI1->cmd_w2 = 0;
  QSPI1->cmd_w3 = 0xFF0000C2;
  while((flag_status)(QSPI1->cmdsts_bit.cmdsts) == RESET);
  QSPI1->cmdsts_bit.cmdsts = TRUE;
}

/**
  * @brief  esmt32m rsten command
  * @param  none
  * @retval none
  */
void custom_qspi_rsten(void)
{
  QSPI1->cmd_w0 = 0;
  QSPI1->cmd_w1 = 0x01000000;
  QSPI1->cmd_w2 = 0;
  QSPI1->cmd_w3 = 0x66000002;
  while((flag_status)(QSPI1->cmdsts_bit.cmdsts) == RESET);
  QSPI1->cmdsts_bit.cmdsts = TRUE;
}

/**
  * @brief  esmt32m rst command
  * @param  none
  * @retval none
  */
void custom_qspi_rst(void)
{
  QSPI1->cmd_w0 = 0;
  QSPI1->cmd_w1 = 0x01000000;
  QSPI1->cmd_w2 = 0;
  QSPI1->cmd_w3 = 0x99000002;
  while((flag_status)(QSPI1->cmdsts_bit.cmdsts) == RESET);
  QSPI1->cmdsts_bit.cmdsts = TRUE;
}

/**
  * @brief  esmt32m write enable command
  * @param  none
  * @retval none
  */
void custom_qspi_write_enable(void)
{
  QSPI1->cmd_w0 = 0;
  QSPI1->cmd_w1 = 0x01000000;
  QSPI1->cmd_w2 = 0;
  QSPI1->cmd_w3 = 0x06000002;
  while((flag_status)(QSPI1->cmdsts_bit.cmdsts) == RESET);
  QSPI1->cmdsts_bit.cmdsts = TRUE;
}

/**
  * @brief  esmt32m check busy command
  * @param  none
  * @retval none
  */
void custom_qspi_busy_check(void)
{
  QSPI1->cmd_w0 = 0;
  QSPI1->cmd_w1 = 0x01000000;
  QSPI1->cmd_w2 = 0;
  QSPI1->cmd_w3 = 0x05000004;
  while((flag_status)(QSPI1->cmdsts_bit.cmdsts) == RESET);
  QSPI1->cmdsts_bit.cmdsts = TRUE;
}

/**
  * @brief  esmt32m sector erase command
  * @param  none
  * @retval none
  */
void custom_qspi_sector_erase(uint32_t adr)
{
  QSPI1->cmd_w0 = adr;
  QSPI1->cmd_w1 = 0x01000003;
  QSPI1->cmd_w2 = 0;
  QSPI1->cmd_w3 = 0x20000002;
  while((flag_status)(QSPI1->cmdsts_bit.cmdsts) == RESET);
  QSPI1->cmdsts_bit.cmdsts = TRUE;
}

/**
  * @brief  esmt32m chip erase command
  * @param  none
  * @retval none
  */
void custom_qspi_chip_erase(void)
{
  QSPI1->cmd_w0 = 0;
  QSPI1->cmd_w1 = 0x01000000;
  QSPI1->cmd_w2 = 0;
  QSPI1->cmd_w3 = 0x60000002;
  while((flag_status)(QSPI1->cmdsts_bit.cmdsts) == RESET);
  QSPI1->cmdsts_bit.cmdsts = TRUE;
}

/**
  * @brief  sysclk set to pll 96mhz
  * @param  none
  * @retval none
  */
void init_clk(void)
{
 /* pll configuration: 96 mhz */
  CRM->pllcfg = 0x00031801;

  /* enable pll */
  CRM->ctrl_bit.pllen = TRUE;

  /* wait till pll is ready */
  while(CRM->ctrl_bit.pllstbl == RESET);

  /* select pll as system clock source */
  CRM->cfg_bit.sclksel = CRM_SCLK_PLL;

  /* wait till pll is used as system clock source */
  while (CRM->cfg_bit.sclksts != CRM_SCLK_PLL);
}

/**
  * @brief  sysclk uninit
  * @param  none
  * @retval none
  */
void uninit_clk(void)
{
  CRM->cfg_bit.sclksel = CRM_SCLK_HICK;
  CRM->ctrl_bit.pllen = FALSE;
  CRM->pllcfg = 0x00033002; /* default */
}

/**
  * @brief  qspi gpio initilize
  * @param  none
  * @retval none
  */
void qspi_gpio_init(void)
{
  /* enable the qspi clock */
  CRM->ahben3 |= 0x02;

  /* enable the gpio pin clock */
  CRM->ahben1 |= 0x60;

  /* configure the gpio */
  GPIOF->cfgr = 0x002AA000;
  GPIOF->odrvr = 0x00155000;
  GPIOF->muxl = 0x99000000;
  GPIOF->muxh = 0x000009AA;

  GPIOG->cfgr = 0x00002000;
  GPIOG->odrvr = 0x00001000;
  GPIOG->muxl = 0x0A000000;
}

/**
  * @brief  qspi command port init flow
  * @param  none
  * @retval none
  */
void qspi_cmd_init(void)
{
  __IO uint32_t num;
  /* xip disable */
  custom_qspi_xip_enable(FALSE);

  /* clk mode 0 */
  QSPI1->ctrl_bit.sckmode = QSPI_SCK_MODE_0;

  /* busy offset 0 */
  QSPI1->ctrl_bit.busy = QSPI_BUSY_OFFSET_0;

  /* clk div2 */
  QSPI1->ctrl_bit.clkdiv = QSPI_CLK_DIV_2;

  /* reset qpi mode and reset the device */
  custom_qspi_rstqpi();
  custom_qspi_rsten();
  custom_qspi_rst();

  /* wait about 30us */
  num = 600;
  while(num--)
  {
    __NOP();
  }
}

/**
  * @brief  initialize flash.
  * @param  adr: device base address
            clk: clock frequency (Hz)
            fnc: function code (1-erase, 2-program, 3-verify)
  * @retval 0 if success, 1 if failed
  */
int Init (unsigned long adr, unsigned long clk, unsigned long fnc)
{
  init_clk();
  qspi_gpio_init();
  qspi_cmd_init();
  custom_xip_enable_config();
  return (0);
}

/**
  * @brief  de-initialize flash.
  * @param  fnc: function code (1-erase, 2-program, 3-verify)
  * @retval 0 if success, 1 if failed
  */
int UnInit (unsigned long fnc)
{
  uninit_clk();
  custom_xip_enable_config();
  return (0);
}

int EraseChip (void)
{
  custom_qspi_xip_enable(FALSE);

  /* qspi write enable */
  custom_qspi_write_enable();

  /* qspi chip erase */
  custom_qspi_chip_erase();

  /* qspi check busy */
  custom_qspi_busy_check();

  custom_xip_enable_config();
  return (0);
}

/**
  * @brief  erase sector.
  * @param  adr: sector address
  * @retval 0 if success, 1 if failed
  */
int EraseSector (unsigned long adr)
{
  adr -= QSPI1_MEM_BASE;
  custom_qspi_xip_enable(FALSE);

  /* qspi write enable */
  custom_qspi_write_enable();

  /* qspi sector erase */
  custom_qspi_sector_erase(adr);

  /* qspi check busy */
  custom_qspi_busy_check();

  custom_xip_enable_config();
  return (0);
}

/**
  * @brief  program page.
  * @param  adr: page start address
            sz:  page size
            buf: page data
  * @retval 0 if success, 1 if failed
  */
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  uint32_t i;
  adr -= QSPI1_MEM_BASE;
  custom_qspi_xip_enable(FALSE);
  /* qspi write enable */
  custom_qspi_write_enable();

  /* qspi program */
  QSPI1->cmd_w0 = adr;
  QSPI1->cmd_w1 = 0x01000003;
  QSPI1->cmd_w2 = sz;
  QSPI1->cmd_w3 = 0x32000042;
  for(i = 0; i < sz; ++i)
  {
    while((flag_status)(QSPI1->fifosts_bit.txfifordy) == RESET);
    QSPI1->dt_u8 = *buf++;
  }
  while((flag_status)(QSPI1->cmdsts_bit.cmdsts) == RESET);
  QSPI1->cmdsts_bit.cmdsts = TRUE;

  /* qspi check busy */
  custom_qspi_busy_check();

  custom_xip_enable_config();
  return 0;
}

/**
  * @}
  */

/**
  * @}
  */
