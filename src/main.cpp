#include "kernel.h"
#include <iostream.h>

extern int userMain (int argc, char* argv[]);

int main(int argc, char *argv[]){

	cout << "\n\t\t\t~~~WELCOME~~~" << endl;
	Kernel::initialize();
	int result = userMain(argc, argv);
	Kernel::restore();
	cout << "\n\t\t\t~~~GOODBYE~~~" << endl;
	return result;
}

