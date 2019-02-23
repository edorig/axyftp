/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<stdio.h>

#include"axyftp.h"
#include"multi.h"
#include"utils.h"
#include"pixmaps.h"
#include"functions.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>

static int close_event_cb(GtkWidget* w){
  GtkWidget* text;
  gtk_widget_hide(w);
  gtk_grab_remove(w);
  if((text=WXnameToWidget(w,"text"))!=NULL)
    gtk_window_set_focus(GTK_WINDOW(w),text);

  return TRUE;
}

static void help_cb(GtkWidget* w,gpointer app){
  show_help((int)app);
}

static void xfer_close_cb(GtkWidget* w){
  gtk_widget_hide(w);
  gtk_grab_remove(w);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,3);
}

static void xfer_cb(GtkWidget* w,gpointer app){
  GtkWidget *d;

  d=gtk_widget_get_toplevel(w);
  gtk_widget_hide(d);
  gtk_grab_remove(d);

  appdata.job=3;
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,(int)app);
}

void init_xfer_dialog(GtkWidget* dialog,char* file){
  GtkWidget* l;

  l=(GtkWidget*)gtk_object_get_user_data(GTK_OBJECT(dialog));
  gtk_label_set(GTK_LABEL(l),file);

  gtk_widget_show(dialog);
  gtk_grab_add(dialog);
}


GtkWidget* create_xfer_dialog(GtkWidget* parent){
  GtkWidget *dialog,*box,*pict,*lbox,*label,*actions,*button;
  
  dialog=gtk_dialog_new();

  gtk_window_set_title(GTK_WINDOW(dialog),"AxY FTP transfer confirm");
  /*gtk_window_position(GTK_WINDOW(dialog),GTK_WIN_POS_MOUSE);*/
  gtk_signal_connect(GTK_OBJECT(dialog),"delete_event",
      GTK_SIGNAL_FUNC(xfer_close_cb),NULL);

  box=gtk_hbox_new(FALSE,4);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),box,TRUE,TRUE,6);
  gtk_widget_show(box);

  pict=gtk_pixmap_new(question_xpm,questmask_xpm);
  gtk_box_pack_start(GTK_BOX(box),pict,FALSE,FALSE,0);
  gtk_widget_show(pict);

  lbox=gtk_vbox_new(FALSE,4);
  gtk_box_pack_start(GTK_BOX(box),lbox,TRUE,TRUE,6);
  gtk_widget_show(lbox);

  label=gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(lbox),label,TRUE,TRUE,6);
  gtk_widget_show(label);
  gtk_object_set_user_data(GTK_OBJECT(dialog),(gpointer)label);

  label=gtk_label_new("already exists");
  gtk_box_pack_start(GTK_BOX(lbox),label,TRUE,TRUE,6);
  gtk_widget_show(label);

  actions=gtk_hbox_new(TRUE,6);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->action_area),actions);
  gtk_widget_show(actions);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Overwrite ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(xfer_cb),(gpointer)1);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Resume ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(xfer_cb),(gpointer)2);
  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Skip ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(xfer_cb),(gpointer)3);

  return dialog;
}

static void delete_close_cb(GtkWidget* w){
  gtk_widget_hide(w);
  gtk_grab_remove(w);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,3);
}

static void delete_cb(GtkWidget* w,gpointer app){
  GtkWidget* d;

  d=gtk_widget_get_toplevel(w);
  gtk_widget_hide(d);
  gtk_grab_remove(d);

  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,(int)app);
  return;
}

void init_delete_dialog(GtkWidget* dialog,char* file){
  GtkWidget* l;
  
  l=(GtkWidget*)gtk_object_get_user_data(GTK_OBJECT(dialog));
  gtk_label_set(GTK_LABEL(l),file);

  gtk_widget_show(dialog);
  gtk_grab_add(dialog);
}
  

