﻿#include "bsp_sercom.h"

bool sendDataSlaveWIRE( Sercom *sercom, uint8_t data )
{
	//Send data
	sercom->I2CS.DATA.bit.DATA = data;

	//Problems on line? nack received?
	if(!sercom->I2CS.INTFLAG.bit.DRDY || sercom->I2CS.STATUS.bit.RXNACK)
	return false;
	else
	return true;
}

int availableWIRE(  Sercom *sercom )
{
	if(isMasterWIRE(sercom))
	return sercom->I2CM.INTFLAG.bit.SB;
	else
	return sercom->I2CS.INTFLAG.bit.DRDY;
}

void prepareNackBitWIRE( Sercom *sercom )
{
	if(isMasterWIRE(sercom)) {
		// Send a NACK
		sercom->I2CM.CTRLB.bit.ACKACT = 1;
		} else {
		sercom->I2CS.CTRLB.bit.ACKACT = 1;
	}
}

void prepareAckBitWIRE( Sercom *sercom )
{
	if(isMasterWIRE(sercom)) {
		// Send an ACK
		sercom->I2CM.CTRLB.bit.ACKACT = 0;
		} else {
		sercom->I2CS.CTRLB.bit.ACKACT = 0;
	}
}

void prepareCommandBitsWire(Sercom *sercom, uint8_t cmd)
{
	if(isMasterWIRE(sercom)) {
		sercom->I2CM.CTRLB.bit.CMD = cmd;

		while(sercom->I2CM.SYNCBUSY.bit.SYSOP)
		{
			// Waiting for synchronization
		}
		} else {
		sercom->I2CS.CTRLB.bit.CMD = cmd;
	}
}

uint8_t readDataWIRE(  Sercom *sercom )
{
	if(isMasterWIRE(sercom))
	{
		while( sercom->I2CM.INTFLAG.bit.SB == 0 )
		{
			// Waiting complete receive
		}

		return sercom->I2CM.DATA.bit.DATA ;
	}
	else
	{
		return sercom->I2CS.DATA.reg ;
	}
}

void initClock( Sercom *sercom )
{
	uint8_t clockId = 0;

	if(sercom == SERCOM0)
	{
		clockId = GCLK_CLKCTRL_ID_SERCOM0_CORE;
	}
	else if(sercom == SERCOM1)
	{
		clockId = GCLK_CLKCTRL_ID_SERCOM1_CORE;
	}

#if defined(SERCOM2)
	else if(sercom == SERCOM2)
	{
		clockId = GCLK_CLKCTRL_ID_SERCOM2_CORE;
	}
#endif

#if defined(SERCOM3)
	else if(sercom == SERCOM3)
	{
		clockId = GCLK_CLKCTRL_ID_SERCOM3_CORE;
	}
#endif

#if defined(SERCOM4)
	else if(sercom == SERCOM4)
	{
		clockId = GCLK_CLKCTRL_ID_SERCOM4_CORE;
	}
#endif

#if defined(SERCOM5)
	else if(sercom == SERCOM5)
	{
		clockId = GCLK_CLKCTRL_ID_SERCOM5_CORE;
	}
#endif

	//Setting clock
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( clockId ) | // Generic Clock 0 (SERCOMx)
	GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is source
	GCLK_CLKCTRL_CLKEN ;
	
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( GCLK_CLKCTRL_ID_SERCOMX_SLOW ) |
	GCLK_CLKCTRL_GEN_GCLK1 |
	GCLK_CLKCTRL_CLKEN ;

	while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
	{
		/* Wait for synchronization */
	}
}


/*	=========================
 *	===== Sercom WIRE
 *	=========================
 */
void resetWIRE(Sercom *sercom)
{	
  //I2CM OR I2CS, no matter SWRST is the same bit.

  //Setting the Software bit to 1
  sercom->I2CM.CTRLA.bit.SWRST = 1;

  //Wait both bits Software Reset from CTRLA and SYNCBUSY are equal to 0
  while(sercom->I2CM.CTRLA.bit.SWRST || sercom->I2CM.SYNCBUSY.bit.SWRST);
}

