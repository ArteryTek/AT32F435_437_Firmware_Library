/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.4
  * @date     2021-12-31
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board,  in this demo,  provides a  description
  of how to use a dmamux genertor channel send dma request to transfer a halfword 
  data buffer memory to another halfword data buffer in embedded sram.  dma2 channel4
  is configured to transfer the contents of a 16-halfword data buffer stored in 
  embedded sram memory.pa1 is input trigger signal for dmamux genertor channel1,
  genertor channel1 will send four dma request when the rising edge of pa0.when
  all data transfered, the three leds will turn on.