GtkWidget* create_delete_dialog(GtkWidget* parent){
  GtkWidget *dialog,*box,*pict,*label,*actions,*button,*lbox;
  
  dialog=gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog),"AxY FTP delete confirm");
  /*gtk_window_position(GTK_WINDOW(dialog),GTK_WIN_POS_MOUSE);*/
  gtk_signal_connect(GTK_OBJECT(dialog),"delete_event",
      GTK_SIGNAL_FUNC(delete_close_cb),NULL);

  box=gtk_hbox_new(FALSE,4);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),box,TRUE,TRUE,6);
  gtk_widget_show(box);

  pict=gtk_pixmap_new(question_xpm,questmask_xpm);
  gtk_box_pack_start(GTK_BOX(box),pict,FALSE,FALSE,0);
  gtk_widget_show(pict);

  lbox=gtk_vbox_new(FALSE,4);
  gtk_box_pack_start(GTK_BOX(box),lbox,TRUE,TRUE,6);
  gtk_widget_show(lbox);

  label=gtk_label_new("Delete");
  gtk_box_pack_start(GTK_BOX(lbox),label,TRUE,TRUE,6);
  gtk_widget_show(label);

  label=gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(lbox),label,TRUE,TRUE,6);
  gtk_widget_show(label);
  gtk_object_set_user_data(GTK_OBJECT(dialog),(gpointer)label);

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
      GTK_SIGNAL_FUNC(delete_cb),(gpointer)1);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Yes to all ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(delete_cb),(gpointer)2);
  
  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" No ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(delete_cb),(gpointer)3);

  return dialog;
}

static void rename_cb(GtkWidget* w,gpointer app){
  GtkWidget* d;
  int which;
  char* text;
  char* orig;

  which=(int)app;
  d=gtk_widget_get_toplevel(w);

  close_event_cb(d);

  if(appdata.job && which==REMOTE){
    return;
  }

  text=gtk_entry_get_text(GTK_ENTRY(WXnameToWidget(d,"text")));
  if(text){
    orig=(char*)gtk_object_get_user_data(GTK_OBJECT(d));
    if(orig){
      switch(which){
	case LOCAL:
	  busy_cursor(TRUE);
	  if(!rename_local(orig,text)){
	    char* mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
	    update_local(mask);
	  }
	  if(!appdata.job)busy_cursor(FALSE);
	  break;
	case REMOTE:
	  appdata.job=9;
	  busy_cursor(TRUE);
	  if(!rename_remote(orig,text)){
	    char* mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));
	    update_remote(mask);
	  }
	  busy_cursor(FALSE);
	  appdata.job=0;
	  break;
       default:
	  break;
      }
    }
  }
  return;
}

void init_rename_dialog(GtkWidget* dialog,char* text,char* orig){
  GtkWidget* t;
  
  t=WXnameToWidget(dialog,"label");
  gtk_label_set(GTK_LABEL(t),text);

  gtk_object_set_user_data(GTK_OBJECT(dialog),(gpointer)orig);

  t=WXnameToWidget(dialog,"text");

  gtk_entry_set_text(GTK_ENTRY(t),orig);

  gtk_editable_select_region(GTK_EDITABLE(t),0,-1);

  gtk_window_set_focus(GTK_WINDOW(dialog),t);
  gtk_widget_show(dialog);
}

GtkWidget* create_rename_dialog(GtkWidget* parent,int which){
  GtkWidget *dialog,*label,*text,*actions,*button;
  
  dialog=gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog),"AxY FTP rename prompt");
  /*gtk_window_position(GTK_WINDOW(dialog),GTK_WIN_POS_MOUSE);*/
  gtk_signal_connect(GTK_OBJECT(dialog),"delete_event",
      GTK_SIGNAL_FUNC(close_event_cb),NULL);

  text=gtk_entry_new();
  gtk_box_pack_end(GTK_BOX(GTK_DIALOG(dialog)->vbox),text,TRUE,TRUE,4);
  gtk_widget_set_name(text,"text");
  gtk_widget_show(text);
  gtk_widget_set_usize(text,0,gtkfontheight*2);
  gtk_signal_connect(GTK_OBJECT(text),"activate",
      GTK_SIGNAL_FUNC(rename_cb),(gpointer)which);

  label=gtk_label_new("");
  gtk_box_pack_end(GTK_BOX(GTK_DIALOG(dialog)->vbox),label,TRUE,TRUE,4);
  gtk_widget_set_name(label,"label");
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
      GTK_SIGNAL_FUNC(rename_cb),(gpointer)which);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Cancel ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect_object(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(close_event_cb),GTK_OBJECT(dialog));

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Help ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect_object(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(help_cb),GTK_OBJECT(dialog));

  return dialog;
}

