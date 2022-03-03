/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.5
  * @date     2022-02-11
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  dma recieve data. spi2 use jtag pin as spi pin,and config spi in hardware
  cs mode. after download the code,please disconnect the pa13 and pa14 from
  downloader to make sure communication is ok.
  the pins connection as follow:
  - spi2                     spi3 
    pd0(cs)        <--->     pa15(cs)
    pd1(sck)       <--->     pb3 (sck)
    pc2(miso)      <--->     pa13(miso)
    pd4(mosi)      <--->     pa14(mosi)
