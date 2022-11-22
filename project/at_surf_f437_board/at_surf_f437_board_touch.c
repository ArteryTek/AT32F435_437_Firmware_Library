/**
  **************************************************************************
  * @file     at_surf_f437_board_touch.c
  * @brief    touch application libray header file.
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

#include "at_surf_f437_board_touch.h"
#include "at_surf_f437_board_delay.h"
#include "string.h"

i2c_handle_type hi2c_gt;

/**
  * @brief touch functions definition
  */
touch_dev_type touch_dev_struct =
{
  touch_init,          /*!< function for init touch pin */
  touch_read_xy,       /*!< function for read data from touch */
  touch_scan,          /*!< function for scanning the screen */
  touch_adjust,        /*!< function for adjust the screen */
  TOUCH_SCAN_VERTICAL, /*!< touch scan direction */
  0,
  0,
};

uint8_t cmd_rdx = 0xd0;
uint8_t cmd_rdy = 0x90;

touch_dev_type *touch_struct;

/* gt911 parameter configuration table */
const uint8_t GT911_CFG_TBL[]=
{
  0x41,   /* 0x8047 */
  0x40,   /* 0x8048 */
  0x01,   /* 0x8049 */
  0xE0,   /* 0x804A */
  0x01,   /* 0x804B */
  0x05,   /* 0x804C */
  0x07,   /* 0x804D */
  0x00,   /* 0x804E */
  0x01,   /* 0x804F */
  0x08,   /* 0x8050 */
  0x28,   /* 0x8051 */
  0x05,   /* 0x8052 */
  0x50,   /* 0x8053 */
  0x32,   /* 0x8054 */
  0x03,   /* 0x8055 */
  0x05,   /* 0x8056 */
  0x00,   /* 0x8057 */
  0x00,   /* 0x8058 */
  0x00,   /* 0x8059 */
  0x00,   /* 0x805A */
  0x00,   /* 0x805B */
  0x00,   /* 0x805C */
  0x00,   /* 0x805D */
  0x00,   /* 0x805E */
  0x00,   /* 0x805F */
  0x00,   /* 0x8060 */
  0x00,   /* 0x8061 */
  0x87,   /* 0x8062 */
  0x28,   /* 0x8063 */
  0x0A,   /* 0x8064 */
  0x17,   /* 0x8065 */
  0x15,   /* 0x8066 */
  0x31,   /* 0x8067 */
  0x0D,   /* 0x8068 */
  0x00,   /* 0x8069 */
  0x00,   /* 0x806A */
  0x00,   /* 0x806B */
  0x9A,   /* 0x806C */
  0x04,   /* 0x806D */
  0x25,   /* 0x806E */
  0x00,   /* 0x806F */
  0x00,   /* 0x8070 */
  0x00,   /* 0x8071 */
  0x00,   /* 0x8072 */
  0x00,   /* 0x8073 */
  0x03,   /* 0x8074 */
  0x64,   /* 0x8075 */
  0x32,   /* 0x8076 */
  0x00,   /* 0x8077 */
  0x00,   /* 0x8078 */
  0x00,   /* 0x8079 */
  0x0F,   /* 0x807A */
  0x94,   /* 0x807B */
  0x94,   /* 0x807C */
  0xC5,   /* 0x807D */
  0x02,   /* 0x807E */
  0x07,   /* 0x807F */
  0x00,   /* 0x8080 */
  0x00,   /* 0x8081 */
  0x04,   /* 0x8082 */
  0x8D,   /* 0x8083 */
  0x13,   /* 0x8084 */
  0x00,   /* 0x8085 */
  0x5C,   /* 0x8086 */
  0x1E,   /* 0x8087 */
  0x00,   /* 0x8088 */
  0x3B,   /* 0x8089 */
  0x30,   /* 0x808A */
  0x00,   /* 0x808B */
  0x28,   /* 0x808C */
  0x4C,   /* 0x808D */
  0x00,   /* 0x808E */
  0x1C,   /* 0x808F */
  0x78,   /* 0x8090 */
  0x00,   /* 0x8091 */
  0x1C,   /* 0x8092 */
  0x00,   /* 0x8093 */
  0x00,   /* 0x8094 */
  0x00,   /* 0x8095 */
  0x00,   /* 0x8096 */
  0x00,   /* 0x8097 */
  0x00,   /* 0x8098 */
  0x00,   /* 0x8099 */
  0x00,   /* 0x809A */
  0x00,   /* 0x809B */
  0x00,   /* 0x809C */
  0x00,   /* 0x809D */
  0x00,   /* 0x809E */
  0x00,   /* 0x809F */
  0x00,   /* 0x80A0 */
  0x00,   /* 0x80A1 */
  0x00,   /* 0x80A2 */
  0x00,   /* 0x80A3 */
  0x00,   /* 0x80A4 */
  0x00,   /* 0x80A5 */
  0x00,   /* 0x80A6 */
  0x00,   /* 0x80A7 */
  0x00,   /* 0x80A8 */
  0x00,   /* 0x80A9 */
  0x00,   /* 0x80AA */
  0x00,   /* 0x80AB */
  0x00,   /* 0x80AC */
  0x00,   /* 0x80AD */
  0x00,   /* 0x80AE */
  0x00,   /* 0x80AF */
  0x00,   /* 0x80B0 */
  0x00,   /* 0x80B1 */
  0x00,   /* 0x80B2 */
  0x00,   /* 0x80B3 */
  0x00,   /* 0x80B4 */
  0x00,   /* 0x80B5 */
  0x00,   /* 0x80B6 */
  0x14,   /* 0x80B7 */
  0x12,   /* 0x80B8 */
  0x10,   /* 0x80B9 */
  0x0E,   /* 0x80BA */
  0x0C,   /* 0x80BB */
  0x0A,   /* 0x80BC */
  0x08,   /* 0x80BD */
  0x06,   /* 0x80BE */
  0x04,   /* 0x80BF */
  0x02,   /* 0x80C0 */
  0xFF,   /* 0x80C1 */
  0xFF,   /* 0x80C2 */
  0xFF,   /* 0x80C3 */
  0xFF,   /* 0x80C4 */
  0x00,   /* 0x80C5 */
  0x00,   /* 0x80C6 */
  0x00,   /* 0x80C7 */
  0x00,   /* 0x80C8 */
  0x00,   /* 0x80C9 */
  0x00,   /* 0x80CA */
  0x00,   /* 0x80CB */
  0x00,   /* 0x80CC */
  0x00,   /* 0x80CD */
  0x00,   /* 0x80CE */
  0x00,   /* 0x80CF */
  0x00,   /* 0x80D0 */
  0x00,   /* 0x80D1 */
  0x00,   /* 0x80D2 */
  0x00,   /* 0x80D3 */
  0x00,   /* 0x80D4 */
  0x00,   /* 0x80D5 */
  0x02,   /* 0x80D6 */
  0x04,   /* 0x80D7 */
  0x06,   /* 0x80D8 */
  0x08,   /* 0x80D9 */
  0x0A,   /* 0x80DA */
  0x0C,   /* 0x80DB */
  0x26,   /* 0x80DC */
  0x24,   /* 0x80DD */
  0x22,   /* 0x80DE */
  0x21,   /* 0x80DF */
  0x20,   /* 0x80E0 */
  0x1F,   /* 0x80E1 */
  0x1E,   /* 0x80E2 */
  0x1D,   /* 0x80E3 */
  0xFF,   /* 0x80E4 */
  0xFF,   /* 0x80E5 */
  0xFF,   /* 0x80E6 */
  0xFF,   /* 0x80E7 */
  0xFF,   /* 0x80E8 */
  0xFF,   /* 0x80E9 */
  0xFF,   /* 0x80EA */
  0xFF,   /* 0x80EB */
  0xFF,   /* 0x80EC */
  0xFF,   /* 0x80ED */
  0xFF,   /* 0x80EE */
  0x00,   /* 0x80EF */
  0x00,   /* 0x80F0 */
  0x00,   /* 0x80F1 */
  0x00,   /* 0x80F2 */
  0x00,   /* 0x80F3 */
  0x00,   /* 0x80F4 */
  0x00,   /* 0x80F5 */
  0x00,   /* 0x80F6 */
  0x00,   /* 0x80F7 */
  0x00,   /* 0x80F8 */
  0x00,   /* 0x80F9 */
  0x00,   /* 0x80FA */
  0x00,   /* 0x80FB */
  0x00,   /* 0x80FC */
  0x00,   /* 0x80FD */
  0x00,   /* 0x80FE */

};

