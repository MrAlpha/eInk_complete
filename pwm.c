/*
 * pwm.c
 *
 *  Created on: 29.11.2014
 *      Author: Dave
 */
#include <pwm.h>
#include <msp430g2553.h>

//startPWM() outputs a 200kHz 50% dutycycle PWM Signal on P2.1

void startPWM(){

	TA1CTL |= TASSEL_2;		//select submainclock == masterclock==16MHz
	TA1CTL |= ID_0;			//divide clock by 1 -> 16MHz

	TA1CCR0 = 80/2;			//set TA1CCR0 to 40 to cownt down after half of the periode

	TA1CCTL1 |= OUTMOD_4;	//set toggle

	TA1CCR1 = 20;			//set point of timer register at wich to toggle

	TA1CTL |= MC_3;			//start Timer in Up-Down Mode
}

void stopPWM(){
	TA1CTL = 0;			//stop Timer
	TA1CCTL1=0;
	TA1CCTL1 &= ~OUT;		//set Output to 0
}
