/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows in halfduplex
  mode how to use dma transfer data.
  the pins connection as follow:
  - pc7 as mck out
  - i2s2 slaver              i2s3 master
    pd0(ws)        <--->     pa4 (ws)
    pd1(sck)       <--->     pc10(sck)
    pd4(sd)        <--->     pc12(sd)

  for more detailed information. please refer to the application note document AN0102.