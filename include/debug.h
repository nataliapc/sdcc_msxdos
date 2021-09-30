
#ifndef __OPENMSX_DEBUG_H__
#define __OPENMSX_DEBUG_H__


// To debug type at openmsx console:
//    debug set_watchpoint write_io 0x18
#define DEBUG_BREAKPOINT __asm out (0x18),a __endasm;

#define DEBUG_BORDERCOLOR \
	ld  a,X \
	out (0x99),a \
	ld  a,\#0x87 \
	out (0x99),a

#define DEBUG_PRINTNUM \
	push af \
	ld  a,#0x60 \
	out (0x2e),a \
	ld  a,X \
	out (0x2f),a \
	pop af

#define DEBUG_PRINT_A \
	push af \
	ld  a,#0x60 \
	out (0x2e),a \
	pop af \
	out (0x2f),a

#define DEBUG_PRINT_HL \
	push af \
	ld  a,#0x60 \
	out (0x2e),a \
	ld  a,h \
	out (0x2f),a \
	ld  a,l \
	out (0x2f),a \
	pop af

#define DEBUG_PRINT_DE \
	push af \
	ld  a,#0x60 \
	out (0x2e),a \
	ld  a,d \
	out (0x2f),a \
	ld  a,e \
	out (0x2f),a \
	pop af

#define DEBUG_PRINT_BC \
	push af \
	ld  a,#0x60 \
	out (0x2e),a \
	ld  a,b \
	out (0x2f),a \
	ld  a,c \
	out (0x2f),a \
	pop af


#endif  // __OPENMSX_DEBUG_H__
