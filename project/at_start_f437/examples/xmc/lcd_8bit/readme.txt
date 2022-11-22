/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, provides a basic 
  example of how to use the xmc firmware library and an associate driver to 
  perform read/write operations on the lcd. the lcd drive ic is st7789v and
  will display some number, it's actually showing pictures.this demo support
  144pin pakeage.
  
  the pin to pin with lcd and xmc:
  - gpio      pb9     --->   lcd_reset
  - xmc_a0    pf0     --->   lcd_rs
  - xmc_ne1   pd7     --->   lcd_cs
  - xmc_nwe   pd5     --->   lcd_wr
  - xmc_noe   pd4     --->   lcd_rd
  - xmc_d0    pd14    --->   data[0]
  - xmc_d1    pd15    --->   data[1]
  - xmc_d2    pd0     --->   data[2]
  - xmc_d3    pd1     --->   data[3]
  - xmc_d4    pe7     --->   data[4]
  - xmc_d5    pe8     --->   data[5]
  - xmc_d6    pe9     --->   data[6]
  - xmc_d7    pe10    --->   data[7]
  for more detailed information. please refer to the application note document AN0106.


