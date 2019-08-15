/*
 * kersem.h
 *
 *  Created on: Jun 2, 2018
 *      Author: OS1
 */

#ifndef _kersem_h_
#define _kersem_h_

#include "SCHEDULE.H"
#include "thread.h"

class PCB;

class KernelSem {
private:
	struct SemNode {
		PCB* thread;
		SemNode* next;

		SemNode(PCB* thread) {this->thread = thread; this->next = 0;}
	};

	SemNode* head;
	SemNode* tail;

public:
	static int helper;
	static KernelSem* kersems[256];
	ID id;
	volatile int value;

	KernelSem(int init);
	~KernelSem();
	int wait(int toBlock);
	void signal();
};

#endif /* HEDERI_KERSEM_H_ */
