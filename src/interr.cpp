/*
 * interr.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: OS1
 */

#include "interr.h"
#include "locking.h"
#include "syscall.h"
#include "sleeplis.h"
#include "global.h"
#include "pcb.h"
#include "SCHEDULE.H"

#include <dos.h>

volatile unsigned tbp;
volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tcx;
volatile unsigned tdx;
volatile int brojac;
SleepList* list = new SleepList();

void interrupt timer(...) {
	brojac--;
	if (list->unlocked())
		list->release();

	if ((brojac == 0 && !running->unlimited) || zahtevana_promena_konteksta) {
		if (lockFlag) {
			zahtevana_promena_konteksta = 0;

			asm {
				mov tbp, bp
				mov tsp, sp
				mov tss, ss
			}

			running->bp = tbp;
			running->sp = tsp;
			running->ss = tss;

			Scheduler::put((PCB*)running);
			running = Scheduler::get();

			tbp = running->bp;
			tsp = running->sp;
			tss = running->ss;

			brojac = running->kvant;

			asm {
				mov bp, tbp
				mov sp, tsp
				mov ss, tss
			}
		} else
			zahtevana_promena_konteksta = 1;
	}

	old_timer();
}

void interrupt syscall(...) {

	asm {
		mov tbp, bp
		mov tsp, sp
		mov tss, ss
		mov tcx, cx
		mov tdx, dx
	}

	running->bp = tbp;
	running->sp = tsp;
	running->ss = tss;

	#ifndef BCC_BLOCK_IGNORE
	syscallData = (SysData*)MK_FP(tcx, tdx);
	#endif

	tbp = runningKernelThread->bp;
	tsp = runningKernelThread->sp;
	tss = runningKernelThread->ss;

	asm {
		mov bp, tbp
		mov sp, tsp
		mov ss, tss
	}

	lockCout

}

void interrupt dispatch(...) {
	zahtevana_promena_konteksta = 0;

	asm {
		mov tbp, bp
		mov tsp, sp
		mov tss, ss
	}

	running->bp = tbp;
	running->sp = tsp;
	running->ss = tss;

	if (running->state == executing)
		Scheduler::put((PCB*)running);

	running = Scheduler::get();
	if (!running)
		running = idleThread;

	tbp = running->bp;
	tsp = running->sp;
	tss = running->ss;

	brojac = running->kvant;

	asm {
		mov bp, tbp
		mov sp, tsp
		mov ss, tss
	}
}

void interrupt cond_dispatch(...) {
	asm {
		mov tbp, bp
		mov tsp, sp
		mov tss, ss
	}

	runningKernelThread->bp = tbp;
	runningKernelThread->sp = tsp;
	runningKernelThread->ss = tss;

	if (zahtevana_promena_konteksta) {
		if (running->state == executing)
			Scheduler::put((PCB*)running);

		running = Scheduler::get();
		if (!running)
			running = idleThread;
		brojac = running->kvant;

		zahtevana_promena_konteksta = 0;
	}

	tbp = running->bp;
	tsp = running->sp;
	tss = running->ss;

	asm {
		mov bp, tbp
		mov sp, tsp
		mov ss, tss
	}

	unlockCout

}
