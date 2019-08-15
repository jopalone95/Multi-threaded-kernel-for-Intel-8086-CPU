/*
 * event.cpp
 *
 *  Created on: Jun 4, 2018
 *      Author: OS1
 */

#include "event.h"
#include "kernelev.h"
#include "syscall.h"

#include <dos.h>

int Event::helper = 0;
Event* Event::events[256] = {0};

Event::Event(IVTNo ivtNo) {
	id = helper++;
	events[id] = this;
	SysData newData = SysData();
	newData.id = id;
	newData.ivtNo = ivtNo;
	newData.index = i_event_cons;

	init_sys(&newData);
}

Event::~Event() {
	SysData newData = SysData();
	newData.id = id;
	newData.index = i_event_dest;

	init_sys(&newData);
}

void Event::wait() {
	SysData newData = SysData();
	newData.id = id;
	newData.index = i_event_wait;

	init_sys(&newData);
}

void event_cons() {
	SysData* sysData = (SysData*)syscallData;
	Event* event = Event::events[sysData->id];

	KernelEv* kerev = new KernelEv(sysData->ivtNo);
	event->myImpl = kerev->id;
}

void event_dest() {
	SysData* sysData = (SysData*)syscallData;
	Event* event = Event::events[sysData->id];
	KernelEv* kerev = KernelEv::kerevs[event->myImpl];

	Event::events[event->id] = 0;
	delete kerev;
}

void event_wait() {
	SysData* sysData = (SysData*)syscallData;
	Event* event = Event::events[sysData->id];
	KernelEv* kerev = KernelEv::kerevs[event->myImpl];

	kerev->wait();
}
