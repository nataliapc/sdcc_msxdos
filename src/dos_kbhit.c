#include "dos.h"

#ifndef DISABLE_CONIO

int kbhit(void) __naked
{
  __asm
	push af
	push bc

	ld c,#CONST
	DOSCALL
	ld h,#0

	pop bc
	pop af
	ret
  __endasm;
}

#endif
