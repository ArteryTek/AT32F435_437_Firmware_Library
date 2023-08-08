/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, provides a basic
  communication between usart2 (synchronous mode) and spi1. the usart2 simulate
  spi master, the spi1 is spi slave. if both usart2 and spi1 received data right,
  the three leds will turn on.

  usart2 configured as follow:
  - 115200 baudrate
  - 8 bits data
  - one stop bit
  - no parity
  - receive and transmit enabled
  - usart clock enabled
  - usart clkpol: clock is active high
  - usart clkpha: data is captured on the second edge
  - usart lbcp: last bit clock pulse is output to the sclk pin

  spi1 configured as follow:
  - fullduplex
  - slave mode
  - 8 bits datasize
  - clkpol: clock is active high
  - clkpha: data is captured on the second edge
  - nss: nss software
  - first bit: first bit is the lsb

  set-up
  - connect usart2_tx(pa2) to spi1_mosi(pa7)
  - connect usart2_rx(pa3) to spi1_miso(pa6)
  - connect usart2_ck(pa4) to spi1_sck(pa5)

  for more detailed information. please refer to the application note document AN0099.