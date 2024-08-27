/**
  **************************************************************************
  * @file     ov5640.c
  * @brief    ov5640 program
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

#include "at32f435_437.h"
#include "at32f435_437_board.h"
#include "ov5640.h"
#include "ov5640cfg.h"
#include "ov5640af.h"
#include "i2c_application.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_DVP_ov5640_capture
  * @{
  */

#define I2C_TIMEOUT                      0x2FFFFFF

#define I2Cx_CLKCTRL                     0x30F05292
#define I2Cx_ADDRESS                     0xA0

#define I2Cx_PORT                        I2C1
#define I2Cx_CLK                         CRM_I2C1_PERIPH_CLOCK
#define I2Cx_DMA                         DMA1
#define I2Cx_DMA_CLK                     CRM_DMA1_PERIPH_CLOCK

#define I2Cx_SCL_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK
#define I2Cx_SCL_GPIO_PIN                GPIO_PINS_6
#define I2Cx_SCL_GPIO_PinsSource         GPIO_PINS_SOURCE6
#define I2Cx_SCL_GPIO_PORT               GPIOB
#define I2Cx_SCL_GPIO_AF                 GPIO_MUX_4

#define I2Cx_SDA_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK
#define I2Cx_SDA_GPIO_PIN                GPIO_PINS_7
#define I2Cx_SDA_GPIO_PinsSource         GPIO_PINS_SOURCE7
#define I2Cx_SDA_GPIO_PORT               GPIOB
#define I2Cx_SDA_GPIO_AF                 GPIO_MUX_4

i2c_handle_type hi2cx;
i2c_status_type STA;

const uint8_t OV5640_EXPOSURE_TBL[7][6] =
{
  0x10, 0x08, 0x10, 0x08, 0x20, 0x10, /* -3 */
  0x20, 0x18, 0x41, 0x20, 0x18, 0x10, /* -2 */
  0x30, 0x28, 0x61, 0x30, 0x28, 0x10, /* -1 */
  0x38, 0x30, 0x61, 0x38, 0x30, 0x10, /*  0 */
  0x40, 0x38, 0x71, 0x40, 0x38, 0x10, /* +1 */
  0x50, 0x48, 0x90, 0x50, 0x48, 0x20, /* +2 */
  0x60, 0x58, 0xa0, 0x60, 0x58, 0x20, /* +3 */
};

const uint8_t OV5640_LIGHTMODE_TBL[5][7] =
{
  0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, /* auto */
  0x06, 0x1C, 0x04, 0x00, 0x04, 0xF3, 0x01, /* sunny */
  0x05, 0x48, 0x04, 0x00, 0x07, 0xCF, 0x01, /* office */
  0x06, 0x48, 0x04, 0x00, 0x04, 0xD3, 0x01, /* cloudy */
  0x04, 0x10, 0x04, 0x00, 0x08, 0x40, 0x01, /* home */
};

const uint8_t OV5640_SATURATION_TBL[7][6] =
{
  0x0C, 0x30, 0x3D, 0x3E, 0x3D, 0x01, /* -3 */
  0x10, 0x3D, 0x4D, 0x4E, 0x4D, 0x01, /* -2 */
  0x15, 0x52, 0x66, 0x68, 0x66, 0x02, /* -1 */
  0x1A, 0x66, 0x80, 0x82, 0x80, 0x02, /* +0 */
  0x1F, 0x7A, 0x9A, 0x9C, 0x9A, 0x02, /* +1 */
  0x24, 0x8F, 0xB3, 0xB6, 0xB3, 0x03, /* +2 */
  0x2B, 0xAB, 0xD6, 0xDA, 0xD6, 0x04, /* +3 */
};

const uint8_t OV5640_EFFECTS_TBL[7][3] =
{
  0x06, 0x40, 0x10,
  0x1E, 0xA0, 0x40,
  0x1E, 0x80, 0xC0,
  0x1E, 0x80, 0x80,
  0x1E, 0x40, 0xA0,
  0x40, 0x40, 0x10,
  0x1E, 0x60, 0x60,
};

