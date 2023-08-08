/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, mcu run 200MHz frequency,
  show how to use function crm_pll_parameter_calculate().
  
  the function crm_pll_parameter_calculate() calculate parameter pll_ms, pll_ns
  and pll_fr according to the pll source and pll frequency. in system clock
  config flow, function crm_pll_config() will call pll parameter pll_ms, pll_ns
  and pll_fr. 
  
  clock out will output frequency wave. if crm_pll_parameter_calculate() can 
  calculate pll parameter value accurately, led4 blink, else led2 blink.

  for more detailed information. please refer to the application note document AN0084.