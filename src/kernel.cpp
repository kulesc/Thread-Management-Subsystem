#include "pcb.h"
#include "list.h"
#include "thread.h"
#include "kernelev.h"
#include "kernesem.h"
#include "ivtentry.h"
#include "macros.h"
#include "idle.h"
#include "SCHEDULE.h"
#include "kernel.h"
#include <dos.h>

extern void tick();

PCB* Kernel::running = 0;
IdleThread* Kernel::idle = 0;
volatile unsigned Kernel::explicitContextChange = 0;
volatile unsigned Kernel::counter = 1;
InterruptHandler Kernel::oldTimerHandler = 0;
Thread* Kernel::first = 0;

void Kernel::initialize(){

#ifndef BCC_BLOCK_IGNORE
	lock
	oldTimerHandler = getvect(0x8);
	setvect(0x8, dispatchRoutine);
#endif
	first = new Thread();
	first->start();
	running = first->myPCB;
	idle = new IdleThread();
	idle->start();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Kernel::restore(){

#ifndef BCC_BLOCK_IGNORE
	lock
	setvect(0x8, oldTimerHandler);
#endif
	delete first;
	delete idle;
	delete PCB::allPCBs;
	delete PCB::sleeping;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Kernel::changeContext(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	explicitContextChange = 1;
	dispatchRoutine();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Kernel::wrapper(){

	running->myThread->run();
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	running->setStatus(FINISHED);
	PCB::allPCBs->remove(running);
	while(running->waiting->isEmpty() != 1){

		PCB* tmp = running->waiting->getFirst();
		tmp->setStatus(READY);
		Scheduler::put(tmp);
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	changeContext();
}

void interrupt Kernel::dispatchRoutine(...){

	static volatile unsigned tmpSS, tmpSP, tmpBP;
	static PCB* nextThreadToExecute;

	if(!explicitContextChange){

		(*oldTimerHandler)();
		tick();
		PCB::sleeping->checkAndWake();
		if(running->timeSlice != 0) counter--;
		if(counter != 0 || running->timeSlice == 0) return;
	}
	else explicitContextChange = 0;

	if(running->getStatus() == READY && running->myThread != idle) Scheduler::put(running);
	nextThreadToExecute = Scheduler::get();
	if(nextThreadToExecute == 0) nextThreadToExecute = idle->myPCB;
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov tmpSS, ss
		mov tmpSP, sp
		mov tmpBP, bp
	}
#endif
	running->ss = tmpSS;
	running->sp = tmpSP;
	running->bp = tmpBP;
	running = nextThreadToExecute;
	tmpSS = running->ss;
	tmpSP = running->sp;
	tmpBP = running->bp;
	counter = running->timeSlice;
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov ss, tmpSS
		mov sp, tmpSP
		mov bp, tmpBP
	}
#endif
}

