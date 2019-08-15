/*
 * kernelev.h
 *
 *  Created on: Jun 4, 2018
 *      Author: OS1
 */

#ifndef _kernelev_h_
#define _kernelev_h_

#include "event.h"

class PCB;

class KernelEv {
public:
	static int helper;
	static KernelEv* kerevs[256];
	ID id;
	ID myEntry;
	int value;
	int blocked_flag;
	PCB* maker;

	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
};

#endif