void enableWIRE(Sercom *sercom)
{
  
  // Enable the I²C master mode
  sercom->I2CM.CTRLA.bit.ENABLE = 1 ;

  while ( sercom->I2CM.SYNCBUSY.bit.ENABLE != 0 )
  {
    // Waiting the enable bit from SYNCBUSY is equal to 0;
  }

  // Setting bus idle mode
  sercom->I2CM.STATUS.bit.BUSSTATE = 1 ;

  while ( sercom->I2CM.SYNCBUSY.bit.SYSOP != 0 )
  {
    // Wait the SYSOP bit from SYNCBUSY coming back to 0
  }
}

void initSlaveWIRE( Sercom *sercom, uint8_t ucAddress )
{
  // Initialize the peripheral clock and interruption
  initClock(sercom) ;
  resetWIRE(sercom) ;

  // Set slave mode
  sercom->I2CS.CTRLA.bit.MODE = I2C_SLAVE_OPERATION;

  sercom->I2CS.ADDR.reg = SERCOM_I2CS_ADDR_ADDR( ucAddress & 0x7Ful ) | // 0x7F, select only 7 bits
                          SERCOM_I2CS_ADDR_ADDRMASK( 0x00ul ) ;         // 0x00, only match exact address

  enableInterruptsWIRE( sercom );

  while ( sercom->I2CM.SYNCBUSY.bit.SYSOP != 0 )
  {
    // Wait the SYSOP bit from SYNCBUSY to come back to 0
  }
}

/* 	=========================
 *	===== Sercom UART
 *	=========================
*/

void resetUART( Sercom * sercom )
{
	// Start the Software Reset
	sercom->USART.CTRLA.bit.SWRST = 1 ;

	while ( sercom->USART.CTRLA.bit.SWRST || sercom->USART.SYNCBUSY.bit.SWRST )
	{
		// Wait for both bits Software Reset from CTRLA and SYNCBUSY coming back to 0
	}
} 

bool isDataRegisterEmptyUART( Sercom * sercom )
{
	//DRE : Data Register Empty
	return sercom->USART.INTFLAG.bit.DRE;
}

void initUART( Sercom * sercom, SercomUartSampleRate sampleRate, uint32_t baudrate)
{
  initClock(sercom);
  resetUART(sercom);

  //Setting the CTRLA register
  sercom->USART.CTRLA.reg =	SERCOM_USART_CTRLA_MODE(0x01) |
                SERCOM_USART_CTRLA_SAMPR(sampleRate);

  //Setting the Interrupt register
  sercom->USART.INTENSET.reg =	SERCOM_USART_INTENSET_RXC |  //Received complete
                                SERCOM_USART_INTENSET_ERROR; //All others errors

	uint16_t sampleRateValue;

	if (sampleRate == SAMPLE_RATE_x16) {
		sampleRateValue = 16;
	} else {
		sampleRateValue = 8;
	}

	// Asynchronous fractional mode (Table 24-2 in datasheet)
	//   BAUD = fref / (sampleRateValue * fbaud)
	// (multiply by 8, to calculate fractional piece)
	uint32_t baudTimes8 = (SystemCoreClock * 8) / (sampleRateValue * baudrate);

	sercom->USART.BAUD.FRAC.FP   = (baudTimes8 % 8);
	sercom->USART.BAUD.FRAC.BAUD = (baudTimes8 / 8);
}

void disableInterruptsUART(Sercom * sercom)
{
	  sercom->USART.INTENCLR.reg =	SERCOM_USART_INTENCLR_RXC |  //Received complete
                                SERCOM_USART_INTENCLR_ERROR; //All others errors
}

void initFrame( Sercom * sercom , SercomUartCharSize charSize, SercomDataOrder dataOrder, SercomParityMode parityMode, SercomNumberStopBit nbStopBits)
{
  //Setting the CTRLA register
  sercom->USART.CTRLA.reg |=	SERCOM_USART_CTRLA_FORM( (parityMode == SERCOM_NO_PARITY ? 0 : 1) ) |
                dataOrder << SERCOM_USART_CTRLA_DORD_Pos;

  //Setting the CTRLB register
  sercom->USART.CTRLB.reg |=	SERCOM_USART_CTRLB_CHSIZE(charSize) |
                nbStopBits << SERCOM_USART_CTRLB_SBMODE_Pos |
                (parityMode == SERCOM_NO_PARITY ? 0 : parityMode) << SERCOM_USART_CTRLB_PMODE_Pos; //If no parity use default value
}

