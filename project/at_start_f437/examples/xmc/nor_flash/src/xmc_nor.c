/**
  ******************************************************************************
  * @file     xmc_nor.h
  * @brief    header file for the nor configuration.
  ******************************************************************************
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

#include "xmc_nor.h"

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_XMC_nor_flash
  * @{
  */

#define BANK1_NOR2_ADDR                  ((uint32_t)0x64000000)

/* delay definition */
#define BLOCKERASE_TIMEOUT               ((uint32_t)0x00A00000)
#define CHIPERASE_TIMEOUT                ((uint32_t)0x30000000)
#define PROGRAM_TIMEOUT                  ((uint32_t)0x00001400)

#define ADDR_SHIFT(A)                    (BANK1_NOR2_ADDR + (2 * (A)))
#define NOR_WRITE(address, data)         (*(__IO uint16_t *)(address) = (data))
/**
  * @brief  init nor flash interface
  * @param  none
  * @retval none
  */
void nor_init(void)
{
  gpio_init_type  gpio_init_struct = {0};
  xmc_norsram_init_type  xmc_norsram_init_struct;
  xmc_norsram_timing_init_type rw_timing_struct, w_timing_struct;

  /* enable the gpio clock */
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* enable the xmc clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);

  /*-- gpio configuration ------------------------------------------------------*/
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE2, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE3, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOF, GPIO_PINS_SOURCE15, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE2, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE3, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE15, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE6, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE3, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE4, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE5, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE6, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE2, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE13, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE7, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE8, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE9, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE10, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE11, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE12, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE14, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE15, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE0, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE1, GPIO_MUX_12);
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE7, GPIO_MUX_12);

  /* address lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2 | GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_12 | GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOF, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2 | GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_13 | GPIO_PINS_14;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOG, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6 | GPIO_PINS_2;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOE, &gpio_init_struct);

  /* data lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_7 | GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOE, &gpio_init_struct);

  /* ne/noe/nwe/nbl0/nbl1/nwait lines configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOE, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOG, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOB, &gpio_init_struct);


  /*-- xmc configuration ------------------------------------------------------*/
  xmc_norsram_default_para_init(&xmc_norsram_init_struct);
  xmc_norsram_init_struct.subbank = XMC_BANK1_NOR_SRAM2;
  xmc_norsram_init_struct.data_addr_multiplex = XMC_DATA_ADDR_MUX_DISABLE;
  xmc_norsram_init_struct.device = XMC_DEVICE_NOR;
  xmc_norsram_init_struct.bus_type = XMC_BUSTYPE_16_BITS;
  xmc_norsram_init_struct.burst_mode_enable = XMC_BURST_MODE_DISABLE;
  xmc_norsram_init_struct.asynwait_enable = XMC_ASYN_WAIT_DISABLE;
  xmc_norsram_init_struct.wait_signal_lv = XMC_WAIT_SIGNAL_LEVEL_LOW;
  xmc_norsram_init_struct.wrapped_mode_enable = XMC_WRAPPED_MODE_DISABLE;
  xmc_norsram_init_struct.wait_signal_config = XMC_WAIT_SIGNAL_SYN_BEFORE;
  xmc_norsram_init_struct.write_enable = XMC_WRITE_OPERATION_ENABLE;
  xmc_norsram_init_struct.wait_signal_enable = XMC_WAIT_SIGNAL_DISABLE;
  xmc_norsram_init_struct.write_timing_enable = XMC_WRITE_TIMING_DISABLE;
  xmc_norsram_init_struct.write_burst_syn = XMC_WRITE_BURST_SYN_DISABLE;
  xmc_nor_sram_init(&xmc_norsram_init_struct);

  /* timing configuration */
  xmc_norsram_timing_default_para_init(&rw_timing_struct, &w_timing_struct);
  rw_timing_struct.subbank = XMC_BANK1_NOR_SRAM2;
  rw_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  rw_timing_struct.addr_setup_time = 0x8;
  rw_timing_struct.addr_hold_time = 0x0;
  rw_timing_struct.data_setup_time = 0x10;
  rw_timing_struct.bus_latency_time = 0x0;
  rw_timing_struct.clk_psc = 0x0;
  rw_timing_struct.data_latency_time = 0x0;
  rw_timing_struct.mode = XMC_ACCESS_MODE_B;
  w_timing_struct.subbank = XMC_BANK1_NOR_SRAM2;
  w_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  w_timing_struct.addr_setup_time = 0x8;
  w_timing_struct.addr_hold_time = 0x0;
  w_timing_struct.data_setup_time = 0x10;
  w_timing_struct.bus_latency_time = 0x0;
  w_timing_struct.clk_psc = 0x0;
  w_timing_struct.data_latency_time = 0x0;
  w_timing_struct.mode = XMC_ACCESS_MODE_B;
  xmc_nor_sram_timing_config(&rw_timing_struct, &w_timing_struct);

  /* enable xmc_bank1_nor_sram2 */
  xmc_nor_sram_enable(XMC_BANK1_NOR_SRAM2, TRUE);
}