/**
  * @brief  write register to ov5640
  * @param  reg: register write to ov5640
  * @param  data: data write to ov5640
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov5640_reg_write(uint16_t reg, uint8_t data)
{
  uint8_t temp_data[3];
  temp_data[0] = reg >> 8;
  temp_data[1] = reg;
  temp_data[2] = data;

  if((STA = i2c_master_transmit(&hi2cx, (uint16_t)OV5640_ADDR, (uint8_t*)temp_data, 3, 1000000)) != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  read register from ov5640
  * @param  reg: register read from ov5640
  * @param  data: data read from ov5640
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov5640_reg_read(uint16_t reg, uint8_t *data)
{
  uint8_t temp_data[2];
  temp_data[0] = reg >> 8;
  temp_data[1] = reg;

  if(i2c_master_transmit(&hi2cx, (uint16_t)OV5640_ADDR, (uint8_t*)temp_data, 2, 10000) != I2C_OK)
  {
    return ERROR;
  }

  if(i2c_master_receive(&hi2cx, (uint16_t)OV5640_ADDR, data, 1, 10000) != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  initialize ov5640
  * @param  none
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov5640_init(void)
{
  uint16_t i = 0;
  uint8_t temp = 0;
  uint16_t reg;
  gpio_init_type gpio_initure;

  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  gpio_default_para_init(&gpio_initure);
  /* DVP_RST Pin-PD6 */
  gpio_initure.gpio_pins = GPIO_PINS_3;
  gpio_initure.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_initure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initure.gpio_pull = GPIO_PULL_UP;
  gpio_initure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOC, &gpio_initure);

  OV5640_RST_LOW;
  delay_ms(100);
  OV5640_RST_HIGH;
  delay_ms(20);
  ov5640_i2c_init();
  delay_ms(5);
  ov5640_reg_read(OV5640_CHIPIDH, &temp);
  reg  = (uint16_t)temp << 8;
  ov5640_reg_read(OV5640_CHIPIDL, &temp);
  reg |= temp;

  if(reg != OV5640_ID)
  {
    return ERROR;
  }

  ov5640_reg_write(0x3103, 0x11);
  ov5640_reg_write(0x3008, 0x82);
  delay_ms(10);

  for(i = 0; i < sizeof(ov5640_uxga_init_reg_tbl) / 4; i++)
  {
    ov5640_reg_write(ov5640_uxga_init_reg_tbl[i][0], ov5640_uxga_init_reg_tbl[i][1]);
  }

#if LIGHT_SWITCH
  ov5640_flash_ctrl(1);
  delay_ms(500);
  ov5640_flash_ctrl(0);
#endif

  return SUCCESS;
}

/**
  * @brief  ov5640 jpeg mode configure
  * @param  none
  * @retval none
  */
void ov5640_jpeg_mode(void)
{
  uint16_t i = 0;

  for(i = 0; i < (sizeof(OV5640_jpeg_reg_tbl) / 4); i++)
  {
    ov5640_reg_write(OV5640_jpeg_reg_tbl[i][0], OV5640_jpeg_reg_tbl[i][1]);
  }
}

/**
  * @brief  ov5640 rgb565 mode configure
  * @param  none
  * @retval none
  */
void ov5640_rgb565_mode(void)
{
  uint16_t i = 0;

  for(i = 0; i < (sizeof(ov5640_rgb565_reg_tbl) / 4); i++)
  {
    ov5640_reg_write(ov5640_rgb565_reg_tbl[i][0], ov5640_rgb565_reg_tbl[i][1]);
  }
}

/**
  * @brief  ov5640 exposure configure
  * @param  exposure: exposure value
  * @retval none
  */
void ov5640_exposure(uint8_t exposure)
{
  ov5640_reg_write(0x3212, 0x03);
  ov5640_reg_write(0x3a0f, OV5640_EXPOSURE_TBL[exposure][0]);
  ov5640_reg_write(0x3a10, OV5640_EXPOSURE_TBL[exposure][1]);
  ov5640_reg_write(0x3a1b, OV5640_EXPOSURE_TBL[exposure][2]);
  ov5640_reg_write(0x3a1e, OV5640_EXPOSURE_TBL[exposure][3]);
  ov5640_reg_write(0x3a11, OV5640_EXPOSURE_TBL[exposure][4]);
  ov5640_reg_write(0x3a1f, OV5640_EXPOSURE_TBL[exposure][5]);
  ov5640_reg_write(0x3212, 0x13);
  ov5640_reg_write(0x3212, 0xa3);
}

/**
  * @brief  this function is draw point to lcd
  * @param  mode : the light mode.
  *         0:auto
  *         1:sunny
  *         2,office
  *         3,cloudy
  *         4,home
  * @retval none
  */
