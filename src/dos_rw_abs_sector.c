#include "dos.h"


static ERR8 _abs_sector(uint8_t drv, uint16_t startsec, uint8_t nsec, uint8_t doscall) __naked __sdcccall(0)
{
	doscall, drv, nsec, startsec;
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld l,0(ix)    ; drv
		ld e,1(ix)    ; startsec
		ld d,2(ix)
		ld h,3(ix)    ; nsec
		ld c,4(ix)    ; doscall
		pop ix

		DOSCALL

		ld h, #0
		ld l, a
		or a
		ret z
		ld h, #0xff
		ret
	__endasm;
}

/*
    ABSOLUTE SECTOR READ (2FH)
    Parameters:    C = 2FH (_RDABS)
                  DE = Sector number
                   L = Drive number (0=A: etc.)
                   H = Number of sectors to read
    Results:       A = Error code (0=> no error)

This function reads sectors directly from the disk without interpreting them as
files. The disk must be a valid DOS disk in order for the sector number to be
translated into a physical position on the disk. The sectors will be read to
the current disk transfer address. Any disk error will be reported by the
system in the usual way.
*/
inline ERR8 read_abs_sector(uint8_t drv, uint16_t startsec, uint8_t nsec) {
	return _abs_sector(drv, startsec, nsec, RDABS);
}

/*
    ABSOLUTE SECTOR WRITE (30H)
    Parameters:    C = 30H (_WRABS)
                  DE = Sector number
                   L = Drive number (0=A: etc.)
                   H = Number of sectors to write
    Results:       A = Error code

This function writes sectors directly to the disk without interpreting them as
files. The disk must be a valid DOS disk in order for the sector number to be
translated into a physical position on the disk. The sectors will be written
from the current disk transfer address. Any disk errors are reported by the
system in the usual way.
*/
inline ERR8 write_abs_sector(uint8_t drv, uint16_t startsec, uint8_t nsec) {
	return _abs_sector(drv, startsec, nsec, WRABS);
}
