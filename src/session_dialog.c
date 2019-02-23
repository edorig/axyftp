/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */

#define XM_NOTEBOOK 0

#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

#include"axyftp.h"
#include"utils.h"
#include"multi.h"
#include"status.h"
#include"read_init.h"
#include"session_data.h"
#include"functions.h"
#include"session_dialog.h"
#include"session_general.h"
#include"session_startup.h"
#include"session_advanced.h"

static void time_to_retry(int sig){
  siglongjmp(jmp_down_env,2);
}

#ifdef BUILD_GTK

#include<gtk/gtk.h>

static void init_session_dialog(GtkWidget*);

void request_connection(){
  char* volatile mask;
  volatile int count,ret,total,delay;
  char t[40];
  
  fetch_session_data(appdata.session,appdata.sdata);
  gtk_entry_set_text(GTK_ENTRY(appdata.remote.text),appdata.sdata->remmask);
  gtk_entry_set_text(GTK_ENTRY(appdata.local.text),appdata.sdata->locmask);
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
}

void update_session_dialog(int pos,char set_profile){
  session_data *sd;
  int i;
  for(i=0,sd=appdata.sdata;i<pos;i++)sd=sd->next;
  put_session_data(appdata.session,sd,set_profile);
}

void put_session_data(GtkWidget *dialog,session_data* sd,char set_profile){
  GtkWidget *current;
  char* pass;
  
  if(set_profile){
    current=WXnameToWidget(dialog,"profile");
    gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(current)->entry),sd->profile);
  }
  
  current=WXnameToWidget(dialog,"host");
  gtk_entry_set_text(GTK_ENTRY(current),sd->host);

  current=WXnameToWidget(dialog,"Anonymous");
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(current),sd->anon);
  gtk_object_set_user_data(GTK_OBJECT(current),(gpointer)(sd->anon?TRUE:FALSE));

  current=WXnameToWidget(dialog,"user");
  gtk_entry_set_text(GTK_ENTRY(current),sd->user);

  current=WXnameToWidget(dialog,"pass");
  pass=(char*)gtk_object_get_user_data(GTK_OBJECT(current));
#ifdef OLD_GTK
  if(sd->anon){
#endif
    gtk_entry_set_text(GTK_ENTRY(current),sd->pass);
    pass[0]='\0';
#ifdef OLD_GTK
  } else {
    char* stars;
    int n;
    stars=WXmalloc((n=strlen(sd->pass))+1);
    strcpy(pass,sd->pass);
    memset(stars,'*',n);
    stars[n]='\0';
    gtk_entry_set_text(GTK_ENTRY(current),stars);
    WXfree(stars);
  }
#endif

  current=WXnameToWidget(dialog,"account");
  gtk_entry_set_text(GTK_ENTRY(current),sd->account);

  current=WXnameToWidget(dialog,"comment");
  gtk_entry_set_text(GTK_ENTRY(current),sd->comment);

  current=WXnameToWidget(dialog,"remdir");
  gtk_entry_set_text(GTK_ENTRY(current),sd->remdir);

  current=WXnameToWidget(dialog,"locdir");
  gtk_entry_set_text(GTK_ENTRY(current),sd->locdir);

  current=WXnameToWidget(dialog,"initcom");
  gtk_entry_set_text(GTK_ENTRY(current),sd->initcom);

  current=WXnameToWidget(dialog,"locmask");
  gtk_entry_set_text(GTK_ENTRY(current),sd->locmask);

  current=WXnameToWidget(dialog,"remmask");
  gtk_entry_set_text(GTK_ENTRY(current),sd->remmask);

  current=WXnameToWidget(dialog,"Save Password");
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(current),sd->save);

  current=WXnameToWidget(dialog,"port");
  gtk_entry_set_text(GTK_ENTRY(current),sd->port);
}

void fetch_session_data(GtkWidget *dialog,session_data* sd){
  GtkWidget *current;
#ifdef OLD_GTK
  char* pass;
#endif

  current=WXnameToWidget(dialog,"profile");
  sd->profile=WXnewstring(gtk_entry_get_text(
	GTK_ENTRY(GTK_COMBO(current)->entry)));
  
  current=WXnameToWidget(dialog,"host");
  sd->host=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  current=WXnameToWidget(dialog,"user");
  sd->user=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  current=WXnameToWidget(dialog,"Anonymous");
  sd->anon=(int)GTK_TOGGLE_BUTTON(current)->active;

  current=WXnameToWidget(dialog,"pass");
#ifdef OLD_GTK
  if(sd->anon){
#endif
    sd->pass=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));
