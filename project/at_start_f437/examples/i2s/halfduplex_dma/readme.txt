/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.2
  * @date     2021-11-26
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
