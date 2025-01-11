#include "dos.h"


// ========================================================
char dos2_toupper(char c) __naked __sdcccall(1)
{
/*
CHECK CHARACTER (5DH)
     Parameters:    C = 5DH (_CHKCHR) 
                    D = Character flags
                    E = Character to be checked
     Results:       A = 0 (never returns an error)
                    D = Updated character flags
                    E = Checked (upper cased) character
This function allow language independent upper casing of characters and also helps with handling 16-bit characters and manipulation of filenames. The bit assignments in the character flags are as follows:

   b0   - set to suppress upper casing
   b1   - set if first byte of 16-bit character
   b2   - set if second byte of 16-bit character
   b3   - set => volume name (rather than filename)
   b4   - set => not a valid file/volume name character
b5...b7 - reserved (always clear)
Bit 0 is used to control upper casing. If it is clear then the character will be upper cased according to the language setting of the machine. If this bit is set then the returned character will always be the same as the character passed.

The two 16-bit character flags (bits 1 and 2) can both be clear when the first character of a string is checked and the settings returned can be passed straight back to this function for each subsequent character. Care must be taken with these flags when moving backwards through strings which may contain 16-bit characters.

Bit 4 is set on return if the character is one of the set of filename or volume name terminator characters. Bit 3 is simply used to determine whether to test for filename or volume name characters since the sets are different. 16-bit characters (either byte) are never considered as volume or filename terminators.
*/
	c;
	__asm
								; A = Param c
		ld   e, a
		ld   d, #0
		ld   c, #CHKCHR
		DOSCALL
		ld   a, e
		ret						; Returns A = upper cased character
	__endasm;
}