.PHONY: default all lib clean test legacy

SDCC_VER := 4.2.0
DOCKER_IMG = nataliapc/sdcc:$(SDCC_VER)
DOCKER_RUN = docker run -i --rm -u $(shell id -u):$(shell id -g) -v .:/src -w /src $(DOCKER_IMG)

AS = $(DOCKER_RUN) sdasz80
AR = $(DOCKER_RUN) sdar
CC = $(DOCKER_RUN) sdcc
HEX2BIN = hex2bin

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	COL_RED = \e[1;31m
	COL_YELLOW = \e[1;33m
	COL_ORANGE = \e[1;38:5:208m
	COL_BLUE = \e[1;34m
	COL_GRAY = \e[1;30m
	COL_WHITE = \e[1;37m
	COL_RESET = \e[0m
endif

CXXFLAGS := -DUNIT_TESTS
#FULLOPT := --max-allocs-per-node 2000000
OPFLAGS := --opt-code-size $(FULLOPT)
WRFLAGS := --less-pedantic --disable-warning 196 --disable-warning 84 --disable-warning 85
CCFLAGS := --code-loc 0x0180 --data-loc 0 -mz80 --no-std-crt0 --out-fmt-ihx $(OPFLAGS) $(WRFLAGS) $(CXXFLAGS)

SRCDIR = ./src
LIBDIR = ./lib
OBJDIR = ./obj
DSKDIR = ./dsk
DIR_GUARD=@mkdir -p $(OBJDIR)
LIB_GUARD=@mkdir -p $(LIBDIR)

EMUSCRIPTS = -script ../emulation/boot.tcl

LIBS := dos.lib
REL_LIBS := $(addprefix $(LIBDIR)/, $(LIBS)) \
			$(addprefix $(OBJDIR)/, crt0msx_msxdos_advanced.rel \
			utils_debug.rel heap.rel assert.rel)

PROGRAMS = $(OBJDIR)/testdos1.com

default: all

all: $(PROGRAMS)

libs: $(LIBDIR)/$(LIBS) $(LIBDIR)/dos.h

$(LIBDIR)/$(LIBS): ../lib/$(LIBS)
	@echo "$(COL_ORANGE)#### LIB $@$(COL_RESET)"
	@$(LIB_GUARD)
	cp $^ $(LIBDIR)
	cp ../include/dos.h $(LIBDIR)

$(OBJDIR)/%.rel: $(SRCDIR)/%.s
	@echo "$(COL_WHITE)#### ASM $@$(COL_RESET)"
	@$(DIR_GUARD)
	$(AS) -g -o $@ $^ ;

$(OBJDIR)/%.rel: $(SRCDIR)/%.c
	@echo "$(COL_WHITE)#### CC $@$(COL_RESET)"
	@$(DIR_GUARD)
	$(CC) $(CCFLAGS) -I$(LIBDIR) -c -o $@ $^ ;

$(OBJDIR)/testdos1.com: $(REL_LIBS) $(SRCDIR)/tests_msxdos1.c
	@echo "$(COL_YELLOW)######## Compiling $@$(COL_RESET)"
	@$(DIR_GUARD)
	$(CC) $(CCFLAGS) -I$(LIBDIR) -L$(LIBDIR) $(subst .com,.c,$^) -o $(subst .com,.ihx,$@) ;
	@$(HEX2BIN) -e com $(subst .com,.ihx,$@) ;
	@echo "**** Copying .COM files to DSK/"
	@cp $@ $(DSKDIR)

clean:
	@echo "$(COL_ORANGE)#### CLEAN$(COL_RESET)"
	rm -v -f $(subst $(OBJDIR)/,$(DSKDIR)/,$(PROGRAMS))
	rm -f *.com *.asm *.lst *.sym *.bin *.ihx *.lk *.map *.noi *.rel
	rm -rf $(OBJDIR) $(LIBDIR)

test: all
	@bash -c 'if pgrep -x "openmsx" > /dev/null \
	; then \
		echo "**** openmsx already running..." \
	; else \
#		openmsx -machine msx1_eu -ext debugdevice -ext Mitsubishi_ML-30DC_ML-30FD -diska dsk/ $(EMUSCRIPTS) \
#		openmsx -machine Philips_NMS_8245 -ext debugdevice -diska dsk/ $(EMUSCRIPTS) \
#		openmsx -machine msx2plus -ext debugdevice -ext ram512k -diska dsk/ $(EMUSCRIPTS) \
#		openmsx -machine turbor -ext debugdevice -diska dsk/ $(EMUSCRIPTS) \
		openmsx -machine Sony_HB-F1XD -ext debugdevice -diska dsk/ $(EMUSCRIPTS) \
	; fi'

legacy:
	cd legacy ; ./compile.sh
	cd legacy ; dosbox run.bat
