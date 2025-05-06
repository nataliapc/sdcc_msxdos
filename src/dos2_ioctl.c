#include <stdint.h>
#include "dos.h"


RETW dos2_ioctl(uint8_t fh, uint8_t cmd, IOCTL_t *arg) __naked __sdcccall(0)
{
/*
    CONTROL FOR DEVICES (4BH)
    Parameters:    C = 4BH (_IOCTL) 
                   B = File handle
                   A = Sub-function code
                       00H => get file handle status
                       01H => set ASCII/binary mode
                       02H => test input ready
                       03H => test output ready
                       04H => find screen size
                  DE = Other parameters
    Results:       A = Error
                  DE = Other results

This function allows various aspects of file handles to be examined and altered.
In particular it can be used to determine whether a file handle refers to a disk
file or a device. This is useful for programs which want to behave differently
for disk files and device I/O.

This function is passed the file handle in register B and a sub-function code in
register A which specifies one of various different operations. Any other
parameters required by the particular sub-function are passed in register DE and
results are returned in register DE. If the sub-function code is invalid then a
".ISBFN" error will be returned.

If A=0 then the operation is "get file handle status". This returns a word of
flags which give various information about the file handle. The format of this
word is different for device file handles and disk file handles, and bit-7
specifies which it is. The format of the word is as follows:

For devices:    DE -     b0  set  => console input device
						 b1  set  => console output device
					 b2..b4  reserved
						 b5  set  => ASCII mode
							 clear=> binary mode
						 b6  set  => end of file
						 b7  always set (=> device)
					b8..b15  reserved

For disk files: DE - b0..b5  drive number (0=A: etc)
						 b6  set  => end of file
						 b7  always clear (=> disk file)
					b8..b15  reserved

Note that the end of file flag is the same for devices as for disk files. For
devices it will be set if the previous attempt to read from the device produced
a ".EOF" error and will be cleared by the next read. For disk files it is worked
out by comparing the file pointer with the file size.

If A=1 then the operation is a "set ASCII/binary mode". This operation is only
allowed for device file handles. An ASCII/binary flag must be passed in bit-5 of
register E (exactly where it is returned by "get file handle status"). This is
set for ASCII mode and clear for binary mode. All other bits of register DE are
ignored.

If A=2 or 3 then the operation is "test input ready" or "test output ready"
respectively. In both cases a flag is returned in register E which is FFh if the
file handle is ready for a character and 00h if not. The exact meaning of "ready
for a character" depends on the device. Disk file handles are always ready for
output, and are always ready for input unless the file pointer is at the end of
file. The "CON" device checks the keyboard status to determine whether it is
ready for input or not.

If A=4 the the operation is "get screen size". This returns the logical screen
size for the file handle with the number of rows in register D and the number of
columns in register E. For devices with no screen size (such as disk files) both
D and E will be zero. Zero for either result should therefore be interpreted as
"unlimited". For example this function is used by the "DIR /W" command to decide
how many files to print per line, and a value of zero for register E is
defaulted to 80.
*/
	__asm
		push ix
		ld   ix,#4
		add  ix,sp
		ld   b,0(ix)			; B  = Param fh
		ld   a,1(ix)			; A  = Param cmd
		ld   e,2(ix)			; DE = Param arg
		ld   d,3(ix)
		pop  ix

		ld   c, #IOCTL			; MSX-DOS2 function: IOCTL
		DOSCALL

		ex   de,hl				; Returns HL
		or   a
		ret  z					; no error = DE (result)
		ld   l,a
		ld   h,#0xff
		ret						; error = 0xff00 | A (error code)
	__endasm;
}