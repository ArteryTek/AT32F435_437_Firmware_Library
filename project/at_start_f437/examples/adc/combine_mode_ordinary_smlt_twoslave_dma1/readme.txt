/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.2
  * @date     2021-11-26
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  combine_mode(double slaver ordinary simultaneous mode only).
  the trigger source is: tmr1trgout2,,and use dma mode 1 transfer conversion data
  the convert data as follow:
  - adccom_ordinary_valuetab[n][0][0] ---> adc1_channel_4
  - adccom_ordinary_valuetab[n][0][1] ---> adc2_channel_7
  - adccom_ordinary_valuetab[n][0][2] ---> adc3_channel_10
  - adccom_ordinary_valuetab[n][1][0] ---> adc1_channel_5
  - adccom_ordinary_valuetab[n][1][1] ---> adc2_channel_8
  - adccom_ordinary_valuetab[n][1][2] ---> adc3_channel_12
  - adccom_ordinary_valuetab[n][2][0] ---> adc1_channel_6
  - adccom_ordinary_valuetab[n][2][1] ---> adc2_channel_9
  - adccom_ordinary_valuetab[n][2][2] ---> adc3_channel_13
