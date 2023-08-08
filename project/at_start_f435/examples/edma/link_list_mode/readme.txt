/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, provides a description
  of how to use a edma link list mode to transfer several data buffer to usart1 
  data register. edma stream1 is configured link list mode to transfer the 
  contents of data buffer. the serial port on pc will received "This is EDMA 
  Link List Mode test! This test case use Usart1_TX!" if edma send data 
  right, the three leds will turn on. 

  for more detailed information. please refer to the application note document AN0090.