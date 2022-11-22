/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  combine_mode(single slaver ordinary simultaneous mode only).
  the trigger source is software,and use edma mode 2 transfer conversion data
  the convert data as follow:
  - adccom_ordinary_valuetab[n][0] ---> (adc2_channel_7<<16) | adc1_channel_4
  - adccom_ordinary_valuetab[n][1] ---> (adc2_channel_8<<16) | adc1_channel_5
  - adccom_ordinary_valuetab[n][2] ---> (adc2_channel_9<<16) | adc1_channel_6

  for more detailed information. please refer to the application note document AN0093.