/**
  * @brief  reads nor memory's manufacturer and device code.
  * @param  nor_id: pointer to a nor_idtypedef structure which will hold the
  *         manufacturer and device code.
  * @retval none
  */
void nor_readid(nor_idtype* nor_id)
{
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
    NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x0090);

    nor_id->manufacturer_code = *(__IO uint16_t *) ADDR_SHIFT(0x0000);
    nor_id->device_code1 = *(__IO uint16_t *) ADDR_SHIFT(0x0001);
    nor_id->device_code2 = *(__IO uint16_t *) ADDR_SHIFT(0x000E);
    nor_id->device_code3 = *(__IO uint16_t *) ADDR_SHIFT(0x000F);
}

/**
  * @brief  erases the specified nor memory block.
  * @param  blockaddr: address of the block to erase.
  * @retval nor_status: the returned value can be: nor_success, nor_error
  *         or nor_timeout
  */
nor_status nor_eraseblock(uint32_t blockaddr)
{
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
    NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x0080);
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
    NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
    NOR_WRITE((BANK1_NOR2_ADDR + blockaddr), 0x30);

    return (nor_getstatus(BLOCKERASE_TIMEOUT));
}

/**
  * @brief  erases the entire chip.
  * @param  none
  * @retval nor_status: the returned value can be: nor_success, nor_error
  *         or nor_timeout
  */
nor_status nor_erasechip(void)
{
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
    NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x0080);
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
    NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x0010);

    return (nor_getstatus(CHIPERASE_TIMEOUT));
}

/**
  * @brief  writes a half-word to the nor memory.
  * @param  writeaddr: nor memory internal address to write to.
  * @param  data: data to write.
  * @retval nor_status: the returned value can be: nor_success, nor_error
  *         or nor_timeout
  */
nor_status nor_writehalfword(uint32_t writeaddr, uint16_t data)
{
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
    NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x00A0);
    NOR_WRITE((BANK1_NOR2_ADDR + writeaddr), data);

    return (nor_getstatus(PROGRAM_TIMEOUT));
}

/**
  * @brief  writes a half-word buffer to the xmc nor memory.
  * @param  pbuffer: pointer to buffer.
  * @param  writeaddr: nor memory internal address from which the data will be
  *         written.
  * @param  numhalfwordtowrite: number of half words to write.
  * @retval nor_status: the returned value can be: nor_success, nor_error
  *         or nor_timeout
  */
nor_status nor_writebuffer(uint16_t* pbuffer, uint32_t writeaddr, uint32_t numhalfwordtowrite)
{
    nor_status status = NOR_ONGOING;

    do
    {
        /*!< Transfer data to the memory */
        status = nor_writehalfword(writeaddr, *pbuffer++);
        writeaddr = writeaddr + 2;
        numhalfwordtowrite--;
    }
    while((status == NOR_SUCCESS) && (numhalfwordtowrite != 0));

    return (status);
}

/**
  * @brief  writes a half-word buffer to the xmc nor memory. this function
  *         must be used only with s29gl128p nor memory.
  * @param  pbuffer: pointer to buffer.
  * @param  writeaddr: nor memory internal address from which the data will be
  *         written.
  * @param  numhalfwordtowrite: number of half words to write.
  *         the maximum allowed value is 32 half words (64 bytes).
  * @retval nor_status: the returned value can be: nor_success, nor_error
  *         or nor_timeout
  */
nor_status nor_programbuffer(uint16_t* pbuffer, uint32_t writeaddr, uint32_t numhalfwordtowrite)
{
    uint32_t lastloadedaddress  = 0x00;
    uint32_t currentaddress     = 0x00;
    uint32_t endaddress         = 0x00;

    /*!< initialize variables */
    currentaddress      = writeaddr;
    endaddress          = writeaddr + numhalfwordtowrite - 1;
    lastloadedaddress   = writeaddr;

    /*!< Issue unlock command sequence */
    NOR_WRITE(ADDR_SHIFT(0x00555), 0x00AA);

    NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);

    /*!< Write Write Buffer Load Command */
    NOR_WRITE(ADDR_SHIFT(writeaddr), 0x0025);
    NOR_WRITE(ADDR_SHIFT(writeaddr), (numhalfwordtowrite - 1));

    /*!< Load Data into NOR Buffer */
    while(currentaddress <= endaddress)
    {
        /*!< Store last loaded address & data value (for polling) */
        lastloadedaddress = currentaddress;

        NOR_WRITE(ADDR_SHIFT(currentaddress), *pbuffer++);
        currentaddress += 1;
    }

    NOR_WRITE(ADDR_SHIFT(lastloadedaddress), 0x29);

    return(nor_getstatus(PROGRAM_TIMEOUT));
}

