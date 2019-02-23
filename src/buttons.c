/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<unistd.h>

#include"axyftp.h"
#include"utils.h"
#include"multi.h"
#include"buttons.h"
#include"dirlist.h"
#include"functions.h"
#include"little_dialogs.h"
#include"prompt_dialogs.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>

static void chgdir_cb(GtkWidget*,gpointer);
static void delete_cb(GtkWidget*,gpointer);
static void refresh_cb(GtkWidget*,gpointer);
static void view_cb(GtkWidget*,gpointer);
static void mkdir_cb(GtkWidget*,gpointer);
static void exec_cb(GtkWidget*,gpointer);
static void rename_cb(GtkWidget*,gpointer);
static void dirinfo_cb(GtkWidget*,gpointer);

void dirinfo_cb(GtkWidget* w,gpointer app){
  int which=(int)app;

  switch(which){
    case LOCAL:
      show_local_dirinfo();
      break;
    case REMOTE:
      if(!appdata.remote.list){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      show_remote_dirinfo();
      break;
  }
  return;
}

void rename_cb(GtkWidget* w,gpointer app){
  int which=(int)app;
  int selrow;

  switch(which){
    case LOCAL:
      selrow=get_selected_row(appdata.local.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one local file should be selected");
	  break;
	default:
	  rename_local_num(selrow);
	  break;
      }
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_row(appdata.remote.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one remote file should be selected");
          break;
	default:
	  rename_remote_num(selrow);
	  break;
      }
      break;
    default:
      break;
  }
  return;
}

void exec_cb(GtkWidget* w,gpointer app){
  int which=(int)app;

  switch(which){
    case LOCAL:
      gtk_widget_show(appdata.exec_local);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      gtk_widget_show(appdata.exec_remote);
      break;
  }
  return;
}

void mkdir_cb(GtkWidget* w,gpointer app){
  int which=(int)app;

  switch(which){
    case LOCAL:
      gtk_widget_show(appdata.mkdir_local);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      gtk_widget_show(appdata.mkdir_remote);
      break;
  }
  return;
}

void view_cb(GtkWidget* w,gpointer app){
  int which=(int)app;
  int selrow;

  switch(which){
    case LOCAL:
      selrow=get_selected_row(appdata.local.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one local file should be selected");
	  break;
	default:
	  busy_cursor(TRUE);
	  view_local_num(selrow);
	  if(!appdata.job)busy_cursor(FALSE);
	  break;
      }
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_row(appdata.remote.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one remote file should be selected");
          break;
	default:
	  appdata.job=3;
	  busy_cursor(TRUE);
	  view_remote_num(selrow);
	  busy_cursor(FALSE);
	  appdata.job=0;
	  break;
      }
      break;
    default:
      break;
  }
  return;
}

void refresh_cb(GtkWidget* w,gpointer app){
  int which=(int)app;
  char* mask;

  switch(which){
    case LOCAL:
      busy_cursor(TRUE);
      mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
      update_local(mask);
      if(!appdata.job)busy_cursor(FALSE);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      appdata.job=7;
      busy_cursor(TRUE);
      mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));
      update_remote(mask);
      busy_cursor(FALSE);
      appdata.job=0;
      break;
  }
  return;
}

void delete_cb(GtkWidget* w,gpointer app){
  int which=(int)app;
  int *selrow;

  switch(which){
    case LOCAL:
      selrow=get_selected_rows(appdata.local.table);
      if(selrow==NULL){
	(void)popup_warning_dialog(toplevel,"Nothing selected");
	return;
      }
      busy_cursor(TRUE);
      if(!delete_local_files(selrow)){
	  char* mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
	  update_local(mask);
      }
      if(!appdata.job)busy_cursor(FALSE);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_rows(appdata.remote.table);
      if(selrow==NULL){
	(void)popup_warning_dialog(toplevel,"Nothing selected");
	return;
      }

      appdata.job=4;
      busy_cursor(TRUE);
      if(!delete_remote_files(selrow)){
	char* mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));
	update_remote(mask);
      }
      busy_cursor(FALSE);
      appdata.job=0;
      break;
    default:
      break;
  }
  return;
}