void ov5640_light_mode(uint8_t mode)
{
  uint8_t i;
  ov5640_reg_write(0x3212, 0x03);

  for(i = 0; i < 7; i++)
  {
    ov5640_reg_write(0x3400 + i, OV5640_LIGHTMODE_TBL[mode][i]);
  }

  ov5640_reg_write(0x3212, 0x13);
  ov5640_reg_write(0x3212, 0xa3);
}

/**
  * @brief  ov5640 color saturation configure
  * @param  sat: color saturation value
  * @retval none
  */
void ov5640_color_saturation(uint8_t sat)
{
  uint8_t i;
  ov5640_reg_write(0x3212, 0x03);
  ov5640_reg_write(0x5381, 0x1c);
  ov5640_reg_write(0x5382, 0x5a);
  ov5640_reg_write(0x5383, 0x06);

  for(i = 0; i < 6; i++)
  {
    ov5640_reg_write(0x5384 + i, OV5640_SATURATION_TBL[sat][i]);
  }

  ov5640_reg_write(0x538b, 0x98);
  ov5640_reg_write(0x538a, 0x01);
  ov5640_reg_write(0x3212, 0x13);
  ov5640_reg_write(0x3212, 0xa3);
}

/**
  * @brief  ov5640 brightness configure
  * @param  bright: brightness value
  * @retval none
  */
void ov5640_brightness(uint8_t bright)
{
  uint8_t brtval;

  if(bright < 4)
  {
    brtval = 4 - bright;
  }
  else
  {
    brtval = bright - 4;
  }

  ov5640_reg_write(0x3212, 0x03);
  ov5640_reg_write(0x5587, brtval << 4);

  if(bright < 4)
  {
    ov5640_reg_write(0x5588, 0x09);
  }
  else
  {
    ov5640_reg_write(0x5588, 0x01);
  }

  ov5640_reg_write(0x3212, 0x13);
  ov5640_reg_write(0x3212, 0xa3);
}

/**
  * @brief  ov5640 contrast configure
  * @param  contrast: contrast value
  * @retval none
  */
void ov5640_contrast(uint8_t contrast)
{
  uint8_t reg0val = 0x00;
  uint8_t reg1val = 0x20;

  switch(contrast)
  {
    case 0:
      reg1val = reg0val = 0x14;
      break;

    case 1:
      reg1val = reg0val = 0x18;
      break;

    case 2:
      reg1val = reg0val = 0x1C;
      break;

    case 4:
      reg0val = 0x10;
      reg1val = 0x24;
      break;

    case 5:
      reg0val = 0x18;
      reg1val = 0x28;
      break;

    case 6:
      reg0val = 0x1C;
      reg1val = 0x2C;
      break;
  }

  ov5640_reg_write(0x3212, 0x03);
  ov5640_reg_write(0x5585, reg0val);
  ov5640_reg_write(0x5586, reg1val);
  ov5640_reg_write(0x3212, 0x13);
  ov5640_reg_write(0x3212, 0xa3);
}

/**
  * @brief  ov5640 sharpness configure
  * @param  sharp: sharpness value
  * @retval none
  */
void ov5640_sharpness(uint8_t sharp)
{
  if(sharp < 33)
  {
    ov5640_reg_write(0x5308, 0x65);
    ov5640_reg_write(0x5302, sharp);
  }
  else
  {
    ov5640_reg_write(0x5308, 0x25);
    ov5640_reg_write(0x5300, 0x08);
    ov5640_reg_write(0x5301, 0x30);
    ov5640_reg_write(0x5302, 0x10);
    ov5640_reg_write(0x5303, 0x00);
    ov5640_reg_write(0x5309, 0x08);
    ov5640_reg_write(0x530a, 0x30);
    ov5640_reg_write(0x530b, 0x04);
    ov5640_reg_write(0x530c, 0x06);
  }

}

/**
  * @brief  ov5640 special effects configure
  * @param  eft: special effects value
  * @retval none
  */
void ov5640_special_effects(uint8_t eft)
{
  ov5640_reg_write(0x3212, 0x03);
  ov5640_reg_write(0x5580, OV5640_EFFECTS_TBL[eft][0]);
  ov5640_reg_write(0x5583, OV5640_EFFECTS_TBL[eft][1]);
  ov5640_reg_write(0x5584, OV5640_EFFECTS_TBL[eft][2]);
  ov5640_reg_write(0x5003, 0x08);
  ov5640_reg_write(0x3212, 0x13);
  ov5640_reg_write(0x3212, 0xa3);
}

