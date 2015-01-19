/*
 * cogSpi.c
 *
 *  Created on: 01.12.2014
 *      Author: Dave
 */

#include <msp430g2553.h>
#include "cogSpi.h"
#include "eInk.h"

const struct CogSpiCommands setChargepumpReduced = {0x04, 1, {0x03}};

void initCogSpi(){

	UCB0CTL1 |= UCSWRST;

	//SPI mode for the COG Drive is 0 so polarity 0 phase 0, but due to TI handles phase
	//different from standart UCCKPH has to be 1

	UCB0CTL0 |= UCCKPH;		//set Clock phase
	UCB0CTL0 |= UCMST;		//set Master Mode
	UCB0CTL0 |= UCMSB;		//set MSB first

	UCB0CTL1 |= UCSSEL_2;	//set SMCLK as clock
	UCB0BR0  |= 2;			//set Clock divider for Spi =2 -> 16MHz/2= 8MHz

//	IE2 |= UCA0TXIE;		//enable transmit interrupt

	UCB0CTL1 &= ~UCSWRST;	//clear Spi reset

//	__bis_SR_register(GIE);	//enable global interrupts
}

void sendCogCommand(const struct CogSpiCommands *commandStruct){

	initCogSpi();

	P2OUT |= EPD_CS;
	__delay_cycles(160);		//10 us delay
	P2OUT &= ~EPD_CS;

	UCB0TXBUF= 0x70;			//send Register Header
	while (!(IFG2 & UCB0TXIFG)) //_NOP()
		;

	UCB0TXBUF= commandStruct->registerIndex;	//send Register
	while (!(IFG2 & UCB0TXIFG)) //_NOP()
		;
	__delay_cycles(16);		//1 us delay to finish transmision befor CS cycle

	P2OUT |= EPD_CS;
	__delay_cycles(160);
	P2OUT &=~EPD_CS;

	UCB0TXBUF= 0x72;					//send data header
	while (!(IFG2 & UCB0TXIFG)) //_NOP()
		;


	int count;
	for(count=0; count < commandStruct->dataLength; count++){
		UCB0TXBUF= commandStruct->data[count];					//send register data

		while (!(IFG2 & UCB0TXIFG)) //_NOP()
			;
	}
	P2OUT |=EPD_CS;

}

/*
 * sendCogLineData: reads the pixels provided by *data and writes it to the spezified line (pixels have to be in 2 bit form)
 *
 * *data ->has to be array of 66
 * line -> the actual line in the picture "*data" provides
 * DUMMY to write dummy Line in shutdown cycle
 * NODUMMY else
 */

void sendCogLineData(unsigned char *data, int line, unsigned char dummy){

	initCogSpi();

	sendCogCommand(&setChargepumpReduced);

	P2OUT |= EPD_CS;
	__delay_cycles(160);		//10 us delay
	P2OUT &= ~EPD_CS;

	UCB0TXBUF= 0x70;						//send register header
	while (!(IFG2 & UCB0TXIFG)) //_NOP()
		;
	UCB0TXBUF= 0x0A;						//send address 0A which is the line buffer on COG
	while (!(IFG2 & UCB0TXIFG)) //_NOP()
		;
	__delay_cycles(16);


	P2OUT |= EPD_CS;
	__delay_cycles(160);
	P2OUT &=~EPD_CS;

	UCB0TXBUF= 0x72;						//send data header
	while (!(IFG2 & UCB0TXIFG)) //_NOP()
		;

	/***************Send line data***********************************/
			/*******Send even bits***********************************/
	int count;
	for(count=0; count<33; count++){

		while(P2IN&BIT0);					//waite till busy pin is low

		UCB0TXBUF= data[count];

		while (!(IFG2 & UCB0TXIFG)) //_NOP()
			;
	}
			/******Send scan bits************************************/
	int scanByte = line/4;		//176*2 divided by 8 bit -> actual Byte the scan line is in
	int scanShift = 6 - (line%4)*2;	//-> number of left shifts needed to put 0b11 on the right place in the byte

	if(dummy==NODUMMY){
		for(count=0; count<scanByte; count++){
			UCB0TXBUF= 0x00;					//send zeros for scan lines before active line
			while (!(IFG2 & UCB0TXIFG)) //_NOP()
				;
		}

		UCB0TXBUF= (0x00|(0x03<<scanShift));		//send 0b11 for active scan line
		while (!(IFG2 & UCB0TXIFG)) //_NOP()
			;

		for(count=scanByte+1; count<HEIGHT/4; count++){
			UCB0TXBUF= 0x00;				//send zeros for scan lines after active line
			while (!(IFG2 & UCB0TXIFG)) //_NOP()
				;
		}
	}
	if(dummy==DUMMY){
		for(count=0; count<HEIGHT/4; count++){
			UCB0TXBUF= 0x00;				//send zeros for all scan lines
			while (!(IFG2 & UCB0TXIFG)) //_NOP()
				;
		}
	}
			/******Send odd bits*************************************/
	for(count=33;count<66;count++){
		while(P2IN&BIT0);					//waite till busy pin is low
		UCB0TXBUF= data[count];
		while (!(IFG2 & UCB0TXIFG)) //_NOP()
			;
	}
			/******Send 0 byte to complete line*************************************/
	while(P2IN&BIT0);					//waite till busy pin is low
	UCB0TXBUF= 0x00;
	__delay_cycles(160);
	P2OUT |=EPD_CS;
}

//#pragma vector =USCIAB0TX_VECTOR
//__interrupt void spiTxIRvector(void){
//
//	__bic_SR_register_on_exit(LPM0_bits);
//}
