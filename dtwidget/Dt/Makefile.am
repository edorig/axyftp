AUTOMAKE_OPTIONS=foreign

EXTRA_DIST=Makefile.orig

CLEANFILES=*~
MAINTAINERCLEANFILES=Makefile.in

AXY_DIRINC=$(includedir)/Dt

dtwidgetdir=@AXY_DIRINC@

EXTRA_DIST=ComboBox.h ComboBoxP.h

EXTRA_HEADERS=ComboBox.h ComboBoxP.h
EXTRA_LIBRARIES=libDtWidget.a

dtwidget_HEADERS=@INSTALL_AXY_H@
lib_LIBRARIES=@INSTALL_AXY_L@

noinst_HEADERS=@AXY_H@
noinst_LIBRARIES=@AXY_L@

noinst_PROGRAMS=demo

MOTIF_CFLAGS=@MOTIF_CFLAGS@
MOTIF_LIBS=@MOTIF_LIBS@

INCLUDES=-I$(srcdir)/.. $(MOTIF_CFLAGS) @X_CFLAGS@ 

libDtWidget_a_SOURCES=ComboBox.c
libDtWidget_a_LIBADD=ComboBox.o
##libDtWidget_a_OBJECTS=ComboBox.o

demo_SOURCES=demo.c
demo_LDADD= $(top_builddir)/Dt/libDtWidget.a \
	$(MOTIF_LIBS) @X_LIBS@ -lXt ${X_PRE_LIBS} -lX11 \
	${X_EXTRA_LIBS} 

