/*
 * FlashTreiber.h
 *
 *  Created on: 22.07.2014
 *      Author: Dave
 */

#ifndef FLASHTREIBER_H_
#define FLASHTREIBER_H_

//define Flash Commands. The used Mnemonics are chosen according to the Datasheed

#define WREN 0x06
#define WRDI 0x04
#define RDID 0x9F
#define RDSR 0x05
#define WRSR 0x01
#define READ 0x03
#define FREAD 0x0B
#define SE 0x20
#define BE 0x52		//???
#define CE 0x60		//???
#define PP 0x02
#define DP 0xB9
#define RDP 0xAB
#define RES 0xAB
#define REMS 0x90

/******************Status Register mask*******************************/
#define WIP 0x01
//volatile extern unsigned char spiReceive;
//volatile extern unsigned int _byteNumber;

//Function Prototypes

void spiFlashInit();
void spiWrite(unsigned char Data);
unsigned char spiRead(unsigned char data);

void delay(unsigned int ms);

void setCSlow();
void setCShigh();
unsigned char checkWriteEnable();
void setWriteEnable();
unsigned char getStatusRegister();
unsigned char getFlashBusy();
unsigned char getFlashID();
void sendFlashAddress(long flashAddress);
void readData(unsigned long address, unsigned char *recive, unsigned long byteNumber);
void readDataSpiStart(unsigned long flashAddress);
unsigned char readDataSpiData();
void readDataSpiEnd();
void writeData(unsigned long flashAddress,unsigned char *send, unsigned long byteNumber);
void writeDataSpiStart(unsigned long flashAdress);
void writeDataSpiTransmit(unsigned char send);
void writeDataSpiEnd();
void eraseSector(unsigned long flashAddress);
void eraseFlash();


#endif /* FLASHTREIBER_H_ */
