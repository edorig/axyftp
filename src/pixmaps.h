/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef PIXMAPS_H
#define PIXMAPS_H

#define NUMTYPES 4

#ifdef BUILD_GTK

#include<gtk/gtk.h>
extern GdkPixmap* pixmap[NUMTYPES];
extern GdkBitmap* mask[NUMTYPES];
extern GdkPixmap* portret;
extern GdkBitmap* portmask;
extern GdkPixmap* question_xpm;
extern GdkBitmap* questmask_xpm;
extern GdkPixmap* warning_xpm;
extern GdkBitmap* warnmask_xpm;
extern GdkPixmap* error_xpm;
extern GdkBitmap* errmask_xpm;

#elif defined BUILD_MOTIF

#include<X11/xpm.h>
extern Pixmap pixmap[NUMTYPES];
extern Pixmap mask[NUMTYPES];
extern Pixmap portret;
extern Pixmap portmask;

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif

void create_pixmaps();

#endif /*PIXMAPS_H*/

