/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board,  in this demo,  provides a  description
  of how to use a dmamux genertor channel send dma request to transfer a halfword data buffer memory
  to another halfword data buffer in embedded sram.  edma stream4 is configured to 
  transfer the contents of a 16-halfword data buffer stored in embedded sram memory.
  pa1 is input trigger signal for dmamux genertor channel1, genertor channel1 will 
  send four dma request when the rising edge of pa1.when all data transfered, the 
  three leds will turn on.

  for more detailed information. please refer to the application note document AN0090.