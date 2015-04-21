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



	if(dummy==NODUMMY){
		for(line=0;line<HEIGHT;line++){

			lineGenerator(display,line, linebuffer);

			sendCogLineData(linebuffer,line,dummy);

			sendCogCommand(&outputEnable);

		}
	}
	if(dummy==DUMMY){
		int i=0;
		for(i=0; i<66; i++){		//generate Line with 0b01 for every pixel
			linebuffer[i]=0x55;
		}

	for(line=0;line<HEIGHT;line++){	//write nothing frame

		sendCogLineData(linebuffer,line,NODUMMY);

		sendCogCommand(&outputEnable);
	}

	sendCogLineData(linebuffer,0,dummy);

	sendCogCommand(&outputEnable);

	}
}
