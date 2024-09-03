#include "dos.h"


RETW availableDrives() __naked __sdcccall(0)
{
/*
    GET LOGIN VECTOR (18H)
    Parameters:    C = 18H (_LOGIN)
    Results:      HL = Login vector

    This function returns a bit set in HL for each drive which is available, 
    bit-0 of L corresponding to drive "A:". Up to eight drives ("A:" to "H:") 
    are supported by the system currently, so register H will usually be zero 
    on return.
*/
	__asm
		push ix
		ld c,#LOGIN
		DOSCALL
		pop  ix
		ret				; Returns HL
	__endasm;
}
