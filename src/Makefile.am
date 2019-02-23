AUTOMAKE_OPTIONS=foreign

EXTRA_DIST=Makefile.orig

CLEANFILES=*~ *-axyftp axyftp-*
MAINTAINERCLEANFILES=Makefile.in

#MALLOC = -u malloc -lefence
MALLOC =

AXYFTP_GUI = @AXYFTP_GUI@
AXYFTP_HELP_DIR = @helpdir@

bin_PROGRAMS=axyftp

axyftp_DEPENDENCIES=@AXYFTP_DEPS@

$(top_builddir)/dtwidget/Dt/libDtWidget.a:
	(cd $(top_builddir)/dtwidget; $(MAKE))

$(top_builddir)/xmaxy/XmAxy/libXmAxy.a:
	(cd $(top_builddir)/xmaxy; $(MAKE))


INCLUDES=-D$(AXYFTP_GUI) -DAXYFTP_HELP_DIR=\"$(AXYFTP_HELP_DIR)\" \
         -I$(top_srcdir)/xpms @AXYFTP_INCLUDES@

DT_LIBS=@DT_LIBS@
XMAXY_LIBS=@XMAXY_LIBS@
MOTIF_LIBS=@MOTIF_LIBS@

axyftp_LDADD=@AXYFTP_LIBS@ $(MALLOC)

axyftp_SOURCES = axyftp.c main_frame.c buttonbar.c arrows.c status.c proto.c \
                 panel.c buttons.c dirname.c dirlist.c session_data.c ftp.c \
		 utils.c  read_init.c functions.c fileinfo.c dirinfo.c \
		 pixmaps.c ftp_xfer.c dialogs.c session_dialog.c \
		 session_general.c session_startup.c about_dialog.c \
		 progress_dialog.c little_dialogs.c prompt_dialogs.c viewer.c \
		 session_advanced.c options_dialog.c options_general.c \
		 options_display.c options_sound.c options_data.c multi.c \
	         options_advanced.c

noinst_HEADERS = axyftp.h main_frame.h buttonbar.h arrows.h status.h proto.h \
                 panel.h buttons.h dirname.h dirlist.h session_data.h ftp.h \
		 utils.h  read_init.h functions.h fileinfo.h dirinfo.h \
		 pixmaps.h ftp_xfer.h dialogs.h session_dialog.h \
		 session_general.h session_startup.h about_dialog.h \
		 progress_dialog.h little_dialogs.h prompt_dialogs.h viewer.h \
		 session_advanced.h options_dialog.h options_general.h \
		 options_display.h options_sound.h options_data.h multi.h \
	         options_advanced.h

