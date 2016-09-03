#include "kernelev.h"
#include "kernel.h"
#include "pcb.h"
#include "thread.h"
#include "ivtentry.h"
#include "SCHEDULE.h"


KernelEv::KernelEv(IVTNo ivtno){

	semValue = 1;
	myPCB = Kernel::running;
	ivtNo = ivtno;
	IVTEntry::getEntry(ivtno)->setKernelEvent(this);
}

KernelEv::~KernelEv(){

	IVTEntry::getEntry(ivtNo)->removeKernelEvent();
}

void KernelEv::wait(){

	if(myPCB != Kernel::running) return;
		semValue = 0;
		myPCB->setStatus(BLOCKED);
		dispatch();
}

void KernelEv::signal(){

	if(semValue == 0){
			semValue = 1;
			myPCB->setStatus(READY);
			Scheduler::put(myPCB);
		}
}
