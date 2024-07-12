#include "dos.h"


RETB dosVersion(void) __naked __z88dk_fastcall
{
/*
    GET MSX-DOS VERSION NUMBER (6FH)
    Parameters:    C = 6FH (_DOSVER)
    Results:       A = Error (always zero)
                  BC = MSX-DOS kernel version
                  DE = MSXDOS2.SYS version number

This function allows a program to determine which version of MSX-DOS it is
running under. Two version numbers are returned, one in BC for the MSX-DOS
kernel in ROM and the other is DE for the MSXDOS2.SYS system file. Both of
these version numbers are BCD values with the major version number in the high
byte and the two digit version number in the low byte. For example if there
were a version 2.34 of the system, it would be represented as 0234h.

For compatibility with MSX-DOS 1.0, the following procedure should always be
followed in using this function. Firstly if there is any error (A<>0) then it
is not MSX-DOS at all. Next look at register B. If this is less than 2 then the
system is earlier than 2.00 and registers C and DE are undefined. If register B
is 2 or greater then registers BC and DE can be used as described above. In
general the version number which should be checked (after this procedure) is
the MSXDOS2.SYS version in register DE.

*** NEXTOR OS have additional functionality in this call ***
*/
	__asm
		push ix

		ld b,  #0x5A		; magic numbers to detect Nextor
		ld hl, #0x1234
		ld de, #0xABCD
		ld ix, #0

		ld c,#DOSVER
		call 0xF37D			; BDOS (upper memory DOSCALL)

		or  a
		jr  z,check_dos1$
		ld  b,#0			; unknown DOS
		jr  ret_version$

	check_dos1$:
		ld  a,b				; B<2 --> MSX-DOS 1
		cp  #2
		jr  nc,check_dos2nextor$
		ld  b,#1			; is MSX-DOS 1
		jr  ret_version$

	check_dos2nextor$:
		push ix				; Nextor: IXh must contain '1'
		pop hl
		ld  a,h
		dec a
		jr  z,is_nextor$
		ld  b,#2			; is MSXDOS 2
		jr  ret_version$
	
	is_nextor$:				; is NextorDOS
		ld  b,#3

	ret_version$:
		ld  l, b

		pop ix
		ret
	__endasm;
}

