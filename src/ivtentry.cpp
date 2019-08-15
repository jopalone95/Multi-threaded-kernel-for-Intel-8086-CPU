/*
 * ivtentry.cpp
 *
 *  Created on: Jun 4, 2018
 *      Author: OS1
 */

#include "ivtentry.h"

#include <dos.h>

IVTEntry* IVTEntry::ivtEntries[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRout) { //msm da ovde nema potrebe za zakljucavanjem
	ivtEntries[ivtNo] = this;

	event = -1;
	this->ivtNo = ivtNo;

	#ifndef BCC_BLOCK_IGNORE
	oldRout = getvect(ivtNo);
	setvect(ivtNo, newRout);
	#endif
}

IVTEntry::~IVTEntry() {
	ivtEntries[ivtNo] = 0;

	#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRout);
	#endif
}

void IVTEntry::callOld() {
	oldRout();
}

void IVTEntry::signal() {
	(KernelEv::kerevs[event])->signal();
}


