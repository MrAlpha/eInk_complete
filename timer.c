/*
 * timer.c
 *
 *  Created on: 25.11.2014
 *      Author: Dave
 */
#include <msp430G2553.h>
#include <timer.h>

/*
 *
 *  startTimerA0
 *	milliseconds: desired time in milliseconds
 *	sleepOnOff: 	SLEEPON to put uC in sleep while waiting
 *				SLEEPOFF to do other stuff while waiting
 */
void startTimerA0(unsigned short milliseconds, unsigned char sleepOnOff){

	TA0CTL |= TASSEL_1;	//Clock source =very low power clock ~12 kHz

	TA0CCR0= 14*milliseconds;

	TACCTL0|=CCIE;		//enable compare interrupt
	TA0CTL |= MC_1;		//start timer A0 in up mode

	if(sleepOnOff){
		__bis_SR_register(GIE);		//enable global interrupts
		__bis_SR_register(LPM3_bits);	//goto sleep
	}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){

	__bic_SR_register_on_exit(GIE);	//dissable global interrupts
	__bic_SR_register_on_exit(LPM3_bits);	//end sleep
	TA0CTL &= MC_0;					//dissable Timer

}
