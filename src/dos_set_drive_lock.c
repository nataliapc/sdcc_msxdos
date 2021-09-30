#include "dos.h"


static char _drive_lock(uint8_t drive, uint8_t cmd, uint8_t value) __naked
{
  drive, cmd, value;
  __asm
    push ix
    ld ix,#4
    add ix,sp

    ld e,0(ix)
    ld a,1(ix)
    ld b,2(ix)
    pop ix

    ld c,#LOCK
    DOSCALL

    ld h, #0
    ld l, a

    ret
  __endasm;
}

void set_drive_lock(uint8_t drive, uint8_t value)
{
  _drive_lock(drive, 1, value);
}

char get_drive_lock(uint8_t drive)
{
  return _drive_lock(drive, 0, 0);
}
