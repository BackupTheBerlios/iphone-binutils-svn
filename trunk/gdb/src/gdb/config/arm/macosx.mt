# APPLE LOCAL file Darwin
# Target: ARM running Mac OS X

MT_CFLAGS = \
	-DTARGET_ARM \
	-I$(srcdir)/macosx

TDEPFILES = \
    arm-tdep.o \
	core-macho.o \
	remote-kdp.o \
	kdp-udp.o \
	kdp-transactions.o \
	kdp-protocol.o \
	macosx-tdep.o \
	machoread.o \
	symread.o \
	pefread.o

DEPRECATED_TM_FILE = tm-arm-macosx.h