static void exec_cb(GtkWidget* w,gpointer app){
  GtkWidget* d;
  int which;
  char* text;

  which=(int)app;
  d=gtk_widget_get_toplevel(w);

  close_event_cb(d);

  if(appdata.job && which==REMOTE)return;

  text=gtk_entry_get_text(GTK_ENTRY(WXnameToWidget(d,"text")));
  if(text){
    switch(which){
      case LOCAL:
	busy_cursor(TRUE);
	exec_local(text);
	if(!appdata.job)busy_cursor(FALSE);
	break;
      case REMOTE:
	appdata.job=8;
	busy_cursor(TRUE);
	exec_remote(text);
	busy_cursor(FALSE);
	appdata.job=0;
	break;
     default:
	break;
    }
  }
  return;
}
  
GtkWidget* create_exec_dialog(GtkWidget* parent,int which){
  GtkWidget *dialog,*label,*text,*actions,*button;
  
  dialog=gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog),"AxY FTP exec prompt");
  /*gtk_window_position(GTK_WINDOW(dialog),GTK_WIN_POS_MOUSE);*/
  gtk_signal_connect(GTK_OBJECT(dialog),"delete_event",
      GTK_SIGNAL_FUNC(close_event_cb),NULL);

  switch(which){
    case LOCAL:
      label=gtk_label_new("Enter command to execute localy:");
      break;
    case REMOTE:
      label=gtk_label_new("Enter command to execute remotely\n(using SITE) :");
      break;
    default:
      label=gtk_label_new("");
      break;
  }

  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  text=gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),text,TRUE,TRUE,4);
  gtk_widget_set_name(text,"text");
  gtk_widget_show(text);
  gtk_widget_set_usize(text,0,gtkfontheight*2);
  gtk_signal_connect(GTK_OBJECT(text),"activate",
      GTK_SIGNAL_FUNC(exec_cb),(gpointer)which);

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
      GTK_SIGNAL_FUNC(exec_cb),(gpointer)which);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Cancel ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect_object(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(close_event_cb),GTK_OBJECT(dialog));

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Help ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect_object(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(help_cb),GTK_OBJECT(dialog));
  gtk_window_set_focus(GTK_WINDOW(dialog),text);

  return dialog;
}

static void mkdir_cb(GtkWidget* w,gpointer app){
  GtkWidget* d;
  int which;
  char* text;
  char* mask;

  which=(int)app;
  d=gtk_widget_get_toplevel(w);

  close_event_cb(d);

  if(appdata.job && which==REMOTE)return;

  text=gtk_entry_get_text(GTK_ENTRY(WXnameToWidget(d,"text")));
  if(text){
    switch(which){
      case LOCAL:
	busy_cursor(TRUE);
	if(!mkdir_local(text)){
          mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
	  update_local(mask);
	}
	if(!appdata.job)busy_cursor(FALSE);
	break;
      case REMOTE:
	appdata.job=8;
	busy_cursor(TRUE);
	if(!mkdir_remote(text)){
          mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));
	  update_remote(mask);
	}
	busy_cursor(FALSE);
	appdata.job=0;
	break;
     default:
	break;
    }
  }
  return;
}
  
