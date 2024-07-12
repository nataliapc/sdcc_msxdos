#include "dos.h"


void dos2_exit(uint8_t code) __naked __sdcccall(1)
{
	code;
/*
    TERMINATE WITH ERROR CODE (62H)
    Parameters:    C = 62H (_TERM) 
                   B = Error code for termination
    Results:       Does not return

This function terminates the program with the specified error code, which may
be zero indicating no error. This function call will never return to the
caller (unless a user abort routine executes forces it to - see function
63h). The operation of this function is different depending on whether it was
called from the MSX-DOS environment via 00005h or from the disk BASIC
environment via 0F37Dh.

If called via 00005h then if a user abort routine has been defined by function
63h it will be called with the specified error code (and a zero secondary
error code). Assuming that this routine returns, or if there was no user abort
routine defined, then control will be passed back to whatever loaded the
transient program via a jump at location 00000h. This will almost always be the
command interpreter, but in some cases it may be another transient program. The
error code will be remembered by the system and the next "join" function
(function 61h) which is done will return this error code. The
command interpreter will print an error message for any code in the range
20h...FFh, but will not print a message for errors below this.

If this function is called from the disk BASIC environment via 0F37Dh then
control will be passed to the abort vector at location "BREAKVECT". In this
environment there is no separately defined user abort routine and the error
code must be remembered by the code at "BREAKVECT" because "join" will not
return the error code.
*/
	__asm
		ld b,a		; exit status parameter

		ld c,#TERM
		DOSCALL

		ret
	__endasm;
}
