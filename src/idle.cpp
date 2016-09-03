#include "idle.h"
#include "macros.h"
#include "pcb.h"
#include <iostream.h>

IdleThread::IdleThread():Thread() {}

IdleThread::~IdleThread() {}

void IdleThread::run(){

	while(1);
}

void IdleThread::start(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->setStatus(READY);
	myPCB->allocateStack();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