#ifdef OLD_GTK
  } else {
    pass=gtk_object_get_user_data(GTK_OBJECT(current));
    sd->pass=WXnewstring(pass);
  }
#endif

  current=WXnameToWidget(dialog,"account");
  sd->account=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  current=WXnameToWidget(dialog,"comment");
  sd->comment=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  current=WXnameToWidget(dialog,"remdir");
  sd->remdir=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  current=WXnameToWidget(dialog,"locdir");
  sd->locdir=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  current=WXnameToWidget(dialog,"initcom");
  sd->initcom=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  current=WXnameToWidget(dialog,"locmask");
  sd->locmask=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  current=WXnameToWidget(dialog,"remmask");
  sd->remmask=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  current=WXnameToWidget(dialog,"Save Password");
  sd->save=(int)GTK_TOGGLE_BUTTON(current)->active;

  current=WXnameToWidget(dialog,"port");
  sd->port=WXnewstring(gtk_entry_get_text(GTK_ENTRY(current)));

  return;
}

static void action_cb(GtkWidget *w){
  GtkWidget *combo,*list,*item;
  char *s;
  session_data *sd;
  GList *sel;
  int pos,i;

  gtk_label_get(GTK_LABEL(GTK_BUTTON(w)->child),&s);

  if(strcmp(s,"OK")==0){
    gtk_widget_hide(appdata.session);
    gtk_grab_remove(appdata.session);
    appdata.job=1;
    busy_cursor(TRUE);
    process_events();
    request_connection();
    busy_cursor(FALSE);
    appdata.job=0;
  } else if(strcmp(s,"Help")==0){
    show_help(2);
  } else if(strcmp(s,"Cancel")==0){
    gtk_widget_hide(appdata.session);
    gtk_grab_remove(appdata.session);
  } else if(strcmp(s,"Save")==0){
    combo=WXnameToWidget(appdata.session,"profile");
    list=GTK_COMBO(combo)->list;
    sel=GTK_LIST(list)->selection;
    if(sel!=0){
      pos=gtk_list_child_position(GTK_LIST(list),sel->data);
      for(i=0,sd=appdata.sdata;i<pos+1;i++)sd=sd->next;
      fetch_session_data(appdata.session,sd);
      gtk_combo_set_item_string(GTK_COMBO(combo),sel->data,sd->profile);
      /*
      gtk_list_select_child(list,poslist[0],False);
      */
      write_session_data(session_file,appdata.sdata);
    } else {
      appdata.sdata->next=create_session_data(appdata.sdata->next);
      fetch_session_data(appdata.session,appdata.sdata->next);
      item=gtk_list_item_new_with_label(appdata.sdata->next->profile);
      gtk_widget_show(item);
      sel=g_list_append(sel,(gpointer)item);
      gtk_list_prepend_items(GTK_LIST(list),sel);
      gtk_list_select_child(GTK_LIST(list),item);
      write_session_data(session_file,appdata.sdata);
    }
  }
}

static GtkWidget* create_actions(GtkWidget* parent){
  static char* label[]={"OK","Cancel","Save","Help"};
  GtkWidget *actions, *button;
  int i;

  actions=gtk_hbox_new(TRUE,2);
  gtk_container_border_width(GTK_CONTAINER(actions),4);
  gtk_widget_show(actions);

  for(i=0;i<4;i++){
    button=gtk_button_new_with_label(label[i]);
    gtk_widget_show(button);
    gtk_signal_connect(GTK_OBJECT(button),"clicked",
	GTK_SIGNAL_FUNC(action_cb),NULL);
    gtk_box_pack_start(GTK_BOX(actions),button,TRUE,TRUE,1);
  }

  return actions;
}

static int close_cb(GtkWidget* w){
  gtk_widget_hide(w);
  gtk_grab_remove(w);
  return TRUE;
}

