/*
 * init.c
 *
 *  Created on: 11.11.2014
 *      Author: Dave
 */
#include <msp430G2553.h>
#include "eInk.h"

void bscPlusInit();
void gpioInit();
void USCI_A0_Init();



void initStartUp(){
    /* Stop watchdog timer from timing out during initial start-up. */
    WDTCTL = WDTPW | WDTHOLD;


	bscPlusInit();	//call Clock Initialisation routine

	gpioInit();		//call Input/Output Initialisation routine

	USCI_A0_Init();	//call UART init routine
}


void USCI_A0_Init(void)
{
    /* Disable USCI */
    UCA0CTL1 |= UCSWRST;

    /*
     * Control Register 1
     *
     * UCSSEL_2 -- SMCLK
     * ~UCRXEIE -- Erroneous characters rejected and UCAxRXIFG is not set
     * ~UCBRKIE -- Received break characters do not set UCAxRXIFG
     * ~UCDORM -- Not dormant. All received characters will set UCAxRXIFG
     * ~UCTXADDR -- Next frame transmitted is data
     * ~UCTXBRK -- Next frame transmitted is not a break
     * UCSWRST -- Enabled. USCI logic held in reset state
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    UCA0CTL1 = UCSSEL_2 | UCSWRST;

    /*
     * Modulation Control Register			//9600Baud @ 16MHz
     *
     * UCBRF_0 -- First stage 0
     * UCBRS_2 -- Second stage 2
     * ~UCOS16 -- Disabled
     *
     * Note: ~UCOS16 indicates that UCOS16 has value zero
     */
    UCA0MCTL = UCBRF_0 | UCBRS_6;

    /* Baud rate control register 0 */
    UCA0BR0 = 0x82;

    /* Baud rate control register 1 */
    UCA0BR1 = 0x06;

    /*Enable receive interrupt on USCI A0 */
    IE2|=UCA0RXIE;

    /* Enable USCI */
    UCA0CTL1 &= ~UCSWRST;

    /* USER CODE START (section: USCI_A0_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: USCI_A0_graceInit_epilogue) */
}




void gpioInit(){
	/*
	 *  Initialize MSP430 General Purpose Input Output Ports
	 *
	 *  The GPIO registers should be set in a specific order:
	 *     PxOUT
	 *     PxSEL or PxSELx
	 *     PxDIR
	 *     PxREN
	 *     PxIES
	 *     PxIFG
	 *     PxIE
	 *
	 *     This will ensure that:
	 *         - IFG doesn't get set by manipulating the pin function, edge
	 *           select, and pull-up/down resistor functionalities (see
	 *           Section 8.2.6 of the MSP430 User's manual)
	 *         - Glitch-free setup (configuring the OUT register _before_
	 *           setting the pin direction)
	 *         - Pull-up/pull-down resistor enable with the correct direction
	 *           (.up. vs. .down.)
	 */

	/* Port 1 Output Register */	//all outputs =0
	P1OUT = 0;

	/* Port 1 Port Select Register */		//select spi functionality for pin 5,6,7
	P1SEL = BIT5 | BIT6 | BIT7;
	P1SEL |= BIT1| BIT2;					//select UART functionality

	/* Port 1 Port Select 2 Register */		////select spi functionality for pin 5,6,7
	P1SEL2 = BIT5 | BIT6 | BIT7;
	P1SEL2|= BIT1| BIT2;					////select UART functionality

	/* Port 1 Direction Register */
	P1DIR = BIT0;							//LED

	/* Port 1 Interrupt Edge Select Register */
	P1IES = 0;

	/* Port 1 Interrupt Flag Register */
	P1IFG = 0;
/*-----------------------------------------------------------------*/
	/* Port 2 Output Register */
	P2OUT |= EPD_CS;			//set Cog /CS high

	/* Port 2 Port Select Register */	//dissable ext Clock pin
	P2SEL &= ~(BIT6);

	/* Port 2 Port Select Register */	//dissable ext Clock pin
	P2SEL &= ~(BIT7);
	P2SEL |= BIT1;						//set P2.1 as PWM output


	/* Port 2 Direction Register */
	P2DIR = FLASH_CS|EPD_CS|PANAL_ON|DISCHARGE|BORDER_CONTROL|RESET|BIT1;		//P2.7 P2.1 out

	/* Port 2 Interrupt Edge Select Register */
	P2IES = 0;

	/* Port 2 Interrupt Flag Register */
	P2IFG = 0;

	/* Port 3 Output Register */
	P3OUT = 0;

	/* Port 3 Direction Register */
	P3DIR = 0;

	P2OUT = BIT7|BIT6;

}

void bscPlusInit(){

    /*
     * Basic Clock System Control 2
     *
     * SELM_0 -- DCOCLK
     * DIVM_0 -- Divide by 1
     * ~SELS -- DCOCLK
     * DIVS_0 -- Divide by 1
     * ~DCOR -- DCO uses internal resistor
     *
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;

    if (CALBC1_16MHZ != 0xFF) {
        /* Adjust this accordingly to your VCC rise time */
//        __delay_cycles(1000);

        /* Follow recommended flow. First, clear all DCOx and MODx bits. Then
         * apply new RSELx values. Finally, apply new DCOx and MODx bit values.
         */
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_16MHZ;     /* Set DCO to 16MHz */
        DCOCTL = CALDCO_16MHZ;
    }

    /*
     * Basic Clock System Control 1
     *
     * XT2OFF -- Disable XT2CLK
     * ~XTS -- Low Frequency
     * DIVA_0 -- Divide by 1
     *
     * Note: ~XTS indicates that XTS has value zero
     */
    BCSCTL1 |= XT2OFF | DIVA_0;

    /*
     * Basic Clock System Control 3
     *
     * XT2S_0 -- 0.4 - 1 MHz
     * LFXT1S_2 -- If XTS = 0, XT1 = VLOCLK ; If XTS = 1, XT1 = 3 - 16-MHz crystal or resonator
     * XCAP_1 -- ~6 pF
     */
    BCSCTL3 = /*XT2S_0 |*/ LFXT1S_2 | XCAP_0;

    IFG1 &= ~OFIFG;
}
