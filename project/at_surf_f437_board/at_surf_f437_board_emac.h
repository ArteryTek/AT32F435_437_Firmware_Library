/**
  **************************************************************************
  * @file     at32_emac.h
  * @brief    header file of emac config program.
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT_SURF_F437_BOARD_EMAC_H
#define __AT_SURF_F437_BOARD_EMAC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"
#include "at_surf_f437_board_pca9555.h"
#include "netif.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_EMAC_http_server
  * @{
  */

#define RX_REMAP                           (1)
#define CRYSTAL_ON_PHY                     (0)


#define DM9162
//#define DP83848

#define LINK_DETECTION                   (1)            /*!< link status detection, 0: no detection, 1: detect with polling */

#if defined (DM9162)
  #define PHY_ADDRESS                      (0x03)       /*!< relative to at32 board */
  #define PHY_CONTROL_REG                  (0x00)       /*!< basic mode control register */
  #define PHY_STATUS_REG                   (0x01)       /*!< basic mode status register */
  #define PHY_SPECIFIED_CS_REG             (0x11)       /*!< specified configuration and status register */
  /* phy control register */
  #define PHY_AUTO_NEGOTIATION_BIT         (0x1000)     /*!< enable auto negotiation */
  #define PHY_LOOPBACK_BIT                 (0x4000)     /*!< enable loopback */
  #define PHY_RESET_BIT                    (0x8000)     /*!< reset phy */
  /* phy status register */
  #define PHY_LINKED_STATUS_BIT            (0x0004)     /*!< link status */
  #define PHY_NEGO_COMPLETE_BIT            (0x0020)     /*!< auto negotiation complete */
  /* phy specified control/status register */
  #define PHY_FULL_DUPLEX_100MBPS_BIT      (0x8000)     /*!< full duplex 100 mbps */
  #define PHY_HALF_DUPLEX_100MBPS_BIT      (0x4000)     /*!< half duplex 100 mbps */
  #define PHY_FULL_DUPLEX_10MBPS_BIT       (0x2000)     /*!< full duplex 10 mbps */
  #define PHY_HALF_DUPLEX_10MBPS_BIT       (0x1000)     /*!< half duplex 10 mbps */
#elif defined (DP83848)
  #define PHY_ADDRESS                      (0x01)       /*!< relative to at32 board */
  #define PHY_CONTROL_REG                  (0x00)       /*!< basic mode control register */
  #define PHY_STATUS_REG                   (0x01)       /*!< basic mode status register */
  #define PHY_SPECIFIED_CS_REG             (0x10)       /*!< phy status register */
  /* phy control register */
  #define PHY_AUTO_NEGOTIATION_BIT         (0x1000)     /*!< enable auto negotiation */
  #define PHY_LOOPBACK_BIT                 (0x4000)     /*!< enable loopback */
  #define PHY_RESET_BIT                    (0x8000)     /*!< reset phy */
  /* phy status register */
  #define PHY_LINKED_STATUS_BIT            (0x0004)     /*!< link status */
  #define PHY_NEGO_COMPLETE_BIT            (0x0020)     /*!< auto negotiation complete */

  #define PHY_DUPLEX_MODE                  (0x0002)     /*!< full duplex mode */
  #define PHY_SPEED_MODE                   (0x0001)     /*!< 10 mbps */
#endif

#define ETH_MDC_GPIO_CLK                   CRM_GPIOC_PERIPH_CLOCK
#define ETH_MDC_GPIO_PIN                   GPIO_PINS_1
#define ETH_MDC_GPIO_PINS_SOURCE           GPIO_PINS_SOURCE1
#define ETH_MDC_GPIO_PORT                  GPIOC
#define ETH_MDC_GPIO_MUX                   GPIO_MUX_11

#define ETH_MDIO_GPIO_CLK                  CRM_GPIOA_PERIPH_CLOCK
#define ETH_MDIO_GPIO_PIN                  GPIO_PINS_2
#define ETH_MDIO_GPIO_PINS_SOURCE          GPIO_PINS_SOURCE2
#define ETH_MDIO_GPIO_PORT                 GPIOA
#define ETH_MDIO_GPIO_MUX                  GPIO_MUX_11

