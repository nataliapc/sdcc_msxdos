#include "dos.h"


/*
Get information for a cluster on a FAT drive (_GETCLUS, 7Eh)
Parameters:  C = 7EH (_GETCLUS)
             A = Drive number (0=default, 1=A: etc.)
             DE = Cluster number
             HL = Pointer to a 16 byte buffer
Results:     A = Error code
*/
ERR8 get_FAT_cluster_info(char drive, uint16_t clusterNumber, CLUSTER_info *info) __naked __sdcccall(0)
{
	drive, info;
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld a,0(ix)		// drive
		ld e,1(ix)		// clusterNumber
		ld d,2(ix)
		ld l,3(ix)		// *info
		ld h,4(ix)
		pop ix

		ld c,#GETCLUS
		DOSCALL

		ld l, a
		ret
	__endasm;
}
