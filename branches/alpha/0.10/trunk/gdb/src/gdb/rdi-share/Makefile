# Makefile.in generated automatically by automake 1.3b from Makefile.am

# Copyright 1994, 1995, 1996, 1997, 1998, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = ../.././gdb/rdi-share
top_srcdir = ../.././gdb/rdi-share
VPATH = ../.././gdb/rdi-share
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
htmldir = $(prefix)/html
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/rdi-share
pkglibdir = $(libdir)/rdi-share
pkgincludedir = $(includedir)/rdi-share

top_builddir = .

ACLOCAL = ${SHELL} /Users/pcwalton/Projects/iphone-binutils/gdb/src/missing --run aclocal-1.9
AUTOCONF = ${SHELL} /Users/pcwalton/Projects/iphone-binutils/gdb/src/missing --run autoconf
AUTOMAKE = ${SHELL} /Users/pcwalton/Projects/iphone-binutils/gdb/src/missing --run automake-1.9
AUTOHEADER = ${SHELL} /Users/pcwalton/Projects/iphone-binutils/gdb/src/missing --run autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,^,arm-darwin-,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
CC = gcc
EXEEXT = 
MAINT = #
MAKEINFO = makeinfo
PACKAGE = rdi-share
RANLIB = ranlib
VERSION = 1.0

AUTOMAKE_OPTIONS = cygnus

noinst_LIBRARIES = libangsd.a

libangsd_a_SOURCES = ardi.c angel_bytesex.c crc.c devsw.c drivers.c etherdrv.c \
                     hostchan.c hsys.c logging.c msgbuild.c params.c rx.c \
                     serdrv.c serpardr.c tx.c unixcomm.c

noinst_HEADERS = adp.h adperr.h angel.h ardi.h armdbg.h buffers.h \
                 chandefs.h channels.h chanpriv.h crc.h dbg_conf.h dbg_cp.h \
                 dbg_hif.h dbg_rdi.h devclnt.h devices.h devsw.h drivers.h \
                 angel_endian.h ethernet.h host.h hostchan.h hsys.h logging.h \
                 msgbuild.h params.h rxtx.h sys.h unixcomm.h angel_bytesex.h

EXTRA_DIST = README.CYGNUS

INCLUDES =  -DRETRANS -DARM_RELEASE='"Berkeley Licence for Cygnus"'
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/../../mkinstalldirs
CONFIG_CLEAN_FILES = 
LIBRARIES =  $(noinst_LIBRARIES)


DEFS = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE=\"rdi-share\" -DVERSION=\"1.0\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_FCNTL_H=1 -DHAVE_SYS_IOCTL_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_UNISTD_H=1 -DHAVE_SYS_FILIO_H=1 -DTIME_WITH_SYS_TIME=1 -DRETSIGTYPE=void -DHAVE_VPRINTF=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_SELECT=1 -DHAVE_SOCKET=1 -DHAVE_STRTOD=1 -DHAVE_STRTOUL=1 -DHAVE_MEMCPY=1 -DHAVE_MEMMOVE=1  -I. -I$(srcdir) 
CPPFLAGS = 
LDFLAGS = 
LIBS = 
libangsd_a_LIBADD = 
libangsd_a_OBJECTS =  ardi.o angel_bytesex.o crc.o devsw.o drivers.o \
etherdrv.o hostchan.o hsys.o logging.o msgbuild.o params.o rx.o \
serdrv.o serpardr.o tx.o unixcomm.o
AR = ar
CFLAGS = -g -O2 
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LINK = $(CC) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  Makefile.am Makefile.in aclocal.m4 configure configure.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP = --best
SOURCES = $(libangsd_a_SOURCES)
OBJECTS = $(libangsd_a_OBJECTS)

all: Makefile $(LIBRARIES) $(HEADERS)

.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: #Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4)
	cd $(top_srcdir) && $(AUTOMAKE) --cygnus Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(ACLOCAL_M4): # configure.in 
	cd $(srcdir) && $(ACLOCAL)

config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: #$(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

mostlyclean-noinstLIBRARIES:

clean-noinstLIBRARIES:
	-test -z "$(noinst_LIBRARIES)" || rm -f $(noinst_LIBRARIES)

distclean-noinstLIBRARIES:

maintainer-clean-noinstLIBRARIES:

.c.o:
	$(COMPILE) -c $<

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

libangsd.a: $(libangsd_a_OBJECTS) $(libangsd_a_DEPENDENCIES)
	-rm -f libangsd.a
	$(AR) cru libangsd.a $(libangsd_a_OBJECTS) $(libangsd_a_LIBADD)
	$(RANLIB) libangsd.a

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $(SOURCES) $(HEADERS) $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags  $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-rm -rf $(distdir)
	GZIP=$(GZIP) $(TAR) zxf $(distdir).tar.gz
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	dc_install_base=`cd $(distdir)/=inst && pwd`; \
	cd $(distdir)/=build \
	  && ../configure --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) dist
	-rm -rf $(distdir)
	@echo "========================"; \
	echo "$(distdir).tar.gz is ready for distribution"; \
	echo "========================"
dist: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
dist-all: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
distdir: $(DISTFILES)
	-rm -rf $(distdir)
	mkdir $(distdir)
	-chmod 777 $(distdir)
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  test -f $(distdir)/$$file \
	  || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	  || cp -p $$d/$$file $(distdir)/$$file; \
	done
info:
dvi:
check:
html:
install-html:
installcheck:
install-info:
install-exec: 
	@$(NORMAL_INSTALL)

install-data: 
	@$(NORMAL_INSTALL)

install: install-exec install-data all
	@:

uninstall: 

install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' INSTALL_SCRIPT='$(INSTALL_PROGRAM)' install
installdirs:


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean:  mostlyclean-noinstLIBRARIES mostlyclean-compile \
		mostlyclean-tags mostlyclean-generic

clean:  clean-noinstLIBRARIES clean-compile clean-tags clean-generic \
		mostlyclean

distclean:  distclean-noinstLIBRARIES distclean-compile distclean-tags \
		distclean-generic clean
	-rm -f config.status

maintainer-clean:  maintainer-clean-noinstLIBRARIES \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-generic distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	-rm -f config.status

.PHONY: mostlyclean-noinstLIBRARIES distclean-noinstLIBRARIES \
clean-noinstLIBRARIES maintainer-clean-noinstLIBRARIES \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile tags mostlyclean-tags distclean-tags \
clean-tags maintainer-clean-tags distdir info dvi installcheck \
install-info install-exec install-data install uninstall all \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
