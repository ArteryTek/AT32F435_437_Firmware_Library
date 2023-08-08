/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  fullduplex mode transfer data by dma mode in ti mode.
  the pins connection as follow:
  - spi2                     spi3 
    pd0(cs)        <--->     pa4(cs)
    pd1(sck)       <--->     pc10(sck)
    pc2(miso)      <--->     pc11 (miso)
    pd4(mosi)      <--->     pc12(mosi)

  for more detailed information. please refer to the application note document AN0102.