#include "dos.h"


// ========================================================
char* dos2_filenameToDOS(char *sourceStr, char *target11) __naked __sdcccall(1)
{
/*
	PARSE FILENAME (5CH)
	Parameters:    C  = 5CH (_PFILE) 
	               DE = ASCIIZ string for parsing
	               HL = Pointer to 11 byte buffer
	Results:       A  = Error (always zero)
	               DE = Pointer to termination character
	               HL = Preserved, buffer filled in
	               B  = Parse flags

	This function is purely a string manipulation function, it will not access 
	disks at all and will not modify the string at all. It is intended mainly 
	to help transient programs in printing out filenames in a formatted way. 
	The ASCIIZ string will be parsed as a single filename item, and the filename 
	will be stored in the user's 11 byte buffer in expanded form, with both the 
	filename and the extension padded out with spaces.

	The parse flags returned in register B are identical to those for the 
	"parse pathname" function above (function 5Bh), except that bits 0, 1 and 2 
	will always be clear. The user's buffer will always be filled in, even if 
	there is no valid filename in the string, in which case the buffer will be 
	filled with spaces. "*" characters will be expanded to the appropriate number 
	of "?"s. If either the filename or the filename extension is too long then 
	the excess characters will be ignored.

	The pointer returned in register DE will point to the first character in the 
	string which was not part of the filename, which may be the null at the end 
	of the string. This character will never be a valid filename character (see 
	the "Command Specification" for details of valid filename characters).
*/
	sourceStr, target11;
	__asm
								; HL = Param sourceStr
								; DE = Param target11
		ex   de,hl
		ld c, #PFILE
		DOSCALL
		ex   de,hl
		ret						; Returns DE = target11
	__endasm;
}