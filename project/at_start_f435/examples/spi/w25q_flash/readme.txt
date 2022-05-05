/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.8
  * @date     2022-04-25
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  only receive mode receive data by polling mode.
  the pins connection as follow:
  - spi1                     w25q128
    pa4(cs)        <--->     cs pin
    pa5(sck)       <--->     clk pin
    pa6(miso)      <--->     di pin
    pa7(mosi)      <--->     do pin

  for more detailed information. please refer to the application note document AN0102.