/*
 * stateMachine.c
 *
 *  Created on: 07.05.2014
 *      Author: Dave
 */
#include <msp430.h>

#include "eInk.h"
#include "timer.h"
#include "cogSpi.h"
#include "powerOnCog.h"
#include "writeFrame.h"
#include "powerOffCog.h"

unsigned char stateMachine(unsigned char deviceID, unsigned char *pOprID, unsigned char *payload, unsigned char *packages){

	static unsigned int state=0;

	switch(state){

	case 0:
		if (uartBuf== 0xAB)		//Preamble (0b10101011)?
			state=1;
		else
			state=0;
		break;

	case 1:
		if (uartBuf== 0xFF)		//Delimiter?
			state=2;
		else
			state=0;
		break;

	case 2:
		if (uartBuf==deviceID)		//Address == own Address?
			state=3;
		else					//not own address
			state=201;

		break;

	case 3:
		*pOprID= uartBuf;		//write received command to operations ID buffer
		if(!(*pOprID & 0xC0)&&((uartBuf& 0x3F))<=01000000){		//if Data will be send and if package count is smaller/equal than 64 (otherwise we leave the array boarders)
			packageCountdown=(uartBuf)& 0x3F; //...write quantity to count-down
			*packages=		 (uartBuf)& 0x3F;
			state=4;
		}
		else if(((*pOprID & 0xC0)==0b01000000)&&((*pOprID & 0x3F)==1)){	//if recived byte is Command and command is 1 ("show"-string)...
		    powerOnCog();
		    initCog();
		    writeFrame(payload,NODUMMY);
		    startTimerA0(500,SLEEPON);
		    writeFrame(payload,NODUMMY);
		    startTimerA0(500,SLEEPON);
		    powerOff();

		    int i=0;
		    for(i=0;i<64;i++){	//reset recived string to 0 -> ensures a (partly) white display if no string or less then 6 characters are transmitted. ->see line linker
		    	payload[i]=0;
		    }
		    state=0;
		}
		else
			state=0;
		break;

	case 4:
		payload[(*packages) - packageCountdown] = uartBuf;		//write incomming data to buffer
		packageCountdown--;
		if(!packageCountdown){
			*packages=0;
			state=3;
		}
		break;

	case 5:

		break;

//=======================================================
		//branch case 2:
	case 201:			//command is not for our Address
		if(!(uartBuf & 0XC0)){			//check if data will be transmitted
			packageCountdown=(*pOprID)& 0x3F; //...if so, write quantity to count-down to be able to discard them at the ISR
			state=0;
		}
		else
			state=0;
		break;

	}
return deviceID;
}
