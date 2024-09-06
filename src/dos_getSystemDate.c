#include "dos.h"


void  getSystemDate(SYSTEMDATE_t *date) __sdcccall(1)
{
	date;
/*
    GET DATE (2AH)
    Parameters:    C = 2AH (_GDATE)
    Results:      HL = Year 1980...2079
                   D = Month (1=Jan...12=Dec)
                   E = Date (1...31)
                   A = Day of week (0=Sun...6=Sat)

    This function simply returns the current value of the internal calender
    in the format shown.
*/
	__asm
		push ix
		push hl					; HL = Param date

		ld   c, #GDATE
		DOSCALL

		pop  ix
		ld   0(ix), l
		ld   1(ix), h
		ld   2(ix), d
		ld   3(ix), e

		pop  ix
		ret
	__endasm;
}