void initPads( Sercom * sercom , SercomUartTXPad txPad, SercomRXPad rxPad)
{
  //Setting the CTRLA register
  sercom->USART.CTRLA.reg |=	SERCOM_USART_CTRLA_TXPO(txPad) |
                SERCOM_USART_CTRLA_RXPO(rxPad);

  // Enable Transceiver and Receiver
  sercom->USART.CTRLB.reg |= SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN ;
}

int writeDataUART( Sercom * sercom ,uint8_t data)
{
	// Wait for data register to be empty
	while(!isDataRegisterEmptyUART(sercom));

	//Put data into DATA register
	sercom->USART.DATA.reg = (uint16_t)data;
	return 1;
}

int writeDataUART( Sercom * sercom , char const *buffer){
      while(*buffer != '\0')
          writeDataUART(sercom, *buffer++);

      return 1;
}

void enableUART( Sercom * sercom )
{
	while(sercom->USART.SYNCBUSY.bit.ENABLE || sercom->USART.SYNCBUSY.bit.SWRST);
	
	//Setting  the enable bit to 1
	sercom->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;

	//Wait for then enable bit from SYNCBUSY is equal to 0;
	while(sercom->USART.SYNCBUSY.bit.ENABLE);
}

void disableUART( Sercom *sercom )
{
	while(sercom->USART.SYNCBUSY.bit.ENABLE || sercom->USART.SYNCBUSY.bit.SWRST);
	
	//Setting  the enable bit to 0
	sercom->USART.CTRLA.bit.ENABLE = 0;

	//Wait for then enable bit from SYNCBUSY is equal to 0;
	while(sercom->USART.SYNCBUSY.bit.ENABLE);
}

void setUARTBaud( Sercom *sercom, uint32_t baudrate )
{
	disableUART(sercom);
	
	// Asynchronous fractional mode (Table 24-2 in datasheet)
	//   BAUD = fref / (sampleRateValue * fbaud)
	// (multiply by 8, to calculate fractional piece)
	uint32_t baudTimes8 = (SystemCoreClock * 8) / (16 * baudrate);

	sercom->USART.BAUD.FRAC.FP   = (baudTimes8 % 8);
	sercom->USART.BAUD.FRAC.BAUD = (baudTimes8 / 8);
	
	enableUART(sercom);
}

/*	=========================
 *	===== Sercom SPI
 *	=========================
*/

uint8_t calculateBaudrateSynchronous(uint32_t baudrate)
{
	return SERCOM_FREQ_REF / (2 * baudrate) - 1;
}

void initSPI( Sercom *sercom, SercomSpiTXPad mosi, SercomRXPad miso, SercomSpiCharSize charSize, SercomDataOrder dataOrder)
{
  resetSPI(sercom);
  initClock(sercom);

  //Setting the CTRLA register
  sercom->SPI.CTRLA.reg =	SERCOM_SPI_CTRLA_MODE_SPI_MASTER |
                          SERCOM_SPI_CTRLA_DOPO(mosi) |
                          SERCOM_SPI_CTRLA_DIPO(miso) |
                          dataOrder << SERCOM_SPI_CTRLA_DORD_Pos;

  //Setting the CTRLB register
  sercom->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_CHSIZE(charSize) |
                          SERCOM_SPI_CTRLB_RXEN;	//Active the SPI receiver.


}

void initSPISlave( Sercom *sercom, SercomSpiTXPad miso, SercomRXPad mosi, SercomSpiCharSize charSize, SercomDataOrder dataOrder)
{
	initClock(sercom);
	resetSPI(sercom);

	//Setting the CTRLA register
	sercom->SPI.CTRLA.reg =	SERCOM_SPI_CTRLA_MODE_SPI_SLAVE |
	SERCOM_SPI_CTRLA_DOPO(miso) |
	SERCOM_SPI_CTRLA_DIPO(mosi) |
	dataOrder << SERCOM_SPI_CTRLA_DORD_Pos;

	//Setting the CTRLB register
	sercom->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_CHSIZE(charSize) |
	SERCOM_SPI_CTRLB_RXEN | SERCOM_SPI_CTRLB_PLOADEN;	//Active the SPI receiver.


}

