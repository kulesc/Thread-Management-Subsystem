#ifndef _kernesem_h_
#define _kernesem_h_

class List;

class KernelSem{

public:

	KernelSem(int init = 1);
	~KernelSem();
	void wait ();
	void signal();
	int val () const;

private:

	int semValue;
	List* waiting;
};


#endif
