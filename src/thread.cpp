/*
 * thread.cpp
 *
 *  Created on: May 16, 2018
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include "global.h"
#include "syscall.h"
#include "interr.h"
#include "sleeplis.h"
#include "SCHEDULE.H"

#include <dos.h>

int Thread::helper = 0;
Thread* Thread::threads[256] = {0};

Thread::Thread(StackSize stackSize, Time timeSlice) {
	id = helper++;
	threads[id] = this;
	SysData newData = SysData();
	newData.stackSize = stackSize;
	newData.timeSlice = timeSlice;
	newData.id = id;
	newData.index = i_thread_cons;

	init_sys(&newData);
}

Thread::~Thread() {
	SysData newData = SysData();
	newData.id = id;
	newData.index = i_thread_dest;

	init_sys(&newData);
}

void Thread::start() {
	SysData newData = SysData();
	newData.id = id;
	newData.index = i_thread_start;

	init_sys(&newData);
}

void Thread::waitToComplete() {
	SysData newData = SysData();
	newData.id = id;
	newData.index = i_thread_waitToComplete;

	init_sys(&newData);
}

void Thread::sleep(Time timeToSleep) {
	SysData newData = SysData();
	newData.timeSlice = timeToSleep;
	newData.index = i_thread_sleep;

	init_sys(&newData);
}

void Thread::wrapper(Thread* thread) { //kako ovo uraditi, treba li ovo da bude sistemski poziv?
	thread->run();

	finish();
}

void Thread::finish() {
	SysData newData = SysData();
	newData.index = i_thread_sleep;

	init_sys(&newData);
}

void thread_cons() {
	SysData* sysData = (SysData*)syscallData;
	Thread* thread = Thread::threads[sysData->id];

	PCB* pcb = new PCB(thread->id, sysData->stackSize, sysData->timeSlice); //zakomentarisane linije vrv treba prebaciti u _start
	thread->myPCB = pcb->id;                                                //razmisli o tome da li zelis da prebacis u _start
}

void thread_dest() {
	SysData* sysData = (SysData*)syscallData;
	Thread* thread = Thread::threads[sysData->id];
	PCB* pcb = PCB::pcbs[thread->myPCB];

	Thread::threads[thread->id] = 0;
	delete pcb;
}

void thread_start() {
	SysData* sysData = (SysData*)syscallData;
	Thread* thread = Thread::threads[sysData->id];
	PCB* myPCB = PCB::pcbs[thread->myPCB];

	if (myPCB->state == created) {
		myPCB->state = executing;
		Scheduler::put(myPCB);
	}
}

void thread_waitToComplete() {
	SysData* sysData = (SysData*)syscallData;
	Thread* thread = Thread::threads[sysData->id];
	PCB* myPCB = PCB::pcbs[thread->myPCB];

	if (myPCB->state != finished && myPCB->state != created && myPCB != (PCB*)running) {
		running->state = blocked;
		PCB::BlockedNode* novi = new PCB::BlockedNode((PCB*)running);
		novi->next = myPCB->blockedHead;
		myPCB->blockedHead = novi;
		zahtevana_promena_konteksta = 1;
	}
}

void thread_sleep() {
	SysData* sysData = (SysData*)syscallData;

	if (sysData->timeSlice > 0) {
		running->state = sleeping;
		list->add((PCB*)running, sysData->timeSlice);
	}
	zahtevana_promena_konteksta = 1;
}

void thread_finish() {
	running->state = finished;
	while (running->blockedHead) {
		running->blockedHead->thread->state = executing;
		Scheduler::put(running->blockedHead->thread);
		running->blockedHead = running->blockedHead->next;
	}
	zahtevana_promena_konteksta = 1;
}
