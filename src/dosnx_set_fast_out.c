#include "dos.h"


static RETB _fast_out(uint8_t getset, uint8_t value) __naked __sdcccall(1)
{
	getset, value;
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
	__asm
						; A = Param getset
		ld b,l			; B = Param value

		ld c,#FOUT
		DOSCALL

		or a			; Returns A
		ret nz			; error
		ld a,b
		ret				; no error
	__endasm;
}

inline RETB nxtr_setFastOut(uint8_t value)
{
	return _fast_out(1, value);
}

inline RETB nxtr_getFastOut(void)
{
	return _fast_out(0, 0);
}
