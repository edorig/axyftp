/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

#include"axyftp.h"
#include"utils.h"
#include"read_init.h"
#include"functions.h"
#include"pixmaps.h"
#include"dialogs.h"
#include"main_frame.h"

struct _appstate appdata;

#ifdef BUILD_GTK

#include<gtk/gtk.h>
static void destroy(){
  gtk_exit(0);
}
static int set_gtk_style();

GtkWidget *toplevel;

int main(int argc,char* argv[]){
  GtkWidget *mainwin;
  gchar *mask,*env;

  read_init();

  gtk_init(&argc,&argv);
  gtkfontheight=set_gtk_style();

  toplevel=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect(GTK_OBJECT(toplevel),"destroy",GTK_SIGNAL_FUNC(destroy),
      NULL);
  gtk_window_set_title(GTK_WINDOW(toplevel),"AxY_FTP");
  mainwin=create_main_frame(toplevel);
  gtk_widget_show(toplevel);

  create_pixmaps();

  make_dialogs(mainwin);

  mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
  update_local(mask);

  appdata.job=0;
  appdata.jump_on_cancel=0;
  appdata.connected=0;
  appdata.interrupt=0;
  appdata.small_interrupt=0;

  signal(SIGALRM,SIG_IGN);
  signal(SIGCHLD,SIG_IGN);
  signal(SIGPIPE,SIG_IGN);

  mask=gdk_get_display();
  env=WXmalloc(strlen(mask)+12);
  sprintf(env,"DISPLAY = %s",mask);
  putenv(env);
  WXfree(env);

  if(appdata.odata->show_session){
    gtk_widget_show(appdata.session);
    gtk_grab_add(appdata.session);
  }

  gtk_main();

  return 0;
}

int gtkfontheight;
GtkStyle* mystyle;

static int set_gtk_style(){
  GtkStyle *old,*new;

  old=gtk_widget_get_default_style();
  mystyle=gtk_style_copy(old);
  gdk_font_unref(mystyle->font);
  mystyle->font=gdk_font_load("fixed");
  gdk_font_ref(mystyle->font);

  mystyle->bg[GTK_STATE_NORMAL].pixel=0;
  mystyle->bg[GTK_STATE_NORMAL].red=50000;
  mystyle->bg[GTK_STATE_NORMAL].green=50000;
  mystyle->bg[GTK_STATE_NORMAL].blue=50000;

  mystyle->bg[GTK_STATE_PRELIGHT].pixel=0;
  mystyle->bg[GTK_STATE_PRELIGHT].red=50000;
  mystyle->bg[GTK_STATE_PRELIGHT].green=50000;
  mystyle->bg[GTK_STATE_PRELIGHT].blue=50000;

  mystyle->bg[GTK_STATE_INSENSITIVE].pixel=0;
  mystyle->bg[GTK_STATE_INSENSITIVE].red=50000;
  mystyle->bg[GTK_STATE_INSENSITIVE].green=50000;
  mystyle->bg[GTK_STATE_INSENSITIVE].blue=50000;

  mystyle->bg[GTK_STATE_ACTIVE].pixel=0;
  mystyle->bg[GTK_STATE_ACTIVE].red=45000;
  mystyle->bg[GTK_STATE_ACTIVE].green=45000;
  mystyle->bg[GTK_STATE_ACTIVE].blue=45000;

  new=gtk_style_copy(mystyle);

  gtk_widget_set_default_style(new);

  return mystyle->font->ascent+mystyle->font->descent;
}

GtkStyle* style_full_copy(GtkStyle *old){
  GtkStyle* copy;

  copy=gtk_style_copy(old);
  copy->klass=g_malloc(sizeof(GtkStyleClass));
  copy->klass->xthickness=old->klass->xthickness;
  copy->klass->ythickness=old->klass->ythickness;
  copy->klass->draw_hline=old->klass->draw_hline;
  copy->klass->draw_vline=old->klass->draw_vline;
  copy->klass->draw_shadow=old->klass->draw_shadow;
  copy->klass->draw_polygon=old->klass->draw_polygon;
  copy->klass->draw_arrow=old->klass->draw_arrow;
  copy->klass->draw_diamond=old->klass->draw_diamond;
  copy->klass->draw_oval=old->klass->draw_oval;
  copy->klass->draw_string=old->klass->draw_string;

#ifndef OLD_GTK
  copy->klass->draw_box=old->klass->draw_box;
  copy->klass->draw_flat_box=old->klass->draw_flat_box;
  copy->klass->draw_check=old->klass->draw_check;
  copy->klass->draw_option=old->klass->draw_option;
  copy->klass->draw_cross=old->klass->draw_cross;
  copy->klass->draw_ramp=old->klass->draw_ramp;
  copy->klass->draw_tab=old->klass->draw_tab;
  copy->klass->draw_shadow_gap=old->klass->draw_shadow_gap;
  copy->klass->draw_box_gap=old->klass->draw_box_gap;
  copy->klass->draw_extension=old->klass->draw_extension;
  copy->klass->draw_focus=old->klass->draw_focus;
  copy->klass->draw_slider=old->klass->draw_slider;
  copy->klass->draw_handle=old->klass->draw_handle;
#endif  

  return copy;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/TextF.h>

void xtmesg(String name,String type,String class, String default_s, 
    String* param,Cardinal* num){
  return;
}

appstate appdata;
XtAppContext appcontext;
Widget toplevel;

int main(int argc, char* argv[]){
  char *mask,*env;
  Widget mainwin;

  read_init();

  toplevel=XtAppInitialize(&appcontext,"AxY_FTP",
      NULL,0,&argc,argv,NULL,NULL,0);

  XtAppSetWarningMsgHandler(appcontext,xtmesg);

  XtVaSetValues(toplevel,XmNforeground,BlackPixelOfScreen(XtScreen(toplevel)),
     XmNtitle,"AxY FTP",NULL);

  mainwin=create_main_frame(toplevel);
  XtRealizeWidget(toplevel);
  adjust_main_frame(mainwin);
  
  create_pixmaps();

  make_dialogs(mainwin);
  
  mask=XmTextFieldGetString(appdata.local.text);
  update_local(mask);
  XtFree(mask);
  appdata.job=0;
  appdata.jump_on_cancel=0;
  appdata.connected=0;
  appdata.interrupt=0;

  signal(SIGALRM,SIG_IGN);
  signal(SIGCHLD,SIG_IGN);
  signal(SIGPIPE,SIG_IGN);
/*printf("SHM=%d\n",XShmQueryExtension(XtDisplay(toplevel)));*/
/*  XSynchronize(XtDisplay(toplevel),1);*/
  mask=DisplayString(XtDisplay(toplevel));
  env=XtMalloc(strlen(mask)+12);
  sprintf(env,"DISPLAY = %s",mask);
  putenv(env);
  XtFree(env);

  if(appdata.odata->show_session){
    XtManageChild(appdata.session);
  }

  XtAppMainLoop(appcontext);

  return 0;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
