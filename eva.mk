# ------------------------------------------------------------------------------
# Configurations.
# ------------------------------------------------------------------------------

# FMT_FOLDERS   =  <user_provide>

BUILD_BASE      = .build
BUILD           = ${BUILD_BASE}
BUILD_RELEASE   = ${BUILD_BASE}_release
UNAME           = $(shell uname)

CFLAGS          += -std=c99 -Wall -Werror -pedantic -Wno-c11-extensions
LDFLAGS         += -lm

CLANG_EXTS      = -iname *.h -o -iname *.c
CLANG_FMT       = clang-format -i --style=file
FMT             = sh -c 'find "$$@" ${CLANG_EXTS} | xargs ${CLANG_FMT}' sh

MK              = make

# enable POSIX and LLD
ifeq ($(UNAME), Linux)
        CFLAGS  += -D_POSIX_C_SOURCE=201410L
	LDFLAGS += -fuse-ld=lld
endif

# enable asan by `make ASAN=1`
#
# note memory leak is enabled by default on linux but needs ASAN_OPTIONS for
# macOS.
#
# https://clang.llvm.org/docs/AddressSanitizer.html
ifdef ASAN
	CFLAGS  += -g -fsanitize=address -D_ASAN=1
	BUILD   := ${BUILD}_asan
ifeq ($(UNAME), Darwin)
	EX      = ASAN_OPTIONS=detect_leaks=1
endif
endif

# enable release by `make RELEASE=1`
ifdef RELEASE
        CFLAGS  += -DNDEBUG -O2
        BUILD   := ${BUILD}_release

compile: check_release_folder
endif

ifeq ($(UNAME), FreeBSD)
        MK      = gmake
endif

# ------------------------------------------------------------------------------
# Color printing.
# ------------------------------------------------------------------------------
EVA_CC          = ${QUIET_CC}${CC} ${CFLAGS}
EVA_LD          = ${QUIET_LD}${CC} ${LDFLAGS} ${CFLAGS}
EVA_AR          = ${QUIET_AR}ar -cr
EVA_EX          = ${QUIET_EX}${EX}
EVA_FM          = ${QUIET_FM}${FMT}

CCCOLOR         = "\033[34m"
LINKCOLOR       = "\033[34;1m"
SRCCOLOR        = "\033[33m"
BINCOLOR        = "\033[36;1m"
ENDCOLOR        = "\033[0m"

# enable verbose cmd by `make V=1`
ifndef V
QUIET_CC  = @printf '    %b %b\n' $(CCCOLOR)CC$(ENDCOLOR) \
				  $(SRCCOLOR)`basename $@`$(ENDCOLOR) 1>&2;
QUIET_LD  = @printf '    %b %b\n' $(LINKCOLOR)LD$(ENDCOLOR) \
				  $(BINCOLOR)`basename $@`$(ENDCOLOR) 1>&2;
QUIET_AR  = @printf '    %b %b\n' $(LINKCOLOR)AR$(ENDCOLOR) \
				  $(BINCOLOR)`basename $@`$(ENDCOLOR) 1>&2;
QUIET_EX  = @printf '    %b %b\n' $(LINKCOLOR)EX$(ENDCOLOR) \
				  $(BINCOLOR)$@$(ENDCOLOR) 1>&2;
QUIET_FM  = @printf '    %b %b\n' $(LINKCOLOR)FM$(ENDCOLOR) \
	                          $(BINCOLOR)"$(FMT_FOLDERS)"$(ENDCOLOR) 1>&2;
endif

# ------------------------------------------------------------------------------
# Common actions.
# ------------------------------------------------------------------------------

${BUILD}:
	@mkdir -p ${BUILD}

clean:
	${EVA_EX} rm -rf ${BUILD_BASE}*

fmt:
	${EVA_FM} ${FMT_FOLDERS}

check_release_folder:
ifneq (${BUILD}, ${BUILD_RELEASE})
	@echo "release mode cannot mix with other modes, e.g., asan."
	@exit 1
endif

# ------------------------------------------------------------------------------
# Template to generate CC binary rules.
# ------------------------------------------------------------------------------

# The convention is for cmd <binary> the main file is cmd/<binary>/main.c.
# $(2) is output dir, $(1) is binary name, and $(3) is library dep.
define objs

.PHONY: $(1)

$(1): compile $(2)/$(1)
	$$(EVA_EX) $(2)/$(1)

$(2)/$(1): cmd/$(1)/main.c $(3)
	$$(EVA_LD) -o $$@ $$^

endef
