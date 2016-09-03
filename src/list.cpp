#include "list.h"
#include "macros.h"
#include "pcb.h"
#include "SCHEDULE.h"
#include <dos.h>

List::Elem::Elem(PCB* p): pcb(p), next(0) {}

List::List(): head(0), tail(0) {}

List::~List(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	while(head){
		tail = head;
		head = head->next;
		delete tail;
	}
	tail = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void List::append(PCB* p){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* elem = new Elem(p);
	if(head == 0) head = elem;
	else tail->next = elem;
	tail = elem;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

List& List::remove(PCB* p){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* tmp = head, *prev = 0;
	while(tmp){
		if(p == tmp->pcb) break;
		prev = tmp;
		tmp = tmp->next;
	}
	if(tmp != 0){
		if(prev == 0){
			head = head->next;
			delete tmp;
			if(head == 0) tail = 0;
		}
		else if(tmp->next == 0){
			tail = prev;
			delete tmp;
			tail->next = 0;
		}
		else {
			prev->next = tmp->next;
			delete tmp;
		}
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return *this;
}

PCB* List::getFirst(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(head == 0){
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
		return 0;
	}
	Elem* tmp = head;
	if(head->next) head = head->next;
	else head = tail = 0;
	PCB* p = tmp->pcb;
	delete tmp;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return p;
}

int List::isEmpty(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	int value = 0;
	if(head == 0) value = 1;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return value;
}

void List::addSorted(PCB* p){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* elem = new Elem(p), *tmp = head, *prev = 0;
	if(tmp == 0) {
		head = tail = elem;
	#ifndef BCC_BLOCK_IGNORE
		unlock
	#endif
		return;
	}
	while(tmp != 0 && (elem->pcb->sleepTime > tmp->pcb->sleepTime)){
		prev = tmp;
		tmp = tmp->next;
	}
	if(prev == 0) {
		elem->next = head;
		head = elem;
	}
	else{
		prev->next = elem;
		elem->next = tmp;
		if(tmp == 0) tail = elem;
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void List::checkAndWake(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* tmp = head;
	PCB* pcb;
	while(tmp != 0){

		tmp->pcb->sleepTime--;
		tmp = tmp->next;
	}
	while(isEmpty() == 0){

		pcb = getFirst();
		if(pcb->sleepTime == 0){
			pcb->setStatus(READY);
			Scheduler::put(pcb);
		}
		else{
			addSorted(pcb);
			break;
		}
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
