/*
 * powerOnCog.c
 *
 *  Created on: 02.12.2014
 *      Author: Dave
 */

#include <msp430g2553.h>
#include "eInk.h"
#include <pwm.h>
#include "timer.h"
#include "cogSpi.h"

/***
 * Instances of CogSpiCommands structur to define the commands send to the COG
 */
const struct CogSpiCommands channelSelect = {0x01, 8, {	0x00,
														0x00,
														0x00,
														0x7F,
														0xFF,
														0xFE,
														0x00,
														0x00 }
};

const struct CogSpiCommands dcdcFrequencySetting = {0x06, 1, {0xFF}};
const struct CogSpiCommands highPowerModeSetting = {0x07, 1, {0x9D}};
const struct CogSpiCommands disableADC			 = {0x08, 1, {0x00}};
const struct CogSpiCommands setVcomLevel		 = {0x09, 2, {0xD0, 0x00}};
const struct CogSpiCommands gateSourceVoltageLevel={0x04, 1, {0x00}};
const struct CogSpiCommands driverLatchOn		 = {0x03, 1, {0x01}};
const struct CogSpiCommands driverLatchOff		 = {0x03, 1, {0x00}};
const struct CogSpiCommands startPosChargepump	 = {0x05, 1, {0x01}};
const struct CogSpiCommands startNegChargepump	 = {0x05, 1, {0x03}};
const struct CogSpiCommands startVcomChargepump	 = {0x05, 1, {0x0F}};
const struct CogSpiCommands outputEnableDisable	 = {0x02, 1, {0x24}};

void powerOnCog(){
	startPWM();
	startTimerA0(5,SLEEPON);
	P2OUT |= PANAL_ON;
	startTimerA0(10,SLEEPON);
	P2OUT |= EPD_CS;
	P2OUT |= BORDER_CONTROL;
	P2OUT |= RESET;
	startTimerA0(5,SLEEPON);
	P2OUT &= ~RESET;
	startTimerA0(5,SLEEPON);
	P2OUT |= RESET;
	startTimerA0(5,SLEEPON);
}

void initCog(){

	while(P2IN&BUSY_);

	initCogSpi();

	sendCogCommand(&channelSelect);
	sendCogCommand(&dcdcFrequencySetting);
	sendCogCommand(&highPowerModeSetting);
	sendCogCommand(&disableADC);
	sendCogCommand(&setVcomLevel);
	sendCogCommand(&gateSourceVoltageLevel);
	startTimerA0(5,SLEEPON);
	sendCogCommand(&driverLatchOn);
	sendCogCommand(&driverLatchOff);
	sendCogCommand(&startPosChargepump);
	startTimerA0(30,SLEEPON);
	stopPWM();
	sendCogCommand(&startNegChargepump);
	startTimerA0(30,SLEEPON);
	sendCogCommand(&startVcomChargepump);
	startTimerA0(30,SLEEPON);
	sendCogCommand(&outputEnableDisable);

	}
