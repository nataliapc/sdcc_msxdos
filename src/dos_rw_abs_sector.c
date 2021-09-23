#include "dos.h"


char _abs_sector(uint8_t drv, uint16_t startsec, uint8_t nsec, uint8_t doscall) __naked {
  doscall, drv, nsec, startsec;
  __asm
    push ix
    ld ix,#4
    add ix,sp

    ld l,0(ix)
    ld e,1(ix)
    ld d,2(ix)
    ld h,3(ix)
    ld c,4(ix)
    DOSCALL

    ld h, #0
    ld l, a

    pop ix
    ret
  __endasm;
}

char read_abs_sector(uint8_t drv, uint16_t startsec, uint8_t nsec) {
  return _abs_sector(drv, startsec, nsec, RDABS);
}

char write_abs_sector(uint8_t drv, uint16_t startsec, uint8_t nsec) {
  return _abs_sector(drv, startsec, nsec, WRABS);
}
