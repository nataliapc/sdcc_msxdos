#include "dos.h"


char get_drive_letter_info(char drive, DRIVE_info *info) __naked
{
  drive, info;
  __asm
    push ix
    ld ix,#4
    add ix,sp

    ld a,0(ix)
    ld l,1(ix)
    ld h,2(ix)
    ld c,#GDLI
    DOSCALL

    ld hl, #0x00
    or a
    jr z, gdli_noerrors$
    ld l, a
  gdli_noerrors$:
    pop ix
    ret
  __endasm;
}
