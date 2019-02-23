/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"utils.h"
#include"functions.h"
#include"little_dialogs.h"
#include"dirname.h"

#define MAX_VISIBLE_ITEMS 10

#ifdef BUILD_GTK

#include<gtk/gtk.h>

void clear_dirname(GtkWidget* w){
  gtk_list_clear_items(GTK_LIST(GTK_COMBO(w)->list),0,-1);
  gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(w)->entry),"");
}

static void dirname_cb(GtkWidget* w,GtkWidget* item,gpointer app){
  int which,ret;
  char *s,*mask;
  which=(int)app;

  gtk_widget_hide(gtk_widget_get_toplevel(w));
  process_events();
  gtk_label_get(GTK_LABEL(GTK_BIN(item)->child),&s);
  if(s!=NULL){
    switch(which){
      case LOCAL:
        if(strcmp(appdata.local.list->dir,s)==0)break;
        busy_cursor(TRUE);
	/*
        XtCallActionProc(w,"CBCancel",NULL,NULL,0);
        xms=XmStringCreateLocalized(appdata.local.list->dir);
        XmListSelectItem(list,xms,False);
        XmStringFree(xms);
	*/
        ret=chg_local_dirname(s);
        if(!ret){
          mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
          update_local(mask);
        }
        if(!appdata.job)busy_cursor(FALSE);
        break;
      case REMOTE:
	/*
        XtCallActionProc(w,"CBCancel",NULL,NULL,0);
        XmUpdateDisplay(toplevel);
	*/
        if(appdata.remote.list){
          if(strcmp(appdata.remote.list->dir,s)==0)break;
	  /*
          xms=XmStringCreateLocalized(appdata.local.list->dir);
          XmListSelectItem(list,xms,False);
          XmStringFree(xms);
	  */
        }
        if(!appdata.connected){
          (void)popup_warning_dialog(toplevel,"Not Connected");
          break;
        }
        if(!appdata.job){
          appdata.job=5;
          busy_cursor(TRUE);
          ret=chg_remote_dirname(s);
          if(!ret){
            mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));
            update_remote(mask);
          }
          busy_cursor(FALSE);
          appdata.job=0;
        }
        break;
      default:
        break;
    }
    return;
  }
}

static int dirtext_cb(GtkWidget* w,gpointer app){
  int which,ret;
  char* s;

  which=(int)app;
  s=gtk_entry_get_text(GTK_ENTRY(w));

  switch(which){
    case LOCAL:
      gtk_widget_hide(gtk_widget_get_toplevel(
	    GTK_WIDGET(GTK_COMBO(appdata.local.combo)->list)));
      process_events();
      if(strcmp(appdata.local.list->dir,s)==0)break;
      busy_cursor(TRUE);
      /*
      XtCallActionProc(w,"CBCancel",NULL,NULL,0);
      xms=XmStringCreateLocalized(appdata.local.list->dir);
      XmListSelectItem(list,xms,False);
      XmStringFree(xms);
      */
      ret=chg_local_dirname(s);
      if(!ret){
	char* mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
	update_local(mask);
      }
      if(!appdata.job)busy_cursor(FALSE);
      break;
    case REMOTE:
      gtk_widget_hide(gtk_widget_get_toplevel(
	    GTK_WIDGET(GTK_COMBO(appdata.remote.combo)->list)));
      process_events();
      /*
      XtCallActionProc(w,"CBCancel",NULL,NULL,0);
      XmUpdateDisplay(toplevel);
      */
      if(appdata.remote.list){
	if(strcmp(appdata.remote.list->dir,s)==0)break;
	/*
	xms=XmStringCreateLocalized(appdata.local.list->dir);
	XmListSelectItem(list,xms,False);
	XmStringFree(xms);
	*/
      }
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not Connected");
	break;
      }
      if(!appdata.job){
	appdata.job=5;
	busy_cursor(TRUE);
	ret=chg_remote_dirname(s);
	if(!ret){
	  char* mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));
	  update_remote(mask);
	}
	busy_cursor(FALSE);
	appdata.job=0;
      }
      break;
    default:
      break;
  }
  
  return TRUE;
}
  
void update_dirname(GtkWidget* combo,dirinfo *di){
  char* s;
  int found;
  GtkWidget* item;
  GList* items;

  items=GTK_LIST(GTK_COMBO(combo)->list)->children;
  found=0;
  while(items!=NULL){
    gtk_label_get(GTK_LABEL(GTK_BIN(items->data)->child),&s);
    if(strcmp(di->dir,s)==0){
      found=1;
      break;
    }
    items=items->next;
  }

  if(!found){
    item=gtk_list_item_new_with_label(di->dir);
    gtk_widget_show(item);
    items=(GList*)WXmalloc(sizeof(GList));
    items->next=NULL;
    items->prev=NULL;
    items->data=(gpointer)item;
    gtk_list_prepend_items(GTK_LIST(GTK_COMBO(combo)->list),items);
  }
  gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),di->dir);
  return;
}


