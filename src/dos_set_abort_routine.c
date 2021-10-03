#include "dos.h"


void set_abort_routine(void *routine) __naked __z88dk_fastcall {
  routine;
#ifdef MSXDOS2
  __asm
	ex de,hl

    ld c,#DEFAB
    DOSCALL

    ret
  __endasm;
#else
  __asm ret __endasm;
#endif
}
