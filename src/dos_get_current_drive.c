#include "dos.h"


char get_current_drive() __naked
{
  __asm
    ld c,#CURDRV
    DOSJP
  __endasm;
}
