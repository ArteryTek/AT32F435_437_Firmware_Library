/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board and AT32-Comm-EV board, in this demo, 
  shows how to use spi access the w25q flash chip.
  the pins connection as follow:
  - spi2                     w25qxx
    pd0(cs)        <--->     cs pin
    pd1(sck)       <--->     clk pin
    pc2(miso)      <--->     di pin
    pd4(mosi)      <--->     do pin

  for more detailed information. please refer to the application note document AN0102.