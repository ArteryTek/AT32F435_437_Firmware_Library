/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board,in this demo, uses the tmr1 in dma synchronized 
  mode to send the contents of a 16-halfword data buffer stored in embedded sram memory.
  the dmamux synchronization block is configured to synchronize the dma transfer with the 
  tmr1 overflow dma request.each rising edge of pa1 the synchronization signal will authorize
  4 tmr1 requests to be transmitted src_buffer to dst_buffer using the dma.when all data 
  transfered, the three leds will turn on.
  for more detailed information. please refer to the application note document AN0103.