GtkWidget* create_dirname(GtkWidget* parent,int which){
  GtkWidget *dirname;

  dirname=gtk_combo_new();
  gtk_widget_set_usize(GTK_COMBO(dirname)->entry,0,gtkfontheight*2);
  gtk_widget_show(dirname);

  switch(which){
    case LOCAL:
      appdata.local.combo=dirname;
      break;
    case REMOTE:
      appdata.remote.combo=dirname;
      break;
    default:
      break;
  }

  gtk_signal_connect(GTK_OBJECT(GTK_COMBO(dirname)->list),"select_child",
    GTK_SIGNAL_FUNC(dirname_cb),(gpointer)which);

  gtk_signal_connect(GTK_OBJECT(GTK_COMBO(dirname)->entry),"activate",
    GTK_SIGNAL_FUNC(dirtext_cb),(gpointer)which);

  return dirname;

}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/RowColumn.h>
#include<Dt/ComboBox.h>
#include<Xm/TextF.h>
#include<Xm/List.h>

void clear_dirname(Widget w){
  Widget list,text;

  list=XtNameToWidget(w,"*List");
  text=XtNameToWidget(w,"*Text");
  /*XtVaGetValues(w,DtNtextField,&child,NULL);*/

  XmTextFieldSetString(text,"");

  /*XtVaGetValues(w,DtNlist,&child,NULL);*/
  XmListDeleteAllItems(list);
  /*
  XtVaSetValues(w,DtNvisibleItemCount,1,NULL);
  XtVaSetValues(list,XmNvisibleItemCount,1,NULL);
  */
  /*XmComboBoxUpdate(w);*/
}

void dirfield_cb(Widget w,XtPointer app,XtPointer call){
  XmAnyCallbackStruct *cbs;
  int which,ret;
  String s,mask;
  Widget text,list,combo;
  /*XmString xms;*/
  cbs=(XmAnyCallbackStruct*)call;
  which=(int)app;
  
  if(cbs->event==NULL || 
     cbs->event->type==ButtonPress || cbs->event->type==ButtonRelease){
    return;
  } 

  text=w;
  do {
    combo=XtParent(w);
  } while(!XtIsSubclass(combo,dtComboBoxWidgetClass));
  list=XtNameToWidget(combo,"*List");
  /*XtVaGetValues(w,DtNlist,&list,DtNtextField,&text,NULL);*/

  s=XmTextFieldGetString(text);

  switch(which){
    case LOCAL:
      if(strcmp(appdata.local.list->dir,s)==0) {
	int p=XmTextFieldGetLastPosition(text);
	XmTextFieldShowPosition(text,p);
	XmTextFieldSetInsertionPosition(text,p);
	break;
      }
      busy_cursor(True);
      /*XtCallActionProc(list,"ListKbdCancel",NULL,NULL,0);*/
      /*XtCallActionProc(w,"ComboBoxKbdCancel",NULL,NULL,0);*/
      /*XmUpdateDisplay(toplevel);*/
      /*xms=XmStringCreateLocalized(appdata.local.list->dir);*/
      /*DtComboBoxSelectItem(w,xms);*/
      /*XmListSelectItem(list,xms,False);*/
      /*XmStringFree(xms);*/
      ret=chg_local_dirname(s);
      if(!ret){
	mask=XmTextFieldGetString(appdata.local.text);
	update_local(mask);
	XtFree(mask);
      }
      if(!appdata.job)busy_cursor(False);
      break;
    case REMOTE:
      /*XtCallActionProc(list,"ListKbdCancel",NULL,NULL,0);*/
      /*XtCallActionProc(w,"ComboBoxKbdCancel",NULL,NULL,0);*/
      /*XmUpdateDisplay(toplevel);*/
      if(appdata.remote.list){
	if(strcmp(appdata.remote.list->dir,s)==0){
	  int p=XmTextFieldGetLastPosition(text);
	  XmTextFieldShowPosition(text,p);
	  XmTextFieldSetInsertionPosition(text,p);
	  break;
	}
	/*xms=XmStringCreateLocalized(appdata.local.list->dir);*/
	/*XmListSelectItem(list,xms,False);*/
	/*XmStringFree(xms);*/
      }
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not Connected");
	break;
      }
      if(!appdata.job){
	appdata.job=5;
	busy_cursor(True);
	ret=chg_remote_dirname(s);
	if(!ret){
	  mask=XmTextFieldGetString(appdata.remote.text);
	  update_remote(mask);
	  XtFree(mask);
	}
	busy_cursor(False);
	appdata.job=0;
      }
      break;
    default:
      break;
  }
  XtFree(s);
}
  
