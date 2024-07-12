#include "dos.h"

#ifndef DISABLE_CONIO

int getchar(void) __naked __sdcccall(1)
{
/*
    CONSOLE INPUT WITHOUT ECHO (08H)
    Parameters:    C = 08H (_INNOE)
    Results:     L=A = Input character

This function is identical to function 01h (_CONIN) except that the input 
character will not be echoed to the standard output. The same control 
character checks will be done. This function is not compatible with CP/M 
which uses this function number for "set I/O byte".
*/
	__asm
		ld c,#INNOE
		DOSCALL

		ld d, #0x00		; Returns DE
		ld e, l
		ret
	__endasm;
}

#endif  //DISABLE_CONIO
