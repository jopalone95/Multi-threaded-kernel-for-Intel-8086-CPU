/*
 * thread.h
 *
 *  Created on: May 15, 2018
 *      Author: OS1
 */

#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
typedef int ID;

class PCB;

class Thread {
private:
	static int helper;
	static Thread* threads[256];

	static void wrapper(Thread* thread);
	static void finish();

protected:
	friend class PCB;
	friend void thread_cons();
	friend void thread_dest();
	friend void thread_start();
	friend void thread_waitToComplete();
	friend void thread_sleep();
	friend void thread_finish();

	ID myPCB;
	ID id;

	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	static void sleep(Time timeToSleep);
};

#endif /* HEDERI_THREAD_ */
