#include "dos.h"
#include "debug.h"


char dosver(void) __naked {
  __asm
    push ix

    ld b,  #0x5A      ; magic numbers to detect Nextor
    ld hl, #0x1234
    ld de, #0xABCD
    ld ix, #0

    ld c,#DOSVER
    call 0xF37D       ; BDOS (upper memory DOSCALL)

    or  a
    jr  z,check_dos1$
    ld  b,#0          ; unknown DOS
    jr  ret_version$

  check_dos1$:
    ld  a,b           ; B<2 if is MSX-DOS 1
    cp  #2
    jr  nc,check_dos2nextor$
    ld  b,#1          ; is MSX-DOS 1
    jr  ret_version$

  check_dos2nextor$:
    push ix           ; Nextor: IXh must contain '1'
    pop hl
    ld  a,h
    dec a
    jr  z,is_nextor$
    ld  b,#2          ; is MSXDOS 2
    jr  ret_version$
  
  is_nextor$:         ; is NextorDOS
    ld  b,#3

  ret_version$:
    ld  h, #0x00
    ld  l, b

    pop ix
    ret
  __endasm;
}

