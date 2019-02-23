/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>
#include<stdio.h>

#include"axyftp.h"
#include"utils.h"

char* WXnewstring(char* orig){
  char* ret;

  if(orig==NULL)return NULL;

  if((ret=strdup(orig))==NULL){
    perror("AxY FTP");
    exit(1);
  }
  return ret;
}

#ifdef BUILD_GTK

#include<gtk/gtk.h>

void WXsetLabel(WXwidget label,char* string){
  gtk_label_set(GTK_LABEL(label),string);
}

void beep(){
  gdk_beep();
}

void busy_cursor(int busy){
  GdkCursor* cursor;

  if(busy){
    cursor=gdk_cursor_new(GDK_WATCH);
    gdk_window_set_cursor((toplevel->window),cursor);
    gdk_cursor_destroy(cursor);
  } else {
    cursor=gdk_cursor_new(GDK_TOP_LEFT_ARROW);
    gdk_window_set_cursor((toplevel->window),cursor);
    gdk_cursor_destroy(cursor);
  }
}

void process_events(){
  while (gtk_events_pending())gtk_main_iteration ();
}

GtkWidget* WXnameToWidget(GtkWidget* w,char* name){
  GList *ch;
  GtkWidget *ret;
  if(strcmp(name,gtk_widget_get_name(w))==0) return w;
  if(!GTK_IS_CONTAINER(w)) return NULL;
  ch=gtk_container_children(GTK_CONTAINER(w));
  while(ch!=NULL){
    if((ret=WXnameToWidget(GTK_WIDGET(ch->data),name))!=NULL){
      g_list_free(ch);
      return ret;
    } else {
      ch=g_list_remove_link(ch,ch);
    }
  }
  return NULL;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<X11/cursorfont.h>

void WXsetLabel(WXwidget label,char* string){
  XmString xms;

  xms=XmStringCreateLocalized(string);
  XtVaSetValues(label,XmNlabelString,xms,NULL);
  XmStringFree(xms);
}

void beep(){
  XBell(XtDisplay(toplevel),100);
}

void busy_cursor(int busy){
  Cursor cursor;
  XSetWindowAttributes at;

  if(busy){
    cursor=XCreateFontCursor(XtDisplay(toplevel),XC_watch);
    at.cursor=cursor;
  } else {
    at.cursor=None;
  }
  XChangeWindowAttributes(XtDisplay(toplevel),XtWindow(toplevel),CWCursor,&at);
  XFlush(XtDisplay(toplevel));
}

void process_events(){
  XEvent e;
/*  XFlush(XtDisplay(toplevel));*/
  XmUpdateDisplay(toplevel);
  while(XEventsQueued(XtDisplay(toplevel),QueuedAfterFlush)){
    XNextEvent(XtDisplay(toplevel),&e);
    XtDispatchEvent(&e);
  }
/*  while(XCheckMaskEvent(XtDisplay(toplevel), ButtonPressMask | 
      ButtonReleaseMask | ButtonMotionMask | 
      PointerMotionMask | KeyPressMask, &e)){
    XtDispatchEvent(&e);
  }
*/   
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif

