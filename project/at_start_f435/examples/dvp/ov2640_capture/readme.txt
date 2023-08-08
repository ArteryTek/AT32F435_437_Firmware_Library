/**
  **************************************************************************
  * @file     readme.txt 
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, this demo shows how to use DVP
to collect camera(ov2640) data. DVP data will be transported to XMC through
DMA, and the image will be displayed on LCD. 

  the pins of lcd:
  - spi4_sck  pb13    --->   spi_sck
  - spi4_miso pb8     --->   spi_miso
  - spi4_mosi pa1     --->   spi_mosi
  - gpio      pb11    --->   tp_int
  - gpio      pb12    --->   tp_cs
  - gpio      pc0     --->   lcd_bl
  - nrst      nrst    --->   lcd_reset
  - xmc_a10   pd5     --->   lcd_rs
  - xmc_ne4   pc4     --->   lcd_cs
  - xmc_nwe   pc2     --->   lcd_wr
  - xmc_noe   pc5     --->   lcd_rd
  - xmc_d0    pd14    --->   data[0]
  - xmc_d1    pd15    --->   data[1]
  - xmc_d2    pc11    --->   data[2]
  - xmc_d3    pc12    --->   data[3]
  - xmc_d4    pe7     --->   data[4]
  - xmc_d5    pe8     --->   data[5]
  - xmc_d6    pe9     --->   data[6]
  - xmc_d7    pe10    --->   data[7]
  - xmc_d8    pe11    --->   data[8]
  - xmc_d9    pe12    --->   data[9]
  - xmc_d10   pe13    --->   data[10]
  - xmc_d11   pe14    --->   data[11]
  - xmc_d12   pe15    --->   data[12]
  - xmc_d13   pd8     --->   data[13]
  - xmc_d14   pd9     --->   data[14]
  - xmc_d15   pd10    --->   data[15]
  
  the pins of ov2640:
  - dvp_vsync pb7     --->   vsync
  - dvp_hsync pa4     --->   href
  - gpio      pc3     --->   rst
  - dvp_d0    pa9     --->   d0
  - dvp_d1    pa10    --->   d1
  - dvp_d2    pa11    --->   d2
  - dvp_d3    pa12    --->   d3
  - dvp_d4    pe4     --->   d4
  - dvp_d5    pb4     --->   d5
  - dvp_d6    pe5     --->   d6
  - dvp_d7    pb9     --->   d7
  - dvp_pclk  pa6     --->   pclk
  - gpio      pc1     --->   pwon
  - i2c2_scl  ph2     --->   scl
  - i2c2_sda  ph3     --->   sda

  for more detailed information. please refer to the application note document AN0087.