/**
  * @brief  reads a half-word from the nor memory.
  * @param  readaddr: nor memory internal address to read from.
  * @retval half-word read from the nor memory
  */
uint16_t nor_readhalfword(uint32_t readaddr)
{
    NOR_WRITE(ADDR_SHIFT(0x00555), 0x00AA);
    NOR_WRITE(ADDR_SHIFT(0x002AA), 0x0055);
    NOR_WRITE((BANK1_NOR2_ADDR + readaddr), 0x00F0 );

    return (*(__IO uint16_t *)((BANK1_NOR2_ADDR + readaddr)));
}

/**
  * @brief  reads a block of data from the xmc nor memory.
  * @param  pbuffer: pointer to the buffer that receives the data read from the
  *         nor memory.
  * @param  readaddr: nor memory internal address to read from.
  * @param  numhalfwordtoread : number of half word to read.
  * @retval none
  */
void nor_readbuffer(uint16_t* pbuffer, uint32_t readaddr, uint32_t numhalfwordtoread)
{
    NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
    NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
    NOR_WRITE((BANK1_NOR2_ADDR + readaddr), 0x00F0);

    for(; numhalfwordtoread != 0x00; numhalfwordtoread--) /*!< while there is data to read */
    {
        /*!< read a halfword from the nor */
        *pbuffer++ = *(__IO uint16_t *)((BANK1_NOR2_ADDR + readaddr));
        readaddr = readaddr + 2;
    }
}

/**
  * @brief  returns the nor memory to read mode.
  * @param  none
  * @retval nor_success
  */
nor_status nor_returntoreadmode(void)
{
    NOR_WRITE(BANK1_NOR2_ADDR, 0x00F0);

    return (NOR_SUCCESS);
}

/**
  * @brief  returns the nor memory to read mode and resets the errors in the nor
  *         memory status register.
  * @param  none
  * @retval nor_success
  */
nor_status nor_reset(void)
{
    NOR_WRITE(ADDR_SHIFT(0x00555), 0x00AA);
    NOR_WRITE(ADDR_SHIFT(0x002AA), 0x0055);
    NOR_WRITE(BANK1_NOR2_ADDR, 0x00F0);

    return (NOR_SUCCESS);
}

/**
  * @brief  returns the nor operation status.
  * @param  timeout: nor progamming timeout
  * @retval nor_status: the returned value can be: nor_success, nor_error
  *         or nor_timeout
  */
nor_status nor_getstatus(uint32_t timeout)
{
    uint16_t val1 = 0x00, val2 = 0x00;
    nor_status status = NOR_ONGOING;
    uint32_t timeout1 = timeout;

    /*!< poll on nor memory ready/busy signal ----------------------------------*/
    while((gpio_input_data_bit_read(GPIOD, GPIO_PINS_6) != RESET) && (timeout1 > 0))
    {
        timeout1--;
    }

    timeout1 = timeout;

    while((gpio_input_data_bit_read(GPIOD, GPIO_PINS_6) == RESET) && (timeout1 > 0))
    {
        timeout1--;
    }

    /*!< get the nor memory operation status -----------------------------------*/
    while((timeout != 0x00) && (status != NOR_SUCCESS))
    {
        timeout--;

        /*!< read dq6 and dq5 */
        val1 = *(__IO uint16_t *)(BANK1_NOR2_ADDR);
        val2 = *(__IO uint16_t *)(BANK1_NOR2_ADDR);

        /*!< if dq6 did not toggle between the two reads then return nor_success */
        if((val1 & 0x0040) == (val2 & 0x0040))
        {
            return NOR_SUCCESS;
        }

        if((val1 & 0x0020) != 0x0020)
        {
            status = NOR_ONGOING;
        }

        val1 = *(__IO uint16_t *)(BANK1_NOR2_ADDR);
        val2 = *(__IO uint16_t *)(BANK1_NOR2_ADDR);

        if((val1 & 0x0040) == (val2 & 0x0040))
        {
            return NOR_SUCCESS;
        }
        else if((val1 & 0x0020) == 0x0020)
        {
            return NOR_ERROR;
        }
    }

    if(timeout == 0x00)
    {
        status = NOR_TIMEOUT;
    }

    /*!< return the operation status */
    return (status);
}

/**
  * @brief  fill the global buffer
  * @param  pbuffer: pointer on the buffer to fill
  * @param  buffersize: size of the buffer to fill
  * @param  offset: first value to fill on the buffer
  */
void fill_buffer(uint16_t *pbuffer, uint16_t bufferlenght, uint32_t offset)
{
    uint16_t indextmp = 0;

    /* put in global buffer same values */
    for (indextmp = 0; indextmp < bufferlenght; indextmp++ )
    {
        pbuffer[indextmp] = indextmp + offset;
    }
}

/**
  * @}
  */

/**
  * @}
  */
