#include "ivtentry.h"
#include "macros.h"
#include "dos.h"
#include "kernelev.h"
#include "thread.h"

IVTEntry* IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtno, InterruptHandler newInterruptHandler){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	ivtNo = ivtno;
	myInterruptHandler = newInterruptHandler;
#ifndef BCC_BLOCK_IGNORE
	oldInterruptHandler = getvect(ivtNo);
	setvect(ivtNo, myInterruptHandler);
#endif
	myKernelEvent = 0;
	IVTable[ivtNo] = this;
	oldFlag = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

IVTEntry::~IVTEntry(){

#ifndef BCC_BLOCK_IGNORE
	lock
	IVTable[ivtNo] = 0;
	setvect(ivtNo, oldInterruptHandler);
	(*oldInterruptHandler)();
	unlock
#endif
}

void IVTEntry::setKernelEvent(KernelEv* event){

	myKernelEvent = event;
}

void IVTEntry::removeKernelEvent(){

	myKernelEvent = 0;
}

IVTEntry* IVTEntry::getEntry(IVTNo ivtno){

	return IVTable[ivtno];
}

void IVTEntry::signal(){

	if(oldFlag) (*oldInterruptHandler)();
	if(myKernelEvent) myKernelEvent->signal();
	dispatch();
}

void IVTEntry::setOldFlag(int callOld){

	oldFlag = callOld;
}
