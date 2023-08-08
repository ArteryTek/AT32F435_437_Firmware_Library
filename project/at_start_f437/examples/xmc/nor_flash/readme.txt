/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this example provides a basic example of how to use the xmc firmware library and
  an associate driver to perform erase/read/write operations on the m29w128fl or
  m29w128gl nor memories.
  usart1 using pa9 to printf the result.

  the pin to pin with nor flash and xmc:
  - xmc_a0    pf0     --->   norflash_a0
  - xmc_a1    pf1     --->   norflash_a1
  - xmc_a2    pf2     --->   norflash_a2
  - xmc_a3    pf3     --->   norflash_a3
  - xmc_a4    pf4     --->   norflash_a4
  - xmc_a5    pf5     --->   norflash_a5
  - xmc_a6    pf12    --->   norflash_a6
  - xmc_a7    pf13    --->   norflash_a7
  - xmc_a8    pf14    --->   norflash_a8
  - xmc_a9    pf15    --->   norflash_a9
  - xmc_a10   pg0     --->   norflash_a10
  - xmc_a11   pg1     --->   norflash_a11
  - xmc_a12   pg2     --->   norflash_a12
  - xmc_a13   pg3     --->   norflash_a13
  - xmc_a14   pg4     --->   norflash_a14
  - xmc_a15   pg5     --->   norflash_a15
  - xmc_a16   pd11    --->   norflash_a16
  - xmc_a17   pd12    --->   norflash_a17
  - xmc_a18   pd13    --->   norflash_a18
  - xmc_a19   pe3     --->   norflash_a19
  - xmc_a20   pe4     --->   norflash_a20
  - xmc_a21   pe5     --->   norflash_a21
  - xmc_a22   pe6     --->   norflash_a22
  - xmc_a23   pe2     --->   norflash_a23
  - xmc_a24   pg13    --->   norflash_a24
  - xmc_a25   pg14    --->   norflash_a25

  - xmc_d0    pd14    --->   norflash_data[0]
  - xmc_d1    pd15    --->   norflash_data[1]
  - xmc_d2    pd0     --->   norflash_data[2]
  - xmc_d3    pd1     --->   norflash_data[3]
  - xmc_d4    pe7     --->   norflash_data[4]
  - xmc_d5    pe8     --->   norflash_data[5]
  - xmc_d6    pe9     --->   norflash_data[6]
  - xmc_d7    pe10    --->   norflash_data[7]
  - xmc_d8    pe11    --->   norflash_data[8]
  - xmc_d9    pe12    --->   norflash_data[9]
  - xmc_d10   pe13    --->   norflash_data[10]
  - xmc_d11   pe14    --->   norflash_data[11]
  - xmc_d12   pe15    --->   norflash_data[12]
  - xmc_d13   pd8     --->   norflash_data[13]
  - xmc_d14   pd9     --->   norflash_data[14]
  - xmc_d15   pd10    --->   norflash_data[15]
  
  - xmc_ne2   pg9     --->   norflash_cs
  - xmc_noe   pd4     --->   norflash_rd
  - xmc_nwe   pd5     --->   norflash_wr
  -   --      vdd     --->   norflash_byte
  - xmc_nwait pd6     --->   norflash_busy
  - xmc_nadv  pb7     --->   norflash_nadv
  for more detailed information. please refer to the application note document AN0106.



