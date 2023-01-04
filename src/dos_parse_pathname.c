#include "dos.h"


#ifdef MSXDOS2
/*
    PARSE PATHNAME (5BH)
    Parameters:    C = 5BH (_PARSE) 
                   B = Volume name flag (bit 4)
                  DE = ASCIIZ string for parsing
    Results:       A = Error
                  DE = Pointer to termination character
                  HL = Pointer to start of last item
                   B = Parse flags
                   C = Logical drive number (1=A: etc)

This function is purely a string manipulation function, it will not access the
disks at all and it will not modify the user's string at all. It is intended to
help transient programs in parsing command lines.

The volume name flag (bit 4 of register B; it is in the same bit position as
the volume name bit in an attributes byte) determines whether the string will
be parsed as a "drive/path/file" string (if the bit is cleared) or a
"drive/volume" string (if the bit is set).

The pointer returned in DE will point to the first character which is not valid
in a pathname string, and may be the null at the end of the string. See the
"Command Specification" for details of the syntax of pathname strings and also
for a list of valid characters.

The pointer returned in HL will point to the first character of the last item
of a string (filename portion). For example, when a string "A:\XYZ\P.Q /F" was
passed, DE will point to the white space character before "/F" and HL will
point to "P". If the parsed string ends with a character "\" or is null (apart
from drive name), then there will be no "last item", thus HL and DE will point
to the same character. In this case, some special procedures will be needed to
all the programs which use this function.

The drive number returned in register C is the logical drive specified in the
string. If the string did not start with a drive letter then register C will
contain the default drive number, since the default drive has been implicitly
specified. Register C will never be zero.

The parse flags returned in register B indicate various useful things about the
string. For a volume name bits 1, 4, 5, 6 and 7 will always be clear. For a
filename, bits 3 to 7 relate to the last item on the string (the "filename"
component). The bit assignments are as follows:

    b0 - set if any characters parsed other than drive name
    b1 - set if any directory path specified
    b2 - set if drive name specified
    b3 - set if main filename specified in last item
    b4 - set if filename extension specified in last item
    b5 - set if last item is ambiguous
    b6 - set if last item is "." or ".."
    b7 - set if last item is ".."
*/
#define STRUCT_DRIVE		0
#define STRUCT_LASTITEM		1
#define STRUCT_TERMCHAR		3
#define STRUCT_FLAGS		5

ERR8 parse_pathname(char* str, PATH_parsed *info) __naked __sdcccall(0)
{
	str, info;
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld e,0(ix)				; str pointer
		ld d,1(ix)

		ld a,2(ix)				; info str
		.db 0xfd,0x68+7				;ld iyl,a [FD 68+q]

		ld a,3(ix)
		.db 0xfd,0x60+7				;ld iyh,a [FD 60+q]
		pop ix

		ld b,#0
		ld c,#PARSE
		DOSCALL

		or a
		jr z, parse_noerrors$
		ld l, a
		ret

	parse_noerrors$:
		ld STRUCT_DRIVE(iy),c
		ld STRUCT_FLAGS(iy), b
		ld STRUCT_TERMCHAR+0(iy), e
		ld STRUCT_TERMCHAR+1(iy), d
		ld STRUCT_LASTITEM+0(iy), l
		ld STRUCT_LASTITEM+1(iy), h

		ld l, #0
		ret
	__endasm;
}
#endif

#ifdef MSXDOS1
ERR8 parse_pathname(char* str, PATH_parsed *info)
{
	//TODO
	cputs("parse_pathname(): Not implemented!");
	exit0();
	return 0;
}
#endif