#ifndef _pcb_h_
#define _pcb_h_

class List;
class Thread;
class Kernel;

typedef unsigned long StackSize;
typedef unsigned int Time;

typedef enum Status {NEW, READY, BLOCKED, FINISHED};

class PCB{
	public:

		PCB(StackSize , Time , Thread* );
		Status getStatus();
		void setStatus(Status );
		~PCB();
		void allocateStack();


	private:

		static List *allPCBs;
		static List *sleeping;

		Thread *myThread;
		List *waiting;
		Time timeSlice, sleepTime;
		StackSize stackSize;
		Status myThreadStatus;
		unsigned* stack;
		unsigned sp, ss, bp;

		friend class Kernel;
		friend class Thread;
		friend class List;
};


#endif
