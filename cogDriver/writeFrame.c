/*
 * writeFrame.c
 *
 *  Created on: 08.12.2014
 *      Author: Dave
 */

/*	2.7" Display -> 264 dots * 176 lines
 *
 */

#include "cogSpi.h"
#include "lineGenerator.h"


const struct CogSpiCommands outputEnable		 = {0x02, 1, {0x2F}};

/*
 * atm writes one line of frame which has to be suplayed by display([66]
 */
void writeFrame(unsigned char *display,int dummy){

	int line=0;
	unsigned char linebuffer[66]={0};
//	int x=0;
//	int scan=0;

	if(dummy==NODUMMY){
		for(line=0;line<HEIGHT;line++){

			lineGenerator(display,line, linebuffer);

			sendCogLineData(linebuffer,line,dummy);

			sendCogCommand(&outputEnable);

		}
	}
	if(dummy==DUMMY){

		sendCogLineData(linebuffer,line,dummy);

		sendCogCommand(&outputEnable);
	}
}
