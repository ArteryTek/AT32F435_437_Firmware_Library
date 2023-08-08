/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this example provides a basic example of how to use the xmc firmware library and
  an associate driver to perform read/write operations on the is61wv51216bll sram
  memory.
  usart1 using pa9 to printf the result.

  the pin to pin with sram and xmc:
  - xmc_a0    pf0     --->   sram_a0
  - xmc_a1    pf1     --->   sram_a1
  - xmc_a2    pf2     --->   sram_a2
  - xmc_a3    pf3     --->   sram_a3
  - xmc_a4    pf4     --->   sram_a4
  - xmc_a5    pf5     --->   sram_a5
  - xmc_a6    pf12    --->   sram_a6
  - xmc_a7    pf13    --->   sram_a7
  - xmc_a8    pf14    --->   sram_a8
  - xmc_a9    pf15    --->   sram_a9
  - xmc_a10   pg0     --->   sram_a10
  - xmc_a11   pg1     --->   sram_a11
  - xmc_a12   pg2     --->   sram_a12
  - xmc_a13   pg3     --->   sram_a13
  - xmc_a14   pg4     --->   sram_a14
  - xmc_a15   pg5     --->   sram_a15
  - xmc_a16   pd11    --->   sram_a16
  - xmc_a17   pd12    --->   sram_a17
  - xmc_a18   pd13    --->   sram_a18
  - xmc_a19   pe3     --->   sram_a19
  - xmc_a20   pe4     --->   sram_a20
  - xmc_a21   pe5     --->   sram_a21
  - xmc_a22   pe6     --->   sram_a22
  - xmc_a23   pe2     --->   sram_a23
  - xmc_a24   pg13    --->   sram_a24
  - xmc_a25   pg14    --->   sram_a25

  - xmc_d0    pd14    --->   sram_data[0]
  - xmc_d1    pd15    --->   sram_data[1]
  - xmc_d2    pd0     --->   sram_data[2]
  - xmc_d3    pd1     --->   sram_data[3]
  - xmc_d4    pe7     --->   sram_data[4]
  - xmc_d5    pe8     --->   sram_data[5]
  - xmc_d6    pe9     --->   sram_data[6]
  - xmc_d7    pe10    --->   sram_data[7]
  - xmc_d8    pe11    --->   sram_data[8]
  - xmc_d9    pe12    --->   sram_data[9]
  - xmc_d10   pe13    --->   sram_data[10]
  - xmc_d11   pe14    --->   sram_data[11]
  - xmc_d12   pe15    --->   sram_data[12]
  - xmc_d13   pd8     --->   sram_data[13]
  - xmc_d14   pd9     --->   sram_data[14]
  - xmc_d15   pd10    --->   sram_data[15]
  
  - xmc_ne3   pg10    --->   sram_cs
  - xmc_noe   pd4     --->   sram_rd
  - xmc_nwe   pd5     --->   sram_wr
  - xmc_lb    pe0     --->   sram_nbl0
  - xmc_ub    pe1     --->   sram_nbl1
  for more detailed information. please refer to the application note document AN0106.