/**
  * @brief  ov5640 flash control
  * @param  sw: switch on / switch off the flash
  * @retval none
  */
void ov5640_flash_ctrl(uint8_t sw)
{
  ov5640_reg_write(0x3016, 0x02);
  ov5640_reg_write(0x301C, 0x02);

  if(sw)
  {
    ov5640_reg_write(0x3019, 0x02);
  }
  else
  {
    ov5640_reg_write(0x3019, 0x00);
  }
}

/**
  * @brief  ov5640 output size set
  * @param  offx: image offset x
  * @param  offy: image offset y
  * @param  width: output width
  * @param  height: output height
  * @retval none
  */
void ov5640_outsize_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
  ov5640_reg_write(0x3212, 0x03);
  ov5640_reg_write(0x3808, width >> 8);
  ov5640_reg_write(0x3809, width & 0xFF);
  ov5640_reg_write(0x380a, height >> 8);
  ov5640_reg_write(0x380b, height & 0xFF);

  ov5640_reg_write(0x3810, offx >> 8);
  ov5640_reg_write(0x3811, offx & 0xFF);

  ov5640_reg_write(0x3812, offy >> 8);
  ov5640_reg_write(0x3813, offy & 0xFF);

  ov5640_reg_write(0x3212, 0x13);
  ov5640_reg_write(0x3212, 0xA3);
}

/**
  * @brief  ov5640 image window set
  * @param  offx: image offset x
  * @param  offy: image offset y
  * @param  width: output width
  * @param  height: output height
  * @retval none
  */
void ov5640_imagewin_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
  uint16_t xst, yst, xend, yend;
  xst = offx;
  yst = offy;
  xend = offx + width - 1;
  yend = offy + height - 1;
  ov5640_reg_write(0x3212, 0x03);
  ov5640_reg_write(0x3800, xst >> 8);
  ov5640_reg_write(0x3801, xst & 0xFF);
  ov5640_reg_write(0x3802, yst >> 8);
  ov5640_reg_write(0x3803, yst & 0xFF);
  ov5640_reg_write(0x3804, xend >> 8);
  ov5640_reg_write(0x3805, xend & 0xFF);
  ov5640_reg_write(0x3806, yend >> 8);
  ov5640_reg_write(0x3807, yend & 0xFF);
  ov5640_reg_write(0x3212, 0x13);
  ov5640_reg_write(0x3212, 0xA3);
}

/**
  * @brief  initialize ov5640 focus function
  * @param  none
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov5640_focus_init(void)
{
  uint16_t i;
  uint16_t addr = 0x8000;
  uint8_t state = 0x8F;
  ov5640_reg_write(0x3000, 0x20);

  for(i = 0; i < sizeof(OV5640_AF_Config); i++)
  {
    ov5640_reg_write(addr, OV5640_AF_Config[i]);
    addr++;
  }

  ov5640_reg_write(0x3022, 0x00);
  ov5640_reg_write(0x3023, 0x00);
  ov5640_reg_write(0x3024, 0x00);
  ov5640_reg_write(0x3025, 0x00);
  ov5640_reg_write(0x3026, 0x00);
  ov5640_reg_write(0x3027, 0x00);
  ov5640_reg_write(0x3028, 0x00);
  ov5640_reg_write(0x3029, 0x7f);
  ov5640_reg_write(0x3000, 0x00);
  i = 0;

  do
  {
    ov5640_reg_read(0x3029, &state);
    delay_ms(5);
    i++;

    if(i > 1000)
    {
      return ERROR;
    }
  }
  while(state != 0x70);

  return SUCCESS;
}

/**
  * @brief  ov5640 focus single
  * @param  none
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov5640_focus_single(void)
{
  uint8_t temp;
  uint16_t retry = 0;
  ov5640_reg_write(0x3022, 0x03);

  while(1)
  {
    retry++;
    ov5640_reg_read(0x3029, &temp);

    if(temp == 0x10)
    {
      break;
    }

    delay_ms(5);

    if(retry > 1000)
    {
      return SUCCESS;
    }
  }

  return ERROR;
}

/**
  * @brief  ov5640 focus constant
  * @param  none
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov5640_focus_constant(void)
{
  uint8_t temp = 0;
  uint16_t retry = 0;
  ov5640_reg_write(0x3023, 0x01);
  ov5640_reg_write(0x3022, 0x08);

  do
  {
    ov5640_reg_read(0x3023, &temp);
    retry++;

    if(retry > 1000)
    {
      return ERROR;
    }

    delay_ms(5);
  }
  while(temp != 0x00);

  ov5640_reg_write(0x3023, 0x01);
  ov5640_reg_write(0x3022, 0x04);
  retry = 0;

  do
  {
    ov5640_reg_read(0x3023, &temp);
    retry++;

    if(retry > 1000)
    {
      return ERROR;
    }

    delay_ms(5);
  }
  while(temp != 0x00);

  return SUCCESS;
}

/**
  * @brief  ov5640 i2c initialize
  * @param  none
  * @retval none
  */
