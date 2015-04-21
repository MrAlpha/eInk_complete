
/*
 * main.c
 */

#include <msp430G2553.h>

#include "eInk.h"


unsigned char stateMachine(unsigned char, unsigned char*, unsigned char*, unsigned char*);

//========= Globale Variablen deklarationen ===================================

extern volatile unsigned char uartBuf=0;	// ||
extern volatile unsigned char Flag=0;	// used for Flags 0bxxxxxxxx <-LSB
											//Bit:	Defined in eInk.h
											//		0: INCOMING	new Byte written to uartBuf
											//		1: DISCARD	this and upcoming data Packages are not for our address
											//
extern volatile unsigned char packageCountdown=0;	// states the number of upcoming packages from the uart to be discarded or written to memory

//==============================================================================

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initStartUp();
	
//***************************************************************************************************
	unsigned char deviceID=0;

	unsigned char operationsID=0;				//buffers OperationsID (last Instruction)

	//unsigned char payload=0;

	unsigned char payload[64]={0};
//	unsigned char check[64]={0};

	unsigned char packages=0;

//***************************************************************************************************
	__bis_SR_register(GIE);		//enable global interrupts
	__bis_SR_register(LPM3_bits);	//goto sleep

    while(1){

    	if(Flag & INCOMING){
    		Flag &= ~INCOMING;
    		stateMachine(deviceID, &operationsID, payload, &packages);
    	}
		__bis_SR_register(GIE);		//enable global interrupts
		__bis_SR_register(LPM3_bits);		//goto sleep
    }
}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART_ISR_HOOK(void){

	__bic_SR_register_on_exit(GIE);		//disable global interrupts
	__bic_SR_register_on_exit(LPM3_bits);	//end sleep

	// USCI_A0 TX buffer ready?
	while (!(IFG2 & UCA0TXIFG)); // Poll TXIFG to until set
	UCA0TXBUF = UCA0RXBUF;       // TX -> RXed character

	if(!(Flag & DISCARD)){
	uartBuf=UCA0RXBUF;
	Flag|=INCOMING;
	}
	else{
		packageCountdown--;
		if(!packageCountdown)
			Flag & ~DISCARD;
		__bis_SR_register_on_exit(LPM3_bits);		//goto sleep
	}
}