void dirname_cb(Widget w,XtPointer app,XtPointer call){
  DtComboBoxCallbackStruct *cbs;
  int which,ret;
  String s,mask;
  Widget text,list;
  /*XmString xms;*/
  cbs=(DtComboBoxCallbackStruct*)call;
  which=(int)app;
  

  list=XtNameToWidget(w,"*List");
  text=XtNameToWidget(w,"*Text");
  /*XtVaGetValues(w,DtNlist,&list,DtNtextField,&text,NULL);*/
  
  if(XmStringGetLtoR(cbs->item_or_text,XmFONTLIST_DEFAULT_TAG,&s)){

    switch(which){
      case LOCAL:
	if(strcmp(appdata.local.list->dir,s)==0){
	  int p=XmTextFieldGetLastPosition(text);
	  XmTextFieldShowPosition(text,p);
	  XmTextFieldSetInsertionPosition(text,p);
	  XmUpdateDisplay(text);
	  break;
	}
        busy_cursor(True);
	/*XtCallActionProc(list,"ListKbdCancel",NULL,NULL,0);*/
	/*XtCallActionProc(w,"ComboBoxKbdCancel",NULL,NULL,0);*/
	/*XmUpdateDisplay(toplevel);*/
	/*xms=XmStringCreateLocalized(appdata.local.list->dir);*/
	/*DtComboBoxSelectItem(w,xms);*/
	/*XmListSelectItem(list,xms,False);*/
	/*XmStringFree(xms);*/
	ret=chg_local_dirname(s);
        if(!ret){
	  mask=XmTextFieldGetString(appdata.local.text);
          update_local(mask);
	  XtFree(mask);
	}
	if(!appdata.job)busy_cursor(False);
	break;
      case REMOTE:
	/*XtCallActionProc(list,"ListKbdCancel",NULL,NULL,0);*/
	/*XtCallActionProc(w,"ComboBoxKbdCancel",NULL,NULL,0);*/
	/*XmUpdateDisplay(toplevel);*/
	if(appdata.remote.list){
	  if(strcmp(appdata.remote.list->dir,s)==0){
	    int p=XmTextFieldGetLastPosition(text);
	    XmTextFieldShowPosition(text,p);
	    XmTextFieldSetInsertionPosition(text,p);
	    break;
	  }
	  /*xms=XmStringCreateLocalized(appdata.local.list->dir);*/
	  /*XmListSelectItem(list,xms,False);*/
	  /*XmStringFree(xms);*/
	}
        if(!appdata.connected){
	  (void)popup_warning_dialog(toplevel,"Not Connected");
	  break;
	}
	if(!appdata.job){
	  appdata.job=5;
	  busy_cursor(True);
	  ret=chg_remote_dirname(s);
	  if(!ret){
	    mask=XmTextFieldGetString(appdata.remote.text);
	    update_remote(mask);
	    XtFree(mask);
	  }
	  busy_cursor(False);
	  appdata.job=0;
	}
	break;
      default:
	break;
    }
    XtFree(s);
    return;
  }
}
  
void update_dirname(Widget combo,dirinfo *di){
  Widget text,list;
  XmString item;
  XmStringTable xmt;
  int count,i;
  XmTextPosition p;

  list=XtNameToWidget(combo,"*List");
  text=XtNameToWidget(combo,"*Text");

  item=XmStringCreateLocalized(di->dir);
  XtVaGetValues(list,XmNitems,&xmt,XmNitemCount,&count,NULL);
  for(i=0;i<count;i++){
    if(XmStringByteCompare(xmt[i],item))break;
  }
  if(i==count){
    DtComboBoxAddItem(combo,item,1,FALSE);
    /*XmListAddItemUnselected(list,item,1);*/
    count++;
    if(count<=MAX_VISIBLE_ITEMS){
      XtVaSetValues(combo,DtNvisibleItemCount,count,NULL);
      XtVaSetValues(list,XmNvisibleItemCount,count,NULL);
    }
  }
  XmListSelectItem(list,item,False);
  XmStringFree(item);
  XmTextFieldSetString(text,di->dir);
  p=XmTextFieldGetLastPosition(text);
  XmTextFieldShowPosition(text,p);
  XmTextFieldSetInsertionPosition(text,p);
  /*XmComboBoxUpdate(combo);*/
  return;
}

Widget create_dirname(Widget parent,int which){
  Arg args[10];
  Cardinal n;
  Widget dirname,list,text;
  Pixel white_pixel,black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],DtNcomboBoxType,DtDROP_DOWN_COMBO_BOX);n++;
  dirname=DtCreateComboBox(parent,"dirname",args,n);
  XtManageChild(dirname);
  XtAddCallback(dirname,DtNselectionCallback,dirname_cb,(XtPointer)which);

  list=XtNameToWidget(dirname,"*List");
  text=XtNameToWidget(dirname,"*Text");

  n=0; 
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNmarginWidth,3);n++;/* Motif 2.x workaround */
  XtSetArg(args[n],XmNmarginHeight,3);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetValues(text,args,n);
  XtAddCallback(text,XmNactivateCallback,dirfield_cb,(XtPointer)which);


  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNselectionPolicy,XmBROWSE_SELECT);n++;
  XtSetValues(list,args,n);

  switch(which){
    case LOCAL:
      appdata.local.combo=dirname;
      break;
    case REMOTE:
      appdata.remote.combo=dirname;
      break;
    default:
      break;
  }

  return dirname;

}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
