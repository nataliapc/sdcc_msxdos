.PHONY: clean test release

SDCC_SRV := sdcc420
DOCKER_RUN = docker-compose run --rm -u $(shell id -u):$(shell id -g) $(SDCC_SRV)

AS = $(DOCKER_RUN) sdasz80
AR = $(DOCKER_RUN) sdar
CC = $(DOCKER_RUN) sdcc
HEX2BIN = $(DOCKER_RUN) hex2bin
MAKE = make -s --no-print-directory
EMUSCRIPTS = -script ./emulation/boot.tcl

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

INCDIR = include/
SRCDIR = src/
TESTDIR = test/

OBJDIR=out/
ODIR_GUARD=@mkdir -p $(OBJDIR)

LIBDIR = lib/
LIBDIR_GUARD=@mkdir -p $(LIBDIR)

LDFLAGS = -rc
CRT = $(OBJDIR)crt0msx_msxdos_advanced.s.rel

#DEBUG = -DDEBUG
#--allow-undocumented-instructions
DEFINES :=  -DMSXDOS1
FULLOPT :=  --max-allocs-per-node 2000000
CCFLAGS :=  --code-loc 0x0178 --data-loc 0 --no-std-crt0 \
			--out-fmt-ihx --fomit-frame-pointer --opt-code-speed \
			--disable-warning 196 --disable-warning 85 -mz80 $(FULLOPT) $(DEBUG) $(DEFINES) $(EXTERNFLAGS)

LIBS = $(LIBDIR)dos.lib
TEST = test.com

all: $(LIBS)

$(OBJDIR)%.s.rel: */%.s
	@echo "$(COL_WHITE)======== ASM $@$(COL_RESET)"
	$(ODIR_GUARD)
	$(AS) -I$(INCDIR) -o $@ $^

$(OBJDIR)%.c.rel: */%.c
	@echo "$(COL_WHITE)======== CC $@$(COL_RESET)"
	$(ODIR_GUARD)
	$(CC) -I$(INCDIR) $(CCFLAGS) -c -o $@ $^

$(LIBDIR)dos.lib: $(patsubst $(SRCDIR)%, $(OBJDIR)%.rel, $(wildcard $(SRCDIR)dos_*))
	@echo "$(COL_ORANGE)################ Compiling $@$(COL_RESET)"
	$(LIBDIR_GUARD)
	$(AR) $(LDFLAGS) $@ $^

$(TEST): $(LIBS) $(CRT) $(patsubst $(TESTDIR)%, $(OBJDIR), $(wildcard $(TESTDIR)*))
	@echo "$(COL_YELLOW)################ Compiling $@$(COL_RESET)"
	$(CC) $(CCFLAGS) -I$(INCDIR) -L$(INCDIR) -L$(LIBDIR) $(LIBS) $(CRT) $(TESTDIR)$(subst .com,.c,$@) -o $(OBJDIR)$(subst .com,.ihx,$@)
	$(HEX2BIN) -e com $(OBJDIR)$(subst .com,.ihx,$@)
	@mv $(OBJDIR)$(TEST) $(TESTDIR)$(TEST)

test: $(LIBS) $(TEST)
	openmsx -machine msx2_eu -ext msxdos2 -ext debugdevice -diska $(TESTDIR) $(EMUSCRIPTS)

unittest: $(LIBS)
	$(MAKE) -C unitTests SDCC_SRV=$(SDCC_SRV)

clean:
	rm -rf $(LIBDIR) $(OBJDIR)
	rm -f $(TESTDIR)$(TEST)
	$(MAKE) -C unitTests clean
