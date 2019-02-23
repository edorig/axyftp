/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

#include"axyftp.h"
#include"multi.h"
#include"utils.h"
#include"pixmaps.h"
#include"dirinfo.h"
#include"functions.h"
#include"status.h"
#include"little_dialogs.h"

static void time_to_retry(int sig){
  siglongjmp(jmp_down_env,2);
}

#ifdef BUILD_GTK

#include<gtk/gtk.h>

static void ack_cb(GtkWidget* w,gpointer d){
  gtk_grab_remove(GTK_WIDGET(d));
  gtk_widget_destroy(GTK_WIDGET(d));
  return;
}

static void reconnect_cb(GtkWidget* w,gpointer app){
  GtkWidget *d;
  char * volatile mask;
  volatile int count,ret,total,delay;
  char t[40];
 
  d=(GtkWidget*)app;
  gtk_grab_remove(d);
  gtk_widget_destroy(d);

  appdata.job=1;
  busy_cursor(TRUE);
  appdata.sdata->locdir[0]='\0';
  WXfree(appdata.sdata->remdir);

  appdata.sdata->remdir=WXnewstring(appdata.remote.list->dir);

  appdata.sdata->locmask[0]='\0';
  appdata.sdata->remmask[0]='\0';
  mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));

  total=atoi(appdata.odata->redial);
  if(total<0)total=0;
  delay=atoi(appdata.odata->delay);
  if(delay<0)delay=0;
  count=0;
  while(1){
    start_session(appdata.sdata,mask);
    if(appdata.connected){
      gtk_label_set(GTK_LABEL(GTK_BUTTON(appdata.conbutton)->child),
	  "disconnect");
      signal(SIGALRM,SIG_IGN);
      break;
    } else {
      sprintf(t,"Attempt %d failed\n",count++);
      append_status(t);
      if(count>total){
	signal(SIGALRM,SIG_IGN);
	break;
      }
      if(delay>0){
	signal(SIGALRM,time_to_retry);
	alarm(delay);
	if(!(ret=sigsetjmp(jmp_down_env,1))){
	  appdata.jump_on_cancel=1;
	  LOOP();
	} 
	appdata.jump_on_cancel=0;
      }
      if(appdata.interrupt){
	signal(SIGALRM,SIG_IGN);
	appdata.interrupt=0;
	break;
      }
      sprintf(t,"Attempt %d ...\n",count);
      append_status(t);
    }
  }

  busy_cursor(FALSE);  
  appdata.job=0;
}

GtkWidget* popup_reconnect_dialog(GtkWidget* parent){
  GtkWidget *dialog,*box,*pict,*label,*actions,*button;
  
  dialog=gtk_dialog_new();

  gtk_window_set_title(GTK_WINDOW(dialog),"AxY FTP reconnect");
  /*gtk_window_position(GTK_WINDOW(dialog),GTK_WIN_POS_MOUSE);*/

  box=gtk_hbox_new(FALSE,4);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),box,TRUE,TRUE,6);
  gtk_widget_show(box);

  pict=gtk_pixmap_new(question_xpm,questmask_xpm);
  gtk_box_pack_start(GTK_BOX(box),pict,FALSE,FALSE,0);
  gtk_widget_show(pict);

  label=gtk_label_new("Connection lost. Reconnect?");
  gtk_box_pack_start(GTK_BOX(box),label,TRUE,TRUE,6);
  gtk_widget_show(label);

  actions=gtk_hbox_new(TRUE,6);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->action_area),actions);
  gtk_widget_show(actions);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Yes ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(reconnect_cb),(gpointer)dialog);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" No ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(ack_cb),(gpointer)dialog);

  gtk_widget_show(dialog);
  gtk_grab_add(dialog);

  return dialog;
}
  
GtkWidget* popup_error_dialog(GtkWidget* parent,char* message){
  GtkWidget *dialog,*box,*pict,*label,*actions,*button;
  
  dialog=gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog),"AxY FTP error");
  /*gtk_window_position(GTK_WINDOW(dialog),GTK_WIN_POS_MOUSE);*/

  box=gtk_hbox_new(FALSE,4);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),box,TRUE,TRUE,6);
  gtk_widget_show(box);

  pict=gtk_pixmap_new(error_xpm,errmask_xpm);
  gtk_box_pack_start(GTK_BOX(box),pict,FALSE,FALSE,0);
  gtk_widget_show(pict);

  label=gtk_label_new(message);
  gtk_box_pack_start(GTK_BOX(box),label,TRUE,TRUE,6);
  gtk_widget_show(label);

  actions=gtk_hbox_new(TRUE,6);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->action_area),actions);
  gtk_widget_show(actions);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" OK ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(ack_cb),(gpointer)dialog);

  gtk_widget_show(dialog);
  gtk_grab_add(dialog);

  return dialog;
}

GtkWidget* popup_warning_dialog(GtkWidget* parent,char* message){
  GtkWidget *dialog,*box,*pict,*label,*actions,*button;
  
  dialog=gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog),"AxY FTP warning");
  /*gtk_window_position(GTK_WINDOW(dialog),GTK_WIN_POS_MOUSE);*/

  box=gtk_hbox_new(FALSE,4);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),box,TRUE,TRUE,6);
  gtk_widget_show(box);

  pict=gtk_pixmap_new(warning_xpm,warnmask_xpm);
  gtk_box_pack_start(GTK_BOX(box),pict,FALSE,FALSE,0);
  gtk_widget_show(pict);

  label=gtk_label_new(message);
  gtk_box_pack_start(GTK_BOX(box),label,TRUE,TRUE,6);
  gtk_widget_show(label);

  actions=gtk_hbox_new(TRUE,6);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->action_area),actions);
  gtk_widget_show(actions);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" OK ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(ack_cb),(gpointer)dialog);

  gtk_widget_show(dialog);
  gtk_grab_add(dialog);

  return dialog;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/TextF.h>