GtkWidget* create_session_dialog(GtkWidget *parent){
  GtkWidget *session,*box,*actions;
  GtkWidget *notebook,*general,*startup,*advanced;
  
  session=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_realize(session);
  /*
  gdk_window_set_decorations(session->window,GDK_DECOR_BORDER|GDK_DECOR_TITLE);
  gdk_window_set_functions(session->window,GDK_FUNC_CLOSE|GDK_FUNC_MOVE);
  gtk_window_position(GTK_WINDOW(session),GTK_WIN_POS_NONE);
  */
  gtk_window_set_title(GTK_WINDOW(session),"AxY FTP sessions");

  gtk_signal_connect(GTK_OBJECT(session),"delete_event",
      GTK_SIGNAL_FUNC(close_cb),NULL);

  box=gtk_vbox_new(FALSE,4);
  gtk_widget_show(box);
  gtk_container_add(GTK_CONTAINER(session),box);
  actions=create_actions(box);
  gtk_box_pack_end(GTK_BOX(box),actions,FALSE,FALSE,0);

  notebook=gtk_notebook_new();
  gtk_container_border_width(GTK_CONTAINER(notebook),4);
  gtk_widget_show(notebook);
  gtk_box_pack_end(GTK_BOX(box),notebook,TRUE,TRUE,0);

  general=create_session_general(notebook);

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook),general,
      gtk_label_new("General"));

  startup=create_session_startup(notebook);

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook),startup,
      gtk_label_new("Startup"));

  advanced=create_session_advanced(notebook);

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook),advanced,
      gtk_label_new("Advanced"));

  init_session_dialog(session);

  return session;

}

static void listselect_cb(GtkWidget *w,GtkWidget* item){
  update_session_dialog(gtk_list_child_position(GTK_LIST(w),item)+1,'\0');
  return;
}

void set_profile_strings(GtkWidget* profile){
  GList *list;
  session_data *top;

  list=NULL;
  top=appdata.sdata;
  while(top->next!=NULL){
    top=top->next;
    list=g_list_append(list,top->profile);
  }
  if(list){
    gtk_combo_set_popdown_strings(GTK_COMBO(profile),list);
    g_list_free(list);
  } else {
    gtk_list_clear_items(GTK_LIST(GTK_COMBO(profile)->list),0,-1);
  }

}

