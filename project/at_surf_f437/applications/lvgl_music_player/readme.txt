/**
  **************************************************************************
  * @file     templates/readme.txt 
  * @brief    readme
  **************************************************************************
  */

  this lvgl(v8) music demo is based on the at-surf board. 
  the  pin to pin with LCD and XMC(16 bits):          
  GPIO        LCD_RESET      NRST
  GPIO        LCD_BL         IO0_0
  XMC_A10     LCD_RS         PG0
  XMC_NE1     LCD_CS         PD7
  XMC_NWE     LCD_WR         PD5
  XMC_NWE     LCD_RD         PD4
  XMC_D0      DATA[0]        PD14
  XMC_D1      DATA[1]        PD15
  XMC_D2      DATA[2]        PD0
  XMC_D3      DATA[3]        PD1
  XMC_D4      DATA[4]        PE7
  XMC_D5      DATA[5]        PE8
  XMC_D6      DATA[6]        PE9
  XMC_D7      DATA[7]        PE10
  XMC_D8      DATA[8]        PE11
  XMC_D9      DATA[9]        PE12
  XMC_D10     DATA[10]       PE13
  XMC_D11     DATA[11]       PE14
  XMC_D12     DATA[12]       PE15
  XMC_D13     DATA[13]       PD8
  XMC_D14     DATA[14]       PD9
  XMC_D15     DATA[15]       PD10
  
  the  pin to pin with touch pad:
  I2C1 SCL      PB6  
  I2C1 SDA      PB7  
  TP INT        PE3  
  TP RS         PD11 