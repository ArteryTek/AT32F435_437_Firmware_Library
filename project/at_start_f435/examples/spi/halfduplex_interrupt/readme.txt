/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.7
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  halfduplex mode transfer data by interrupt mode.
  the pins connection as follow:
  - spi2 slaver              spi3 master
    pd1(sck)       <--->     pc10(sck)
    pc2(miso)      <--->     pc12(mosi)
