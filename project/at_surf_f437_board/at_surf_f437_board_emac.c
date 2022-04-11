/**
  **************************************************************************
  * @file     at32_emac.c
  * @version  v2.0.7
  * @date     2022-04-02
  * @brief    emac config program
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
#include "at_surf_f437_board_emac.h"
#include "at_surf_f437_board_delay.h"

/**
  * @brief  enable emac clock and gpio clock
  * @param  none
  * @retval success or error
  */
error_status emac_system_init(void)
{
  error_status status;

  emac_nvic_configuration();

  /* emac periph clock enable */
  crm_periph_clock_enable(CRM_EMAC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_EMACTX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_EMACRX_PERIPH_CLOCK, TRUE);

  emac_pins_configuration();

  status = emac_layer2_configuration();

  return status;
}

/**
  * @brief  configures emac irq channel.
  * @param  none
  * @retval none
  */
void emac_nvic_configuration(void)
{
  nvic_irq_enable(EMAC_IRQn, 1, 0);
}

/**
  * @brief  configures emac required pins.
  * @param  none
  * @retval none
  */
void emac_pins_configuration(void)
{
  gpio_init_type gpio_init_struct = {0};

  /* emac pins clock enable */
  crm_periph_clock_enable(ETH_MDC_GPIO_CLK, TRUE);
  crm_periph_clock_enable(ETH_MDIO_GPIO_CLK, TRUE);
  crm_periph_clock_enable(ETH_RMII_TX_EN_GPIO_CLK, TRUE);
  crm_periph_clock_enable(ETH_RMII_TXD0_GPIO_CLK, TRUE);
  crm_periph_clock_enable(ETH_RMII_TXD1_GPIO_CLK, TRUE);
  crm_periph_clock_enable(ETH_RMII_CRS_DV_GPIO_CLK, TRUE);
  crm_periph_clock_enable(ETH_RMII_RXD0_GPIO_CLK, TRUE);
  crm_periph_clock_enable(ETH_RMII_RXD1_GPIO_CLK, TRUE);
  crm_periph_clock_enable(ETH_RMII_REF_CLK_GPIO_CLK, TRUE);

  gpio_pin_mux_config(ETH_MDC_GPIO_PORT,          ETH_MDC_GPIO_PINS_SOURCE,          ETH_MDC_GPIO_MUX);
  gpio_pin_mux_config(ETH_MDIO_GPIO_PORT,         ETH_MDIO_GPIO_PINS_SOURCE,         ETH_MDIO_GPIO_MUX);
  gpio_pin_mux_config(ETH_RMII_TX_EN_GPIO_PORT,   ETH_RMII_TX_EN_GPIO_PINS_SOURCE,   ETH_RMII_TX_EN_GPIO_MUX);
  gpio_pin_mux_config(ETH_RMII_TXD0_GPIO_PORT,    ETH_RMII_TXD0_GPIO_PINS_SOURCE,    ETH_RMII_TXD0_GPIO_MUX);
  gpio_pin_mux_config(ETH_RMII_TXD1_GPIO_PORT,    ETH_RMII_TXD1_GPIO_PINS_SOURCE,    ETH_RMII_TXD1_GPIO_MUX);
  gpio_pin_mux_config(ETH_RMII_CRS_DV_GPIO_PORT,  ETH_RMII_CRS_DV_GPIO_PINS_SOURCE,  ETH_RMII_CRS_DV_GPIO_MUX);
  gpio_pin_mux_config(ETH_RMII_RXD0_GPIO_PORT,    ETH_RMII_RXD0_GPIO_PINS_SOURCE,    ETH_RMII_RXD0_GPIO_MUX);
  gpio_pin_mux_config(ETH_RMII_RXD1_GPIO_PORT,    ETH_RMII_RXD1_GPIO_PINS_SOURCE,    ETH_RMII_RXD1_GPIO_MUX);
  gpio_pin_mux_config(ETH_RMII_REF_CLK_GPIO_PORT, ETH_RMII_REF_CLK_GPIO_PINS_SOURCE, ETH_RMII_REF_CLK_GPIO_MUX);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

  gpio_init_struct.gpio_pins = ETH_MDC_GPIO_PIN;
  gpio_init(ETH_MDC_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = ETH_MDIO_GPIO_PIN;
  gpio_init(ETH_MDIO_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = ETH_RMII_TX_EN_GPIO_PIN;
  gpio_init(ETH_RMII_TX_EN_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = ETH_RMII_TXD0_GPIO_PIN;
  gpio_init(ETH_RMII_TXD0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = ETH_RMII_TXD1_GPIO_PIN;
  gpio_init(ETH_RMII_TXD1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = ETH_RMII_CRS_DV_GPIO_PIN;
  gpio_init(ETH_RMII_CRS_DV_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = ETH_RMII_RXD0_GPIO_PIN;
  gpio_init(ETH_RMII_RXD0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = ETH_RMII_RXD1_GPIO_PIN;
  gpio_init(ETH_RMII_RXD1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = ETH_RMII_REF_CLK_GPIO_PIN;
  gpio_init(ETH_RMII_REF_CLK_GPIO_PORT, &gpio_init_struct);

  #if !CRYSTAL_ON_PHY
  crm_periph_clock_enable(ETH_INPUT_CLK_GPIO_CLK, TRUE);

  gpio_pin_mux_config(ETH_INPUT_CLK_GPIO_PORT, ETH_INPUT_CLK_GPIO_PINS_SOURCE, ETH_INPUT_CLK_GPIO_MUX);

  gpio_init_struct.gpio_pins = ETH_INPUT_CLK_GPIO_PIN;
  gpio_init(ETH_INPUT_CLK_GPIO_PORT, &gpio_init_struct);
  #endif
}

/**
  * @brief  configures emac layer2
  * @param  none
  * @retval error or success
  */
error_status emac_layer2_configuration(void)
{
  emac_control_config_type mac_control_para;
  emac_dma_config_type dma_control_para;
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

  scfg_emac_interface_set(SCFG_EMAC_SELECT_RMII);

  crm_clock_out1_set(CRM_CLKOUT1_PLL);
  crm_clkout_div_set(CRM_CLKOUT_INDEX_1, CRM_CLKOUT_DIV1_5, CRM_CLKOUT_DIV2_2);

  /* reset phy */
  reset_phy();

  /* reset emac ahb bus */
  emac_reset();

  /* software reset emac dma */
  emac_dma_software_reset_set();

  while(emac_dma_software_reset_get() == SET);

  emac_control_para_init(&mac_control_para);

  mac_control_para.auto_nego = EMAC_AUTO_NEGOTIATION_ON;

  if(emac_phy_init(&mac_control_para) == ERROR)
  {
    return ERROR;
  }

  emac_dma_para_init(&dma_control_para);

  dma_control_para.rsf_enable = TRUE;
  dma_control_para.tsf_enable = TRUE;
  dma_control_para.osf_enable = TRUE;
  dma_control_para.aab_enable = TRUE;
  dma_control_para.usp_enable = TRUE;
  dma_control_para.fb_enable = TRUE;
  dma_control_para.flush_rx_disable = TRUE;
  dma_control_para.rx_dma_pal = EMAC_DMA_PBL_32;
  dma_control_para.tx_dma_pal = EMAC_DMA_PBL_32;
  dma_control_para.priority_ratio = EMAC_DMA_2_RX_1_TX;

  emac_dma_config(&dma_control_para);
  emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_NORMAL_SUMMARY, TRUE);
  emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_RX, TRUE);

  return SUCCESS;
}

/**
  * @brief  reset layer 1
  * @param  none
  * @retval none
  */
void static reset_phy(void)
{
  emac_phy_power_on();

  ETH_RESET_LOW();

  delay_ms(2);

  ETH_RESET_HIGH();

  delay_ms(2);
}

/**
  * @brief  reset phy register
  * @param  none
  * @retval SUCCESS or ERROR
  */
error_status emac_phy_register_reset(void)
{
  uint16_t data = 0;
  uint32_t timeout = 0;
  uint32_t i = 0;

  if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_RESET_BIT) == ERROR)
  {
    return ERROR;
  }

  for(i = 0; i < 0x000FFFFF; i++);

  do
  {
    timeout++;
    if(emac_phy_register_read(PHY_ADDRESS, PHY_CONTROL_REG, &data) == ERROR)
    {
      return ERROR;
    }
  } while((data & PHY_RESET_BIT) && (timeout < PHY_TIMEOUT));

  for(i = 0; i < 0x00FFFFF; i++);
  if(timeout == PHY_TIMEOUT)
  {
    return ERROR;
  }
  return SUCCESS;
}

/**
  * @brief  set mac speed related parameters
  * @param  nego: auto negotiation on or off.
  *         this parameter can be one of the following values:
  *         - EMAC_AUTO_NEGOTIATION_OFF
  *         - EMAC_AUTO_NEGOTIATION_ON.
  * @param  mode: half-duplex or full-duplex.
  *         this parameter can be one of the following values:
  *         - EMAC_HALF_DUPLEX
  *         - EMAC_FULL_DUPLEX.
  * @param  speed: 10 mbps or 100 mbps
  *         this parameter can be one of the following values:
  *         - EMAC_SPEED_10MBPS
  *         - EMAC_SPEED_100MBPS.
  * @retval none
  */
error_status emac_speed_config(emac_auto_negotiation_type nego, emac_duplex_type mode, emac_speed_type speed)
{
  uint16_t data = 0;
  uint32_t timeout = 0;
  if(nego == EMAC_AUTO_NEGOTIATION_ON)
  {
    do
    {
      timeout++;
      if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR)
      {
        return ERROR;
      }
    } while(!(data & PHY_LINKED_STATUS_BIT) && (timeout < PHY_TIMEOUT));

    if(timeout == PHY_TIMEOUT)
    {
      return ERROR;
    }

    timeout = 0;

    if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_AUTO_NEGOTIATION_BIT) == ERROR)
    {
      return ERROR;
    }


    do
    {
      timeout++;
      if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR)
      {
        return ERROR;
      }
    } while(!(data & PHY_NEGO_COMPLETE_BIT) && (timeout < PHY_TIMEOUT));

    if(timeout == PHY_TIMEOUT)
    {
      return ERROR;
    }

    if(emac_phy_register_read(PHY_ADDRESS, PHY_SPECIFIED_CS_REG, &data) == ERROR)
    {
      return ERROR;
    }
    #ifdef DM9162
    if(data & PHY_FULL_DUPLEX_100MBPS_BIT)
    {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else if(data & PHY_HALF_DUPLEX_100MBPS_BIT)
    {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
    else if(data & PHY_FULL_DUPLEX_10MBPS_BIT)
    {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else if(data & PHY_HALF_DUPLEX_10MBPS_BIT)
    {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
    #else
    if(data & PHY_DUPLEX_MODE)
    {
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else
    {
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
    if(data & PHY_SPEED_MODE)
    {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
    }
    else
    {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }
    #endif
  }
  else
  {
    if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, (uint16_t)((mode << 8) | (speed << 13))) == ERROR)
    {
      return ERROR;
    }
    if(speed == EMAC_SPEED_100MBPS)
    {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }
    else
    {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
    }
    if(mode == EMAC_FULL_DUPLEX)
    {
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else
    {
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
  }

  return SUCCESS;
}

/**
  * @brief  initialize emac phy
  * @param  none
  * @retval SUCCESS or ERROR
  */
error_status emac_phy_init(emac_control_config_type *control_para)
{
  emac_clock_range_set();
  if(emac_phy_register_reset() == ERROR)
  {
    return ERROR;
  }
  if(emac_speed_config(control_para->auto_nego, control_para->duplex_mode, control_para->fast_ethernet_speed) == ERROR)
  {
    return ERROR;
  }

  emac_control_config(control_para);
  return SUCCESS;
}

/**
  * @brief  power on
  * @param  none
  * @retval none
  */
void emac_phy_power_on(void)
{
  pca9555_out_mode_config(ETH_POWER_ON_PIN);

  pca9555_bits_reset(ETH_POWER_ON_PIN);
}

/**
  * @brief  power off
  * @param  none
  * @retval none
  */
void emac_phy_power_off(void)
{
  pca9555_out_mode_config(ETH_POWER_ON_PIN);

  pca9555_bits_set(ETH_POWER_ON_PIN);
}
