/*
 * cogSpi.h
 *
 *  Created on: 01.12.2014
 *      Author: Dave
 */

#ifndef COGSPI_H_
#define COGSPI_H_

#include "eInk.h"

#define HEIGHT 176
#define LENGTH 264

#define DUMMY 2
#define NODUMMY 1

struct CogSpiCommands{
	int registerIndex;
	int dataLength;
	int data[8];
};



void initCogSpi();
void sendCogCommand(const struct CogSpiCommands *commandStruct);
void sendCogLineData(unsigned char *data, int line, unsigned char dummy);





#endif /* COGSPI_H_ */
