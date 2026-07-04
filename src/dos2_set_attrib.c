#include "dos.h"


RETW dos2_setAttrib(char *filename, uint8_t attrib) __naked __sdcccall(1)
{
	filename, attrib;
/*
	GET/SET FILE ATTRIBUTES (50H)
	Parameters: C = 50H (_ATTR)
	            DE = Drive/path/file ASCIIZ string or fileinfo block pointer
	            A = 1 => set attributes
	            L = New attributes byte
	Results:    A = Error
	            L = Current attributes byte

	Returns 0x00LL on success (LL = resulting attributes byte), or 0xFFEE on
	failure (EE = MSX-DOS error code). Wildcards are not allowed by MSX-DOS for
	this call.
*/
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld a,0(ix)		; A  = Param attrib (2nd arg: stack)
		pop ix

		ex de,hl		; DE = Param filename (1st arg: HL)
		ld l,a			; L  = Param attrib

		ld a,#1			; A = 1 => set attributes
		ld c,#ATTR
		DOSCALL

		or a
		jr nz,.error	; error = 0xff00 | A
		ld h,#0			; success = 0x00LL, L already holds resulting attributes
		ret
	.error:
		ld l,a
		ld h,#0xff
		ret
	__endasm;
}
