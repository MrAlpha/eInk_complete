/*
 * timer.h
 *
 *  Created on: 25.11.2014
 *      Author: Dave
 */

#ifndef TIMER_H_
#define TIMER_H_

#define SLEEPON 1
#define SLEEPOFF 0

//void setupTimerA0();
void startTimerA0(unsigned short milliseconds, unsigned char sleepOnOff);
//__interrupt void timerOverflowInterrupt(void);



#endif /* TIMER_H_ */
