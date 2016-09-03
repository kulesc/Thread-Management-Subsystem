#include "thread.h"
#include "pcb.h"
#include "macros.h"
#include "list.h"
#include "kernel.h"
#include <dos.h>

List* PCB::allPCBs = new List();

List* PCB::sleeping = new List();

PCB::PCB(StackSize stackS, Time ts, Thread* t){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myThread = t;
	waiting = new List();
	timeSlice = ts;
	sleepTime = 0;
	stackSize = stackS;
	myThreadStatus = NEW;
	stack = 0;
	ss = sp = bp = 0;
	allPCBs->append(this);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCB::setStatus(Status s){

	myThreadStatus = s;
}

Status PCB::getStatus(){

	return myThreadStatus;
}

PCB::~PCB(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(waiting) delete waiting;
	if(stack) delete[] stack;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCB::allocateStack(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	stack = new unsigned[stackSize];
	stack[stackSize - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 2] = FP_SEG(Kernel::wrapper);
	stack[stackSize - 3] = FP_OFF(Kernel::wrapper);
	sp = bp = FP_OFF(&stack[stackSize - 12]);
	ss = FP_SEG(&stack[stackSize - 12]);
	unlock
#endif
}
