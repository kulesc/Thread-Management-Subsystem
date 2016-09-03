#ifndef _ivtentry_h_
#define _ivtentry_h_

typedef void interrupt (*InterruptHandler)(...);

typedef unsigned char IVTNo;

class KernelEv;

class IVTEntry{

public:

	IVTEntry(IVTNo , InterruptHandler );
	~IVTEntry();
	static IVTEntry* getEntry(IVTNo );
	void setKernelEvent(KernelEv* );
	void removeKernelEvent();
	void signal();
	void setOldFlag(int );

private:

	static IVTEntry* IVTable[256];
	KernelEv* myKernelEvent;
	IVTNo ivtNo;
	InterruptHandler oldInterruptHandler, myInterruptHandler;
	int oldFlag;
};


#endif
