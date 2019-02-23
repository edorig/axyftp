# Makefile.in generated automatically by automake 1.4-p6 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/bash

srcdir = .
top_srcdir = ..

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
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/XmAxy
pkglibdir = $(libdir)/XmAxy
pkgincludedir = $(includedir)/XmAxy

top_builddir = ..

ACLOCAL = /home/edmond/software/axyftp-0.5.1/xmaxy/missing aclocal-1.4
AUTOCONF = /home/edmond/software/axyftp-0.5.1/xmaxy/missing autoconf
AUTOMAKE = /home/edmond/software/axyftp-0.5.1/xmaxy/missing automake-1.4
AUTOHEADER = /home/edmond/software/axyftp-0.5.1/xmaxy/missing autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
AXY_H = $(EXTRA_HEADRES)
AXY_L = $(EXTRA_LIBRARIES)
CC = gcc
CPP = gcc -E
DEMO_PROG = demo
DT_CFLAGS = @DT_CFLAGS@
DT_LIBS = @DT_LIBS@
INSTALL_AXY_H = 
INSTALL_AXY_L = 
MAKEINFO = /home/edmond/software/axyftp-0.5.1/xmaxy/missing makeinfo
PACKAGE = XmAxy
RANLIB = ranlib
VERSION = 0.0.1
XMAXY_CFLAGS = @XMAXY_CFLAGS@
XMAXY_LIBS = @XMAXY_LIBS@
axy_doc = 

AUTOMAKE_OPTIONS = foreign

CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in

AXY_DIRINC = $(includedir)/XmAxy
xmaxydir = 

EXTRA_DIST = Notebook.h List.h NotebookP.h ListP.h version.h

EXTRA_HEADERS = Notebook.h List.h NotebookP.h ListP.h version.h
EXTRA_LIBRARIES = libXmAxy.a

xmaxy_HEADERS = 
lib_LIBRARIES = 

noinst_HEADERS = $(EXTRA_HEADRES)
noinst_LIBRARIES = $(EXTRA_LIBRARIES)

noinst_PROGRAMS = demo
EXTRA_PROGRAMS = demo

MOTIF_CFLAGS = 
MOTIF_LIBS = -lXm
XPM_CFLAGS = 
XPM_LIBS = -lXpm

INCLUDES = -I$(srcdir)/.. $(MOTIF_CFLAGS)  $(XPM_CFLAGS) 

libXmAxy_a_SOURCES = Notebook.c List.c Util.c Util.h
libXmAxy_a_LIBADD = Notebook.o List.o Util.o

demo_SOURCES = demo.c link.xpm
demo_LDADD = $(top_builddir)/XmAxy/libXmAxy.a 	$(MOTIF_LIBS)   -lXt ${X_PRE_LIBS} -lX11 	${X_EXTRA_LIBS} $(XPM_LIBS) 

mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_CLEAN_FILES = 
LIBRARIES =  $(lib_LIBRARIES) $(noinst_LIBRARIES)


DEFS = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE=\"XmAxy\" -DVERSION=\"0.0.1\" -DXTHREADS=1  -I. -I$(srcdir) 
CPPFLAGS = 
LDFLAGS = 
LIBS = 
X_CFLAGS = 
X_LIBS = 
X_EXTRA_LIBS = 
X_PRE_LIBS =  -lSM -lICE
libXmAxy_a_DEPENDENCIES =  Notebook.o List.o Util.o
libXmAxy_a_OBJECTS =  Notebook.o List.o Util.o
AR = ar
PROGRAMS =  $(noinst_PROGRAMS)

demo_OBJECTS =  demo.o
demo_DEPENDENCIES =  $(top_builddir)/XmAxy/libXmAxy.a
demo_LDFLAGS = 
CFLAGS = -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
HEADERS =  $(noinst_HEADERS) $(xmaxy_HEADERS)

DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
SOURCES = $(libXmAxy_a_SOURCES) $(demo_SOURCES)
OBJECTS = $(libXmAxy_a_OBJECTS) $(demo_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --foreign --include-deps XmAxy/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-libLIBRARIES:

clean-libLIBRARIES:
	-test -z "$(lib_LIBRARIES)" || rm -f $(lib_LIBRARIES)

distclean-libLIBRARIES:

maintainer-clean-libLIBRARIES:

install-libLIBRARIES: $(lib_LIBRARIES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libdir)
	@list='$(lib_LIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(libdir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(libdir)/$$p; \
	  else :; fi; \
	done
	@$(POST_INSTALL)
	@list='$(lib_LIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(RANLIB) $(DESTDIR)$(libdir)/$$p"; \
	    $(RANLIB) $(DESTDIR)$(libdir)/$$p; \
	  else :; fi; \
	done

uninstall-libLIBRARIES:
	@$(NORMAL_UNINSTALL)
	list='$(lib_LIBRARIES)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(libdir)/$$p; \
	done

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

libXmAxy.a: $(libXmAxy_a_OBJECTS) $(libXmAxy_a_DEPENDENCIES)
	-rm -f libXmAxy.a
	$(AR) cru libXmAxy.a $(libXmAxy_a_OBJECTS) $(libXmAxy_a_LIBADD)
	$(RANLIB) libXmAxy.a

mostlyclean-noinstPROGRAMS:

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)

distclean-noinstPROGRAMS:

maintainer-clean-noinstPROGRAMS:

demo: $(demo_OBJECTS) $(demo_DEPENDENCIES)
	@rm -f demo
	$(LINK) $(demo_LDFLAGS) $(demo_OBJECTS) $(demo_LDADD) $(LIBS)

install-xmaxyHEADERS: $(xmaxy_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(xmaxydir)
	@list='$(xmaxy_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(xmaxydir)/$$p"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(xmaxydir)/$$p; \
	done

uninstall-xmaxyHEADERS:
	@$(NORMAL_UNINSTALL)
	list='$(xmaxy_HEADERS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(xmaxydir)/$$p; \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags -o $$here/TAGS $(ETAGS_ARGS) $$tags  $$unique $(LISP))

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = XmAxy

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done

info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am: install-libLIBRARIES
install-exec: install-exec-am

install-data-am: install-xmaxyHEADERS
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-libLIBRARIES uninstall-xmaxyHEADERS
uninstall: uninstall-am
all-am: Makefile $(LIBRARIES) $(PROGRAMS) $(HEADERS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(libdir) $(DESTDIR)$(xmaxydir)


mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	-test -z "$(MAINTAINERCLEANFILES)" || rm -f $(MAINTAINERCLEANFILES)
mostlyclean-am:  mostlyclean-libLIBRARIES mostlyclean-noinstLIBRARIES \
		mostlyclean-compile mostlyclean-noinstPROGRAMS \
		mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-libLIBRARIES clean-noinstLIBRARIES clean-compile \
		clean-noinstPROGRAMS clean-tags clean-generic \
		mostlyclean-am

clean: clean-am

distclean-am:  distclean-libLIBRARIES distclean-noinstLIBRARIES \
		distclean-compile distclean-noinstPROGRAMS \
		distclean-tags distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-libLIBRARIES \
		maintainer-clean-noinstLIBRARIES \
		maintainer-clean-compile \
		maintainer-clean-noinstPROGRAMS maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-libLIBRARIES distclean-libLIBRARIES \
clean-libLIBRARIES maintainer-clean-libLIBRARIES uninstall-libLIBRARIES \
install-libLIBRARIES mostlyclean-noinstLIBRARIES \
distclean-noinstLIBRARIES clean-noinstLIBRARIES \
maintainer-clean-noinstLIBRARIES mostlyclean-compile distclean-compile \
clean-compile maintainer-clean-compile mostlyclean-noinstPROGRAMS \
distclean-noinstPROGRAMS clean-noinstPROGRAMS \
maintainer-clean-noinstPROGRAMS uninstall-xmaxyHEADERS \
install-xmaxyHEADERS tags mostlyclean-tags distclean-tags clean-tags \
maintainer-clean-tags distdir info-am info dvi-am dvi check check-am \
installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
