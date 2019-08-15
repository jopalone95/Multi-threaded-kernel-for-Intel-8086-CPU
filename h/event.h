/*
 * event.h
 *
 *  Created on: Jun 1, 2018
 *      Author: OS1
 */

#ifndef _event_h_
#define _event_h_

#include "thread.h"

#define PREPAREENTRY(ivtNo, oldInt)\
void interrupt int##ivtNo(...){\
	IVTEntry::ivtEntries[##ivtNo]->signal();\
	if(oldInt) IVTEntry::ivtEntries[##ivtNo]->callOld();\
}\
IVTEntry entry##ivtNo(##ivtNo, int##ivtNo)

typedef unsigned char IVTNo;

class KernelEv;

class Event {
private:
	static int helper;
	static Event* events[256];
	ID id;
	ID myImpl;

protected:
	friend class KernelEv;
	friend void event_cons();
	friend void event_dest();
	friend void event_wait();

	void signal() {}

public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
};



#endif
