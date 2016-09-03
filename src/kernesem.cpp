#include "kernesem.h"
#include "list.h"
#include "thread.h"
#include "SCHEDULE.h"
#include "pcb.h"
#include "kernel.h"


KernelSem::KernelSem(int init){

	semValue = init;
	waiting = new List();
}

KernelSem::~KernelSem(){

	while(semValue < 0) signal();
	delete waiting;
}

void KernelSem::wait(){

	semValue--;
	if(semValue < 0){

		Kernel::running->setStatus(BLOCKED);
		waiting->append(Kernel::running);
		dispatch();
	}
}

void KernelSem::signal(){

	semValue++;
	if(semValue <= 0){

		PCB* p = waiting->getFirst();
		p->setStatus(READY);
		Scheduler::put(p);
	}
}

int KernelSem::val() const{

	return semValue;
}
