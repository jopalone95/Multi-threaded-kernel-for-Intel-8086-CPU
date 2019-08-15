/*
 * sleeplis.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: OS1
 */

#include "sleeplis.h"
#include "pcb.h"
#include "SCHEDULE.H"

#include <iostream.h>

SleepList::SleepList() {head = 0; flag = 1;}

SleepList::~SleepList() {
    while (head) {
        ListNode* toDelete = (ListNode*)head;
        head = head->next;
        delete toDelete;
    }
}

void SleepList::add(PCB* thread, Time timeToSleep) {
	lock();

    ListNode* novi = new ListNode(thread, timeToSleep);

    ListNode* pret = 0;
    ListNode* temp = (ListNode*)head;

    Time t_pret = 0;
    Time t_temp = 0;
    if (temp)
    	t_temp = temp->timeToSleep;

    while (temp && t_temp <= timeToSleep) {
    	pret = temp;
    	t_pret = t_temp;

    	temp = temp->next;
    	if (temp)
    		t_temp += temp->timeToSleep;
    }

    if (!pret) {
    	novi->next = (ListNode*)head;
    	head = novi;
    	if (temp)
    		temp->timeToSleep -= timeToSleep;
	} else if (!temp) {
		novi->timeToSleep = timeToSleep - t_pret;
		pret->next = novi;
	} else {
		novi->timeToSleep = timeToSleep - t_pret;
		pret->next = novi;
		novi->next = temp;
		temp->timeToSleep = t_temp - timeToSleep;
	}

    unlock();
}

void SleepList::print() {
    cout << "Lista:\n";

    ListNode* temp = (ListNode*)head;
    while (temp) {
        cout << temp->timeToSleep << ' ';
        temp = temp->next;
    }
}

unsigned SleepList::empty() {
	if (head)
		return 0;
	else
		return 1;
}

unsigned SleepList::unlocked() {
	return head && flag; //kada je head ili lock nula sleeplista je zakljucana -> lock treba da bude inicijalizovan na jedinicu
}

void SleepList::lock() {flag = 0;}

void SleepList::unlock() {flag = 1;}

void SleepList::release() {
	head->timeToSleep--;

	while (head && !head->timeToSleep) {
		head->thread->state = executing;
		Scheduler::put(head->thread);

		ListNode* toDelete = (ListNode*)head;
		head = head->next;
		delete toDelete;
	}
}
