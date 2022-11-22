/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use edma continuous mode to change timer1's channel1/2/3/4 duty cycle.
  timer1 enable update dma request and every update event edma will use continuous mode transfer four data to timer1's cc1/cc2/cc3/cc4.

  set-up
  connect the tmr1 pins to an oscilloscope to monitor the different waveforms:
  - tmr1_ch1  --->  pa8
  - tmr1_ch2  --->  pa9
  - tmr1_ch3  --->  pa10
  - tmr1_ch4  --->  pa11

  for more detailed information. please refer to the application note document AN0090.
