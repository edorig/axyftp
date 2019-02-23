/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef AXYFTP_H
#define AXYFTP_H

#define AXYFTP_MAJOR 0
#define AXYFTP_MINOR 5
#define AXYFTP_MICRO 1

#define LOCAL 1
#define REMOTE 2

#include"session_data.h"
#include"options_data.h"
#include"ftp.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>
typedef GtkWidget* WXwidget;

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
typedef Widget WXwidget;

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif

typedef struct _sitestate {
  WXwidget table;
  WXwidget combo;
  WXwidget text;
  struct _dirinfo* list;
}  sitestate;

typedef struct _appstate {
  int job;
  int connected;
  int interrupt;
  int small_interrupt;
  struct _sitestate local;
  struct _sitestate remote;
  WXwidget status;
  WXwidget proto;
  WXwidget listprogress;
  WXwidget conbutton;

  /* dialogs */
  WXwidget session;
  WXwidget options;
  WXwidget progress;
  int progress_shown;
  WXwidget about;
  WXwidget mkdir_local;
  WXwidget mkdir_remote;
  WXwidget exec_local;
  WXwidget exec_remote;
  WXwidget rename_local;
  WXwidget rename_remote;
  /*WXwidget delete_local;
  WXwidget delete_remote;*/
  WXwidget delete_dialog;
  WXwidget xfer_dialog;
  
  /* connection */
  connect_data connect;
  /* session data linked list */
  session_data* sdata;
  options_data* odata;

  int jump_on_cancel;
} appstate;

extern struct _appstate appdata;
extern WXwidget toplevel;

#ifdef BUILD_GTK

extern int gtkfontheight;
extern GtkStyle* mystyle;
GtkStyle* style_full_copy(GtkStyle*);

#elif defined BUILD_MOTIF

extern XtAppContext appcontext;

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif

#endif /* AXYFTP_H */
