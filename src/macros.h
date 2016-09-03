#ifndef _macros_h_
#define _macros_h_

#include <dos.h>

#define lock asm{ pushf; cli;}

#define unlock asm popf;

#endif
