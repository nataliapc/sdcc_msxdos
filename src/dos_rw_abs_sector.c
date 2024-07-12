#include "dos.h"


static ERRB _abs_sector(uint8_t drive, uint16_t startsec, uint8_t nsec, uint8_t doscall) __naked __sdcccall(0)
{
	doscall, drive, nsec, startsec;
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld l,0(ix)		; L  = Param drive
		ld e,1(ix)		; DE = Param startsec
		ld d,2(ix)
		ld h,3(ix)		; H  = Param nsec
		ld c,4(ix)		; C  = Param doscall
		pop ix

		DOSCALL

		ld l,a			; Returns L
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
inline ERRB readAbsoluteSector(uint8_t drive, uint16_t startsec, uint8_t nsec) {
	return _abs_sector(drive, startsec, nsec, RDABS);
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
inline ERRB writeAbsoluteSector(uint8_t drive, uint16_t startsec, uint8_t nsec) {
	return _abs_sector(drive, startsec, nsec, WRABS);
}
