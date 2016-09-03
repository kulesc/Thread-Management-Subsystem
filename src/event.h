#ifndef _event_h_
#define _event_h_

#include "ivtentry.h"

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:

	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:

	friend class KernelEv;

	void signal(); // can call KernelEv

private:

	KernelEv* myImpl;
};

#define PREPAREENTRY(ivtNo, callOld) \
		void interrupt intRoutine##ivtNo(...); \
		IVTEntry* entry##ivtNo = new IVTEntry(ivtNo, intRoutine##ivtNo); \
		void interrupt intRoutine##ivtNo(...){ \
			entry##ivtNo->setOldFlag(callOld); \
			IVTEntry::getEntry(ivtNo)->signal(); \
		} \

#endif
