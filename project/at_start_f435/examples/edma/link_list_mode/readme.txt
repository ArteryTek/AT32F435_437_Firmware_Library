/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.4
  * @date     2021-12-31
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, provides a description
  of how to use a edma link list mode to transfer several data buffer to usart1 
  data register. edma stream1 is configured link list mode to transfer the 
  contents of data buffer. the serial port on pc will received "This is EDMA 
  Link List Mode test! This test case use Usart1_TX!" if edma send data 
  right, the three leds will turn on. 