void ov5640_i2c_init(void)
{
  hi2cx.i2cx = I2C2;
  i2c_config(&hi2cx);
}

/**
  * @brief  initializes peripherals used by the i2c.
  * @param  hi2c: i2c_handle_type
  * @retval none
  */
void i2c_lowlevel_init(i2c_handle_type* hi2c)
{
  gpio_init_type gpio_init_structure;

  /* i2c periph clock enable */
  crm_periph_clock_enable(CRM_I2C2_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOH_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);

  /* gpio configuration */
  gpio_pin_mux_config(GPIOH, GPIO_PINS_SOURCE2, GPIO_MUX_4);

  gpio_pin_mux_config(GPIOH, GPIO_PINS_SOURCE3, GPIO_MUX_4);

  gpio_default_para_init(&gpio_init_structure);
  /* configure i2c pins: scl */
  gpio_init_structure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_structure.gpio_mode           = GPIO_MODE_MUX;
  gpio_init_structure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
  gpio_init_structure.gpio_pull           = GPIO_PULL_UP;
  gpio_init_structure.gpio_pins           = GPIO_PINS_2;
  gpio_init(GPIOH, &gpio_init_structure);

  gpio_init_structure.gpio_pins           = GPIO_PINS_3;
  gpio_init(GPIOH, &gpio_init_structure);

  /* configure and enable i2c interrupt */
  nvic_irq_enable(I2C2_EVT_IRQn, 0, 0);
  nvic_irq_enable(I2C2_ERR_IRQn, 0, 0);

  /* configure and enable i2c dma channel interrupt */
  nvic_irq_enable(DMA1_Channel1_IRQn, 0, 0);
  nvic_irq_enable(DMA1_Channel2_IRQn, 0, 0);

  /* i2c dma tx and rx channels configuration */
  /* enable the dma clock */

  /* i2c dma channel configuration */
  hi2c->dma_tx_channel = DMA1_CHANNEL1;
  hi2c->dma_rx_channel = DMA1_CHANNEL2;

  dma_reset(hi2c->dma_tx_channel);
  dma_reset(hi2c->dma_rx_channel);

  hi2c->dma_init_struct.peripheral_base_addr    = (uint32_t)&hi2c->i2cx->txdt;
  hi2c->dma_init_struct.memory_base_addr        = 0;
  hi2c->dma_init_struct.direction               = DMA_DIR_MEMORY_TO_PERIPHERAL;
  hi2c->dma_init_struct.buffer_size             = 0xFFFF;
  hi2c->dma_init_struct.peripheral_inc_enable   = FALSE;
  hi2c->dma_init_struct.memory_inc_enable       = TRUE;
  hi2c->dma_init_struct.peripheral_data_width   = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  hi2c->dma_init_struct.memory_data_width       = DMA_MEMORY_DATA_WIDTH_BYTE;
  hi2c->dma_init_struct.loop_mode_enable        = FALSE;
  hi2c->dma_init_struct.priority                = DMA_PRIORITY_LOW;

  dma_init(hi2c->dma_tx_channel, &hi2c->dma_init_struct);
  dma_init(hi2c->dma_rx_channel, &hi2c->dma_init_struct);

  dmamux_init(DMA1MUX_CHANNEL1, DMAMUX_DMAREQ_ID_I2C1_TX);
  dmamux_init(DMA1MUX_CHANNEL2, DMAMUX_DMAREQ_ID_I2C1_RX);

  dmamux_enable(DMA1, TRUE);

  /* config i2c */
  i2c_init(hi2c->i2cx, 0x0F, I2Cx_CLKCTRL);

  i2c_own_address1_set(hi2c->i2cx, I2C_ADDRESS_MODE_7BIT, I2Cx_ADDRESS);
}

/**
  * @}
  */

/**
  * @}
  */
