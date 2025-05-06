#include "dos.h"


RETB dos2_feof(FILEH fh)
{
	RETW result = dos2_ioctl(fh, IOCTL_FILESTATUS, 0);
	IOCTL_t *ioctl = (IOCTL_t*)&result;

	if (ioctl->error.mark == 0xff) {
		return ioctl->error.code;
	} else {
		return ioctl->file.eof;
	}
}