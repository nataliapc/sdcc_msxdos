#include "dos.h"

#ifndef DISABLE_CONIO

#if __SDCC_VERSION_NUM < 40112
int putchar(int c) __naked __z88dk_fastcall
#else	//SDCC_VER < 4.1.12
int putchar(int c) __naked  __sdcccall(1)
#endif	//SDCC_VER < 4.1.12
{
	// @parameter [int c] -> HL
	c;
	__asm
		ld e,l
		ld c,#CONOUT

		ld a,e
		cp #0x0a			; \n
		jp nz, jumpPutchar$

#if defined(DEBUG) || defined(TEST)
		push af				; to use with openmsx and '-ext debugdevice' extension
		ld  a, #0x63
		out (#0x2e),a
		ld  a,e
		out (#0x2f),a
		pop af
#endif	//DEBUG || TEST

		DOSCALL
		ld e,#0x0d			; \n + \r
jumpPutchar$:
		DOSCALL

#if defined(DEBUG) || defined(TEST)
		push af				; to use with openmsx and '-ext debugdevice' extension
		ld  a, #0x63
		out (#0x2e),a
		ld  a,e
		out (#0x2f),a
		pop af
#endif	//DEBUG || TEST

		ret
	__endasm;
}

#endif	//DISABLE_CONIO
