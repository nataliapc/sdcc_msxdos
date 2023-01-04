#include "dos.h"


#ifdef MSXDOS2

char get_current_directory(char drive, char *path) __naked __sdcccall(0)
{
  drive, path;
  __asm
    push ix
    ld ix,#4
    add ix,sp

    ld b,0(ix)
    ld e,1(ix)
    ld d,2(ix)
    pop ix

    ld c, #GETCD
    DOSCALL

    ld h, #0x00
    ld l, a
    ret
  __endasm;
}

#endif
#ifdef MSXDOS1

char get_current_directory(char drive, char *path)
{
    *path = '\0';
    return drive < 8 ? 0 : ERR_IDRV;
}

#endif