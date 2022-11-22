/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  combine_mode(double slaver preempt interleaved trigger mode only).
  the trigger source is software
  the convert data as follow:
  - adc1_preempt_valuetab[n][0] --->  adc1_channel_4
  - adc1_preempt_valuetab[n][1] --->  adc1_channel_5
  - adc1_preempt_valuetab[n][2] --->  adc1_channel_6
  - adc2_preempt_valuetab[n][0] --->  adc2_channel_7
  - adc2_preempt_valuetab[n][1] --->  adc2_channel_8
  - adc2_preempt_valuetab[n][2] --->  adc2_channel_9
  - adc3_preempt_valuetab[n][0] --->  adc3_channel_10
  - adc3_preempt_valuetab[n][1] --->  adc3_channel_12
  - adc3_preempt_valuetab[n][2] --->  adc3_channel_13

  for more detailed information. please refer to the application note document AN0093.
