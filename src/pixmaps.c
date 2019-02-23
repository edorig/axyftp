/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"pixmaps.h"

#include<photo16.xpm>

#include<up.xpm>
#include<folder.xpm>
#include<doc.xpm>
#include<link.xpm>

#ifdef BUILD_GTK

#include<question.xpm>
#include<warning.xpm>
#include<error.xpm>

#include<gtk/gtk.h>

GdkPixmap* pixmap[NUMTYPES];
GdkBitmap* mask[NUMTYPES];

GdkPixmap* portret;
GdkBitmap* portmask;

GdkPixmap* question_xpm;
GdkBitmap* questmask_xpm;
GdkPixmap* warning_xpm;
GdkBitmap* warnmask_xpm;
GdkPixmap* error_xpm;
GdkBitmap* errmask_xpm;

int get_pixmap(char** data,GdkPixmap **pix,GdkPixmap **mas){
  *pix=gdk_pixmap_create_from_xpm_d(toplevel->window,mas,NULL,data);

  return (*pix!=NULL);
}


void create_pixmaps(){
  get_pixmap(up_p,&pixmap[0],&mask[0]);
  get_pixmap(folder_p,&pixmap[1],&mask[1]);
  get_pixmap(doc_p,&pixmap[2],&mask[2]);
  get_pixmap(link_p,&pixmap[3],&mask[3]);
  get_pixmap(question,&question_xpm,&questmask_xpm);
  get_pixmap(warning,&warning_xpm,&warnmask_xpm);
  get_pixmap(error,&error_xpm,&errmask_xpm);
  get_pixmap(photo,&portret,&portmask);
}

#elif defined BUILD_MOTIF

Pixmap pixmap[NUMTYPES];
Pixmap mask[NUMTYPES];

Pixmap portret;
Pixmap portmask;

int get_pixmap(char** data,Pixmap *pix,Pixmap *mas){
  XpmAttributes attr;
  int result;

  attr.valuemask=XpmExactColors|XpmCloseness;
  attr.exactColors=FALSE;
  attr.closeness=10000;
  result=XpmCreatePixmapFromData(XtDisplay(appdata.status),
                          DefaultRootWindow(XtDisplay(appdata.status)),
			  data,pix,mas,&attr);

  return result;
}


void create_pixmaps(){
  get_pixmap(up_p,&pixmap[0],&mask[0]);
  get_pixmap(folder_p,&pixmap[1],&mask[1]);
  get_pixmap(doc_p,&pixmap[2],&mask[2]);
  get_pixmap(link_p,&pixmap[3],&mask[3]);
  get_pixmap(photo,&portret,&portmask);
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
