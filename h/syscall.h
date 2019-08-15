/*
 * syscall.h
 *
 *  Created on: Jul 7, 2018
 *      Author: OS1
 */

#ifndef _syscall_h_
#define _syscall_h_

#include "thread.h"
#include "event.h"

typedef void (*pFunction)(void);

enum Call {i_thread_cons, i_thread_dest, i_thread_start, i_thread_waitToComplete, i_thread_sleep, i_thread_finish,  //thread
		   i_sem_cons, i_sem_dest, i_sem_wait, i_sem_signal, i_sem_val,                                             //semaphore
		   i_event_cons, i_event_dest, i_event_wait};                                                               //event

struct SysData {
	StackSize stackSize;
	Time timeSlice;
	ID id;
	int val;
	int ret;
	IVTNo ivtNo;
	Call index;

	SysData() {}
};

void init_sys(SysData*);

void thread_cons();
void thread_dest();
void thread_start();
void thread_waitToComplete();
void thread_sleep();
void thread_finish();

void sem_cons();
void sem_dest();
void sem_wait();
void sem_signal();
void sem_val();

void event_cons();
void event_dest();
void event_wait();

extern volatile SysData* syscallData;
extern pFunction syscalls[14];

#endif /* HEDERI_SYSCALL_H_ */
