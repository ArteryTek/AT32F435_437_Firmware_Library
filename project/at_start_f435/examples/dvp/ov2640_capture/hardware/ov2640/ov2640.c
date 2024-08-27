#include "ov2640.h"
#include "ov2640cfg.h"
#include "at32f435_437.h"
#include "i2c_application.h"
#include "at32f435_437_board.h"

/** @addtogroup AT32F435_periph_examples
  * @{
  */

/** @addtogroup 435_DVP_ov2640_capture
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

/**
  * @brief  write register to ov2640
  * @param  reg: register write to ov2640
  * @param  data: data write to ov2640
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov2640_reg_write(uint8_t reg, uint8_t data)
{
  uint8_t temp_data[2];
  temp_data[0] = reg;
  temp_data[1] = data;

  if((STA = i2c_master_transmit(&hi2cx, (uint16_t)OV2640_ADDR, (uint8_t*)temp_data, 2, 1000000)) != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

/**
  * @brief  read register from ov2640
  * @param  reg: register read from ov2640
  * @param  data: data read from ov2640
  * @retval error_status: SUCCESS, ERROR
  */
error_status ov2640_reg_read(uint8_t reg, uint8_t *data)
{
  uint8_t temp_data[1];
  temp_data[0] = reg;

  if(i2c_master_transmit(&hi2cx, (uint16_t)OV2640_ADDR, (uint8_t*)temp_data, 1, 10000) != I2C_OK)
  {
    return ERROR;
  }

  if(i2c_master_receive(&hi2cx, (uint16_t)OV2640_ADDR, data, 1, 10000) != I2C_OK)
  {
    return ERROR;
  }

  return SUCCESS;
}

error_status ov2640_init(void)
{
  u16 i=0;
  uint16_t reg;
  uint8_t temp;
  gpio_init_type gpio_initure;

  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  gpio_default_para_init(&gpio_initure);
  /* DVP_RST Pin-PD6 */
  gpio_initure.gpio_pins = GPIO_PINS_3 | GPIO_PINS_1;
  gpio_initure.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_initure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initure.gpio_pull = GPIO_PULL_UP;
  gpio_initure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOC, &gpio_initure);

  OV2640_RST_LOW;    //POWER ON;
  delay_ms(10);
  OV2640_PWRON_LOW;
  delay_ms(10);
  OV2640_RST_HIGH;
  delay_ms(10);

  ov2640_i2c_init();
  ov2640_reg_write(OV2640_DSP_RA_DLMT, 0x01);
   ov2640_reg_write(OV2640_SENSOR_COM7, 0x80);
  delay_ms(50);
  ov2640_reg_read(OV2640_SENSOR_MIDH, &temp);
  reg  = (uint16_t)temp << 8;
  ov2640_reg_read(OV2640_SENSOR_MIDL, &temp);
  reg |= temp;
  if(reg!=OV2640_MID)
  {
    return ERROR;
  }
  ov2640_reg_read(OV2640_SENSOR_PIDH, &temp);
  reg  = (uint16_t)temp << 8;
  ov2640_reg_read(OV2640_SENSOR_PIDL, &temp);
  reg |= temp;
  if(reg!=OV2640_PID)
  {
    return ERROR;
  }
  for(i=0;i<sizeof(ov2640_svga_init_reg_tbl)/2;i++)
  {
       ov2640_reg_write(ov2640_svga_init_reg_tbl[i][0],ov2640_svga_init_reg_tbl[i][1]);
   }
  return SUCCESS;   //ok
}

//OV2640 jpeg mode
void ov2640_jpeg_mode(void)
{
  u16 i=0;

  for(i=0;i<(sizeof(ov2640_yuv422_reg_tbl)/2);i++)
  {
    ov2640_reg_write(ov2640_yuv422_reg_tbl[i][0],ov2640_yuv422_reg_tbl[i][1]);
  }

  for(i=0;i<(sizeof(ov2640_jpeg_reg_tbl)/2);i++)
  {
    ov2640_reg_write(ov2640_jpeg_reg_tbl[i][0],ov2640_jpeg_reg_tbl[i][1]);
  }
}
//OV2640 RGB565 mode
void ov2640_rgb565_mode(void)
{
  u16 i=0;
  for(i=0;i<(sizeof(ov2640_rgb565_reg_tbl)/2);i++)
  {
    ov2640_reg_write(ov2640_rgb565_reg_tbl[i][0],ov2640_rgb565_reg_tbl[i][1]);
  }
}

