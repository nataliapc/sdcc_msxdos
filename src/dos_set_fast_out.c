#include "dos.h"


static char _fast_out(uint8_t getset, uint8_t value) __naked
{
  getset, value;
  __asm
    push ix
    ld ix,#4
    add ix,sp

    ld a,0(ix)
    ld b,1(ix)
    pop ix

    ld c,#FOUT
    DOSCALL

    ld h, #0
    ld l, b

    ret
  __endasm;
}

void set_fast_out(uint8_t value)
{
  _fast_out(1, value);
}

char get_fast_out()
{
  return _fast_out(0, 0);
}
