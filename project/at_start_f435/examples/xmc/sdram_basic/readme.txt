/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this example provides a basic example of how to use the xmc firmware library and
  an associate driver to perform read/write operations on the winbond sdram
  memory.
  led2/led3/led4 on : write and read all are ok
  led2/led3/led4 off: some errors

  the pin to pin with sram and xmc:
  - xmc_a0    pf0     --->   sdram_a0
  - xmc_a1    pf1     --->   sdram_a1
  - xmc_a2    pf2     --->   sdram_a2
  - xmc_a3    pf3     --->   sdram_a3
  - xmc_a4    pf4     --->   sdram_a4
  - xmc_a5    pf5     --->   sdram_a5
  - xmc_a6    pf12    --->   sdram_a6
  - xmc_a7    pf13    --->   sdram_a7
  - xmc_a8    pf14    --->   sdram_a8
  - xmc_a9    pf15    --->   sdram_a9
  - xmc_a10   pg0     --->   sdram_a10
  - xmc_a11   pg1     --->   sdram_a11
  - xmc_a12   pg2     --->   sdram_a12

  - xmc_d0    pd14    --->   sdram_data[0]
  - xmc_d1    pd15    --->   sdram_data[1]
  - xmc_d2    pd0     --->   sdram_data[2]
  - xmc_d3    pd1     --->   sdram_data[3]
  - xmc_d4    pe7     --->   sdram_data[4]
  - xmc_d5    pe8     --->   sdram_data[5]
  - xmc_d6    pe9     --->   sdram_data[6]
  - xmc_d7    pe10    --->   sdram_data[7]
  - xmc_d8    pe11    --->   sdram_data[8]
  - xmc_d9    pe12    --->   sdram_data[9]
  - xmc_d10   pe13    --->   sdram_data[10]
  - xmc_d11   pe14    --->   sdram_data[11]
  - xmc_d12   pe15    --->   sdram_data[12]
  - xmc_d13   pd8     --->   sdram_data[13]
  - xmc_d14   pd9     --->   sdram_data[14]
  - xmc_d15   pd10    --->   sdram_data[15]
  
  - xmc_clk   pg8     --->   sdram_clk
  - xmc_cke0  pc3     --->   sdram_cke0
  - xmc_cke1  pb5     --->   sdram_cke1
  - xmc_ne0   pc2     --->   sdram_ne0
  - xmc_ne1   pb6     --->   sdram_ne1
  
  - xmc_ba0   pg4     --->   sdram_ba0
  - xmc_ba1   pg5     --->   sdram_ba1
  - xmc_nras  pf11    --->   sdram_nras
  - xmc_ncas  pg15    --->   sdram_ncas
  - xmc_sdnwe pc0     --->   sdram_sdnwe
  - xmc_lb    pe0     --->   sdram_nbl0
  - xmc_ub    pe1     --->   sdram_nbl1

  for more detailed information. please refer to the application note document AN0089.