const static u8 OV2640_AUTOEXPOSURE_LEVEL[5][8]=
{
  {
    0xFF,0x01,
    0x24,0x20,
    0x25,0x18,
    0x26,0x60,
  },
  {
    0xFF,0x01,
    0x24,0x34,
    0x25,0x1c,
    0x26,0x00,
  },
  {
    0xFF,0x01,
    0x24,0x3e,
    0x25,0x38,
    0x26,0x81,
  },
  {
    0xFF,0x01,
    0x24,0x48,
    0x25,0x40,
    0x26,0x81,
  },
  {
    0xFF,0x01,
    0x24,0x58,
    0x25,0x50,
    0x26,0x92,
  },
};

//level:0~4
void ov2640_auto_exposure(u8 level)
{
  u8 i;
  u8 *p=(u8*)OV2640_AUTOEXPOSURE_LEVEL[level];
  for(i=0;i<4;i++)
  {
    ov2640_reg_write(p[i*2],p[i*2+1]);
  }
}

//0:auto
//1:sunny
//2:cloudy
//3:office
//4:home
void ov2640_light_mode(u8 mode)
{
  u8 regccval=0X5E;//Sunny
  u8 regcdval=0X41;
  u8 regceval=0X54;
  switch(mode)
  {
    case 0://auto
      ov2640_reg_write(0XFF,0X00);
      ov2640_reg_write(0XC7,0X10);//AWB ON
      return;
    case 2://cloudy
      regccval=0X65;
      regcdval=0X41;
      regceval=0X4F;
      break;
    case 3://office
      regccval=0X52;
      regcdval=0X41;
      regceval=0X66;
      break;
    case 4://home
      regccval=0X42;
      regcdval=0X3F;
      regceval=0X71;
      break;
  }
  ov2640_reg_write(0XFF,0X00);
  ov2640_reg_write(0XC7,0X40);  //AWB OFF
  ov2640_reg_write(0XCC,regccval);
  ov2640_reg_write(0XCD,regcdval);
  ov2640_reg_write(0XCE,regceval);
}

//0:-2
//1:-1
//2,0
//3,+1
//4,+2
void ov2640_color_saturation(u8 sat)
{
  u8 reg7dval=((sat+2)<<4)|0X08;
  ov2640_reg_write(0XFF,0X00);
  ov2640_reg_write(0X7C,0X00);
  ov2640_reg_write(0X7D,0X02);
  ov2640_reg_write(0X7C,0X03);
  ov2640_reg_write(0X7D,reg7dval);
  ov2640_reg_write(0X7D,reg7dval);
}

//0:(0X00)-2
//1:(0X10)-1
//2,(0X20) 0
//3,(0X30)+1
//4,(0X40)+2
void ov2640_brightness(u8 bright)
{
  ov2640_reg_write(0xff, 0x00);
  ov2640_reg_write(0x7c, 0x00);
  ov2640_reg_write(0x7d, 0x04);
  ov2640_reg_write(0x7c, 0x09);
  ov2640_reg_write(0x7d, bright<<4);
  ov2640_reg_write(0x7d, 0x00);
}

//0:-2
//1:-1
//2,0
//3,+1
//4,+2
void ov2640_contrast(u8 contrast)
{
  u8 reg7d0val=0X20;
  u8 reg7d1val=0X20;
    switch(contrast)
  {
    case 0://-2
      reg7d0val=0X18;
      reg7d1val=0X34;
      break;
    case 1://-1
      reg7d0val=0X1C;
      reg7d1val=0X2A;
      break;
    case 3://1
      reg7d0val=0X24;
      reg7d1val=0X16;
      break;
    case 4://2
      reg7d0val=0X28;
      reg7d1val=0X0C;
      break;
  }
  ov2640_reg_write(0xff,0x00);
  ov2640_reg_write(0x7c,0x00);
  ov2640_reg_write(0x7d,0x04);
  ov2640_reg_write(0x7c,0x07);
  ov2640_reg_write(0x7d,0x20);
  ov2640_reg_write(0x7d,reg7d0val);
  ov2640_reg_write(0x7d,reg7d1val);
  ov2640_reg_write(0x7d,0x06);
}

