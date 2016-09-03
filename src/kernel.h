#ifndef _kernel_h_
#define _kernel_h_

class Thread;
class IdleThread;
class PCB;

typedef void interrupt (*InterruptHandler)(...);

class Kernel{

public:

	static void initialize();
	static void restore();
	static void changeContext();

private:

	static PCB* running;
	static IdleThread* idle;
	static volatile unsigned explicitContextChange;
	static volatile unsigned counter;
	static void wrapper();
	static void interrupt dispatchRoutine(...);
	static InterruptHandler oldTimerHandler;
	static Thread* first;

	friend class PCB;
	friend class Thread;
	friend class KernelEv;
	friend class KernelSem;
};



#endif
