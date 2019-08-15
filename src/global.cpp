/*
 * global.cpp
 *
 *  Created on: May 23, 2018
 *      Author: OS1
 */

#include "global.h"

volatile PCB* running;
volatile PCB* runningKernelThread;
volatile PCB* idleThread;
volatile int zahtevana_promena_konteksta = 0;
pInterrupt old_sys;
pInterrupt old_timer;