void initSPIClock( Sercom *sercom, SercomSpiClockMode clockMode, uint32_t baudrate)
{
  //Extract data from clockMode
  int cpha, cpol;

  if((clockMode & (0x1ul)) == 0 )
    cpha = 0;
  else
    cpha = 1;

  if((clockMode & (0x2ul)) == 0)
    cpol = 0;
  else
    cpol = 1;

  //Setting the CTRLA register
  sercom->SPI.CTRLA.reg |=	( cpha << SERCOM_SPI_CTRLA_CPHA_Pos ) |
                            ( cpol << SERCOM_SPI_CTRLA_CPOL_Pos );

  //Synchronous arithmetic
  sercom->SPI.BAUD.reg = calculateBaudrateSynchronous(baudrate);
}

void resetSPI( Sercom *sercom )
{
  //Setting the Software Reset bit to 1
  sercom->SPI.CTRLA.bit.SWRST = 1;

  //Wait both bits Software Reset from CTRLA and SYNCBUSY are equal to 0
  while(sercom->SPI.CTRLA.bit.SWRST || sercom->SPI.SYNCBUSY.bit.SWRST);
}

void enableSPI( Sercom *sercom )
{
  //Setting the enable bit to 1
  sercom->SPI.CTRLA.bit.ENABLE = 1;

  while(sercom->SPI.SYNCBUSY.bit.ENABLE)
  {
    //Waiting then enable bit from SYNCBUSY is equal to 0;
  }
}

void disableSPI( Sercom *sercom )
{
  while(sercom->SPI.SYNCBUSY.bit.ENABLE)
  {
    //Waiting then enable bit from SYNCBUSY is equal to 0;
  }

  //Setting the enable bit to 0
  sercom->SPI.CTRLA.bit.ENABLE = 0;
  
  while(sercom->SPI.SYNCBUSY.bit.ENABLE);
}

void setDataOrderSPI( Sercom *sercom, SercomDataOrder dataOrder)
{
  //Register enable-protected
  disableSPI(sercom);

  sercom->SPI.CTRLA.bit.DORD = dataOrder;

  enableSPI(sercom);
}

SercomDataOrder getDataOrderSPI( Sercom *sercom )
{
  return (sercom->SPI.CTRLA.bit.DORD ? LSB_FIRST : MSB_FIRST);
}

void setBaudrateSPI( Sercom *sercom, uint8_t divider)
{
  //Can't divide by 0
  if(divider == 0)
    return;

  //Register enable-protected
  disableSPI(sercom);

  sercom->SPI.BAUD.reg = calculateBaudrateSynchronous( SERCOM_FREQ_REF / divider );

  enableSPI(sercom);
}

void setClockModeSPI( Sercom *sercom, SercomSpiClockMode clockMode)
{
  int cpha, cpol;
  if((clockMode & (0x1ul)) == 0)
    cpha = 0;
  else
    cpha = 1;

  if((clockMode & (0x2ul)) == 0)
    cpol = 0;
  else
    cpol = 1;

  //Register enable-protected
  disableSPI(sercom);

  sercom->SPI.CTRLA.bit.CPOL = cpol;
  sercom->SPI.CTRLA.bit.CPHA = cpha;

  enableSPI(sercom);
}

uint8_t transferDataSPI( Sercom *sercom, uint8_t data)
{
  sercom->SPI.DATA.bit.DATA = data; // Writing data into Data register

  while( sercom->SPI.INTFLAG.bit.RXC == 0 )
  {
    // Waiting Complete Reception
  }

  return sercom->SPI.DATA.bit.DATA;  // Reading data
}

bool isBufferOverflowErrorSPI( Sercom *sercom )
{
  return sercom->SPI.STATUS.bit.BUFOVF;
}

bool isDataRegisterEmptySPI( Sercom *sercom )
{
  //DRE : Data Register Empty
  return sercom->SPI.INTFLAG.bit.DRE;
}

//bool isTransmitCompleteSPI()
//{
//	//TXC : Transmit complete
//	return sercom->SPI.INTFLAG.bit.TXC;
//}
//
//bool isReceiveCompleteSPI()
//{
//	//RXC : Receive complete
//	return sercom->SPI.INTFLAG.bit.RXC;
//}
