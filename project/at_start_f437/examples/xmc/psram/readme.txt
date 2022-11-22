/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, provides a basic example
  of how to use the xmc firmware library and an associate driver to perform read/write
  operations on the nand flash memory w957d6hb(winbond).
  usart1 using pa9 to printf the result.

  the pin to pin with sram and xmc:
  - xmc_a16   pd11    --->   psram_a16
  - xmc_a17   pd12    --->   psram_a17
  - xmc_a18   pd13    --->   psram_a18
  - xmc_a19   pe3     --->   psram_a19
  - xmc_a20   pe4     --->   psram_a20
  - xmc_a21   pe5     --->   psram_a21
  - xmc_a22   pe6     --->   psram_a22
  - xmc_a23   pe2     --->   psram_a23
  - xmc_a24   pg13    --->   psram_a24
  - xmc_a25   pg14    --->   psram_a25

  - xmc_ad0   pd14    --->   psram_data[0]
  - xmc_ad1   pd15    --->   psram_data[1]
  - xmc_ad2   pd0     --->   psram_data[2]
  - xmc_ad3   pd1     --->   psram_data[3]
  - xmc_ad4   pe7     --->   psram_data[4]
  - xmc_ad5   pe8     --->   psram_data[5]
  - xmc_ad6   pe9     --->   psram_data[6]
  - xmc_ad7   pe10    --->   psram_data[7]
  - xmc_ad8   pe11    --->   psram_data[8]
  - xmc_ad9   pe12    --->   psram_data[9]
  - xmc_ad10  pe13    --->   psram_data[10]
  - xmc_ad11  pe14    --->   psram_data[11]
  - xmc_ad12  pe15    --->   psram_data[12]
  - xmc_ad13  pd8     --->   psram_data[13]
  - xmc_ad14  pd9     --->   psram_data[14]
  - xmc_ad15  pd10    --->   psram_data[15]
  
  - xmc_ne3   pg10    --->   psram_cs
  - xmc_noe   pd4     --->   psram_rd
  - xmc_nwe   pd5     --->   psram_wr
  - xmc_lb    pe0     --->   psram_nbl0
  - xmc_ub    pe1     --->   psram_nbl1
  - xmc_nadv  pb7     --->   psram_nadv
  for more detailed information. please refer to the application note document AN0106.


