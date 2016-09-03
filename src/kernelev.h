#ifndef _kernelev_h_
#define _kernelev_h_

typedef unsigned char IVTNo;

class PCB;

class KernelEv{

public:

	KernelEv(IVTNo ivtno);
	~KernelEv();
	void wait();
	void signal();

private:

	PCB* myPCB;
	int semValue;
	IVTNo ivtNo;
};



#endif
