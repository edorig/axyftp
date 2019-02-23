/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"status.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>

void append_status(char* start){
  int len;

  if(start==NULL) return;
  len=strlen(start);
  if(len && start[--len]=='\r'){
    start[len]='\n';
    start[len+1]='\0';
  }
  if(len && start[--len]=='\r'){
    start[len]='\n';
    start[len+1]='\0';
  }
  gtk_text_set_point(GTK_TEXT(appdata.status),
      gtk_text_get_length(GTK_TEXT(appdata.status)));
  gtk_text_insert(GTK_TEXT(appdata.status),NULL,NULL,NULL,start,-1);
}

GtkWidget* create_status(GtkWidget *parent){
  GtkWidget *statusb,*box,*text,*texta,*scroll;
  GtkStyle *ns;
  gint i;

  statusb=gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(statusb),GTK_SHADOW_ETCHED_OUT);
  gtk_container_border_width(GTK_CONTAINER(statusb),3);
  gtk_widget_show(statusb);

  box=gtk_hbox_new(FALSE,2);
  gtk_container_border_width(GTK_CONTAINER(box),6);
  gtk_container_add(GTK_CONTAINER(statusb),box);
  gtk_widget_show(box);
  
  texta=gtk_alignment_new(0,0.5,1,0);
  gtk_box_pack_start(GTK_BOX(box),texta,TRUE,TRUE,3);
  gtk_widget_show(texta);
  text=gtk_text_new(NULL,NULL);
  appdata.status=text;
  gtk_text_set_editable(GTK_TEXT(text),FALSE);
  /*gtk_widget_set_sensitive(text,FALSE);*/
  gtk_widget_set_usize(text,100,gtkfontheight*3+2);
  gtk_container_add(GTK_CONTAINER(texta),text);
  gtk_widget_show(text);

  scroll=gtk_vscrollbar_new(GTK_TEXT(text)->vadj);
  gtk_box_pack_end(GTK_BOX(box),scroll,FALSE,FALSE,3);
  gtk_widget_show(scroll);

  ns=style_full_copy(gtk_widget_get_style(text));
  
  for(i=0;i<5;i++){
    ns->base[i].pixel=ns->bg[GTK_STATE_NORMAL].pixel;
    ns->base[i].red=ns->bg[GTK_STATE_NORMAL].red;
    ns->base[i].green=ns->bg[GTK_STATE_NORMAL].green;
    ns->base[i].blue=ns->bg[GTK_STATE_NORMAL].blue;
  }
  ns->klass->xthickness=0;
  ns->klass->ythickness=0;

  gtk_widget_set_style(text,ns);

  return statusb;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/List.h>
#include<Xm/ScrolledW.h>
#include<Xm/Text.h>
#include<Xm/Label.h>
#include<Xm/Frame.h>
#include<string.h>

void append_status(char* start){
  int len;

  if(start==NULL) return;
  len=strlen(start);
  if(len && start[--len]=='\r'){
    start[len]='\n';
    start[len+1]='\0';
  }
  if(len && start[--len]=='\r'){
    start[len]='\n';
    start[len+1]='\0';
  }

  len=XmTextGetLastPosition(appdata.status);
  XmTextInsert(appdata.status,len,start);
  len+=strlen(start);
  XmTextSetInsertionPosition(appdata.status,len);
  XmTextShowPosition(appdata.status,len);
  XmUpdateDisplay(toplevel);
  return;
}

Widget create_status(Widget parent){
  Arg args[10];
  Cardinal n;
  Widget status,text;
  Pixel bg;

  n=0;
  XtSetArg(args[n],XmNshadowType,XmSHADOW_ETCHED_OUT);n++;
  XtSetArg(args[n],XmNmarginWidth,3);n++;
  XtSetArg(args[n],XmNmarginHeight,3);n++;
  status=XmCreateFrame(parent,"_status",args,n);
  XtManageChild(status);

  XtVaGetValues(status,XmNbackground,&bg,NULL);

  n=0;
  XtSetArg(args[n],XmNchildType,XmFRAME_WORKAREA_CHILD);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNeditable,False);n++;
  XtSetArg(args[n],XmNscrollHorizontal,False);n++;
  XtSetArg(args[n],XmNcursorPositionVisible,False);n++;
  XtSetArg(args[n],XmNeditMode,XmMULTI_LINE_EDIT);n++;
  XtSetArg(args[n],XmNverifyBell,False);n++;
  XtSetArg(args[n],XmNwordWrap,True);n++;
  XtSetArg(args[n],XmNrows,3);n++;
  text=XmCreateScrolledText(status,"status",args,n);
  XtManageChild(text);

  XtVaSetValues(text,XmNshadowThickness,0,XmNbackground,bg,NULL);

  appdata.status=text;

  return status;

}
#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
