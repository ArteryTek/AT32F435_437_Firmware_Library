/**
  **************************************************************************
  * @file     readme.txt 
  * @version  v2.0.4
  * @date     2021-12-31
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, this demo shows how to use DVP
to collect camera(ov5640) data. DVP data will be transported to XMC through
DMA, and the image will be displayed on LCD. 
  The following macro are used to select the data transmission mode from 
camera to DVP:
  #define HARDWARE_MODE
  #define EMBEDDED_MODE
