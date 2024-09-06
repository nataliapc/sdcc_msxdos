#include "dos.h"


void  getSystemTime(SYSTEMTIME_t *time) __sdcccall(1)
{
	time;
/*
    GET TIME (2CH)
    Parameters:    C = 2CH (_GTIME)
    Results:       H = Hours (0...23)
                   L = Minutes (0...59)
                   D = Seconds (0...59)
                   E = Centiseconds (always zero)

    This function returns the current value of the system clock in the
    format shown.
*/
	__asm
		push ix
		push hl					; HL = Param time

		ld   c, #GDATE
		DOSCALL

		pop  ix
		ld   0(ix), h
		ld   1(ix), l
		ld   2(ix), d

		pop  ix
		ret
	__endasm;
}