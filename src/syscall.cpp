/*
 * syscall.cpp
 *
 *  Created on: Jul 7, 2018
 *      Author: OS1
 */

#include "syscall.h"

#include <dos.h>

volatile SysData* syscallData;
pFunction syscalls[14] = {thread_cons, thread_dest, thread_start, thread_waitToComplete, thread_sleep, thread_finish, //thread
						  sem_cons, sem_dest, sem_wait, sem_signal, sem_val,                                    	  //semaphore
						  event_cons, event_dest, event_wait};                                                        //event

void init_sys(SysData* newData) {

	#ifndef BCC_BLOCK_IGNORE
	unsigned tcx = FP_SEG(newData);
	unsigned tdx = FP_OFF(newData);
	#endif

	asm {
		push cx
		push dx
		mov cx, tcx
		mov dx, tdx
		int 60h
		pop dx
		pop cx
	}
}
