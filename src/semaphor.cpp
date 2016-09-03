#include "semaphor.h"
#include "kernesem.h"
#include "macros.h"

Semaphore::Semaphore(int init){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl = new KernelSem(init);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Semaphore::~Semaphore(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Semaphore::wait(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Semaphore::signal(){

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int Semaphore::val() const {

#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	int v = myImpl->val();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return v;
}
