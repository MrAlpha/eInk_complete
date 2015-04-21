/*
 * powerOffCog.c
 *
 *  Created on: 11.12.2014
 *      Author: Dave
 */

#include "msp430g2553.h"

#include "timer.h"
#include "cogSpi.h"
#include "writeFrame.h"
#include "eInk.h"

void powerOff(){

	const struct CogSpiCommands latchResetOn		 = {0x03, 1, {0x01}};
	const struct CogSpiCommands outputEnableOff		 = {0x02, 1, {0x05}};
	const struct CogSpiCommands chargepumpVcomOff	 = {0x05, 1, {0x0E}};
	const struct CogSpiCommands negChargepumpOff	 = {0x05, 1, {0x02}};
	const struct CogSpiCommands discharge			 = {0x04, 1, {0x0c}};
	const struct CogSpiCommands allChargepumpsOff	 = {0x05, 1, {0x00}};
	const struct CogSpiCommands oscOff				 = {0x07, 1, {0x0D}};
	const struct CogSpiCommands dischargeInt		 = {0x04, 1, {0x50}};
	const struct CogSpiCommands dischargeInt2		 = {0x04, 1, {0xA0}};
	const struct CogSpiCommands dischargeInt3		 = {0x04, 1, {0x00}};

	initCogSpi();

	startTimerA0(25,SLEEPON);
	P2OUT &=~BORDER_CONTROL;			//toggle border Pin
	startTimerA0(250,SLEEPON);
	P2OUT |= BORDER_CONTROL;

	sendCogCommand(&latchResetOn);
	sendCogCommand(&outputEnableOff);
	sendCogCommand(&chargepumpVcomOff);
	sendCogCommand(&negChargepumpOff);
	sendCogCommand(&discharge);
	startTimerA0(120,SLEEPON);
	sendCogCommand(&allChargepumpsOff);
	sendCogCommand(&oscOff);
	sendCogCommand(&dischargeInt);
	startTimerA0(40,SLEEPON);
	sendCogCommand(&dischargeInt2);
	startTimerA0(40,SLEEPON);
	sendCogCommand(&dischargeInt3);

	P2OUT &= ~(BORDER_CONTROL|RESET|PANAL_ON|EPD_CS);	//set reset, CS, border and voltage to 0

	P2OUT |= DISCHARGE;
	startTimerA0(150,SLEEPON);
	P2OUT &= ~DISCHARGE;

}
