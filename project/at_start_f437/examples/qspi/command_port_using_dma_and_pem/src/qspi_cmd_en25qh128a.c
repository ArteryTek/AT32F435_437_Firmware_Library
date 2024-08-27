/**
  **************************************************************************
  * @file     qspi_cmd_en25qh128a.c
  * @brief    qspi_cmd_en25qh128a program
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

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_QSPI_command_port_using_dma_and_pem
  * @{
  */

#define FLASH_PAGE_PROGRAM_SIZE          256
#define PEMOPC_SET                       0xA5
#define PEMOPC_RESET                     0xFF

typedef enum
{
  PEM_STEP_BEGIN,
  PEM_STEP_KEEP,
  PEM_STEP_END
}pem_step_type;

/* en25qh128a cmd read pem begin parameters, the address_code and data_counter need to be set in application */
static const qspi_cmd_type en25qh128a_read_pem_bigin_para = {
TRUE,PEMOPC_SET,0xEB,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_3_BYTE,0,4,QSPI_OPERATE_MODE_144,QSPI_RSTSC_HW_AUTO,FALSE,FALSE};

/* en25qh128a cmd read pem keep parameters, the address_code and data_counter need to be set in application */
static const qspi_cmd_type en25qh128a_read_pem_keep_para = {
TRUE,PEMOPC_SET,0xEB,QSPI_CMD_INSLEN_0_BYTE,0,QSPI_CMD_ADRLEN_3_BYTE,0,4,QSPI_OPERATE_MODE_144,QSPI_RSTSC_HW_AUTO,FALSE,FALSE};

/* en25qh128a cmd read pem end parameters, the address_code and data_counter need to be set in application */
static const qspi_cmd_type en25qh128a_read_pem_end_para = {
TRUE,PEMOPC_RESET,0xEB,QSPI_CMD_INSLEN_0_BYTE,0,QSPI_CMD_ADRLEN_3_BYTE,0,4,QSPI_OPERATE_MODE_144,QSPI_RSTSC_HW_AUTO,FALSE,FALSE};

