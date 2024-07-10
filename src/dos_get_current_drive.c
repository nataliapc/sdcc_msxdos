#include "dos.h"


char get_current_drive(void) __naked
{
  __asm
    ld c,#CURDRV
    DOSJP
  __endasm;
}
