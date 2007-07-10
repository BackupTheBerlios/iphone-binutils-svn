VPATH = ../../.././gdb/testsuite/gdb.objc
srcdir = ../../.././gdb/testsuite/gdb.objc

EXECUTABLES = myclass

all: 
	@echo "Nothing to be done for all..."

info:
install-info:
dvi:
install:
uninstall: force
installcheck:
check:

clean mostlyclean:
	-rm -f *~ *.o *.ci
	-rm -f core ${EXECUTABLES}

distclean maintainer-clean realclean: clean
	-rm -f Makefile config.status config.log

Makefile: $(srcdir)/Makefile.in $(srcdir)/configure.in
	$(SHELL) ./config.status --recheck
