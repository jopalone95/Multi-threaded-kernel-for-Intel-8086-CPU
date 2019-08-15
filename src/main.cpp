/*
 * main.cpp
 *
 *  Created on: May 17, 2018
 *      Author: OS1
 */

#include "global.h"
#include "interr.h"
#include "pcb.h"
#include "sleeplis.h"
#include "syscall.h"

#include <dos.h>

extern int userMain(int, char**);

void run() {
	while (1) {
		syscalls[syscallData->index]();
		cond_dispatch();
	}
}

void inf() {while (1);}

void inic() {
	//runningKernelThread
	runningKernelThread = new PCB(0, defaultStackSize, 0);
	unsigned* stack = idleThread->stack;
	StackSize size = idleThread->size;

	stack[size - 1] = 0x200;

	#ifndef BCC_BLOCK_IGNORE
	stack[size - 2] = FP_SEG(run);
	stack[size - 3] = FP_OFF(run);

	runningKernelThread->ss = FP_SEG(stack + size - 12);
	runningKernelThread->sp = FP_OFF(stack + size - 12);
	runningKernelThread->bp = runningKernelThread->sp;
	#endif*/

	//runningThread
	running = new PCB(0, defaultStackSize, 0);
	running->state = executing;

	//idleThread
	idleThread = new PCB(0, defaultStackSize, defaultTimeSlice);
	idleThread->state = idle;
	stack = idleThread->stack;
	size = idleThread->size;

	stack[size - 1] = 0x200;

	#ifndef BCC_BLOCK_IGNORE
	stack[size - 2] = FP_SEG(inf);
	stack[size - 3] = FP_OFF(inf);

	idleThread->ss = FP_SEG(stack + size - 12);
	idleThread->sp = FP_OFF(stack + size - 12);
	idleThread->bp = idleThread->sp;
	#endif

	#ifndef BCC_BLOCK_IGNORE
	old_sys = getvect(0x60);
	setvect(0x60, syscall);
	old_timer = getvect(0x08);
	setvect(0x08, timer);
	#endif

}

void restore() {

	#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, old_timer);
	setvect(0x60, old_sys);
	#endif

	delete runningKernelThread;
	delete running;
	delete idleThread;
	delete list;
}

int main(int argc, char* argv[]) {

	inic();

	userMain(argc, argv);

	restore();

	return 0;
}










