AUTOMAKE_OPTIONS=foreign

CLEANFILES=*~
MAINTAINERCLEANFILES=Makefile.in

AXY_DIRINC=$(includedir)/XmAxy
xmaxydir=@AXY_DIRINC@

EXTRA_DIST=Notebook.h List.h NotebookP.h ListP.h version.h

EXTRA_HEADERS=Notebook.h List.h NotebookP.h ListP.h version.h
EXTRA_LIBRARIES=libXmAxy.a

xmaxy_HEADERS=@INSTALL_AXY_H@
lib_LIBRARIES=@INSTALL_AXY_L@

noinst_HEADERS=@AXY_H@
noinst_LIBRARIES=@AXY_L@

noinst_PROGRAMS=@DEMO_PROG@
EXTRA_PROGRAMS=demo

MOTIF_CFLAGS=@MOTIF_CFLAGS@
MOTIF_LIBS=@MOTIF_LIBS@
XPM_CFLAGS=@XPM_CFLAGS@
XPM_LIBS=@XPM_LIBS@

INCLUDES= -I$(srcdir)/.. $(MOTIF_CFLAGS) @X_CFLAGS@ $(XPM_CFLAGS) 

libXmAxy_a_SOURCES=Notebook.c List.c Util.c Util.h
libXmAxy_a_LIBADD=Notebook.o List.o Util.o

demo_SOURCES=demo.c link.xpm
demo_LDADD= $(top_builddir)/XmAxy/libXmAxy.a \
	$(MOTIF_LIBS)  @X_LIBS@ -lXt ${X_PRE_LIBS} -lX11 \
	${X_EXTRA_LIBS} $(XPM_LIBS) 
