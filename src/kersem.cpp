/*
 * kersem.cpp
 *
 *  Created on: Jun 2, 2018
 *      Author: OS1
 */

#include "kersem.h"
#include "pcb.h"
#include "global.h"

int KernelSem::helper = 0;
KernelSem* KernelSem::kersems[256] = {0};

KernelSem::KernelSem(int init) {
	id = helper++;
	kersems[id] = this;
 	value = init;
	head = 0;
	tail = 0;
}

KernelSem::~KernelSem() {
	kersems[id] = 0;
	while (head) {
		SemNode* toDelete = head;
		head = head->next;
		delete toDelete;
	}
}

int KernelSem::wait(int toBlock) {
	if (value <= 0) {
		if (!toBlock) {
			return -1;
		} else {
			value--;

			SemNode* novi = new SemNode((PCB*)running);
			if (!head)
				head = tail = novi;
			else {
				tail->next = novi;
				tail = novi;
			}
			running->state = blocked;
			zahtevana_promena_konteksta = 1;
			return 1;
		}
	} else {
		value--;
		return 0;
	}
}

void KernelSem::signal() {
	value++;

	if (value <= 0) {
		head->thread->state = executing;
		Scheduler::put(head->thread);

		SemNode* toDelete = head;
		if (head == tail)
			head = tail = 0;
		else
			head = head->next;
		delete toDelete;
	}
}



