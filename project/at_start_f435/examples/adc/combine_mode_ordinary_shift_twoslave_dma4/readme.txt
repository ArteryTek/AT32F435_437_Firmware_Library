/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  combine_mode(double slaver ordinary shifting mode only).
  the trigger source is tmr1trgout,and use dma mode 4 transfer conversion data
  the convert data as follow:
  - adccom_ordinary_valuetab[n] ---> (adc3_channel_10<<16) | (adc2_channel_7<<8) | adc1_channel_4

  for more detailed information. please refer to the application note document AN0093.
