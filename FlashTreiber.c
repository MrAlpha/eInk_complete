/*
 * FlashTreiber.c
 *
 *  Created on: 22.07.2014
 *      Author: Dave
 *
 *  Driver for the MX25V8005 SPI Flash Memory
 */

#include "FlashTreiber.h"
#include <msp430G2553.h>


/*************************************SPI**************************************************/
/**
 * SPI synchronous write
 */

void spiFlashInit(){
	/*
	 *  ======== USCI_B0 ========
	 *  Initialize Universal Serial Communication Interface B0 SPI 2xx
	 */

    /* Disable USCI */
    UCB0CTL1 |= UCSWRST;

    /*
     * Control Register 0
     *
     * ~UCCKPH -- Data is changed on the first UCLK edge and captured on the following edge
     * ~UCCKPL -- Inactive state is low
     * UCMSB -- MSB first
     * ~UC7BIT -- 8-bit
     * UCMST -- Master mode
     * UCMODE_0 -- 3-Pin SPI
     * UCSYNC -- Synchronous Mode
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    UCB0CTL0 = UCMSB | UCMST | UCMODE_0 | UCSYNC /*| UCCKPH*/ | UCCKPL;

    /*
     * Control Register 1
     *
     * UCSSEL_2 -- SMCLK
     * UCSWRST -- Enabled. USCI logic held in reset state
     */
    UCB0CTL1 = UCSSEL_2 | UCSWRST;

     //Bit Rate Control Register 0 	//not used we transmitt with MasterCLk speed
    UCB0BR0 = 0;

    /* Bit Rate Control Register 1 */
    UCB0BR1 = 0;

    /* Enable USCI */
    UCB0CTL1 &= ~UCSWRST;

}

void spiWrite(unsigned char Data) {
	UCB0TXBUF = Data;
	//delay(1);					//<-This made the difference
	while (!(IFG2 & UCB0TXIFG)) //_NOP()
		;
}


/**
 * SPI synchronous read
 */
unsigned char spiRead(unsigned char data) {
	UCB0TXBUF = data;
	//delay(1);
	while ((UCB0STAT & UCBUSY));// _NOP();
	data = UCB0RXBUF;
	return data;
}

/***********************************************end SPI*********************************/
//Implement Timer funktion here or remove all delay use!!!!!!!!
void delay(unsigned int ms){
	while(ms--)
		__delay_cycles(16000); // set for 16Mhz change it to 1000 for 1 Mhz
}


void setCSlow(){
	P2OUT &= ~(BIT7);
}
void setCShigh(){
	P2OUT |= BIT7;
}

unsigned char checkWriteEnable() {
	unsigned char gDataBuffer;

	gDataBuffer= getStatusRegister();
	if( (gDataBuffer & 0x02)  == 0x02 )
		return 1;
	else
		return 0;
}

void setWriteEnable(){ /////WHAT IS WRONG?
	do{
		//delay(5);
		setCSlow();
		spiWrite(WREN);
		setCShigh();
		//delay(5);
		//_NOP();_NOP();_NOP();_NOP();_NOP();	//<- mayby this made NOT the difference
	}while(!checkWriteEnable());
}

unsigned char getStatusRegister(){
	unsigned char spiRecive=0;
	setCSlow();
	spiWrite(RDSR);
	spiRecive=spiRead(0x00);
	setCShigh();
	return spiRecive;
}

unsigned char getFlashBusy(){
	unsigned char status=0;
	status=getStatusRegister();
//	delay(1);-------------------------------------
	if((status & WIP)== WIP){
		return 1;
	}
	else
		return 0;
}

unsigned char getFlashID() {
	unsigned char id[3]={0};
	setCSlow();
	spiWrite(RDID);
	id[0]=spiRead(0x00);
	id[1]=spiRead(0x00);
	id[2]=spiRead(0x00);
	setCShigh();
return id[0];
}

void sendFlashAddress(long flashAddress){
	spiWrite((flashAddress >> 16));
	spiWrite((flashAddress >> 8));
	spiWrite((flashAddress));
}

void readData(unsigned long flashAddress, unsigned char *recive, unsigned long byteNumber){
	unsigned long i=0;
	while(getFlashBusy());

	setCSlow();
	spiWrite(FREAD);
	//delay(1);
	sendFlashAddress(flashAddress);
	spiWrite(0x00);


	for(i=0; i<byteNumber; i++){
		*(recive +i) =spiRead(0x00);
	}
	setCShigh();
}

void readDataSpiStart(unsigned long flashAddress){
	setCSlow();
	spiWrite(FREAD);
	//delay(1);
	sendFlashAddress(flashAddress);
	spiWrite(0x00);
}

unsigned char readDataSpiData(){
	return spiRead(0x00);
}
void readDataSpiEnd(){
	setCShigh();
}

void writeData(unsigned long flashAddress,unsigned char *send, unsigned long byteNumber){

	unsigned long i=0;

	while(getFlashBusy()) /*__no_operation()*/;

	setWriteEnable();
	while(getFlashBusy()) /*__no_operation()*/;

	setCSlow();
	spiWrite(PP);
	sendFlashAddress(flashAddress);


	for(i=0; i<byteNumber; i++){
		spiWrite(*(send+i));
	}

	setCShigh();

	while(getFlashBusy()) __no_operation();
}

void writeDataSpiStart(unsigned long flashAdress){

	while(getFlashBusy()) /*__no_operation()*/;

	setWriteEnable();
	while(getFlashBusy()) /*__no_operation()*/;

	setCSlow();
	spiWrite(PP);
	sendFlashAddress(flashAdress);
}

void writeDataSpiTransmit(unsigned char send){
	spiWrite(send);
}

void writeDataSpiEnd(){
	setCShigh();

	while(getFlashBusy()) __no_operation();
}

void eraseSector(unsigned long flashAddress ) {
	while( getFlashBusy()) /*_NOP()*/;
	// Setting Write Enable Latch bit
	setWriteEnable();
	while(getFlashBusy()) /*__no_operation()*/;
	// Chip select go low to start a flash command
	setCSlow();
	//delay(1);

	//Write Sector Erase command = 0x20;
	spiWrite(SE);
	//delay(1);

	sendFlashAddress( flashAddress );

	// Chip select go high to end a flash command
	setCShigh();
	while(getFlashBusy());
}


void eraseFlash(){
	while(getFlashBusy()) /*_NOP()*/;

	setWriteEnable();
	setCSlow();
	//spiWrite(WREN);
	//delay(2);
	spiWrite(CE);
	setCShigh();

	while(getFlashBusy());
}

