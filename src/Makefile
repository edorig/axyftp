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

pkgdatadir = $(datadir)/axyftp
pkglibdir = $(libdir)/axyftp
pkgincludedir = $(includedir)/axyftp

top_builddir = ..

ACLOCAL = /home/edmond/software/axyftp-0.5.1/missing aclocal-1.4
AUTOCONF = /home/edmond/software/axyftp-0.5.1/missing autoconf
AUTOMAKE = /home/edmond/software/axyftp-0.5.1/missing automake-1.4
AUTOHEADER = /home/edmond/software/axyftp-0.5.1/missing autoheader

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
AXYFTP_DEPS =  $(top_builddir)/xmaxy/XmAxy/libXmAxy.a
AXYFTP_INCLUDES = $(XMAXY_CFLAGS) $(DT_CFLAGS) $(MOTIF_CFLAGS)                     $(XPM_CFLAGS)
AXYFTP_LIBS = $(XMAXY_LIBS) $(DT_LIBS) $(MOTIF_LIBS)                 -lXt  -lSM -lICE -lX11  $(XPM_LIBS)
CC = gcc
CPP = gcc -E
DT_CFLAGS = -I/usr/dt/include
GTK_CFLAGS = 
GTK_CONFIG = 
GTK_LIBS = 
MAKEINFO = /home/edmond/software/axyftp-0.5.1/missing makeinfo
MOTIF_CFLAGS = 
PACKAGE = axyftp
VERSION = 0.5.1
XMAXY_CFLAGS = -I$(top_srcdir)/xmaxy
XPM_CFLAGS = 
XPM_LIBS = -lXpm
helpdir = ${prefix}/share/axyftp/help

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = Makefile.orig

CLEANFILES = *~ *-axyftp axyftp-*
MAINTAINERCLEANFILES = Makefile.in

#MALLOC = -u malloc -lefence
MALLOC = 

AXYFTP_GUI = BUILD_MOTIF
AXYFTP_HELP_DIR = ${prefix}/share/axyftp/help

bin_PROGRAMS = axyftp

axyftp_DEPENDENCIES =  $(top_builddir)/xmaxy/XmAxy/libXmAxy.a

INCLUDES = -D$(AXYFTP_GUI) -DAXYFTP_HELP_DIR=\"$(AXYFTP_HELP_DIR)\"          -I$(top_srcdir)/xpms $(XMAXY_CFLAGS) $(DT_CFLAGS) $(MOTIF_CFLAGS)                     $(XPM_CFLAGS)


DT_LIBS = -L/usr/dt/lib -lDtWidget -lDtSvc -ltt
XMAXY_LIBS = $(top_builddir)/xmaxy/XmAxy/libXmAxy.a
MOTIF_LIBS = -lXm

axyftp_LDADD = $(XMAXY_LIBS) $(DT_LIBS) $(MOTIF_LIBS)                 -lXt  -lSM -lICE -lX11  $(XPM_LIBS) $(MALLOC)

axyftp_SOURCES = axyftp.c main_frame.c buttonbar.c arrows.c status.c proto.c                  panel.c buttons.c dirname.c dirlist.c session_data.c ftp.c 		 utils.c  read_init.c functions.c fileinfo.c dirinfo.c 		 pixmaps.c ftp_xfer.c dialogs.c session_dialog.c 		 session_general.c session_startup.c about_dialog.c 		 progress_dialog.c little_dialogs.c prompt_dialogs.c viewer.c 		 session_advanced.c options_dialog.c options_general.c 		 options_display.c options_sound.c options_data.c multi.c 	         options_advanced.c


noinst_HEADERS = axyftp.h main_frame.h buttonbar.h arrows.h status.h proto.h                  panel.h buttons.h dirname.h dirlist.h session_data.h ftp.h 		 utils.h  read_init.h functions.h fileinfo.h dirinfo.h 		 pixmaps.h ftp_xfer.h dialogs.h session_dialog.h 		 session_general.h session_startup.h about_dialog.h 		 progress_dialog.h little_dialogs.h prompt_dialogs.h viewer.h 		 session_advanced.h options_dialog.h options_general.h 		 options_display.h options_sound.h options_data.h multi.h 	         options_advanced.h

mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(bin_PROGRAMS)


DEFS = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE=\"axyftp\" -DVERSION=\"0.5.1\"  -I. -I$(srcdir) 
CPPFLAGS = 
LDFLAGS = 
LIBS = 
X_CFLAGS = 
X_LIBS = 
X_EXTRA_LIBS = 
X_PRE_LIBS =  -lSM -lICE
axyftp_OBJECTS =  axyftp.o main_frame.o buttonbar.o arrows.o status.o \
proto.o panel.o buttons.o dirname.o dirlist.o session_data.o ftp.o \
utils.o read_init.o functions.o fileinfo.o dirinfo.o pixmaps.o \
ftp_xfer.o dialogs.o session_dialog.o session_general.o \
session_startup.o about_dialog.o progress_dialog.o little_dialogs.o \
prompt_dialogs.o viewer.o session_advanced.o options_dialog.o \
options_general.o options_display.o options_sound.o options_data.o \
multi.o options_advanced.o
axyftp_LDFLAGS = 
CFLAGS = -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
SOURCES = $(axyftp_SOURCES)
OBJECTS = $(axyftp_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --foreign --include-deps src/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

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

axyftp: $(axyftp_OBJECTS) $(axyftp_DEPENDENCIES)
	@rm -f axyftp
	$(LINK) $(axyftp_LDFLAGS) $(axyftp_OBJECTS) $(axyftp_LDADD) $(LIBS)

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

subdir = src

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
install-exec-am: install-binPROGRAMS
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS
uninstall: uninstall-am
all-am: Makefile $(PROGRAMS) $(HEADERS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir)


mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	-test -z "$(MAINTAINERCLEANFILES)" || rm -f $(MAINTAINERCLEANFILES)
mostlyclean-am:  mostlyclean-binPROGRAMS mostlyclean-compile \
		mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-binPROGRAMS clean-compile clean-tags clean-generic \
		mostlyclean-am

clean: clean-am

distclean-am:  distclean-binPROGRAMS distclean-compile distclean-tags \
		distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-binPROGRAMS \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile tags mostlyclean-tags distclean-tags \
clean-tags maintainer-clean-tags distdir info-am info dvi-am dvi check \
check-am installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


$(top_builddir)/dtwidget/Dt/libDtWidget.a:
	(cd $(top_builddir)/dtwidget; $(MAKE))

$(top_builddir)/xmaxy/XmAxy/libXmAxy.a:
	(cd $(top_builddir)/xmaxy; $(MAKE))

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
