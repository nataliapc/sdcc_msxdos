#include "dos.h"

#ifndef DISABLE_CONIO

#if __SDCC_VERSION_NUM < 40112
int putchar(int c) __naked __z88dk_fastcall
{
  c;
  __asm
	ld a,l
#else
int putchar(int c) __naked
{
  c;
  __asm
#endif
	ld e,a
	ld c,#CONOUT

	cp #0x0a
	jp nz,jumpPutchar$

#if defined(DEBUG) || defined(TEST)
	push af            ; to use with openmsx and '-ext debugdevice' extension
	ld  a, #0x63
	out (#0x2e),a
	ld  a,e
	out (#0x2f),a
	pop af
#endif

	DOSCALL
	ld e,#0x0d
jumpPutchar$:
	DOSCALL

#if defined(DEBUG) || defined(TEST)
	push af            ; to use with openmsx and '-ext debugdevice' extension
	ld  a, #0x63
	out (#0x2e),a
	ld  a,e
	out (#0x2f),a
	pop af
#endif

	ret
  __endasm;
}

#endif
