/*
 * semaphor.h
 *
 *  Created on: Jun 1, 2018
 *      Author: OS1
 */

#ifndef _semaphor_h_
#define _semaphor_h_

#include "thread.h"

class KernelSem;

class Semaphore {
private:
	static int helper;
	static Semaphore* sems[256];
	ID id;
	ID myImpl;

protected:
	friend void sem_cons();
	friend void sem_dest();
	friend void sem_wait();
	friend void sem_signal();
	friend void sem_val();

public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait(int toBlock);
	virtual void signal();

	int val() const;
};

#endif /* HEDERI_SEMAPHOR_H_ */
