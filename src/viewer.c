/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#include"axyftp.h"
#include"utils.h"
#include"dirinfo.h"
#include"viewer.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>

void fill_dirinfo(GtkWidget* text,dirinfo* di){
  int i;
  char* line;
  for(i=1;i<=di->total;i++){
    line=get_fileinfo_string(di->files[i]);
    if(line){
      gtk_text_set_point(GTK_TEXT(text),gtk_text_get_length(GTK_TEXT(text)));
      gtk_text_insert(GTK_TEXT(text),NULL,NULL,NULL,line,-1);
      WXfree(line);
    }
  }
  gtk_text_set_point(GTK_TEXT(text),0);
}

int fill_viewer(GtkWidget* text,int fd){
  char buf[1025];
  int len;

  while((len=read(fd,buf,1024))>0){
    buf[len]='\0';
    gtk_text_set_point(GTK_TEXT(text),gtk_text_get_length(GTK_TEXT(text)));
    gtk_text_insert(GTK_TEXT(text),NULL,NULL,NULL,buf,-1);
  }
  close(fd);
  return len;
}

static void destroy_cb(GtkWidget* w){
  while(!GTK_IS_WINDOW(w))w=gtk_widget_get_toplevel(w);
  gtk_widget_destroy(w);
}

GtkWidget* create_viewer(GtkWidget* parent,char* name){
  GtkWidget *viewer,*box,*text,*actions,*align,*button,*scroll;
  char* title;

  title=WXmalloc(strlen(name)+20);
  sprintf(title,"AxY FTP viewer - %s",name);

  viewer=gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(viewer),title);
  WXfree(title);

  gtk_container_border_width(GTK_CONTAINER(viewer),6);

  box=gtk_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(viewer)->vbox),box,TRUE,TRUE,0);
  gtk_widget_show(box);

  text=gtk_text_new(NULL,NULL);
  gtk_box_pack_start(GTK_BOX(box),text,TRUE,TRUE,0);
  gtk_widget_show(text);
  gtk_widget_set_usize(text,gdk_string_width(mystyle->font,"W")*80+8,
      gtkfontheight*24+8);

  scroll=gtk_vscrollbar_new(GTK_TEXT(text)->vadj);
  gtk_box_pack_start(GTK_BOX(box),scroll,FALSE,FALSE,0);
  gtk_widget_show(scroll);

  /*scroll=gtk_hscrollbar_new(GTK_TEXT(text)->hadj);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(viewer)->vbox),scroll,FALSE,FALSE,0);
  gtk_widget_show(scroll);*/
  
  actions=gtk_hbox_new(TRUE,6);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(viewer)->action_area),actions);
  gtk_widget_show(actions);

  align=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),align,TRUE,TRUE,4);
  gtk_widget_show(align);

  button=gtk_button_new_with_label(" CANCEL ");
  gtk_container_add(GTK_CONTAINER(align),button);
  gtk_widget_show(button);

  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(destroy_cb),NULL);
  gtk_signal_connect(GTK_OBJECT(viewer),"destroy",
      GTK_SIGNAL_FUNC(destroy_cb),NULL);
  /*
  XtAddCallback(XtParent(viewer),XmNpopdownCallback,destroy_cb,NULL);
  */

  gtk_widget_show(viewer);

  return text;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Text.h>
#include<Xm/Label.h>
#include<Xm/PushB.h>

void fill_dirinfo(Widget text,dirinfo* di){
  int i;
  char* line;
  for(i=1;i<=di->total;i++){
    line=get_fileinfo_string(di->files[i]);
    if(line){
      XmTextInsert(text,XmTextGetLastPosition(text),line);
      XtFree(line);
    }
  }
  XmTextShowPosition(text,0);
  XmUpdateDisplay(text);
}


int fill_viewer(Widget text,int fd){
  char buf[1025];
  int len;

  while((len=read(fd,buf,1024))>0){
    buf[len]='\0';
    XmTextInsert(text,XmTextGetLastPosition(text),buf);
    XmUpdateDisplay(text);
  }
  close(fd);
  return len;
}

static void destroy_cb(Widget w,XtPointer app,XtPointer call){
  while(!XtIsWMShell(w))w=XtParent(w);
  XtDestroyWidget(w);
}

Widget create_viewer(Widget parent,char* name){
  Widget viewer,text,button;
  Arg args[10];
  Cardinal n;
  Dimension d;
  Pixel white_pixel,black_pixel;
  char* title;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  title=XtMalloc(strlen(name)+20);
  sprintf(title,"AxY FTP viewer - %s",name);

  n=0;
  XtSetArg(args[n],XmNtitle,title);n++;
  XtSetArg(args[n],XmNtransient,False);n++;
  XtSetArg(args[n],XmNverticalSpacing,8);n++;
  viewer=XmCreateFormDialog(parent,"viewer",args,n);
  XtFree(title);
/*
  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  label=XmCreateLabel(viewer,name,args,n);
  XtManageChild(label);
*/

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNrows,24);n++;
  XtSetArg(args[n],XmNcolumns,80);n++;
  XtSetArg(args[n],XmNeditable,False);n++;
  XtSetArg(args[n],XmNeditMode,XmMULTI_LINE_EDIT);n++;
  XtSetArg(args[n],XmNcursorPositionVisible,False);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  text=XmCreateScrolledText(viewer,"text",args,n);
  XtManageChild(text);
  XtVaSetValues(text,XmNbackground,white_pixel,
		     XmNforeground,black_pixel,NULL);
  

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNtopWidget,XtParent(text));n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,50);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  button=XmCreatePushButton(viewer,"CANCEL",args,n);
  XtManageChild(button);
  XtVaGetValues(button,XmNwidth,&d,NULL);
  XtVaSetValues(button,XmNleftOffset,-d/2,NULL);

  XtVaSetValues(XtParent(text),XmNbottomAttachment,XmATTACH_WIDGET,
      XmNbottomWidget,button,NULL);

  /*XtAddCallback(button,XmNactivateCallback,destroy_cb,NULL);*/
  XtAddCallback(XtParent(viewer),XmNpopdownCallback,destroy_cb,NULL);

  XtManageChild(viewer);

  return text;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