/* gt911 point coordinate table */
const uint16_t GT911_TPX_TBL[5] = {GT_TP1_REG, GT_TP2_REG, GT_TP3_REG, GT_TP4_REG, GT_TP5_REG};

uint16_t  gt_error_code;
uint8_t  *gt_pdata;
uint8_t   gt_psize;

/**
  * @brief  i2c peripheral initialization.
  * @param  i2c_x: the handle points to the operation information.
  * @retval none.
  */
void gt_i2c_init(i2c_type *i2c_x)
{
  gpio_init_type gpio_init_structure;

  hi2c_gt.i2cx = i2c_x;

  /* reset i2c peripheral */
  i2c_reset(i2c_x);

  /* i2c periph clock enable */
  crm_periph_clock_enable(GT_I2C_CLK, TRUE);
  crm_periph_clock_enable(GT_SCL_GPIO_CLK, TRUE);
  crm_periph_clock_enable(GT_SDA_GPIO_CLK, TRUE);

  /* gpio configuration */
  gpio_pin_mux_config(GT_SCL_GPIO_PORT, GT_SCL_GPIO_PINS_SOURCE, GT_SCL_GPIO_MUX);

  gpio_pin_mux_config(GT_SDA_GPIO_PORT, GT_SDA_GPIO_PINS_SOURCE, GT_SDA_GPIO_MUX);

  /* configure i2c pins: scl */
  gpio_init_structure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_structure.gpio_mode           = GPIO_MODE_MUX;
  gpio_init_structure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
  gpio_init_structure.gpio_pull           = GPIO_PULL_UP;

  gpio_init_structure.gpio_pins           = GT_SCL_GPIO_PIN;
  gpio_init(GT_SCL_GPIO_PORT, &gpio_init_structure);

  /* configure i2c pins: sda */
  gpio_init_structure.gpio_pins           = GT_SDA_GPIO_PIN;
  gpio_init(GT_SDA_GPIO_PORT, &gpio_init_structure);

  /* config i2c */
  i2c_init(i2c_x, 0x0F, GT_I2C_CLKCTRL_100K);

  /* i2c peripheral enable */
  i2c_enable(i2c_x, TRUE);
}

