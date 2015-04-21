/*
 * lineGenerator.c
 *
 *  Created on: 12.01.2015
 *      Author: Dave
 */

#include "lineGenerator.h"
#include "FlashTreiber.h"

/* lineGenerator: generats the data needed to write one line of the frame.
 *
 * *display 	->the string to be displayed
 * line			->which line of the frame is needed
 * *linebuffer	->a pointer ( 66 byte as allocated in writeFrame) to return the generated line data to the calling function
 */
void lineGenerator(unsigned char *display, int line, unsigned char *linebuffer){

	unsigned char temp[33]={0};		//33*8=264 (dots per line) To store generated intermediate line data

	int x=0;

	int shifts=0;

	for(x=0;x<66;x++){
		linebuffer[x]=0;
	}

	unsigned int lineBufCount=0;

	unsigned char pixel;

	lineLinker(display, line, temp);

	x=263;
	shifts=6;

	//start generating even bits, start at rightmost pixel in line
	while(x>0){

		//get pixel from temp
		pixel=(temp[x/8]>>(7-(x%8))) & 0x1;

		//write two bit pixel to linebuffer at right place in byte. pixel black -> write 0b11, pixel white -> write 0b10
		if(pixel){
			linebuffer[lineBufCount]|=(0x2<<shifts);
		}
		else{
			linebuffer[lineBufCount]|=(0x3<<shifts);
		}

		//go to next even bit
		x-=2;

		//write next two bits at the right position in byte
		shifts-=2;

		//if byte is full reset shifts and start writing into next byte
		if(shifts<0){
			shifts=6;
			lineBufCount++;
		}
	}

	x=0;

	//start generation odd bits, start at leftmost pixel in line
	while(x<264){

		pixel=(temp[x/8]>>(7-(x%8))) &0x1;

		if(pixel){
			linebuffer[lineBufCount]|=((unsigned char)0x2<<shifts);
		}
		else{
			linebuffer[lineBufCount]|=(0x3<<shifts);
		}

		x+=2;
		shifts-=2;

		if(shifts<0){
			shifts=6;
			lineBufCount++;
		}
	}
}

/*
 * lineLinker: recives the string to be displayed, evaluates which line of the picture is needed and links the line data of each
 * character (retrived from the flash) together.
 *
 * *display:string to be displayed (max 7 characters)
 * line: wich line of the picture is needed (0-175)
 * *temp: pointer to the address where the linked line should be stored (allocated memory space has to be 33 Bytes)
 */

void lineLinker(unsigned char *display, int line, unsigned char *temp){
	/*
	 * the bitmaps stored in flash have 0 values for black dots and 1 values for white
	 */
		spiFlashInit();

		unsigned int position=0;
		int i =0;

		for(position=0;position<7;position++){
			if(line==120){
				i=42;
			}

//*** Paint white space at top 55 lines of display to offset the displayed string
			if(line<55 && display[position]!=255){
				for(i=0;i<5;i++){
					*(temp+position*5+i) = 0xFF;
				}
			}


//*** if value for current position is 0 write 0xFF into next 5 bytes(one position in the string) of line data to display white
			if(display[position]==0){
				if(position<6){
					for(i=0;i<5;i++){
						*(temp+position*5+i) = 0xFF;
					}
				}
				else{
					for(i=0;i<3;i++){
						*(temp+position*5+i) = 0xFF;
					}
				}
			}

//*** if value for current position is 255 write 0x00 into next 5 bytes(one position in the string) of line data to display black
			else if(display[position]==255){
				if(position<6){
					for(i=0;i<5;i++){
						*(temp+position*5+i) = 0x00;
					}
				}
				else{
					for(i=0;i<3;i++){
						*(temp+position*5+i) = 0x00;
					}
				}
			}
//*** write the corrosponding data from flash to temp
			else if((line>54)&&(line<120)){
				if(display[position]=='0'){
					readData(0x000000+5*(line-54), (temp+5*position) ,5);
				}
				else if(display[position]=='1'){
					readData(0x000200+5*(line-54), (temp+5*position),5);
				}
				else if(display[position]=='2'){
					readData(0x000400+5*(line-54),(temp+5*position) ,5 );
				}
				else if(display[position]=='3'){
					readData(0x000600+5*(line-54),(temp+5*position) ,5 );
				}
				else if(display[position]=='4'){
					readData(0x000800+5*(line-54),(temp+5*position) ,5 );
				}
				else if(display[position]=='5'){
					readData(0x000A00+5*(line-54),(temp+5*position) ,5 );
				}
				else if(display[position]=='6'){
					readData(0x000C00+5*(line-54),(temp+5*position) ,5 );
				}
				else if(display[position]=='7'){
					readData(0x000E00+5*(line-54), (temp+5*position) ,5);
				}
				else if(display[position]=='8'){
					readData(0x001000+5*(line-54), (temp+5*position) ,5);
				}
				else if(display[position]=='9'){
					readData(0x001200+5*(line-54), (temp+5*position) ,5);
				}
				else if(display[position]==','){
					readData(0x001400+5*(line-54), (temp+5*position) ,5);
				}
				else if(display[position]=='E'){
					readData(0x001600+5*(line-54), (temp+5*position) ,5);
				}
			}
//*** write white space at bottom of display
			else if(line>119){
				if((display[position]!=0)&&(display[position]!=255)){
					for(i=0;i<5;i++){
						*(temp+position*5+i) = 0xFF;
					}
				}
			}

		}
}
