/*
 * ivtentry.h
 *
 *  Created on: Jun 4, 2018
 *      Author: OS1
 */

#ifndef _ivtentry_h_
#define _ivtentry_h_

#include "global.h"
#include "thread.h"
#include "kernelev.h"

class IVTEntry {
public:
	static IVTEntry* ivtEntries[256];

	ID event;
	pInterrupt oldRout;
	IVTNo ivtNo;

	IVTEntry(IVTNo ivtNo, pInterrupt newRout);
	~IVTEntry();
	void callOld();
	void signal();
};

#endif
