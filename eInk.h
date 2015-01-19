/*
 * eInk.h
 *
 *  Created on: 13.11.2014
 *      Author: Dave
 */

#ifndef EINK_H_
#define EINK_H_

/*		Defines to handle pin IO more convinient. Port stil has to be
 * 		done by hand
 */

#define PANAL_ON BIT3		//P2.3
#define DISCHARGE BIT4		//P2.4
#define BORDER_CONTROL BIT5	//P2.5
#define FLASH_CS BIT7		//P2.7
#define EPD_CS BIT6			//P2.6
#define RESET BIT2			//P2.2
#define BUSY_ BIT0			//P2.0

//========================================================================
extern volatile unsigned char uartBuf;		//
extern volatile unsigned char Flag;		// Für Datenübergabe aus ISR
extern volatile unsigned char packageCountdown;

//========= defines for Flag Word ============================
#define INCOMING 	(1<<0)
#define DISCARD		(1<<1)

/* Prototype for initialisation routine */

void initStartUp(void);

#endif /* EINK_H_ */