/* en25qh128a cmd write parameters, the address_code and data_counter need to be set in application */
static const qspi_cmd_type en25qh128a_write_para = {
FALSE,0,0x32,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_3_BYTE,0,0,QSPI_OPERATE_MODE_114,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* en25qh128a cmd sector erase parameters, the address_code need to be set in application */
static const qspi_cmd_type en25qh128a_erase_para = {
FALSE,0,0x20,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_3_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* en25qh128a cmd wren parameters */
static const qspi_cmd_type en25qh128a_wren_para = {
FALSE,0,0x06,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_0_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,FALSE,TRUE};

/* en25qh128a cmd rdsr parameters */
static const qspi_cmd_type en25qh128a_rdsr_para = {
FALSE,0,0x05,QSPI_CMD_INSLEN_1_BYTE,0,QSPI_CMD_ADRLEN_0_BYTE,0,0,QSPI_OPERATE_MODE_111,QSPI_RSTSC_HW_AUTO,TRUE,FALSE};

qspi_cmd_type en25qh128a_cmd_config;

void qspi_busy_check(void);
void qspi_write_enable(void);
void qspi_cmd_send(qspi_cmd_type* qspi_cmd_struct);

/**
  * @brief  qspi dma set
  * @param  dir: dma transfer direction
  * @param  buf: the pointer for dma data
  * @param  length: data length
  * @retval none
  */
void qspi_dma_set(dma_dir_type dir, uint8_t* buf, uint32_t length)
{
  dma_init_type dma_init_struct;
  dma_reset(DMA2_CHANNEL1);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = length / 4;  /* using word unit */
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init_struct.direction = dir;
  dma_init_struct.memory_base_addr = (uint32_t)buf;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)(&(QSPI1->dt));
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_WORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;

  dma_init(DMA2_CHANNEL1, &dma_init_struct);

  dmamux_init(DMA2MUX_CHANNEL1, DMAMUX_DMAREQ_ID_QSPI1);
  dmamux_enable(DMA2, TRUE);

  dma_channel_enable(DMA2_CHANNEL1, TRUE);
}

/**
  * @brief  qspi read data using pem
  * @param  pem_step: pem step
  * @param  addr: the address for read
  * @param  total_len: the length for read
  * @param  buf: the pointer for read data
  * @retval none
  */
void qspi_read_data_using_pem(pem_step_type pem_step, uint32_t addr, uint32_t total_len, uint8_t* buf)
{
  qspi_dma_enable(QSPI1, TRUE);
  qspi_dma_rx_threshold_set(QSPI1, QSPI_DMA_FIFO_THOD_WORD08);
  /* config dma */
  qspi_dma_set(DMA_DIR_PERIPHERAL_TO_MEMORY, buf, total_len);

  if(pem_step == PEM_STEP_BEGIN)
  {
    en25qh128a_cmd_config = en25qh128a_read_pem_bigin_para;
  }
  else if(pem_step == PEM_STEP_KEEP)
  {
    en25qh128a_cmd_config = en25qh128a_read_pem_keep_para;
  }
  else if(pem_step == PEM_STEP_END)
  {
    en25qh128a_cmd_config = en25qh128a_read_pem_end_para;
  }
  en25qh128a_cmd_config.address_code = addr;
  en25qh128a_cmd_config.data_counter = total_len;
  qspi_cmd_send(&en25qh128a_cmd_config);

  /* wait dma completed */
  while(dma_flag_get(DMA2_FDT1_FLAG) == RESET)
  {
  }
  dma_flag_clear(DMA2_FDT1_FLAG);
  qspi_dma_enable(QSPI1, FALSE);
}

/**
  * @brief  qspi read data
  * @param  addr: the address for read
  * @param  total_len: the length for read
  * @param  buf: the pointer for read data
  * @retval none
  */
void qspi_data_read(uint32_t addr, uint32_t total_len, uint8_t* buf)
{
  uint32_t rdsz = sizeof(uint32_t);  /* dma transfer must word aligning */
  /* fist phase */
  qspi_read_data_using_pem(PEM_STEP_BEGIN, addr, rdsz, buf);
  addr += rdsz;
  buf += rdsz;
  total_len -= rdsz;

  /* keep phase */
  qspi_read_data_using_pem(PEM_STEP_KEEP, addr, rdsz, buf);
  addr += rdsz;
  buf += rdsz;
  total_len -= rdsz;

  /* last phase */
  qspi_read_data_using_pem(PEM_STEP_END, addr, total_len, buf);
}

/**
  * @brief  qspi write data for one time
  * @param  addr: the address for write
  * @param  sz: the length for write
  * @param  buf: the pointer for write data
  * @retval none
  */
static void qspi_data_once_write(uint32_t addr, uint32_t sz, uint8_t* buf)
{
  qspi_write_enable();

  /* config qspi's dma mode */
  qspi_dma_enable(QSPI1, TRUE);
  qspi_dma_tx_threshold_set(QSPI1, QSPI_DMA_FIFO_THOD_WORD08);

  /* config and enable dma */
  qspi_dma_set(DMA_DIR_MEMORY_TO_PERIPHERAL, buf, sz);

  en25qh128a_cmd_config = en25qh128a_write_para;
  en25qh128a_cmd_config.address_code = addr;
  en25qh128a_cmd_config.data_counter = sz;
  qspi_cmd_send(&en25qh128a_cmd_config);

  /* wait dma completed */
  while(dma_flag_get(DMA2_FDT1_FLAG) == RESET)
  {
  }
  dma_flag_clear(DMA2_FDT1_FLAG);
  qspi_dma_enable(QSPI1, FALSE);

  qspi_busy_check();
}

/**
  * @brief  qspi write data
  * @param  addr: the address for write
  * @param  total_len: the length for write
  * @param  buf: the pointer for write data
  * @retval none
  */
void qspi_data_write(uint32_t addr, uint32_t total_len, uint8_t* buf)
{
  uint32_t blk_sz;
  do
  { /* send up to 256 bytes at one time, and only one page */
    blk_sz = (addr / FLASH_PAGE_PROGRAM_SIZE + 1) * FLASH_PAGE_PROGRAM_SIZE - addr;
    if(total_len < blk_sz)
      blk_sz = total_len;
    qspi_data_once_write(addr, blk_sz, buf);
    addr += blk_sz;
    buf += blk_sz;
    total_len -= blk_sz;
  }while(total_len > 0);
}

/**
  * @brief  qspi erase data
  * @param  sec_addr: the sector address for erase
  * @retval none
  */
void qspi_erase(uint32_t sec_addr)
{
  qspi_write_enable();

  en25qh128a_cmd_config = en25qh128a_erase_para;
  en25qh128a_cmd_config.address_code = sec_addr;
  qspi_cmd_send(&en25qh128a_cmd_config);

  qspi_busy_check();
}

/**
  * @brief  qspi check busy
  * @param  none
  * @retval none
  */
void qspi_busy_check(void)
{
  qspi_cmd_send((qspi_cmd_type*)&en25qh128a_rdsr_para);
}

/**
  * @brief  qspi write enable
  * @param  none
  * @retval none
  */
void qspi_write_enable(void)
{
  qspi_cmd_send((qspi_cmd_type*)&en25qh128a_wren_para);
}

/**
  * @brief  qspi cmd kick and wait completed
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
void qspi_cmd_send(qspi_cmd_type* qspi_cmd_struct)
{
  /* kick command */
  qspi_cmd_operation_kick(QSPI1, qspi_cmd_struct);
 
  /* wait command completed */
  while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}

/**
  * @}
  */

/**
  * @}
  */
