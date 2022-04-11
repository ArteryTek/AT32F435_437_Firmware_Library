/**
  **************************************************************************
  * @file     readme.txt 
  * @version  v2.0.7
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, systick used for 
  delay function. qspi command port operate erase/write/read with polling,
  rdsr using software mode, if qspi test pass, led3 fresh, else led2 fresh.
  the qspi flash is esmt32m. 
  - qspi io0   --->   pf8
  - qspi io1   --->   pf9
  - qspi io2   --->   pf7
  - qspi io3   --->   pf6
  - qspi sck   --->   pf10
  - qspi cs    --->   pg6