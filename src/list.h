#ifndef _list_h_
#define _list_h_

class PCB;

class List{

private:

	struct Elem{
		PCB* pcb;
		Elem* next;
		Elem(PCB* p);
	};

	Elem* head, *tail;

public:

	List();
	~List();
	void append(PCB* );
	List& remove(PCB* );
	PCB* getFirst();
	int isEmpty();
	void addSorted(PCB*);
	void checkAndWake();
};

#endif
