/*
 * temp.c
 *
 *  Created on: 27.11.2014
 *      Author: Dave
 */

#include <msp430g2553.h>
#include "temp.h"

unsigned short getTemp(){

	ADC10CTL0 |= SREF_1 ;		//set reference Voltage to Vref+ and Ground
	ADC10CTL0 |= ADC10SHT_3;	//set sample and hold time to 64xadc10CLKs
	ADC10CTL0 |= ADC10SR;		//set reference buffer to support 50 ksps
	ADC10CTL0 |= REF2_5V;		//set ref voltage generator to 2.5V
	ADC10CTL0 |= REFON;			//enable ref voltage generator
	ADC10CTL0 |= ADC10IE;		//enable ADC10 interrupt

	ADC10CTL1 |= INCH_4;		//select inputchanal A4 -> P1.4
	ADC10CTL1 |= SHS_0;			//select sample and hold source ->ADC10SC bit
	ADC10CTL1 |= ADC10SSEL_0;	//select internal ADC-Clock
	ADC10CTL1 |= CONSEQ_0;		//select single channel single conversion

	ADC10AE0  |= BIT4;			//enable input pin P1.4 as analog input


	ADC10CTL0 |= ADC10ON;		//enable AD-Converter
	ADC10CTL0 |= ENC;			//enable conversion
	__bis_SR_register(GIE);		//enable global interrupts
	ADC10CTL0 |= ADC10SC;		//start conversion

	//goto sleep

}
#pragma vector=ADC10_VECTOR
__interrupt void adConversion(void){

	tempLastConvertion= ADC10MEM;

	ADC10CTL0 &= ~(ENC);
	ADC10CTL0 &= ~(ADC10ON);
	ADC10CTL0 &= ~(REFON);			//disable ref voltage
	//wakeup from sleep

	__bic_SR_register_on_exit(GIE);

}

