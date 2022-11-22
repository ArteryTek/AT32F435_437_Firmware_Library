/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  combine_mode(double slaver ordinary shifting mode only).
  the trigger source is tmr1trgout,and use dma mode 3 transfer conversion data
  the convert data as follow:
  - adccom_ordinary_valuetab[0] ---> (adc2_channel_7<<8)  | adc1_channel_4
  - adccom_ordinary_valuetab[1] ---> (adc1_channel_4<<8)  | adc3_channel_10
  - adccom_ordinary_valuetab[2] ---> (adc3_channel_10<<8) | adc2_channel_7
  - adccom_ordinary_valuetab[3] ---> (adc2_channel_7<<8)  | adc1_channel_4
  - adccom_ordinary_valuetab[4] ---> (adc1_channel_4<<8)  | adc3_channel_10
  - adccom_ordinary_valuetab[5] ---> (adc3_channel_10<<8) | adc2_channel_7

  for more detailed information. please refer to the application note document AN0093.
