# sdcc_msxdos
A simple sdcc MSX library to access MSX-DOS 1, 2 and Nextor BIOS routines.

You need **docker** to compile the library.

---

### MSX-DOS 1.x Functions

	void dos_initializeFCB(void);
	RETB dosVersion(void);
	void exit(void);

	RETB  getCurrentDrive(void);
	char* getProgramPath(char *path);

	bool  fopen(char *filename);
	bool  fcreate(char *filename);
	bool  fclose(void) __sdcccall(1);
	bool  remove(char *filename);
	RETW  fread(char* buf, uint16_t size);
	RETW  fwrite(char* buf, uint16_t size);
	bool  fflush();
	RETW  fputs(char* str);
	char* fgets(char* buf, uint16_t size);
	RETDW fseek(uint32_t offset, uint8_t origin);
	RETDW ftell(void);
	RETDW filesize(char *filename);
	bool  fileexists(char* filename);

	void setTransferAddress(void *memaddress);
	ERRB readAbsoluteSector(uint8_t drive, uint16_t startsec, uint8_t nsec);
	ERRB writeAbsoluteSector(uint8_t drive, uint16_t startsec, uint8_t nsec);

### MSX-DOS 2.x Functions

	ERRB dos2_getDriveParams(char drive, DPARM_info *param);
	ERRB dos2_getCurrentDirectory(char drive, char *path);
	ERRB dos2_parsePathname(char* str, PATH_parsed *info);

	FILEH dos2_fopen(char *filename, char mode);
	FILEH dos2_fcreate(char *filename, char mode, char attributes);
	FILEH dos2_fflush(FILEH fh);
	ERRB  dos2_fclose(FILEH fh);
	ERRB  dos2_remove(char *filename);
	RETW  dos2_fread(char* buf, uint16_t size, FILEH fh);
	RETW  dos2_fwrite(char* buf, uint16_t size, FILEH fh);
	RETW  dos2_fputs(char* str, FILEH fh);
	char* dos2_fgets(char* buf, uint16_t size, FILEH fh);
	RETDW dos2_fseek(FILEH fh, uint32_t offset, char origin);
	RETDW dos2_ftell(FILEH fh);

	ERRB dos2_findfirst(const char *pathname, FFBLK *ffblk, uint8_t attrib);
	ERRB dos2_findnext(FFBLK *ffblk );

	void dos2_setAbortRoutine(void *routine);
	RETW dos2_getScreenSize(void);
	ERRB dos2_getEnv(char* name, char* buffer, uint8_t buffer_size);
	void dos2_explain(uint8_t error_code, char* buffer);
	void dos2_exit(uint8_t code);

### Nextor-DOS Functions

	RETB nxtr_setFastOut(uint8_t value);
	RETB nxtr_getFastOut(void);
	ERRB nxtr_getDriveLetterInfo(char drive, DRIVE_info *info);
	ERRB nxtr_getClusterInfoFAT(char drive, uint16_t clusterNumber, CLUSTER_info *info);
	ERRB nxtr_readAbsoluteSectorDrv(uint8_t drive, uint32_t startsec, uint8_t nsec);
	ERRB nxtr_writeAbsoluteSectorDrv(uint8_t drive, uint32_t startsec, uint8_t nsec);
	ERRB nxtr_set_drive_lock(uint8_t drive, uint8_t value);
	ERRB nxtr_get_drive_lock(uint8_t drive);

### Memory mapper Functions (MSX-DOS 2.x)

	RETB mapperInit(void);
	RETB mapperGetSlot(void);
	RETB mapperGetTotalSegments(void);
	RETB mapperGetFreeSegments(void);
	ERRB mapperAllocateSegment(MAPPER_Segment *returnedData);
	ERRB mapperFreeSegment(MAPPER_Segment *segmentToFree);
	void mapperSetSegment(uint8_t page, MAPPER_Segment *segment);
	RETB mapperGetCurrentSegment(uint8_t page);
	void mapperSetOriginalSegmentBack(uint8_t page);

