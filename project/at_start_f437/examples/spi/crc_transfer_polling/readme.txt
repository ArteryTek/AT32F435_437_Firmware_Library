/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.7
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to transfer
  crc value by polling mode.
  the pins connection as follow:
  - spi2 slaver              spi3 master
    pd1(sck)       <--->     pc10(sck)
    pc2(miso)      <--->     pc11 (miso)
    pd4(mosi)      <--->     pc12(mosi)