/**
  * @brief  write data to the memory device through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: memory device address.
  * @param  mem_address: memory address.
  * @param  data: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type gt_i2c_write(i2c_handle_type* hi2c, uint16_t address, uint16_t mem_address, uint8_t* data, uint8_t size, uint32_t timeout)
{
  /* initialization parameters */
  gt_pdata = data;
  gt_psize = size + 2;

  gt_error_code = I2C_OK;

  /* wait for the busy falg to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSYF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_transmit_set(hi2c->i2cx, address, gt_psize, I2C_AUTO_STOP_MODE, I2C_GEN_START_WRITE);

  /* wait for the tdis falg to be set */
  if(i2c_wait_flag(hi2c, I2C_TDIS_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* send memory address */
  i2c_data_send(hi2c->i2cx, mem_address >> 8);


  /* wait for the tdis falg to be set */
  if(i2c_wait_flag(hi2c, I2C_TDIS_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* send memory address */
  i2c_data_send(hi2c->i2cx, mem_address & 0xFF);

  gt_psize -= 2;

  while (gt_psize > 0)
  {
    /* wait for the tdis falg to be set */
    if(i2c_wait_flag(hi2c, I2C_TDIS_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
    {
      return I2C_ERR_STEP_3;
    }

    /* send data */
    i2c_data_send(hi2c->i2cx, *gt_pdata++);
    gt_psize--;
  }

  /* wait for the stop falg to be set  */
  if(i2c_wait_flag(hi2c, I2C_STOPF_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_5;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_FLAG);

  /* reset ctrl2 register */
  i2c_reset_ctrl2_register(hi2c);

  return I2C_OK;
}

/**
  * @brief  read data from memory device through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: memory device address.
  * @param  mem_address: memory address.
  * @param  data: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type gt_i2c_read(i2c_handle_type* hi2c, uint16_t address, uint16_t mem_address, uint8_t* data, uint8_t size, uint32_t timeout)
{
  /* initialization parameters */
  gt_pdata = data;
  gt_psize = size;

  gt_error_code = I2C_OK;

  /* wait for the busy falg to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSYF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_transmit_set(hi2c->i2cx, address, 2, I2C_SOFT_STOP_MODE, I2C_GEN_START_WRITE);

  /* wait for the tdis falg to be set */
  if(i2c_wait_flag(hi2c, I2C_TDIS_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* send memory address */
  i2c_data_send(hi2c->i2cx, mem_address >> 8);

  /* wait for the tdis falg to be set */
  if(i2c_wait_flag(hi2c, I2C_TDIS_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_3;
  }

  /* send memory address */
  i2c_data_send(hi2c->i2cx, mem_address & 0xFF);

  /* wait for the tdc falg to be set */
  if (i2c_wait_flag(hi2c, I2C_TDC_FLAG, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* start transfer */
  i2c_transmit_set(hi2c->i2cx, address, gt_psize, I2C_AUTO_STOP_MODE, I2C_GEN_START_READ);

  while (gt_psize > 0)
  {
    /* wait for the rdbf falg to be set  */
    if (i2c_wait_flag(hi2c, I2C_RDBF_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
    {
      return I2C_ERR_STEP_5;
    }

    /* read data */
    (*gt_pdata++) = i2c_data_receive(hi2c->i2cx);
    gt_psize--;
  }

  /* wait for the stop falg to be set  */
  if (i2c_wait_flag(hi2c, I2C_STOPF_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_7;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_FLAG);

  /* reset ctrl2 register */
  i2c_reset_ctrl2_register(hi2c);

  return I2C_OK;
}

/**
  * @brief  gt911 register write.
  * @param  reg: register address.
  * @param  buf: data buffer.
  * @param  len: data size.
  * @retval function execution result.
  *         - SUCCESS.
  *         - ERROR.
  */
error_status gt911_reg_write(uint16_t reg, uint8_t* buf, uint8_t len)
{
  if(gt_i2c_write(&hi2c_gt, GT_I2C_ADDRESS, reg, buf, len, GT_I2C_TIMEOUT) != I2C_OK)
  {
    return ERROR;
  }
  else
  {
    return SUCCESS;
  }
}

/**
  * @brief  gt911 register read.
  * @param  reg: register address.
  * @param  buf: data buffer.
  * @param  len: data size.
  * @retval function execution result.
  *         - SUCCESS.
  *         - ERROR.
  */
error_status gt9111_reg_read(uint16_t reg, uint8_t* buf, uint8_t len)
{
  if(gt_i2c_read(&hi2c_gt, GT_I2C_ADDRESS, reg, buf, len, GT_I2C_TIMEOUT) != I2C_OK)
  {
    return ERROR;
  }
  else
  {
    return SUCCESS;
  }
}

/**
  * @brief  gt911 configure all register.
  * @param  mode: parameter save mode.
  *         - 0: save to ram.
  *         - 1: save to flash(power-down save).
  * @retval function execution result.
  *         - SUCCESS.
  *         - ERROR.
  */
error_status gt911_send_cfg(uint8_t mode)
{
  uint8_t buf[2];
  uint8_t i = 0;
  buf[0] = 0;
  buf[1] = mode;

  for(i = 0; i < sizeof(GT911_CFG_TBL); i++)
  {
    /* calculate the checksum */
    buf[0] += GT911_CFG_TBL[i];
  }

  buf[0] = (~buf[0]) + 1;

  /* write all register */
  gt911_reg_write(GT_CFG_REG, (uint8_t*)GT911_CFG_TBL, sizeof(GT911_CFG_TBL));

  /* write checksum */
  gt911_reg_write(GT_CHECK_REG, buf, 2);

  return SUCCESS;
}

/**
  * @brief  this function is init touch.
  * @param  direction: touch scan direction
  *         this parameter can be one of the following values:
  *         - TOUCH_SCAN_VERTICAL: vertical scan.
  *         - TOUCH_SCAN_HORIZONTAL: horizontal scan.
  * @retval function execution result.
  *         - SUCCESS.
  *         - ERROR.
  */
error_status touch_init(touch_scan_type direction)
{
  error_status status = ERROR;
  uint8_t temp[5];
  gpio_init_type  gpio_init_struct = {0};

  touch_dev_struct.direction = direction;

  /* enable the gpio clock */
  crm_periph_clock_enable(GT_RST_GPIO_CLK, TRUE);
  crm_periph_clock_enable(GT_INT_GPIO_CLK, TRUE);

  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

  GT_RST_LOW();
  gpio_init_struct.gpio_pins = GT_RST_GPIO_PIN;
  gpio_init(GT_RST_GPIO_PORT, &gpio_init_struct);

  GT_INT_HIGH();
  gpio_init_struct.gpio_pins = GT_INT_GPIO_PIN;
  gpio_init(GT_INT_GPIO_PORT, &gpio_init_struct);

  /* init i2c */
  gt_i2c_init(GT_I2C_PORT);

  /* set RST pin low */
  GT_RST_LOW();

  delay_ms(10);

  /* set RST pin high */
  GT_RST_HIGH();

  delay_ms(10);

  /* set INT pin low */
  GT_INT_LOW();

  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GT_INT_GPIO_PIN;
  gpio_init(GT_INT_GPIO_PORT, &gpio_init_struct);

  delay_ms(100);

  /* read touch id */
  gt9111_reg_read(GT_PID_REG, temp, 4);

  temp[3] = 0;
  temp[4] = 0;

  //printf("CTP ID:%s\r\n", temp);

  if(strcmp((char*)temp, "911") == 0)
  {
    temp[0] = 0x02;

    /* software reset gt911 */
    gt911_reg_write(GT_CTRL_REG, temp, 1);

    /* read GT_CFG_REG register */
     gt9111_reg_read(GT_CFG_REG, temp, 1);

    /* if the default version is relatively low, update the flash configuration */
    if(temp[0] <= 0x41)
    {
      //printf("Default Ver:0x%X\r\n", temp[0]);

      /* update and save configuration */
      gt911_send_cfg(1);
    }

    delay_ms(10);

    temp[0] = 0x00;

    /* end software reset */
    gt911_reg_write(GT_CTRL_REG, temp, 1);

    status = SUCCESS;
  }

  return status;
}

/**
  * @brief  this function is read data from touch.
  * @param  x/y : coordinate vaule.
  * @retval function execution result.
  *         - SUCCESS.
  *         - ERROR.
  */
error_status touch_read_xy(uint16_t *x, uint16_t *y)
{
  uint8_t buf[4];
  uint8_t i = 0;
  uint8_t temp = 0;
  uint8_t mode, num = 0;
  error_status status = ERROR;

  if(GT_INT_READ())
  {
    /* read touch point status */
    gt9111_reg_read(GT_STS_REG, &mode, 1);

    num = (mode & 0xF);

    if((mode & 0x80) && (num < 6))
    {
      /* clear touch flag */
      gt911_reg_write(GT_STS_REG, &temp, 1);

      for(i = 0; i < num; i++)
      {
        /* read x y coordinates */
        gt9111_reg_read(GT911_TPX_TBL[i], buf, 4);

        if(touch_dev_struct.direction == TOUCH_SCAN_HORIZONTAL)
        {
          /* calculate actual coordinates */
          y[i] = 320 - (((uint16_t)buf[1] << 8) + buf[0]);
          x[i] = (((uint16_t)buf[3] << 8) + buf[2]);
        }
        else
        {
          /* calculate actual coordinates */
          x[i] = 320 - (((uint16_t)buf[1] << 8) + buf[0]);
          y[i] = 480 - (((uint16_t)buf[3] << 8) + buf[2]);
        }
      }
    }

    if(num)
    {
      status = SUCCESS;
    }
  }

  return status;
}

/**
  * @brief  this function is scanning the screen.
  * @param  none
  * @retval none
  */
void touch_scan(void)
{

}

/**
  * @brief  this function is adjust the screen.
  * @param  none
  * @retval state
  */
error_status touch_adjust(void)
{
  /* touch adjust code */
  return SUCCESS;
}

