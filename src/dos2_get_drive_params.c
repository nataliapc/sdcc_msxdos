#include "dos.h"


ERRB dos2_getDriveParams(char drive, DPARM_info *param) __naked __sdcccall(1)
{
	drive, param;
/*
    GET DISK PARAMETERS (31H)
    Parameters:    C = 31H (_DPARM)
                  DE = Pointer to 32 byte buffer
                   L = Drive number (0=default, 1=A: etc.)

    Results:       A = Error code
                  DE = Preserved

This functions returns a series of parameters relating to the format of the disk 
in the specified drive, to the buffer allocated within the user's program. It is 
useful for programs which are going to do absolute sector reads and writes, in 
order for them to be able to interpret the absolute sector numbers. The 
parameters returned contain some redundant information in order to provide 
parameters which are most useful to transient programs. The format of the 
returned parameter block is:

    DE+0      - Physical drive number (1=A: etc)
    DE+1,2    - Sector size (always 512 currently)
    DE+3      - Sectors per cluster (non-zero power of 2)
    DE+4,5    - Number of reserved sectors (usually 1)
    DE+6      - Number of copies of the FAT (usually 2)
    DE+7,8    - Number of root directory entries
    DE+9,10   - Total number of logical sectors
    DE+11     - Media descriptor byte
    DE+12     - Number of sectors per FAT
    DE+13..14 - First root directory sector number
    DE+15..16 - First data sector number
    DE+17..18 - Maximum cluster number
    DE+19     - Dirty disk flag
    DE+20..23 - Volume id. (-1 => no volume id.)
    DE+24..31 - Reserved (currently always zero)

The dirty disk flag indicates whether in the disk there is a file which can be 
recovered by UNDEL command. It is reset when the file allocation is done.
*/
	__asm
		ld l,a			; L  = Param drive
						; DE = Param param

		ld c,#DPARM
		DOSJP			; Returns A
	__endasm;
}
