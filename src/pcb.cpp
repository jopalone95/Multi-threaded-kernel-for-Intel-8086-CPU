/*
 * pcb.cpp
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#include "pcb.h"

#include <dos.h>

int PCB::helper = 0;
PCB* PCB::pcbs[256] = {0};

PCB::PCB(ID thread, StackSize stackSize, Time timeSlice) {
	id = helper++;
	pcbs[id] = this;
	Thread* pThread = Thread::threads[thread];
	stack = new unsigned[stackSize];
	state = created;
	size = stackSize;
	kvant = timeSlice;
	blockedHead = 0;
	if (timeSlice)
		unlimited = 0;
	else
		unlimited = 1;

	#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 1] = FP_SEG(pThread);
	stack[stackSize - 2] = FP_OFF(pThread);

	stack[stackSize - 5] = 0x200;

	stack[stackSize - 6] = FP_SEG(Thread::wrapper);
	stack[stackSize - 7] = FP_OFF(Thread::wrapper);

	ss = FP_SEG(stack + stackSize - 16);
	sp = FP_OFF(stack + stackSize - 16);
	bp = sp;
	#endif
}

PCB::~PCB() {
	pcbs[id] = 0;
	delete stack;
}