void chgdir_cb(GtkWidget* w,gpointer app){
  int which=(int)app;
  int selrow;
  char* mask;
  int ret;

  switch(which){
    case LOCAL:
      selrow=get_selected_row(appdata.local.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one local entry should be selected");
	  break;
	default:
	  busy_cursor(TRUE);
	  ret=chg_local_dir(selrow);
	  mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
	  if(!ret)update_local(mask);
	  if(!appdata.job)busy_cursor(FALSE);
	  break;
      }
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_row(appdata.remote.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one remote entry should be selected");
          break;
	default:
	  appdata.job=5;
	  busy_cursor(TRUE);
	  ret=chg_remote_dir(selrow);
	  mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));
	  if(!ret)update_remote(mask);
	  busy_cursor(FALSE);
	  appdata.job=0;
	  break;
      }
      break;
    default:
      break;
  }
  return;
}

GtkWidget* create_buttons(GtkWidget* parent,int which){
  GtkWidget *bar;
  GtkWidget *chdir,*mkdir,*text,*view,*exec,*rename,*delete,*refresh,*dirinfo;

  bar=gtk_vbox_new(TRUE,4);
  gtk_widget_show(bar);

  chdir=gtk_button_new_with_label("ChgDir");
  gtk_widget_show(chdir);
  gtk_box_pack_start(GTK_BOX(bar),chdir,FALSE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(chdir),"clicked",
      GTK_SIGNAL_FUNC(chgdir_cb),(gpointer)which);
  
  mkdir=gtk_button_new_with_label("MkDir");
  gtk_widget_show(mkdir);
  gtk_box_pack_start(GTK_BOX(bar),mkdir,FALSE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(mkdir),"clicked",
      GTK_SIGNAL_FUNC(mkdir_cb),(gpointer)which);

  text=gtk_entry_new();
  gtk_widget_show(text);
  gtk_widget_set_usize(text,8+gdk_string_width(mystyle->font,"Refresh"),
      gtkfontheight*2);
  gtk_box_pack_start(GTK_BOX(bar),text,FALSE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(text),"activate",
      GTK_SIGNAL_FUNC(refresh_cb),(gpointer)which);
  
  view=gtk_button_new_with_label("View");
  gtk_widget_show(view);
  gtk_box_pack_start(GTK_BOX(bar),view,FALSE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(view),"clicked",
      GTK_SIGNAL_FUNC(view_cb),(gpointer)which);

  exec=gtk_button_new_with_label("Exec");
  gtk_widget_show(exec);
  gtk_box_pack_start(GTK_BOX(bar),exec,FALSE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(exec),"clicked",
      GTK_SIGNAL_FUNC(exec_cb),(gpointer)which);

  rename=gtk_button_new_with_label("Rename");
  gtk_widget_show(rename);
  gtk_box_pack_start(GTK_BOX(bar),rename,FALSE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(rename),"clicked",
      GTK_SIGNAL_FUNC(rename_cb),(gpointer)which);

  delete=gtk_button_new_with_label("Delete");
  gtk_widget_show(delete);
  gtk_box_pack_start(GTK_BOX(bar),delete,FALSE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(delete),"clicked",
      GTK_SIGNAL_FUNC(delete_cb),(gpointer)which);

  refresh=gtk_button_new_with_label("Refresh");
  gtk_widget_show(refresh);
  gtk_box_pack_start(GTK_BOX(bar),refresh,FALSE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(refresh),"clicked",
      GTK_SIGNAL_FUNC(refresh_cb),(gpointer)which);

  dirinfo=gtk_button_new_with_label("DirInfo");
  gtk_widget_show(dirinfo);
  gtk_box_pack_start(GTK_BOX(bar),dirinfo,FALSE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(dirinfo),"clicked",
      GTK_SIGNAL_FUNC(dirinfo_cb),(gpointer)which);

  switch(which){
    case LOCAL:
      appdata.local.text=text;
      break;
    case REMOTE:
      appdata.remote.text=text;
      break;
    default:
	break;
  }
  
  return bar;
}

#elif defined BUILD_MOTIF

#include<Xm/Screen.h>
#include<Xm/Xm.h>
#include<Xm/PushB.h>
#include<Xm/Form.h>
#include<Xm/TextF.h>


static void chgdir_cb(Widget,XtPointer,XtPointer);
static void delete_cb(Widget,XtPointer,XtPointer);
static void refresh_cb(Widget,XtPointer,XtPointer);
static void view_cb(Widget,XtPointer,XtPointer);
static void mkdir_cb(Widget,XtPointer,XtPointer);
static void exec_cb(Widget,XtPointer,XtPointer);
static void rename_cb(Widget,XtPointer,XtPointer);
static void dirinfo_cb(Widget,XtPointer,XtPointer);

