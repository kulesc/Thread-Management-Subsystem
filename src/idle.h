#ifndef _idle_h_
#define _idle_h_

#include "thread.h"

class Thread;

class IdleThread : public Thread {

public:

	IdleThread();
	virtual ~IdleThread();
	void run();
	void start();

	friend class Kernel;
};

#endif
