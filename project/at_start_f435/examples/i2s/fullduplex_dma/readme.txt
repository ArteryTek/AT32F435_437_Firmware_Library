/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.4
  * @date     2021-12-31
  * @brief    readme
  **************************************************************************
  */

    this demo is based on the at-start board
	this demo shows in fullduplex mode how to use dma transfer data.
	the pins connection as follow:
	- i2s2 master                   i2s3 slaver
		pd0(ws)             <--->     pa4 (ws)
		pd1(sck)            <--->     pc10(sck)
		pc2(ext_sd) rx      <--->     pc11(ext_sd) tx
		pd4(sd) tx          <--->     pc12(sd) rx
