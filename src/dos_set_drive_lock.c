#include "dos.h"


/*
Lock/unlock a drive, or get lock state for a drive (_LOCK, 77h)
Parameters:  C = 77H (_LOCK)
             E = physical drive (0=A:, 1=B:, etc)
             A =  00H => get lock status
                  01H => set lock status
             B =  00H => unlock drive (only if A=01H)
                  FFH => lock drive (only if A=01H)

Results:     A = Error code
             B = Current lock status, same as input
*/
static uint16_t _drive_lock(uint8_t drive, uint8_t cmd, uint8_t value) __naked __sdcccall(0)
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
		ld l, b
		or a
		ret z
		dec h
		ld l, a
		ret
	__endasm;
}

inline RE16 set_drive_lock(uint8_t drive, uint8_t value)
{
	return _drive_lock(drive, 1, value);
}

inline RE16 get_drive_lock(uint8_t drive)
{
	return _drive_lock(drive, 0, 0);
}
