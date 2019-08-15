/*
 * interr.h
 *
 *  Created on: Jul 9, 2018
 *      Author: OS1
 */

#ifndef _interr_h_
#define _interr_h_

class SleepList;

extern volatile int brojac;
extern SleepList* list;

void interrupt timer(...);
void interrupt syscall(...);
void interrupt dispatch(...);
void interrupt cond_dispatch(...);

#endif /* HEDERI_INTERR_H_ */
