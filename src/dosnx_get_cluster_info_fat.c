#include "dos.h"


ERRB nxtr_getClusterInfoFAT(char drive, uint16_t clusterNumber, CLUSTER_info *info) __naked __sdcccall(0)
{
	drive, info;
/*
    Get information for a cluster on a FAT drive (_GETCLUS, 7Eh)
    Parameters:  C = 7EH (_GETCLUS)
                 A = Drive number (0=default, 1=A: etc.)
                 DE = Cluster number
                 HL = Pointer to a 16 byte buffer
    Results:     A = Error code
*/
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld a,0(ix)		; A  = Param drive
		ld e,1(ix)		; DE = Param clusterNumber
		ld d,2(ix)
		ld l,3(ix)		; HL = Param info
		ld h,4(ix)
		pop ix

		ld c,#GETCLUS
		DOSCALL

		ld l, a			; Returns L
		ret
	__endasm;
}
