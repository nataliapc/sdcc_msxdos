#include "dos.h"


RETW dos2_getAttrib(char *filename) __naked __sdcccall(1)
{
	filename;
/*
	GET/SET FILE ATTRIBUTES (50H)
	Parameters: C = 50H (_ATTR)
	            DE = Drive/path/file ASCIIZ string or fileinfo block pointer
	            A = 0 => get attributes
	Results:    A = Error
	            L = Current attributes byte

	Returns 0x00LL on success (LL = attributes byte), or 0xFFEE on failure
	(EE = MSX-DOS error code). Wildcards are not allowed by MSX-DOS for this call.
*/
	__asm
		ex de,hl		; DE = Param filename
		xor a			; A = 0 => get attributes
		ld c,#ATTR
		DOSCALL

		or a
		jr nz,.error	; error = 0xff00 | A
		ld h,#0			; success = 0x00LL, L already holds attributes
		ret
	.error:
		ld l,a
		ld h,#0xff
		ret
	__endasm;
}
