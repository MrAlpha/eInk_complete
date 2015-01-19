
/*
 * main.c
 */

#include <msp430G2553.h>


#include "eInk.h"
#include "FlashTreiber.h"
#include "timer.h"
//#include "temp.h"
#include "pwm.h"
//#include "cogSpi.h"
#include "powerOnCog.h"
#include "writeFrame.h"
#include "cogSpi.h"
#include "powerOffCog.h"






void main(void) {

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initStartUp();
	
//    unsigned char linebuffer[66];
//    int foo=0;
//    for(foo=0;foo<66;foo++){
//    	if(foo<20){
//    		linebuffer[foo]=0xAA;
//    	}
//    	else if(foo<40){
//    		linebuffer[foo]=0xFF;
//    	}
//    	else
//    		linebuffer[foo]=0xAA;
//    }

//    startTimerA0(1,SLEEPOFF);
//    getTemp();
//    unsigned char display[7]={255,255,255,255,255,255,255};
    unsigned char display[7]={0};
    display[0]='4';
    display[1]='2';
    display[2]=',';
    display[3]='1';
    display[4]='3';
    display[5]='E';

    powerOnCog();
    initCog();
    writeFrame(display,NODUMMY);
    startTimerA0(500,SLEEPON);
    powerOff();
//    initCogSpi();
//    sendCogCommand(&test);

    while(1){
    	//P2OUT^=BIT7;
    }



}






#pragma vector=USCIAB0RX_VECTOR       // (7 * 1u)                     /* 0xFFEE USCI A0/B0 Receive */
__interrupt void a05(void){


}

#pragma vector=TIMER0_A1_VECTOR       // (8 * 1u)                     /* 0xFFF0 Timer0)A CC1, TA0 */
__interrupt void a06(void){


}

//#pragma vector=TIMER0_A0_VECTOR      //  (9 * 1u)                     /* 0xFFF2 Timer0_A CC0 */
//__interrupt void a07(void){
//
//
//}

#pragma vector=WDT_VECTOR              //(10 * 1u)                    /* 0xFFF4 Watchdog Timer */
__interrupt void a08(void){


}

#pragma vector=COMPARATORA_VECTOR      //(11 * 1u)                    /* 0xFFF6 Comparator A */
__interrupt void a09(void){


}

#pragma vector=TIMER1_A1_VECTOR        //(12 * 1u)                    /* 0xFFF8 Timer1_A CC1-4, TA1 */
__interrupt void a10(void){


}

#pragma vector=TIMER1_A0_VECTOR        //(13 * 1u)                    /* 0xFFFA Timer1_A CC0 */
__interrupt void a11(void){


}

#pragma vector=NMI_VECTOR              //(14 * 1u)                    /* 0xFFFC Non-maskable */
__interrupt void a12(void){


}

#pragma vector=RESET_VECTOR            //(15 * 1u)                    /* 0xFFFE Reset [Highest Priority] */
__interrupt void a13(void){


}