void dirinfo_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;

  switch(which){
    case LOCAL:
      show_local_dirinfo();
      break;
    case REMOTE:
      if(!appdata.remote.list){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      show_remote_dirinfo();
      break;
  }
  return;
}

void rename_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;
  int selrow;

  switch(which){
    case LOCAL:
      selrow=get_selected_row(appdata.local.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one local file should be selected");
	  break;
	default:
	  rename_local_num(selrow);
	  break;
      }
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_row(appdata.remote.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one remote file should be selected");
          break;
	default:
	  rename_remote_num(selrow);
	  break;
      }
      break;
    default:
      break;
  }
  return;
}

void exec_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;

  switch(which){
    case LOCAL:
      XtManageChild(appdata.exec_local);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      XtManageChild(appdata.exec_remote);
      break;
  }
  return;
}

void mkdir_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;

  switch(which){
    case LOCAL:
      XtManageChild(appdata.mkdir_local);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      XtManageChild(appdata.mkdir_remote);
      break;
  }
  return;
}

void view_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;
  int selrow;

  switch(which){
    case LOCAL:
      selrow=get_selected_row(appdata.local.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one local file should be selected");
	  break;
	default:
	  busy_cursor(True);
	  view_local_num(selrow);
	  if(!appdata.job)busy_cursor(False);
	  break;
      }
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_row(appdata.remote.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one remote file should be selected");
          break;
	default:
	  appdata.job=3;
	  busy_cursor(True);
	  view_remote_num(selrow);
	  busy_cursor(False);
	  appdata.job=0;
	  break;
      }
      break;
    default:
      break;
  }
  return;
}

void refresh_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;
  char* mask;

  switch(which){
    case LOCAL:
      busy_cursor(True);
      XmUpdateDisplay(toplevel);
      mask=XmTextFieldGetString(appdata.local.text);
      update_local(mask);
      XtFree(mask);
      if(!appdata.job)busy_cursor(False);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      appdata.job=7;
      busy_cursor(True);
      XmUpdateDisplay(toplevel);
      mask=XmTextFieldGetString(appdata.remote.text);
      update_remote(mask);
      XtFree(mask);
      busy_cursor(False);
      appdata.job=0;
      break;
  }
  return;
}

void delete_cb(Widget w,XtPointer app,XtPointer call){
  int which=(int)app;
  int *selrow;

  switch(which){
    case LOCAL:
      selrow=get_selected_rows(appdata.local.table);
      if(selrow==NULL){
	(void)popup_warning_dialog(toplevel,"Nothing selected");
	return;
      }
      busy_cursor(TRUE);
      if(!delete_local_files(selrow)){
	char* mask=XmTextFieldGetString(appdata.local.text);
	update_local(mask);
	WXfree(mask);
      }
      if(!appdata.job)busy_cursor(FALSE);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_rows(appdata.remote.table);
      if(selrow==NULL){
	(void)popup_warning_dialog(toplevel,"Nothing selected");
	return;
      }
      appdata.job=4;
      busy_cursor(TRUE);
      if(!delete_remote_files(selrow)){
	char* mask=XmTextFieldGetString(appdata.remote.text);
	update_remote(mask);
	WXfree(mask);
      }
      busy_cursor(FALSE);
      appdata.job=0;
      break;
    default:
      break;
  }
  return;
}

void chgdir_cb(Widget w,XtPointer app,XtPointer cbs){
  int which=(int)app;
  int selrow;
  char* mask;
  int ret;

  switch(which){
    case LOCAL:
      selrow=get_selected_row(appdata.local.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one local entry should be selected");
	  break;
	default:
	  busy_cursor(True);
	  XmUpdateDisplay(toplevel);
	  ret=chg_local_dir(selrow);
	  mask=XmTextFieldGetString(appdata.local.text);
	  if(!ret)update_local(mask);
	  XtFree(mask);
	  if(!appdata.job)busy_cursor(False);
	  break;
      }
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
	break;
      }
      selrow=get_selected_row(appdata.remote.table);
      switch(selrow){
	case NONE:
	case MANY:
	  (void)popup_warning_dialog(toplevel,
	   "Exactly one remote entry should be selected");
          break;
	default:
	  appdata.job=5;
	  busy_cursor(True);
	  XmUpdateDisplay(toplevel);
	  ret=chg_remote_dir(selrow);
	  mask=XmTextFieldGetString(appdata.remote.text);
	  if(!ret)update_remote(mask);
	  XtFree(mask);
	  busy_cursor(False);
	  appdata.job=0;
	  break;
      }
      break;
    default:
      break;
  }
  return;
}


