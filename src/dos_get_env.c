#include "dos.h"


#ifdef MSXDOS2
/*
    GET ENVIRONMENT ITEM (6BH)
    Parameters:    C = 6BH (_GENV)
                  HL = ASCIIZ name string
                  DE = Pointer to buffer for value
                   B = Size of buffer
    Results:       A = Error [IENV, ELONG]
                  DE = Preserved, buffer filled in if A=0

This function gets the current value of the environment item whose name is
passed in register HL. A ".IENV" error is returned if the name string is
invalid. If there is no environment item of that name then a null string will
be returned in the buffer. If there is an item of that name then its value
string will be copied to the buffer. If the buffer is too small then the
value string will be truncated with no terminating null and a ".ELONG" error
will be returned. A buffer 255 bytes will always be large enough since value
strings cannot be longer than this (including the terminating null).
*/
ERR8 get_env(char* name, char* buffer, uint8_t buffer_size) __naked __sdcccall(0)
{
	name, buffer, buffer_size;
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld l,0(ix)
		ld h,1(ix)
		ld e,2(ix)
		ld d,3(ix)
		ld b,4(ix)
		pop ix

		ld c,#GENV
		DOSCALL

		ld l,a
		ret
	__endasm;
}
#endif