static void init_session_dialog(GtkWidget *dialog){
  GtkWidget *profile;
  session_data* top;

  profile=WXnameToWidget(dialog,"profile");

  set_profile_strings(profile);

  top=create_session_data(NULL);
  empty_session_data(top);
  put_session_data(dialog,top,TRUE);
  destroy_session_data(top);

  gtk_signal_connect(GTK_OBJECT(GTK_COMBO(profile)->list),"select_child",
      GTK_SIGNAL_FUNC(listselect_cb),NULL);
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/PushB.h>
#include<Xm/ToggleB.h>
#include<Xm/TextF.h>
#include<Dt/ComboBox.h>
#include<Xm/Label.h>
#include<Xm/List.h>
#if XM_NOTEBOOK
#include<Xm/Notebook.h>
#else
#include<XmAxy/Notebook.h>
#endif

static void init_session_dialog(Widget);
static void action_cb(Widget,XtPointer,XtPointer);

static void request_connection(){
  char* volatile mask;
  volatile int count,ret,total,delay;
  char t[40];
  
  fetch_session_data(appdata.session,appdata.sdata);
  XmTextFieldSetString(appdata.remote.text,appdata.sdata->remmask);
  XmTextFieldSetString(appdata.local.text,appdata.sdata->locmask);
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
}

void update_session_dialog(int pos,char set_profile){
  session_data *sd;
  int i;
  for(i=0,sd=appdata.sdata;i<pos;i++)sd=sd->next;
  put_session_data(appdata.session,sd,set_profile);
}

void put_session_data(Widget dialog,session_data* sd,char set_profile){
  Widget current;
  char* pass;

  if(set_profile){
    current=XtNameToWidget(dialog,"*profile*Text");
    XmTextFieldSetString(current,sd->profile);
  }
  
  current=XtNameToWidget(dialog,"*host");
  XmTextFieldSetString(current,sd->host);

  current=XtNameToWidget(dialog,"*Anonymous");
  XmToggleButtonSetState(current,sd->anon,True);

  current=XtNameToWidget(dialog,"*user");
  XmTextFieldSetString(current,sd->user);

  current=XtNameToWidget(dialog,"*pass");
  XtVaGetValues(current,XmNuserData,&pass,NULL);
  if(sd->anon){
    XmTextFieldSetString(current,sd->pass);
    pass[0]='\0';
  } else {
    XmTextFieldSetString(current,sd->pass);
  }

  current=XtNameToWidget(dialog,"*account");
  XmTextFieldSetString(current,sd->account);

  current=XtNameToWidget(dialog,"*comment");
  XmTextFieldSetString(current,sd->comment);

  current=XtNameToWidget(dialog,"*remdir");
  XmTextFieldSetString(current,sd->remdir);

  current=XtNameToWidget(dialog,"*locdir");
  XmTextFieldSetString(current,sd->locdir);

  current=XtNameToWidget(dialog,"*initcom");
  XmTextFieldSetString(current,sd->initcom);

  current=XtNameToWidget(dialog,"*locmask");
  XmTextFieldSetString(current,sd->locmask);

  current=XtNameToWidget(dialog,"*remmask");
  XmTextFieldSetString(current,sd->remmask);

  current=XtNameToWidget(dialog,"*Save Password");
  XmToggleButtonSetState(current,sd->save,False);

  current=XtNameToWidget(dialog,"*port");
  XmTextFieldSetString(current,sd->port);

  XmUpdateDisplay(toplevel);
}

void fetch_session_data(Widget dialog,session_data* sd){
  Widget current;
  char* pass;

  current=XtNameToWidget(dialog,"*profile*Text");
  sd->profile=XmTextFieldGetString(current);
  
  current=XtNameToWidget(dialog,"*host");
  sd->host=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*user");
  sd->user=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*Anonymous");
  sd->anon=XmToggleButtonGetState(current);

  current=XtNameToWidget(dialog,"*pass");
  if(sd->anon){
    sd->pass=XmTextFieldGetString(current);
  } else {
    XtVaGetValues(current,XmNuserData,&pass,NULL);
    sd->pass=XtNewString(pass);
  }

  current=XtNameToWidget(dialog,"*account");
  sd->account=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*comment");
  sd->comment=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*remdir");
  sd->remdir=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*locdir");
  sd->locdir=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*initcom");
  sd->initcom=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*locmask");
  sd->locmask=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*remmask");
  sd->remmask=XmTextFieldGetString(current);

  current=XtNameToWidget(dialog,"*Save Password");
  sd->save=XmToggleButtonGetState(current);

  current=XtNameToWidget(dialog,"*port");
  sd->port=XmTextFieldGetString(current);

  return;
}

static void action_cb(Widget w,XtPointer app,XtPointer call){
  XmString xms,item;
  String s;
  Widget list,combo;
  int * poslist;
  int poscount,i;
  session_data *sd;

  XtVaGetValues(w,XmNlabelString,&xms,NULL);
  if(XmStringGetLtoR(xms,XmFONTLIST_DEFAULT_TAG,&s)){
    if(strcmp(s,"OK")==0){
      XtUnmanageChild(appdata.session);
      appdata.job=1;
      busy_cursor(True);
      process_events();
      request_connection();
      busy_cursor(False);
      appdata.job=0;
    } else if(strcmp(s,"Help")==0){
      show_help(2);
    } else if(strcmp(s,"Cancel")==0){
      XtUnmanageChild(appdata.session);
    } else if(strcmp(s,"Save")==0){
      combo=XtNameToWidget(appdata.session,"*profile");
      list=XtNameToWidget(combo,"*List");
      if(XmListGetSelectedPos(list,&poslist,&poscount)){
	for(i=0,sd=appdata.sdata;i<poslist[0];i++)sd=sd->next;
	fetch_session_data(appdata.session,sd);
	item=XmStringCreateLocalized(sd->profile);
	XmListReplaceItemsPos(list,&item,1,poslist[0]);
	XmListUpdateSelectedList(list);
	XmListSelectPos(list,poslist[0],False);
	XmStringFree(item);
	XtFree((char*)poslist);
        write_session_data(session_file,appdata.sdata);
      } else {
	appdata.sdata->next=create_session_data(appdata.sdata->next);
	fetch_session_data(appdata.session,appdata.sdata->next);
	item=XmStringCreateLocalized(appdata.sdata->next->profile);
	XtVaGetValues(list,XmNitemCount,&i,NULL);
	DtComboBoxAddItem(combo,item,1,FALSE);
	i++;
	if(i<=MAX_VISIBLE_ITEMS){
	  XtVaSetValues(combo,DtNvisibleItemCount,i,NULL);
	  XtVaSetValues(list,XmNvisibleItemCount,i,NULL);
	}
	XmStringFree(item);
	XmListSelectPos(list,1,TRUE);
        write_session_data(session_file,appdata.sdata);
      }
    } 
    XtFree(s);
  }
  XmStringFree(xms);
}

static Widget create_actions(Widget parent){
  static String label[]={"OK","Cancel","Save","Help"};
  Widget actions,button;
  XmString xml;
  int i;
  Arg args[10];
  Cardinal n;

  n=0;
  XtSetArg(args[n],XmNfractionBase,XtNumber(label));n++;
  actions=XmCreateForm(parent,"Actions",args,n);
  XtManageChild(actions);

  for(i=0;i<XtNumber(label);i++){
    xml=XmStringCreateLocalized(label[i]);

    n=0;
    XtSetArg(args[n],XmNlabelString,xml);n++;
    XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
    XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
    XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
    XtSetArg(args[n],XmNleftPosition,i);n++;
    XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
    XtSetArg(args[n],XmNrightPosition,i+1);n++;
    button=XmCreatePushButton(actions,label[i],args,n);
    XtManageChild(button);
    XtAddCallback(button,XmNactivateCallback,action_cb,NULL);

    XmStringFree(xml);
  }
  
  return actions;
}

Widget create_session_dialog(Widget parent){
  Widget session;
  Arg args[10];
  Cardinal n;
  Widget actions,notebook,general,startup,advanced;
  
  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP sessions");n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  XtSetArg(args[n],XmNverticalSpacing,6);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,6);n++;
  session=XmCreateFormDialog(parent,"session",args,n);

  actions=create_actions(session);

  n=0;
#if XM_NOTEBOOK 
  XtSetArg(args[n],XmNbackPagePlacement,XmTOP_RIGHT);n++;
  XtSetArg(args[n],XmNmajorTabSpacing,0);n++;
  XtSetArg(args[n],XmNorientation,XmVERTICAL);n++;
  XtSetArg(args[n],XmNbindingType,XmNONE);n++;
  XtSetArg(args[n],XmNbackPageNumber,1);n++;
  XtSetArg(args[n],XmNbackPageSize,0);n++;
  notebook=XmCreateNotebook(session,"notebook",args,n);
#else
  notebook=XmAxyCreateNotebook(session,"notebook",args,n);
#endif
  XtManageChild(notebook);

#if XM_NOTEBOOK 
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmPAGE_SCROLLER);n++;
  XtManageChild(XmCreateLabel(notebook,"",args,n));
#endif  

  general=create_session_general(notebook);
  XtManageChild(general);

#if XM_NOTEBOOK 
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"General",args,n));
#endif

  n=0;
  startup=create_session_startup(notebook);
  XtManageChild(startup);

