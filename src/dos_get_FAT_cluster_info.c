#include "dos.h"


char get_FAT_cluster_info(char drive, uint16_t clusterNumber, CLUSTER_info *info) __naked
{
  drive, info;
  __asm
    push ix
    ld ix,#4
    add ix,sp

    ld a,0(ix)
    ld e,1(ix)
    ld d,2(ix)
    ld l,3(ix)
    ld h,4(ix)
    ld c,#GETCLUS
    DOSCALL

    ld hl, #0x00
    or a
    jr z, gclus_noerrors$
    ld l, a
  gclus_noerrors$:
    ld l, #0x00
    pop ix
    ret
  __endasm;
}
