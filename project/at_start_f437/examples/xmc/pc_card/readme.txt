/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this example provides a basic example of how to use the xmc firmware library and
   an associate driver to perform read/write operations on the sandisk compactflash 
   memory.
  usart1 using pa9 to printf the result.

  the pin to pin with compactflash and xmc:
  - xmc_a0    pf0     --->   a0
  - xmc_a1    pf1     --->   a1
  - xmc_a2    pf2     --->   a2
  - xmc_a3    pf3     --->   a3
  - xmc_a4    pf4     --->   a4
  - xmc_a5    pf5     --->   a5
  - xmc_a6    pf12    --->   a6
  - xmc_a7    pf13    --->   a7
  - xmc_a8    pf14    --->   a8
  - xmc_a9    pf15    --->   a9
  - xmc_a10   pd5     --->   a10

  - xmc_d0    pb14    --->   data[0]
  - xmc_d1    pc6     --->   data[1]
  - xmc_d2    pc11    --->   data[2]
  - xmc_d3    pc12    --->   data[3]
  - xmc_d4    pe7     --->   data[4]
  - xmc_d5    pa3     --->   data[5]
  - xmc_d6    pa4     --->   data[6]
  - xmc_d7    pa5     --->   data[7]
  - xmc_d8    pe11    --->   data[8]
  - xmc_d9    pe12    --->   data[9]
  - xmc_d10   pe13    --->   data[10]
  - xmc_d11   pe14    --->   data[11]
  - xmc_d12   pe15    --->   data[12]
  - xmc_d13   pb12    --->   data[13]
  - xmc_d14   pd9     --->   data[14]
  - xmc_d15   pd10    --->   data[15]
  
  - xmc_nce4_1   pg10    --->   CE1
  - xmc_nce4_2   pg11    --->   CE2
  - xmc_noe      pd4     --->   NOE
  - xmc_nwe      pc2     --->   NWE
  - xmc_nwait    pd6     --->   WAIT
  - xmc_intr     pf10    --->   INTR
  - xmc_cd       pf9     --->   CD
  - xmc_nreg     pf7     --->   REG
  - xmc_niowr    pf8     --->   IOWR
  - xmc_niord    pf6     --->   IORD