#if XM_NOTEBOOK 
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"Startup",args,n));
#endif

  n=0;
  advanced=create_session_advanced(notebook);
  XtManageChild(advanced);

#if XM_NOTEBOOK 
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(notebook,"Advanced",args,n));
#endif


  XtVaSetValues(actions,
      XmNbottomAttachment,XmATTACH_FORM,
      XmNrightAttachment,XmATTACH_FORM,
      XmNtopAttachment,XmATTACH_NONE,
      XmNleftAttachment,XmATTACH_FORM,NULL);

  XtVaSetValues(notebook,
      XmNbottomAttachment,XmATTACH_WIDGET,
      XmNbottomWidget,actions,
      XmNrightAttachment,XmATTACH_FORM,
      XmNtopAttachment,XmATTACH_FORM,
      XmNleftAttachment,XmATTACH_FORM,NULL);

#if !XM_NOTEBOOK
  XmAxyNotebookSetCurrentPage(notebook,1,FALSE);
#endif
  init_session_dialog(session);
  
  return session;

}

void set_profile_strings(Widget combo){
  Widget list;
  session_data* top;
  XmString xms;
  int i;

  list=XtNameToWidget(combo,"*List");

  XmListDeleteAllItems(list);
  
  top=appdata.sdata;
  while(top->next!=NULL){
    top=top->next;
    xms=XmStringCreateLocalized(top->profile);
    DtComboBoxAddItem(combo,xms,0,FALSE);
    XmStringFree(xms);
  }
  XtVaGetValues(list,XmNitemCount,&i,NULL);
  if(i>MAX_VISIBLE_ITEMS)i=MAX_VISIBLE_ITEMS;
  XtVaSetValues(combo,DtNvisibleItemCount,i>0?i:1,NULL);
  XtVaSetValues(list,XmNvisibleItemCount,i>0?i:1,NULL);

}

static void init_session_dialog(Widget dialog){
  Widget combo;
  session_data* top;

  combo=XtNameToWidget(dialog,"*profile");

  set_profile_strings(combo);

  top=create_session_data(NULL);
  empty_session_data(top);
  put_session_data(dialog,top,TRUE);
  destroy_session_data(top);
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
