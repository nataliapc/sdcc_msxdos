#include "dos.h"


#ifdef MSXDOS2

char get_drive_params(char drive, DPARM_info *param) __naked __sdcccall(0)
{
  drive, param;
  __asm
    push ix
    ld ix,#4
    add ix,sp

    ld l,0(ix)
    ld e,1(ix)
    ld d,2(ix)
    pop ix

    ld c,#DPARM
    DOSCALL

    ld h, #0x00
    or a
    jr z, dparm_noerrors$
    ld l, a
    ret
  dparm_noerrors$:
    ld l, #0x00
    ret
  __endasm;
}

#else

char get_drive_params(char drive, DPARM_info *param)
{
    return ERR_IBDOS;
}

#endif
