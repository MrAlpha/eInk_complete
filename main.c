
/*
 * main.c
 */

#include <msp430G2553.h>


#include "eInk.h"
//#include "FlashTreiber.h"
//#include "timer.h"
//#include "temp.h"
//#include "pwm.h"
//#include "powerOnCog.h"
//#include "writeFrame.h"
//#include "cogSpi.h"
//#include "powerOffCog.h"


unsigned char stateMachine(unsigned char, unsigned char*, unsigned char*, unsigned char*);

//========= Globale Variablen deklarationen ===================================

extern volatile unsigned char uartBuf=0;	// ||
extern volatile unsigned char Flag=0;	// used for Flags 0bxxxxxxxx <-LSB
											//Bit:
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

	unsigned char payload[64]={1};
//	unsigned char check[64]={0};

	unsigned char packages=0;

//***************************************************************************************************
	__bis_SR_register(GIE);		//enable global interrupts
	__bis_SR_register(LPM3_bits);	//goto sleep

    while(1){

    	if(Flag & INCOMING){
    		Flag &= ~INCOMING;
    		stateMachine(deviceID, &operationsID, payload, &packages);

    		__bis_SR_register(GIE);		//enable global interrupts
    		__bis_SR_register(LPM3_bits);		//goto sleep
    	}
    }
 //****************************************************************************************************

////    unsigned char display[7]={255,255,255,255,255,255,255};
//    unsigned char display[7]={0};
//    display[0]='4';
//    display[1]='2';
//    display[2]=',';
//    display[3]='1';
//    display[4]='3';
//    display[5]='E';


//    initCogSpi();
//    sendCogCommand(&test);

//    while(1){
//    	//P2OUT^=BIT7;
//    }



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


//#pragma vector=USCIAB0RX_VECTOR       // (7 * 1u)                     /* 0xFFEE USCI A0/B0 Receive */
//__interrupt void a05(void){
//}

#pragma vector=TIMER0_A1_VECTOR       // (8 * 1u)                     /* 0xFFF0 Timer0)A CC1, TA0 */
__interrupt void a06(void){


}

//#pragma vector=TIMER0_A0_VECTOR      //  (9 * 1u)                     /* 0xFFF2 Timer0_A CC0 */
//__interrupt void a07(void){
//
//
//}

#pragma vector=WDT_VECTOR              //(10 * 1u)                    /* 0xFFF4 Watchdog Timer */
__interrupt void a08(void){


}

#pragma vector=COMPARATORA_VECTOR      //(11 * 1u)                    /* 0xFFF6 Comparator A */
__interrupt void a09(void){


}

#pragma vector=TIMER1_A1_VECTOR        //(12 * 1u)                    /* 0xFFF8 Timer1_A CC1-4, TA1 */
__interrupt void a10(void){


}

#pragma vector=TIMER1_A0_VECTOR        //(13 * 1u)                    /* 0xFFFA Timer1_A CC0 */
__interrupt void a11(void){


}

#pragma vector=NMI_VECTOR              //(14 * 1u)                    /* 0xFFFC Non-maskable */
__interrupt void a12(void){


}

#pragma vector=RESET_VECTOR            //(15 * 1u)                    /* 0xFFFE Reset [Highest Priority] */
__interrupt void a13(void){


}
