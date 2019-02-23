/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<stdio.h>

#include"axyftp.h"
#include"utils.h"
#include"multi.h"
#include"functions.h"
#include"dirlist.h"
#include"arrows.h"
#include"proto.h"
#include"little_dialogs.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>
static void download_cb(GtkWidget*);
static void upload_cb(GtkWidget*);

static void upload_cb(GtkWidget* w){
  int *selrow;
  int ret;
  char* mask;

  if(appdata.job)return;
  if(!appdata.connected){
    (void)popup_warning_dialog(toplevel,"Not connected");
    return;
  }

  selrow=get_selected_rows(appdata.local.table);
  if(selrow==NULL){
    (void)popup_warning_dialog(toplevel,"Nothing selected");
    return;
  }
  appdata.job=4;
  busy_cursor(TRUE);
  ret=put_files(selrow,get_proto_state(appdata.proto));
  mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));
  if(!ret)update_remote(mask);
  busy_cursor(FALSE);
  appdata.job=0;
  return;
}

static void download_cb(GtkWidget* w){
  int *selrow;
  char* mask;

  if(appdata.job)return;
  if(!appdata.connected){
    (void)popup_warning_dialog(toplevel,"Not connected");
    return;
  }

  selrow=get_selected_rows(appdata.remote.table);
  if(selrow==NULL){
    (void)popup_warning_dialog(toplevel,"Nothing selected");
    return;
  }
  appdata.job=3;
  busy_cursor(TRUE);
  if(!get_files(selrow,get_proto_state(appdata.proto))){
    mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
    update_local(mask);
    WXfree(selrow);
    busy_cursor(FALSE);
    appdata.job=0;
  } else {
    busy_cursor(FALSE);
    appdata.job=0;
  }
  return;
}

GtkWidget* create_arrows(GtkWidget* parent){
  GtkWidget* arrows;
  GtkWidget *leftal,*left,*rightal,*right;

  arrows=gtk_vbox_new(TRUE,2);
  gtk_container_border_width(GTK_CONTAINER(arrows),4);
  gtk_widget_show(arrows);

  leftal=gtk_alignment_new(0.5,0.99,0.0,0.0);
  gtk_widget_show(leftal);
  left=gtk_button_new_with_label(" <- ");
  gtk_widget_show(left);
  gtk_container_add(GTK_CONTAINER(leftal),left);
  gtk_box_pack_start(GTK_BOX(arrows),leftal,TRUE,TRUE,2);
  gtk_signal_connect_after(GTK_OBJECT(left),"clicked",
      GTK_SIGNAL_FUNC(download_cb),NULL);

  gtk_widget_set_usize(left,gtkfontheight*2.5,gtkfontheight*2.5);
  
  rightal=gtk_alignment_new(0.5,0.01,0.0,0.0);
  gtk_widget_show(rightal);
  right=gtk_button_new_with_label(" -> ");
  gtk_widget_show(right);
  gtk_signal_connect_after(GTK_OBJECT(right),"clicked",
      GTK_SIGNAL_FUNC(upload_cb),NULL);

  gtk_container_add(GTK_CONTAINER(rightal),right);
  gtk_box_pack_start(GTK_BOX(arrows),rightal,TRUE,TRUE,2);
  gtk_widget_set_usize(right,gtkfontheight*2.5,gtkfontheight*2.5);

  return arrows;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/PushB.h>
#include<Xm/TextF.h>

void download_cb(Widget,XtPointer,XtPointer);
void upload_cb(Widget,XtPointer,XtPointer);

void upload_cb(Widget w,XtPointer app,XtPointer call){
  int *selrow;
  int ret;
  char* mask;

  if(appdata.job)return;
  if(!appdata.connected){
    (void)popup_warning_dialog(toplevel,"Not connected");
    return;
  }

  selrow=get_selected_rows(appdata.local.table);
  if(selrow==NULL){
    (void)popup_warning_dialog(toplevel,"Nothing selected");
    return;
  }
  appdata.job=4;
  busy_cursor(True);
  XmUpdateDisplay(toplevel);
  ret=put_files(selrow,get_proto_state(appdata.proto));
  mask=XmTextFieldGetString(appdata.remote.text);
  if(!ret)update_remote(mask);
  XtFree(mask);
  busy_cursor(False);
  appdata.job=0;
  return;
}

void download_cb(Widget w,XtPointer app,XtPointer call){
  int *selrow;
  char* mask;

  if(appdata.job)return;
  if(!appdata.connected){
    (void)popup_warning_dialog(toplevel,"Not connected");
    return;
  }

  selrow=get_selected_rows(appdata.remote.table);
  if(selrow==NULL){
    (void)popup_warning_dialog(toplevel,"Nothing selected");
    return;
  }
  appdata.job=3;
  busy_cursor(TRUE);
  if(!get_files(selrow,get_proto_state(appdata.proto))){
    mask=XmTextFieldGetString(appdata.local.text);
    update_local(mask);
    WXfree(mask);
  }
  WXfree((char*)selrow);
  busy_cursor(FALSE);
  appdata.job=0;
  return;
  /*
  selrow=get_selected_row(appdata.remote.table);
  switch(selrow){
    case MANY:
    case NONE:
      (void)popup_warning_dialog(toplevel,
	  "Exactly one remote file should be selected");
      return;
    default:
      appdata.job=3;
      busy_cursor(True);
      XmUpdateDisplay(toplevel);
      ret=download_file(selrow,get_proto_state(appdata.proto));
      mask=XmTextFieldGetString(appdata.local.text);
      if(!ret)update_local(mask);
      XtFree(mask);
      busy_cursor(False);
      appdata.job=0;
      return;
  }
  */
}

Widget create_arrows(Widget parent){
  Widget arrows;
  Arg args[10];
  Cardinal n;
  Widget left,right;
  XmString label;
  Dimension d;
  Pixel b;

  XtVaGetValues(parent,XmNbackground,&b,NULL);


  n=0;
  XtSetArg(args[n],XmNhorizontalSpacing,4);
  arrows=XmCreateForm(parent,"arrows",args,n);
  XtManageChild(arrows);

  label=XmStringCreateLocalized(" <- ");
  n=0;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNhighlightColor,b);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,49);n++;
  left=XmCreatePushButton(arrows,"<-",args,n);
  XtManageChild(left);
  XmStringFree(label);
  XtVaGetValues(left,XmNwidth,&d,NULL);
  XtVaSetValues(left,XmNheight,d,NULL);

  label=XmStringCreateLocalized(" -> ");
  n=0;
  XtSetArg(args[n],XmNlabelString,label);n++;
  XtSetArg(args[n],XmNhighlightColor,b);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,51);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  right=XmCreatePushButton(arrows,"->",args,n);
  XtManageChild(right);
  XmStringFree(label);
  XtVaSetValues(right,XmNheight,d,NULL);

  XtAddCallback(left,XmNactivateCallback,download_cb,NULL);
  XtAddCallback(right,XmNactivateCallback,upload_cb,NULL);

  return arrows;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
