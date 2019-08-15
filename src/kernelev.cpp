/*
 * kernelev.cpp
 *
 *  Created on: Jun 4, 2018
 *      Author: OS1
 */

#include "kernelev.h"
#include "ivtentry.h"
#include "pcb.h"
#include "global.h"
#include "interr.h"
#include "locking.h"
#include "SCHEDULE.H"

int KernelEv::helper = 0;
KernelEv* KernelEv::kerevs[256] = {0};

KernelEv::KernelEv(IVTNo ivtNo) {
	id = helper++;
	kerevs[id] = this;

	IVTEntry* entry = IVTEntry::ivtEntries[ivtNo];
	entry->event = id;
	myEntry = ivtNo;

	value = 0;
	blocked_flag = 0;
	maker = (PCB*)running;
}

KernelEv::~KernelEv() {
	IVTEntry* entry = IVTEntry::ivtEntries[myEntry];

	kerevs[id] = 0;
	delete entry;
}

void KernelEv::wait() {
	if ((PCB*)running == maker) { //samo nit tvorac moze da se blokira na dogadjaju
		asm cli;

		if (value) {
			value = 0;

			asm sti;
		} else {
			blocked_flag = 1;
			running->state = blocked;
			zahtevana_promena_konteksta = 1;

			asm sti;
		}
	}
}

void KernelEv::signal() { //ovo treba sad drugacije odraditi, u zavisnosti od toga da li je na kernel steku program ili ne
	if (blocked) {
		blocked_flag = 0;
		maker->state = executing;
		Scheduler::put(maker);
		if (lockFlag) {
			dispatch();

			asm cli;
		} else
			zahtevana_promena_konteksta = 1;
	} else
		value = 1;
}