#define ETH_RMII_TX_EN_GPIO_CLK            CRM_GPIOB_PERIPH_CLOCK
#define ETH_RMII_TX_EN_GPIO_PIN            GPIO_PINS_11
#define ETH_RMII_TX_EN_GPIO_PINS_SOURCE    GPIO_PINS_SOURCE11
#define ETH_RMII_TX_EN_GPIO_PORT           GPIOB
#define ETH_RMII_TX_EN_GPIO_MUX            GPIO_MUX_11

#define ETH_RMII_TXD0_GPIO_CLK             CRM_GPIOG_PERIPH_CLOCK
#define ETH_RMII_TXD0_GPIO_PIN             GPIO_PINS_13
#define ETH_RMII_TXD0_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE13
#define ETH_RMII_TXD0_GPIO_PORT            GPIOG
#define ETH_RMII_TXD0_GPIO_MUX             GPIO_MUX_11

#define ETH_RMII_TXD1_GPIO_CLK             CRM_GPIOG_PERIPH_CLOCK
#define ETH_RMII_TXD1_GPIO_PIN             GPIO_PINS_14
#define ETH_RMII_TXD1_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE14
#define ETH_RMII_TXD1_GPIO_PORT            GPIOG
#define ETH_RMII_TXD1_GPIO_MUX             GPIO_MUX_11

#define ETH_RMII_CRS_DV_GPIO_CLK           CRM_GPIOA_PERIPH_CLOCK
#define ETH_RMII_CRS_DV_GPIO_PIN           GPIO_PINS_7
#define ETH_RMII_CRS_DV_GPIO_PINS_SOURCE   GPIO_PINS_SOURCE7
#define ETH_RMII_CRS_DV_GPIO_PORT          GPIOA
#define ETH_RMII_CRS_DV_GPIO_MUX           GPIO_MUX_11

#define ETH_RMII_RXD0_GPIO_CLK             CRM_GPIOC_PERIPH_CLOCK
#define ETH_RMII_RXD0_GPIO_PIN             GPIO_PINS_4
#define ETH_RMII_RXD0_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE4
#define ETH_RMII_RXD0_GPIO_PORT            GPIOC
#define ETH_RMII_RXD0_GPIO_MUX             GPIO_MUX_11

#define ETH_RMII_RXD1_GPIO_CLK             CRM_GPIOC_PERIPH_CLOCK
#define ETH_RMII_RXD1_GPIO_PIN             GPIO_PINS_5
#define ETH_RMII_RXD1_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE5
#define ETH_RMII_RXD1_GPIO_PORT            GPIOC
#define ETH_RMII_RXD1_GPIO_MUX             GPIO_MUX_11

#define ETH_RMII_REF_CLK_GPIO_CLK          CRM_GPIOA_PERIPH_CLOCK
#define ETH_RMII_REF_CLK_GPIO_PIN          GPIO_PINS_1
#define ETH_RMII_REF_CLK_GPIO_PINS_SOURCE  GPIO_PINS_SOURCE1
#define ETH_RMII_REF_CLK_GPIO_PORT         GPIOA
#define ETH_RMII_REF_CLK_GPIO_MUX          GPIO_MUX_11

#define ETH_INPUT_CLK_GPIO_CLK             CRM_GPIOA_PERIPH_CLOCK
#define ETH_INPUT_CLK_GPIO_PIN             GPIO_PINS_8
#define ETH_INPUT_CLK_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE8
#define ETH_INPUT_CLK_GPIO_PORT            GPIOA
#define ETH_INPUT_CLK_GPIO_MUX             GPIO_MUX_0

#define ETH_POWER_ON_PIN                   PCA_IO0_PINS_1

#define ETH_RESET_LOW()
#define ETH_RESET_HIGH()

error_status emac_system_init(void);
void emac_nvic_configuration(void);
void emac_pins_configuration(void);
error_status emac_layer2_configuration(void);
void static reset_phy(void);
error_status emac_phy_register_reset(void);
error_status emac_speed_config(emac_auto_negotiation_type nego, emac_duplex_type mode, emac_speed_type speed);
error_status emac_phy_init(emac_control_config_type *control_para);
void emac_phy_power_on(void);
void emac_phy_power_off(void);

uint16_t link_update(void);
void ethernetif_set_link(void const *argument);
void ethernetif_notify_conn_changed(struct netif *netif);
void ethernetif_update_config(struct netif *netif);
void emac_tmr_init(void);


#ifdef __cplusplus
}
#endif

#endif
