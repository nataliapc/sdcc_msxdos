#include "dos.h"


/*
Get/set fast STROUT mode (_FOUT, 71h)
Parameters:  C = 71H (_FOUT)
             A =  00H => get fast STROUT mode
                  01H => set fast STROUT mode
             B =  00H => disable (only if A=01H)
                  FFH => enable (only if A=01H)

Results:     A = Error
             B = Current fast STROUT mode
*/
static uint16_t _fast_out(uint8_t getset, uint8_t value) __naked __sdcccall(0)
{
	getset, value;
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld a,0(ix)
		ld b,1(ix)
		pop ix

		ld c,#FOUT
		DOSCALL

		ld h, #0
		ld l, b
		or a
		ret z
		dec h
		ld l, a
		ret
	__endasm;
}

inline uint16_t set_fast_out(uint8_t value)
{
	return _fast_out(1, value);
}

inline uint16_t get_fast_out(void)
{
	return _fast_out(0, 0);
}
