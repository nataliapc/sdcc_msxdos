#include "dos.h"

#ifndef DISABLE_CONIO

bool kbhit(void) {
	return ADDR_POINTER_WORD(PUTPNT) != ADDR_POINTER_WORD(GETPNT);
}

#endif	//DISABLE_CONIO
