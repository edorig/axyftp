/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#ifndef UTILS_H
#define UTILS_H

#include"axyftp.h"
void WXsetLabel(WXwidget,char*);
void busy_cursor(int);
void beep();
void process_events();
char* WXnewstring(char*);

#ifdef BUILD_GTK

#include<gtk/gtk.h>
#define WXmalloc(x) g_malloc(x)
#define WXfree(x) g_free(x);
#define WXrealloc(x,y) g_realloc(x,y)
GtkWidget* WXnameToWidget(GtkWidget*,char*);
#define LOOP() gtk_main()

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#define WXmalloc(x) XtMalloc(x)
#define WXfree(x) XtFree((char*)(x));
#define WXrealloc(x,y) XtRealloc(x,y)
#define LOOP() XtAppMainLoop(appcontext)

#else 
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif

#endif /* UTILS_H */
