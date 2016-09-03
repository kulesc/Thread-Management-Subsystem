#include "thread.h"
#include "SCHEDULE.h"
#include "pcb.h"
#include "kernel.h"
#include "list.h"
#include "macros.h"
#include <iostream.h>

Thread::~Thread(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	waitToComplete();
	delete myPCB;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Thread::Thread(StackSize stackSize, Time timeSlice){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB = new PCB(stackSize, timeSlice, this);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::start(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->allocateStack();
	myPCB->setStatus(READY);
	Scheduler::put(myPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::waitToComplete(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(myPCB->getStatus() == NEW || myPCB->getStatus() == FINISHED || Kernel::running->myThread == this){
	#ifndef BCC_BLOCK_IGNORE
		unlock
	#endif
		return;
	}
	Kernel::running->setStatus(BLOCKED);
	myPCB->waiting->append(Kernel::running);
	dispatch();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::sleep(Time timeToSleep){

	Kernel::running->sleepTime = timeToSleep;
	Kernel::running->setStatus(BLOCKED);
	PCB::sleeping->addSorted(Kernel::running);
	dispatch();
}

void dispatch(){

	Kernel::changeContext();
}