void ov2640_special_effects(u8 eft)
{
  u8 reg7d0val=0X00;
  u8 reg7d1val=0X80;
  u8 reg7d2val=0X80;
  switch(eft)
  {
    case 1://negative film
      reg7d0val=0X40;
      break;
    case 2://black and white
      reg7d0val=0X18;
      break;
    case 3://red
      reg7d0val=0X18;
      reg7d1val=0X40;
      reg7d2val=0XC0;
      break;
    case 4://green
      reg7d0val=0X18;
      reg7d1val=0X40;
      reg7d2val=0X40;
      break;
    case 5://blue
      reg7d0val=0X18;
      reg7d1val=0XA0;
      reg7d2val=0X40;
      break;
    case 6://retro
      reg7d0val=0X18;
      reg7d1val=0X40;
      reg7d2val=0XA6;
      break;
  }
  ov2640_reg_write(0xff,0x00);
  ov2640_reg_write(0x7c,0x00);
  ov2640_reg_write(0x7d,reg7d0val);
  ov2640_reg_write(0x7c,0x05);
  ov2640_reg_write(0x7d,reg7d1val);
  ov2640_reg_write(0x7d,reg7d2val);
}

void ov2640_color_bar(u8 sw)
{
  u8 reg;
  ov2640_reg_write(0XFF,0X01);
  ov2640_reg_read(0X12, &reg);
  reg&=~(1<<1);
  if(sw)reg|=1<<1;
  ov2640_reg_write(0X12,reg);
}

void ov2640_window_set(u16 sx,u16 sy,u16 width,u16 height)
{
  u16 endx;
  u16 endy;
  u8 temp;
  endx=sx+width/2;  //V*2
   endy=sy+height/2;

  ov2640_reg_write(0XFF,0X01);
  ov2640_reg_read(0X03, &temp);
  temp&=0XF0;
  temp|=((endy&0X03)<<2)|(sy&0X03);
  ov2640_reg_write(0X03,temp);
  ov2640_reg_write(0X19,sy>>2);
  ov2640_reg_write(0X1A,endy>>2);

  ov2640_reg_read(0X32, &temp);
  temp&=0XC0;
  temp|=((endx&0X07)<<3)|(sx&0X07);
  ov2640_reg_write(0X32,temp);
  ov2640_reg_write(0X17,sx>>3);
  ov2640_reg_write(0X18,endx>>3);
}

u8 ov2640_outsize_set(u16 width,u16 height)
{
  u16 outh;
  u16 outw;
  u8 temp;
  if(width%4)return 1;
  if(height%4)return 2;
  outw=width/4;
  outh=height/4;
  ov2640_reg_write(0XFF,0X00);
  ov2640_reg_write(0XE0,0X04);
  ov2640_reg_write(0X5A,outw&0XFF);
  ov2640_reg_write(0X5B,outh&0XFF);
  temp=(outw>>8)&0X03;
  temp|=(outh>>6)&0X04;
  ov2640_reg_write(0X5C,temp);
  ov2640_reg_write(0XE0,0X00);
  return 0;
}

u8 ov2640_imagewin_set(u16 offx,u16 offy,u16 width,u16 height)
{
  u16 hsize;
  u16 vsize;
  u8 temp;
  if(width%4)return 1;
  if(height%4)return 2;
  hsize=width/4;
  vsize=height/4;
  ov2640_reg_write(0XFF,0X00);
  ov2640_reg_write(0XE0,0X04);
  ov2640_reg_write(0X51,hsize&0XFF);
  ov2640_reg_write(0X52,vsize&0XFF);
  ov2640_reg_write(0X53,offx&0XFF);
  ov2640_reg_write(0X54,offy&0XFF);
  temp=(vsize>>1)&0X80;
  temp|=(offy>>4)&0X70;
  temp|=(hsize>>5)&0X08;
  temp|=(offx>>8)&0X07;
  ov2640_reg_write(0X55,temp);
  ov2640_reg_write(0X57,(hsize>>2)&0X80);
  ov2640_reg_write(0XE0,0X00);
  return 0;
}

u8 ov2640_imagesize_set(u16 width,u16 height)
{
  u8 temp;
  ov2640_reg_write(0XFF,0X00);
  ov2640_reg_write(0XE0,0X04);
  ov2640_reg_write(0XC0,(width)>>3&0XFF);
  ov2640_reg_write(0XC1,(height)>>3&0XFF);
  temp=(width&0X07)<<3;
  temp|=height&0X07;
  temp|=(width>>4)&0X80;
  ov2640_reg_write(0X8C,temp);
  ov2640_reg_write(0XE0,0X00);
  return 0;
}

/**
  * @brief  ov2640 i2c initialize
  * @param  none
  * @retval none
  */
void ov2640_i2c_init(void)
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
