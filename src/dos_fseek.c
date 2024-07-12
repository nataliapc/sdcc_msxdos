#include "dos.h"


RETDW fseek(uint32_t offset, char origin)
{
	FCB *fcb = (FCB*)SYSFCB;

	if (origin==SEEK_SET) {
		fcb->rndRecord = offset;
	} else
	if (origin==SEEK_CUR) {
		fcb->rndRecord += offset;
	} else
	if (origin==SEEK_END) {
		fcb->rndRecord = fcb->fileSize - offset;
	} else {
		return -1;
	}
	return (int32_t)fcb->rndRecord;
}