#include<Xm/MessageB.h>
#include<Xm/SelectioB.h>

static void ack_cb(Widget w,XtPointer app,XtPointer call){
  XtDestroyWidget(XtParent(w));
  return;
}

static void reconnect_cb(Widget w,XtPointer app,XtPointer call){
  int act;
  char* volatile mask;
  volatile int count,ret,total,delay;
  char t[40];
 
  act=(int)app;
  XtDestroyWidget(XtParent(w));

  if(act){
    appdata.job=1;
    busy_cursor(True);
    appdata.sdata->locdir[0]='\0';
    XtFree(appdata.sdata->remdir);

    appdata.sdata->remdir=XtNewString(appdata.remote.list->dir);

    appdata.sdata->locmask[0]='\0';
    appdata.sdata->remmask[0]='\0';
    mask=XmTextFieldGetString(appdata.remote.text);

    total=atoi(appdata.odata->redial);
    if(total<0)total=0;
    delay=atoi(appdata.odata->delay);
    if(delay<0)delay=0;
    count=0;
    while(1){
      start_session(appdata.sdata,mask);
      if(appdata.connected){
	XmString l;
	l=XmStringCreateLocalized("disconnect");
	XtVaSetValues(appdata.conbutton,XmNlabelString,l,NULL);
	XmStringFree(l);
	signal(SIGALRM,SIG_IGN);
	break;
      } else {
	sprintf(t,"Attempt %d failed\n",count++);
	append_status(t);
	if(count>total){
	  signal(SIGALRM,SIG_IGN);
	  break;
	}
	if(delay>0){
	  signal(SIGALRM,time_to_retry);
	  alarm(delay);
	  if(!(ret=sigsetjmp(jmp_down_env,1))){
	    appdata.jump_on_cancel=1;
	    LOOP();
	  } 
	  appdata.jump_on_cancel=0;
	}
	if(appdata.interrupt){
	  signal(SIGALRM,SIG_IGN);
	  appdata.interrupt=0;
	  break;
	}
	sprintf(t,"Attempt %d ...\n",count);
	append_status(t);
      }
    }
   
    XtFree(mask);
    busy_cursor(False);
    appdata.job=0;
  }
}

Widget popup_reconnect_dialog(Widget parent){
  Widget dialog,button;
  XmString xms,yes,no;
  Arg args[10];
  Cardinal n;
  xms=XmStringCreateLocalized("Connection lost. Reconnect?");
  yes=XmStringCreateLocalized("Yes");
  no=XmStringCreateLocalized("No");


  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP reconnect");n++;
  XtSetArg(args[n],XmNokLabelString,yes);n++;
  XtSetArg(args[n],XmNcancelLabelString,no);n++;
  XtSetArg(args[n],XmNmessageString,xms);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateQuestionDialog(parent,"reconnect",args,n);
  XtAddCallback(dialog,XmNokCallback,reconnect_cb,(XtPointer)1);
  XtAddCallback(dialog,XmNcancelCallback,reconnect_cb,(XtPointer)0);
  
  button=XmMessageBoxGetChild(dialog,XmDIALOG_HELP_BUTTON);
  XtUnmanageChild(button);

  XtManageChild(dialog);
  
  XmStringFree(yes);
  XmStringFree(no);
  XmStringFree(xms);
  return dialog;
}
  
Widget popup_error_dialog(Widget parent,char* message){
  Widget dialog,button;
  XmString xms;
  Arg args[4];
  Cardinal n;
  
  xms=XmStringCreateLocalized(message);

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP error");n++;
  XtSetArg(args[n],XmNmessageString,xms);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateErrorDialog(parent,"error",args,n);
  XtAddCallback(dialog,XmNokCallback,ack_cb,NULL);
  
  button=XmMessageBoxGetChild(dialog,XmDIALOG_HELP_BUTTON);
  XtUnmanageChild(button);
  button=XmMessageBoxGetChild(dialog,XmDIALOG_CANCEL_BUTTON);
  XtUnmanageChild(button);

  XtManageChild(dialog);
  XmStringFree(xms);

  return dialog;
}

Widget popup_warning_dialog(Widget parent,char* message){
  Widget dialog,button;
  XmString xms;
  Arg args[4];
  Cardinal n;
  
  xms=XmStringCreateLocalized(message);

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP warning");n++;
  XtSetArg(args[n],XmNmessageString,xms);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateWarningDialog(parent,"info",args,n);
  XtAddCallback(dialog,XmNokCallback,ack_cb,NULL);
  
  button=XmMessageBoxGetChild(dialog,XmDIALOG_HELP_BUTTON);
  XtUnmanageChild(button);
  button=XmMessageBoxGetChild(dialog,XmDIALOG_CANCEL_BUTTON);
  XtUnmanageChild(button);

  XtManageChild(dialog);
  XmStringFree(xms);

  return dialog;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
