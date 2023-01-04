#include "dos.h"


static ERR8 _abs_sector_drv(uint8_t drv, uint32_t startsec, uint8_t nsec, uint8_t doscall) __naked __sdcccall(0)
{
	doscall, drv, nsec, startsec;
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld a,0(ix)		// drive
		ld e,1(ix)		// startsec
		ld d,2(ix)
		ld l,3(ix)
		ld h,4(ix)
		ld b,5(ix)		// nsec
		ld c,6(ix)		// doscall
		pop ix

		DOSCALL

		ld l, a
		ret
	__endasm;
}

/*
Read absolute sectors from drive (_RDDRV, 73h)
Parameters:  C = 73H (_RDDRV)
             A = Drive number (0=A: etc.)
             B = Number of sectors to read
             HL:DE = Sector number

Results:     A = Error code (0=> no error)
*/
inline ERR8 read_abs_sector_drv(uint8_t drv, uint32_t startsec, uint8_t nsec)
{
	return _abs_sector_drv(drv, startsec, nsec, RDDRV);
}

/*
Write absolute sectors to drive (_WRDRV, 74h)
Parameters:  C = 74H (_WRDRV)
             A = Drive number (0=A: etc.)
             B = Number of sectors to write
             HL:DE = Sector number

Results:     A = Error code (0=> no error)
*/
inline ERR8 write_abs_sector_drv(uint8_t drv, uint32_t startsec, uint8_t nsec)
{
	return _abs_sector_drv(drv, startsec, nsec, WRDRV);
}