Widget create_buttons(Widget parent,int which){
  Widget bar;
  Arg args[10];
  Cardinal n;
  Widget chdir,mkdir,text,view,exec,rename,delete,refresh,dirinfo;
  Dimension h,w;

  n=0;
  XtSetArg(args[n],XmNfractionBase,9);n++;
  bar=XmCreateForm(parent,"buttons",args,n);
  XtManageChild(bar);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,1);n++;
  */
  chdir=XmCreatePushButton(bar,"ChgDir",args,n);
  XtManageChild(chdir);
  XtAddCallback(chdir,XmNactivateCallback,chgdir_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,chdir);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,2);n++;
  */
  mkdir=XmCreatePushButton(bar,"MkDir",args,n);
  XtManageChild(mkdir);
  XtAddCallback(mkdir,XmNactivateCallback,mkdir_cb,(XtPointer)which);

  n=0;
  /*XtSetArg(args[n],XmNhighlightThickness,0);n++;*/
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,mkdir);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,3);n++;
  */
  XtSetArg(args[n],XmNresizeWidth,True);n++;
  XtSetArg(args[n],XmNbackground,WhitePixelOfScreen(XtScreen(parent)));n++;
  text=XmCreateTextField(bar,"text",args,n);
  XtManageChild(text);
  XtVaGetValues(text,XmNheight,&h,NULL);
  XtVaSetValues(mkdir,XmNheight,h,NULL);
  XtVaSetValues(chdir,XmNheight,h,NULL);
  XtAddCallback(text,XmNactivateCallback,refresh_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,text);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,4);n++;
  */
  view=XmCreatePushButton(bar,"View",args,n);
  XtManageChild(view);
  XtAddCallback(view,XmNactivateCallback,view_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,view);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,5);n++;
  */
  exec=XmCreatePushButton(bar,"Exec",args,n);
  XtManageChild(exec);
  XtAddCallback(exec,XmNactivateCallback,exec_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,exec);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,6);n++;
  */
  rename=XmCreatePushButton(bar,"Rename",args,n);
  XtManageChild(rename);
  XtAddCallback(rename,XmNactivateCallback,rename_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,rename);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,7);n++;
  */
  delete=XmCreatePushButton(bar,"Delete",args,n);
  XtManageChild(delete);
  XtAddCallback(delete,XmNactivateCallback,delete_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,delete);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,8);n++;
  */
  refresh=XmCreatePushButton(bar,"Refresh",args,n);
  XtManageChild(refresh);
  XtAddCallback(refresh,XmNactivateCallback,refresh_cb,(XtPointer)which);

  n=0;
  XtSetArg(args[n],XmNheight,h);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,refresh);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  /*
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  */
  dirinfo=XmCreatePushButton(bar,"DirInfo",args,n);n++;
  XtManageChild(dirinfo);
  XtAddCallback(dirinfo,XmNactivateCallback,dirinfo_cb,(XtPointer)which);

  XtVaGetValues(refresh,XmNwidth,&w,NULL);
  XtVaSetValues(chdir,XmNwidth,w,NULL);
  XtVaSetValues(mkdir,XmNwidth,w,NULL);
  XtVaSetValues(text,XmNwidth,w,NULL);
  XtVaSetValues(text,XmNresizeWidth,False,NULL);
  XtVaSetValues(view,XmNwidth,w,NULL);
  XtVaSetValues(exec,XmNwidth,w,NULL);
  XtVaSetValues(rename,XmNwidth,w,NULL);
  XtVaSetValues(delete,XmNwidth,w,NULL);
  XtVaSetValues(dirinfo,XmNwidth,w,NULL);

  switch(which){
    case LOCAL:
      appdata.local.text=text;
      break;
    case REMOTE:
      appdata.remote.text=text;
      break;
    default:
      break;
  }
  
  return bar;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