GtkWidget* create_mkdir_dialog(GtkWidget* parent,int which){
  GtkWidget *dialog,*label,*text,*actions,*button;
  
  dialog=gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dialog),"AxY FTP mkdir prompt");
  gtk_window_position(GTK_WINDOW(dialog),GTK_WIN_POS_MOUSE);
  gtk_signal_connect(GTK_OBJECT(dialog),"delete_event",
      GTK_SIGNAL_FUNC(close_event_cb),NULL);

  switch(which){
    case LOCAL:
      label=gtk_label_new("Enter name of local directory to create:");
      break;
    case REMOTE:
      label=gtk_label_new("Enter name of remote directory to create:");
      break;
    default:
      label=gtk_label_new("");
      break;
  }

  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  text=gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),text,TRUE,TRUE,4);
  gtk_widget_set_name(text,"text");
  gtk_widget_show(text);
  gtk_widget_set_usize(text,0,gtkfontheight*2);
  gtk_signal_connect(GTK_OBJECT(text),"activate",
      GTK_SIGNAL_FUNC(mkdir_cb),(gpointer)which);

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
      GTK_SIGNAL_FUNC(mkdir_cb),(gpointer)which);

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Cancel ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect_object(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(close_event_cb),GTK_OBJECT(dialog));

  label=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(actions),label,TRUE,TRUE,4);
  gtk_widget_show(label);

  button=gtk_button_new_with_label(" Help ");
  gtk_container_add(GTK_CONTAINER(label),button);
  gtk_widget_show(button);
  gtk_signal_connect_object(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(help_cb),GTK_OBJECT(dialog));

  gtk_window_set_focus(GTK_WINDOW(dialog),text);
  return dialog;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/TextF.h>
#include<Xm/SelectioB.h>
#include<Xm/MessageB.h>

static void help_cb(Widget w,XtPointer app,XtPointer call){
  show_help((int)app);
}

static void close_xfer_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild((Widget)app);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,3);
}

static void xfer_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild(w);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,(int)app);
}

void init_xfer_dialog(Widget dialog,char* file){
  char* p;
  XmString xms;

  p=WXmalloc(strlen(file)+50);
  sprintf(p,"%s\nalready exists",file);
  xms=XmStringCreateLocalized(p);

  XtVaSetValues(dialog,XmNmessageString,xms,NULL);
  WXfree(p);
  XmStringFree(xms);

  XtManageChild(dialog);
}

Widget create_xfer_dialog(Widget parent){
  Widget dialog,shell;
  XmString ok,cancel,help;
  Cardinal n;
  Arg args[10];

  ok=XmStringCreateLocalized("Overwrite");
  cancel=XmStringCreateLocalized("Resume");
  help=XmStringCreateLocalized("Skip");

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP transfer confirm");n++;
  XtSetArg(args[n],XmNokLabelString,ok);n++;
  XtSetArg(args[n],XmNcancelLabelString,cancel);n++;
  XtSetArg(args[n],XmNhelpLabelString,help);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateQuestionDialog(parent,"xfer",args,n);

  XtAddCallback(dialog,XmNokCallback,xfer_cb,(XtPointer)1);
  XtAddCallback(dialog,XmNcancelCallback,xfer_cb,(XtPointer)2);
  XtAddCallback(dialog,XmNhelpCallback,xfer_cb,(XtPointer)3);

  shell=dialog;
  while(!XtIsWMShell(shell))shell=XtParent(shell);

  XtVaSetValues(shell,XmNdeleteResponse,XmDO_NOTHING,
      XmNmwmFunctions,0,NULL);

  XtAddCallback(shell,XmNdestroyCallback,close_xfer_cb,(XtPointer)dialog);

  XmStringFree(ok);
  XmStringFree(cancel);
  XmStringFree(help);

  return dialog;
}

static void close_delete_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild((Widget)app);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,3);
}

static void delete_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild(w);
  busy_cursor(TRUE);
  process_events();
  siglongjmp(jmp_down_env,(int)app);
}

void init_delete_dialog(Widget dialog,char* file){
  char* p;
  XmString xms;

  p=WXmalloc(strlen(file)+50);
  sprintf(p,"Delete\n%s",file);
  xms=XmStringCreateLocalized(p);

  XtVaSetValues(dialog,XmNmessageString,xms,NULL);
  WXfree(p);
  XmStringFree(xms);

  XtManageChild(dialog);
}
  

