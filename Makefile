.PHONY: clean test release

SDCC_SRV := sdcc410
DOCKER_RUN = docker-compose run --rm -u $(shell id -u):$(shell id -g) $(SDCC_SRV)

AS = $(DOCKER_RUN) sdasz80
AR = $(DOCKER_RUN) sdar
CC = $(DOCKER_RUN) sdcc
HEX2BIN = $(DOCKER_RUN) hex2bin
EMUSCRIPTS = -script ./emulation/boot.tcl

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
DEFINES := -D$(shell echo '$(SDCC_SRV)' | tr '[:lower:]' '[:upper:]')
CCFLAGS :=  --code-loc 0x0178 --data-loc 0 --no-std-crt0 \
			--out-fmt-ihx --fomit-frame-pointer --opt-code-speed\
			--disable-warning 196 --disable-warning 85 -mz80 $(DEBUG) $(DEFINES) $(EXTERNFLAGS)

LIBS = $(LIBDIR)dos.lib
TEST = test.com

all: $(LIBS)

$(OBJDIR)%.s.rel: */%.s
	@echo "======== ASM $@"
	$(ODIR_GUARD)
	$(AS) -I$(INCDIR) -o $@ $^

$(OBJDIR)%.c.rel: */%.c
	@echo "======== CC $@"
	$(ODIR_GUARD)
	$(CC) -I$(INCDIR) $(CCFLAGS) -c -o $@ $^

$(LIBDIR)dos.lib: $(patsubst $(SRCDIR)%, $(OBJDIR)%.rel, $(wildcard $(SRCDIR)dos_*))
	@echo "################ Compiling $@"
	$(LIBDIR_GUARD)
	$(AR) $(LDFLAGS) $@ $^

$(TEST): $(LIBS) $(CRT) $(patsubst $(TESTDIR)%, $(OBJDIR), $(wildcard $(TESTDIR)*))
	@echo "################ Compiling $@"
	$(CC) $(CCFLAGS) -I$(INCDIR) -L$(INCDIR) -L$(LIBDIR) $(LIBS) $(CRT) $(TESTDIR)$(subst .com,.c,$@) -o $(OBJDIR)$(subst .com,.ihx,$@)
	$(HEX2BIN) -e com $(OBJDIR)$(subst .com,.ihx,$@)
	@mv $(OBJDIR)$(TEST) $(TESTDIR)$(TEST)

run: $(LIBS) $(TEST)
	openmsx -machine msx2_eu -ext ide-nextor -ext debugdevice -diska $(TESTDIR) $(EMUSCRIPTS)

clean:
	@rm -f $(TESTDIR)$(TEST)
	@rm -rf $(LIBDIR) $(OBJDIR)
