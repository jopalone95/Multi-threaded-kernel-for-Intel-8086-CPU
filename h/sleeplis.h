/*
 * sleeplis.h
 *
 *  Created on: Jun 6, 2018
 *      Author: OS1
 */

#ifndef _sleeplis_h_
#define _sleeplis_h_

#include "thread.h"

class PCB;

class SleepList {
private:
    struct ListNode { //mozda ovde nesto treba da bude volatile???
    	PCB* thread;
        Time timeToSleep;
        ListNode* next;

        ListNode(PCB* thread, Time timeToSleep) {this->thread = thread; this->timeToSleep = timeToSleep; this->next = 0;}
    };

    volatile ListNode* head;
    volatile unsigned flag;

public:
    SleepList();
    ~SleepList();
    void add(PCB* thread, Time timeToSleep);
    void print();
    unsigned empty();
    unsigned unlocked();
    void lock();
    void unlock();
    void release();
};

#endif /* HEDERI_SLEEPLIS_H_ */
