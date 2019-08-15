/*
 * semaphor.cpp
 *
 *  Created on: Jun 2, 2018
 *      Author: OS1
 */

#include "semaphor.h"
#include "kersem.h"
#include "syscall.h"

#include <dos.h>

int Semaphore::helper = 0;
Semaphore* Semaphore::sems[256] = {0};

Semaphore::Semaphore(int init) {
	id = helper++;
	sems[id] = this;
	SysData newData = SysData();
	newData.id = id;
	newData.val = init;
	newData.index = i_sem_cons;

	init_sys(&newData);
}

Semaphore::~Semaphore() {
	SysData newData = SysData();
	newData.id = id;
	newData.index = i_sem_dest;

	init_sys(&newData);
}

int Semaphore::wait(int toBlock) {
	SysData newData = SysData();
	newData.id = id;
	newData.val = toBlock;
	newData.index = i_sem_wait;

	init_sys(&newData);

	return newData.ret;
}

void Semaphore::signal() {
	SysData newData = SysData();
	newData.id = id;
	newData.index = i_sem_signal;

	init_sys(&newData);
}

int Semaphore::val() const { //napravi sistemski poziv
	SysData newData = SysData();
	newData.id = id;
	newData.index = i_sem_val;

	init_sys(&newData);

	return newData.ret;
}

void sem_cons() {
	SysData* sysData = (SysData*)syscallData;
	Semaphore* sem = Semaphore::sems[sysData->id];

	KernelSem* kersem = new KernelSem(sysData->val);
	sem->myImpl = kersem->id;
}

void sem_dest() {
	SysData* sysData = (SysData*)syscallData;
	Semaphore* sem = Semaphore::sems[sysData->id];
	KernelSem* kersem = KernelSem::kersems[sem->myImpl];

	Semaphore::sems[sysData->id] = 0;
	delete kersem;
}

void sem_wait() {
	SysData* sysData = (SysData*)syscallData;
	Semaphore* sem = Semaphore::sems[sysData->id];
	KernelSem* kersem = KernelSem::kersems[sem->myImpl];

	sysData->ret = kersem->wait(sysData->val);
}

void sem_signal() {
	SysData* sysData = (SysData*)syscallData;
	Semaphore* sem = Semaphore::sems[sysData->id];
	KernelSem* kersem = KernelSem::kersems[sem->myImpl];

	kersem->signal();
}

void sem_val() {
	SysData* sysData = (SysData*)syscallData;
	Semaphore* sem = Semaphore::sems[sysData->id];
	KernelSem* kersem = KernelSem::kersems[sem->myImpl];

	sysData->ret = kersem->value;
}
