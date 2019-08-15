/*
 * global.h
 *
 *  Created on: May 23, 2018
 *      Author: OS1
 */

#ifndef _global_h_
#define _global_h_

typedef void interrupt (*pInterrupt)(...);

class PCB;

extern volatile PCB* running;
extern volatile PCB* runningKernelThread;
extern volatile PCB* idleThread;
extern volatile int zahtevana_promena_konteksta;
extern pInterrupt old_sys;
extern pInterrupt old_timer;

#endif /* HEDERI_GLOBAL_H_ */