Widget create_delete_dialog(Widget parent){
  Widget dialog,shell;
  XmString ok,cancel,help;
  Cardinal n;
  Arg args[10];

  ok=XmStringCreateLocalized("Yes");
  cancel=XmStringCreateLocalized("Yes to all");
  help=XmStringCreateLocalized("No");

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP delete confirm");n++;
  XtSetArg(args[n],XmNokLabelString,ok);n++;
  XtSetArg(args[n],XmNcancelLabelString,cancel);n++;
  XtSetArg(args[n],XmNhelpLabelString,help);n++;
  XtSetArg(args[n],XmNmessageAlignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreateQuestionDialog(parent,"delete",args,n);

  XtAddCallback(dialog,XmNokCallback,delete_cb,(XtPointer)1);
  XtAddCallback(dialog,XmNcancelCallback,delete_cb,(XtPointer)2);
  XtAddCallback(dialog,XmNhelpCallback,delete_cb,(XtPointer)3);

  shell=dialog;
  while(!XtIsWMShell(shell))shell=XtParent(shell);

  XtVaSetValues(shell,XmNdeleteResponse,XmDO_NOTHING,
      XmNmwmFunctions,0,NULL);

  XtAddCallback(shell,XmNdestroyCallback,close_delete_cb,(XtPointer)dialog);

  XmStringFree(ok);
  XmStringFree(cancel);
  XmStringFree(help);

  return dialog;
}

static void rename_cb(Widget w,XtPointer app,XtPointer call){
  int which;
  XmSelectionBoxCallbackStruct *cbs;
  char* text;
  char* orig;

  which=(int)app;
  cbs=(XmSelectionBoxCallbackStruct*)call;


  if(appdata.job && which==REMOTE){
    XtUnmanageChild(w);
    return;
  }

  if(XmStringGetLtoR(cbs->value,XmFONTLIST_DEFAULT_TAG,&text)){
    XtUnmanageChild(w);
    XtVaGetValues(w,XmNuserData,&orig,NULL);
    if(orig){
      switch(which){
	case LOCAL:
	  busy_cursor(True);
	  if(!rename_local(orig,text)){
	    char* mask=XmTextFieldGetString(appdata.local.text);
	    update_local(mask);
	    XtFree(mask);
	  }
	  if(!appdata.job)busy_cursor(False);
	  break;
	case REMOTE:
	  appdata.job=9;
	  busy_cursor(True);
	  if(!rename_remote(orig,text)){
	    char* mask=XmTextFieldGetString(appdata.remote.text);
	    update_remote(mask);
	    XtFree(mask);
	  }
	  busy_cursor(False);
	  appdata.job=0;
	  break;
       default:
	  break;
      }
    }
    XtFree(text);
  } else {
    XtUnmanageChild(w);
  }
  return;
}

void init_rename_dialog(Widget dialog,char* text,char* orig){
  XmString xms;
  Widget t;

  xms=XmStringCreateLocalized(text);
  XtVaSetValues(dialog,XmNselectionLabelString,xms,
      XmNtextString,NULL,XmNuserData,(XtPointer)orig,NULL);
  XmStringFree(xms);
  
  t=XmSelectionBoxGetChild(dialog,XmDIALOG_TEXT);
  XmTextFieldReplace(t,0,XmTextFieldGetLastPosition(t),orig);

  XtManageChild(dialog);
  XmTextFieldSetSelection(t,0,strlen(orig),CurrentTime);
}
  

Widget create_rename_dialog(Widget parent,int which){
  Widget dialog,child;
  Arg args[10];
  Cardinal n;
  Pixel white_pixel,black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP rename prompt");n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreatePromptDialog(parent,"rename",args,n);
  
  child=XmSelectionBoxGetChild(dialog,XmDIALOG_TEXT);
  XtVaSetValues(child,XmNbackground,white_pixel,
      XmNforeground,black_pixel,
      XmNhighlightThickness,0,NULL);
  
  XtAddCallback(dialog,XmNokCallback,rename_cb,(XtPointer)which);
  XtAddCallback(dialog,XmNhelpCallback,help_cb,(XtPointer)(10+which));

  return dialog;
}

static void exec_cb(Widget w,XtPointer app,XtPointer call){
  int which;
  XmSelectionBoxCallbackStruct *cbs;
  char* text;

  which=(int)app;
  cbs=(XmSelectionBoxCallbackStruct*)call;

  XtUnmanageChild(w);
  if(appdata.job && which==REMOTE)return;

  if(XmStringGetLtoR(cbs->value,XmFONTLIST_DEFAULT_TAG,&text)){
    switch(which){
      case LOCAL:
	busy_cursor(True);
	exec_local(text);
	if(!appdata.job)busy_cursor(False);
	break;
      case REMOTE:
	appdata.job=8;
	busy_cursor(True);
	exec_remote(text);
	busy_cursor(False);
	appdata.job=0;
	break;
     default:
	break;
    }
    XtFree(text);
  }
  return;
}

  

Widget create_exec_dialog(Widget parent,int which){
  Widget dialog,child;
  Arg args[10];
  Cardinal n;
  XmString xms;
  Pixel white_pixel,black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  switch(which){
    case LOCAL:
      xms=XmStringCreateLocalized("Enter command to execute localy:");
    break;
    case REMOTE:
      xms=XmStringCreateLocalized(
	  "Enter command to execute remotely\n(using SITE) :");
      break;
    default:
      xms=XmStringCreateLocalized("");
      break;
  }

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP exec prompt");n++;
  XtSetArg(args[n],XmNselectionLabelString,xms);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreatePromptDialog(parent,"exec",args,n);
  
  child=XmSelectionBoxGetChild(dialog,XmDIALOG_TEXT);
  XtVaSetValues(child,XmNbackground,white_pixel,
      XmNforeground,black_pixel,
      XmNhighlightThickness,0,NULL);
  
  XmStringFree(xms);
  XtAddCallback(dialog,XmNokCallback,exec_cb,(XtPointer)which);
  XtAddCallback(dialog,XmNhelpCallback,help_cb,(XtPointer)(20+which));

  return dialog;
}

static void mkdir_cb(Widget w,XtPointer app,XtPointer call){
  int which;
  XmSelectionBoxCallbackStruct *cbs;
  char* text;
  char* mask;

  which=(int)app;
  cbs=(XmSelectionBoxCallbackStruct*)call;

  XtUnmanageChild(w);
  if(appdata.job && which==REMOTE)return;

  if(XmStringGetLtoR(cbs->value,XmFONTLIST_DEFAULT_TAG,&text)){
    switch(which){
      case LOCAL:
	busy_cursor(True);
	if(!mkdir_local(text)){
          mask=XmTextFieldGetString(appdata.local.text);
	  update_local(mask);
	  XtFree(mask);
	}
	if(!appdata.job)busy_cursor(False);
	break;
      case REMOTE:
	appdata.job=8;
	busy_cursor(True);
	if(!mkdir_remote(text)){
          mask=XmTextFieldGetString(appdata.remote.text);
	  update_remote(mask);
	  XtFree(mask);
	}
	busy_cursor(False);
	appdata.job=0;
	break;
     default:
	break;
    }
    XtFree(text);
  }
  return;
}

  

Widget create_mkdir_dialog(Widget parent,int which){
  Widget dialog,child;
  Arg args[10];
  Cardinal n;
  XmString xms;
  Pixel white_pixel,black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  switch(which){
    case LOCAL:
      xms=XmStringCreateLocalized("Enter name of local directory to create:");
    break;
    case REMOTE:
      xms=XmStringCreateLocalized("Enter name of remote directory to create:");
      break;
    default:
      xms=XmStringCreateLocalized("");
      break;
  }

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP mkdir prompt");n++;
  XtSetArg(args[n],XmNselectionLabelString,xms);n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  dialog=XmCreatePromptDialog(parent,"mkdir",args,n);
  
  child=XmSelectionBoxGetChild(dialog,XmDIALOG_TEXT);
  XtVaSetValues(child,XmNbackground,white_pixel,
      XmNforeground,black_pixel,
      XmNhighlightThickness,0,NULL);
  
  XmStringFree(xms);
  XtAddCallback(dialog,XmNokCallback,mkdir_cb,(XtPointer)which);
  XtAddCallback(dialog,XmNhelpCallback,help_cb,(XtPointer)(30+which));

  return dialog;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
