#include "dos.h"


RETW dos2_getScreenSize(void) __naked __sdcccall(1)
{
/*
    I/O CONTROL FOR DEVICES (4BH)
    Parameters:    C = 4BH (_IOCTL) 
                   B = File handle
                   A = Sub-function code
                       00H => get file handle status
                       01H => set ASCII/binary mode
                       02H => test input ready
                       03H => test output ready
                       04H => find screen size
                  DE = Other parameters
    Results:       A = Error
                  DE = Other results
*/
	__asm
		ld a, #0x04			; A = find screen size

		ld c,#IOCTL
		DOSJP				; Returns HL
	__endasm;
}
