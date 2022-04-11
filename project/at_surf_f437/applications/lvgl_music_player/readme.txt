/**
  **************************************************************************
  * @file     templates/readme.txt 
  * @version  v2.0.7
  * @date     2022-04-02
  * @brief    readme
  **************************************************************************
  */

  this lvgl(v8) music demo is based on the at-start board ,  . 

	The  pin to pin with LCD and XMC(16 bits):
            MRB3511 ILI9488       SURF Board            
	GPIO      PD3     LCD_RESET      NRST
	GPIO      PB0     LCD_BL         PB0 but no connect
	XMC_A16   PD11    LCD_RS         PF0 XMC_A0
	XMC_NE1   PD7     LCD_CS         PD7 XMC_NE1
	XMC_NWE   PD2     LCD_WR         PD5
	XMC_D0	  PB14	  DATA[0]        PD14
	XMC_D1	  PC6	    DATA[1]        PD15
	XMC_D2	  PC11	  DATA[2]        PD0
	XMC_D3	  PC12	  DATA[3]        PD1
	XMC_D4	  PA2	    DATA[4]        PE7
	XMC_D5	  PA3	    DATA[5]        PE8
	XMC_D6	  PA4	    DATA[6]        PE9
	XMC_D7	  PA5	    DATA[7]        PE10
	XMC_D8	  PE11	  DATA[8]        PE11
	XMC_D9	  PE12	  DATA[9]        PE12
	XMC_D10	  PE13	  DATA[10]       PE13
	XMC_D11	  PE14	  DATA[11]       PE14
	XMC_D12	  PE15	  DATA[12]       PE15
	XMC_D13	  PB12	  DATA[13]       PD8
	XMC_D14	  PD9	    DATA[14]       PD9
	XMC_D15	  PD10	  DATA[15]       PD10
  
  The  pin to pin with touch pad:
  I2C1 SCL      PB6  
  I2C1 SDA      PB7  
  TP INT        PE3  
  TP RS         PD11 