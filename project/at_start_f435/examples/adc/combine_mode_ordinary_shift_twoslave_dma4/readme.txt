/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.4
  * @date     2021-12-31
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  combine_mode(double slaver ordinary shifting mode only).
  the trigger source is tmr1trgout,and use dma mode 4 transfer conversion data
  the convert data as follow:
  - adccom_ordinary_valuetab[n] ---> (adc3_channel_10<<16) | (adc2_channel_7<<8) | adc1_channel_4
