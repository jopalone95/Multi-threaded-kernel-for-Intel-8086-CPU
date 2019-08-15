/*
 * pcb.h
 *
 *  Created on: May 15, 2018
 *      Author: OS1
 */

#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"

enum State {finished, executing, sleeping, created, blocked, idle};

class PCB {
public:
	struct BlockedNode {
		PCB* thread;
		BlockedNode* next;

		BlockedNode(PCB* thread) {this->thread = thread; this->next = 0;}
	};

	static int helper;
	static PCB* pcbs[256];
	ID id;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned *stack;
	State state;
	StackSize size;
	Time kvant;
	BlockedNode* blockedHead;
	unsigned unlimited;

	PCB(ID thread, StackSize stackSize, Time timeSlice);
	~PCB();
};

#endif /* HEDERI_PCB_H_ */
