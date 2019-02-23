/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>

#include"session_general.h"
#include"session_dialog.h"
#include"axyftp.h"
#include"utils.h"
#include"read_init.h"

#define MAXPASS 40

#ifdef BUILD_GTK

#include<gtk/gtk.h>

static void listaction_cb(GtkWidget* w){
  GtkWidget *list,*combo;
  GList *sel;
  session_data *sd,*temp;
  char *s;
  int i,pos;

  gtk_label_get(GTK_LABEL(GTK_BUTTON(w)->child),&s);

  if(strcmp(s,"New")==0){
    combo=WXnameToWidget(appdata.session,"profile");
    list=GTK_COMBO(combo)->list;
    sd=appdata.sdata;
    set_profile_strings(combo);
    /*
    i=0;
    while(sd->next!=NULL){
      gtk_list_unselect_item(GTK_LIST(list),i);
      sd=sd->next;
      i++;
    }
    */
    sd=create_session_data(NULL);
    empty_session_data(sd);
    put_session_data(appdata.session,sd,(char)1);
    destroy_session_data(sd);
  } else if(strcmp(s,"Delete")==0){
    combo=WXnameToWidget(appdata.session,"profile");
    list=GTK_COMBO(combo)->list;
    sel=GTK_LIST(list)->selection;
    if(sel!=NULL){
      pos=gtk_list_child_position(GTK_LIST(list),sel->data);
      for(i=0,sd=appdata.sdata;i<pos;i++)sd=sd->next;
      temp=sd->next;
      sd->next=temp->next;
      temp->next=NULL;
      destroy_session_data(temp);

      set_profile_strings(combo);
      
      /*
      sel=g_list_append(NULL,sel->data);
      gtk_list_unselect_all(GTK_LIST(list));
      gtk_list_remove_items(GTK_LIST(list),sel);
      gtk_list_select_all(GTK_LIST(list));
      gtk_widget_destroy(sel->data);
      g_list_free(sel);
      */
      sd=create_session_data(NULL);
      empty_session_data(sd);
      put_session_data(appdata.session,sd,(char)1);
      destroy_session_data(sd);
      write_session_data(session_file,appdata.sdata);
    }
  }
}

static void anon_cb(GtkWidget *w,gpointer x){
  GtkWidget *user;
  GtkWidget *passfield;
  char *pass;
#ifdef OLD_GTK
  char* stars;
#endif

  user=(GtkWidget*)x;
  passfield=(GtkWidget*)gtk_object_get_user_data(GTK_OBJECT(user));
  pass=(char*)gtk_object_get_user_data(GTK_OBJECT(passfield));

  if(GTK_TOGGLE_BUTTON(w)->active){
    gtk_object_set_user_data(GTK_OBJECT(w),(gpointer)TRUE);
#ifndef OLD_GTK
    strcpy(pass,gtk_entry_get_text(GTK_ENTRY(passfield)));
    gtk_entry_set_visibility(GTK_ENTRY(passfield),TRUE);
#endif
    gtk_entry_set_text(GTK_ENTRY(passfield),appdata.odata->anonpass);
    gtk_entry_set_text(GTK_ENTRY(user),"anonymous");
  } else {
#ifdef OLD_GTK
    gtk_object_set_user_data(GTK_OBJECT(w),(gpointer)TRUE);
#endif
    gtk_entry_set_text(GTK_ENTRY(passfield),pass);
#ifndef OLD_GTK
    gtk_entry_set_visibility(GTK_ENTRY(passfield),FALSE);
#else
    stars=WXmalloc(strlen(pass)+1);
    memset(stars,'*',strlen(pass));
    stars[strlen(pass)]='\0';
    gtk_entry_set_text(GTK_ENTRY(passfield),stars);
    WXfree(stars);
#endif
    gtk_object_set_user_data(GTK_OBJECT(w),(gpointer)FALSE);
  }

}

#ifdef OLD_GTK
static void password_cb(GtkWidget* w,gpointer x){
  GtkWidget *anon;
  char* pass,*text;
  int state,end,length,start;

  pass=(char*)gtk_object_get_user_data(GTK_OBJECT(w));
  anon=(GtkWidget*)x;
  state=(int)gtk_object_get_user_data(GTK_OBJECT(anon));
  start=GTK_EDITABLE(w)->current_pos;
  text=gtk_entry_get_text(GTK_ENTRY(w));

  if(state){
    return;
  }

  length=strlen(text)-strlen(pass);
  if(length==0)return;

  if(length>0){
    end=start+length;
    memmove(&pass[end],&pass[start],strlen(pass)-start+1);
    memmove(&pass[start],&text[start],length);
    memset(&text[start],'*',length);
  } else {
    end=start-length;
    memmove(&pass[start],&pass[end],strlen(pass)-start+1);
  }

  return;
}
#endif

GtkWidget* create_session_general(GtkWidget *parent){
  static char* label[]={
    "Profile Name:",
    "Host Name/Address:",
    "User ID:",
    "Password:",
    "Account:",
    "Comment:"
  };
  GtkWidget *general,*table;
  GtkWidget *current,*child,*user,*passfield,*anontoggle;
  char* passbuf;
  int i;

  general=gtk_vbox_new(FALSE,8);
  gtk_container_border_width(GTK_CONTAINER(general),6);
  gtk_widget_show(general);

  table=gtk_table_new(5,3,TRUE);
  gtk_widget_show(table);
  gtk_box_pack_start(GTK_BOX(general),table,TRUE,TRUE,0);

  gtk_table_set_col_spacing(GTK_TABLE(table),0,8);

  for(i=0;i<5;i++){
    if(i<4)gtk_table_set_row_spacing(GTK_TABLE(table),i,8);
    child=gtk_alignment_new(1,1,0,1);
    gtk_widget_show(child);
    gtk_table_attach_defaults(GTK_TABLE(table),child,0,1,i,i+1);
    current=gtk_label_new(label[i]);
    gtk_widget_show(current);
    gtk_container_add(GTK_CONTAINER(child),current);
  }

  child=gtk_combo_new();
  gtk_widget_set_usize(GTK_COMBO(child)->entry,0,gtkfontheight*2);
  gtk_widget_set_name(child,"profile");
  gtk_widget_show(child);
  gtk_table_attach_defaults(GTK_TABLE(table),child,1,2,0,1);

  child=gtk_entry_new();
  gtk_widget_set_name(child,"host");
  gtk_widget_set_usize(child,0,gtkfontheight*2);
  gtk_widget_show(child);
  gtk_table_attach_defaults(GTK_TABLE(table),child,1,2,1,2);

  user=gtk_entry_new();
  gtk_widget_set_usize(user,0,gtkfontheight*2);
  gtk_widget_set_name(user,"user");
  gtk_widget_show(user);
  gtk_table_attach_defaults(GTK_TABLE(table),user,1,2,2,3);

  passbuf=WXmalloc(MAXPASS+1);
  passbuf[0]='\0';
  passfield=gtk_entry_new_with_max_length(MAXPASS);
  gtk_widget_set_usize(passfield,0,gtkfontheight*2);
  gtk_widget_set_name(passfield,"pass");
  gtk_widget_show(passfield);
  gtk_table_attach_defaults(GTK_TABLE(table),passfield,1,2,3,4);
  gtk_object_set_user_data(GTK_OBJECT(passfield),(gpointer)passbuf);
  gtk_object_set_user_data(GTK_OBJECT(user),(gpointer)passfield);
#ifndef OLD_GTK
  gtk_entry_set_visibility(GTK_ENTRY(passfield),FALSE);
#endif
  
  child=gtk_entry_new();
  gtk_widget_set_usize(child,0,gtkfontheight*2);
  gtk_widget_set_name(child,"account");
  gtk_widget_show(child);
  gtk_table_attach_defaults(GTK_TABLE(table),child,1,2,4,5);

  current=gtk_alignment_new(0.5,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,2,3,0,1);
  child=gtk_button_new_with_label("New");
  gtk_widget_set_usize(child,
      gdk_string_width(mystyle->font,"Delete")+8,gtkfontheight*2);
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);
  gtk_signal_connect(GTK_OBJECT(child),"clicked",
      GTK_SIGNAL_FUNC(listaction_cb),NULL);
  
  current=gtk_alignment_new(0.5,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,2,3,1,2);
  child=gtk_button_new_with_label("Delete");
  gtk_widget_set_usize(child,
      gdk_string_width(mystyle->font,"Delete")+8,gtkfontheight*2);
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);
  gtk_signal_connect(GTK_OBJECT(child),"clicked",
      GTK_SIGNAL_FUNC(listaction_cb),NULL);
  
  current=gtk_alignment_new(0.5,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,2,3,2,3);
  anontoggle=gtk_check_button_new_with_label("Anonymous    ");
  gtk_widget_set_name(anontoggle,"Anonymous");
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(anontoggle),FALSE);
  gtk_object_set_user_data(GTK_OBJECT(anontoggle),(gpointer)FALSE);
  gtk_widget_show(anontoggle);
  gtk_container_add(GTK_CONTAINER(current),anontoggle);
  
  current=gtk_alignment_new(0.5,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,2,3,3,4);
  child=gtk_check_button_new_with_label("Save Password");
  gtk_widget_set_name(child,"Save Password");
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(child),FALSE);
  
  table=gtk_hbox_new(FALSE,0);
  gtk_widget_show(table);
  gtk_box_pack_start(GTK_BOX(general),table,FALSE,TRUE,0);

  current=gtk_label_new(label[5]);
  gtk_widget_show(current);
  gtk_box_pack_start(GTK_BOX(table),current,FALSE,FALSE,4);
  
  current=gtk_entry_new();
  gtk_widget_set_usize(current,0,gtkfontheight*2);
  gtk_widget_set_name(current,"comment");
  gtk_widget_show(current);
  gtk_box_pack_start(GTK_BOX(table),current,TRUE,TRUE,4);

#ifdef OLD_GTK
  gtk_signal_connect(GTK_OBJECT(passfield),"changed",
      GTK_SIGNAL_FUNC(password_cb),(gpointer)anontoggle);
#endif
  gtk_signal_connect(GTK_OBJECT(anontoggle),"toggled",
      GTK_SIGNAL_FUNC(anon_cb),(gpointer)user);
  return general;
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

static void password_cb(Widget,XtPointer,XtPointer);
static void anon_cb(Widget,XtPointer,XtPointer);
static void listaction_cb(Widget,XtPointer,XtPointer);
static void listselect_cb(Widget,XtPointer,XtPointer);

static void listselect_cb(Widget w,XtPointer app,XtPointer call){
  DtComboBoxCallbackStruct *cbs;
  int* pos;
  int num;
  Widget list;

  cbs=(DtComboBoxCallbackStruct*)call;
  list=XtNameToWidget(w,"*List");

  if(XmListGetSelectedPos(list,&pos,&num)){
    update_session_dialog(pos[0],'\0');
  }
  return;
}

static void listaction_cb(Widget w,XtPointer app,XtPointer call){
  XmString xms;
  String s;
  Widget list,combo;
  session_data *sd,*temp;
  int *poslist;
  int poscount,i;

  XtVaGetValues(w,XmNlabelString,&xms,NULL);

  if(XmStringGetLtoR(xms,XmFONTLIST_DEFAULT_TAG,&s)){
    if(strcmp(s,"New")==0){
      list=XtNameToWidget(appdata.session,"*profile*List");
      if(XmListGetSelectedPos(list,&poslist,&poscount))
			  XmListDeselectPos(list,poslist[0]);
      /*
      XmListGetSelectedPos(list,&poslist,&poscount);
      combo=XtNameToWidget(appdata.session,"*profile");
      XtVaGetValues(combo,DtNselectedPosition,&i,NULL);
      printf("Selected positions after deselect: %d %d\n",poscount,i);
      */
      sd=create_session_data(NULL);
      empty_session_data(sd);
      put_session_data(appdata.session,sd,(char)1);
      destroy_session_data(sd);
    } else if(strcmp(s,"Delete")==0){
      combo=XtNameToWidget(appdata.session,"*profile");
      list=XtNameToWidget(combo,"*List");
      if(XmListGetSelectedPos(list,&poslist,&poscount)){
        for(i=0,sd=appdata.sdata;i<poslist[0]-1;i++)sd=sd->next;
	temp=sd->next;
	sd->next=temp->next;
	temp->next=NULL;
	destroy_session_data(temp);

	set_profile_strings(combo);
	/*
	XtVaGetValues(list,XmNitemCount,&i,NULL);
	DtComboBoxDeletePos(list,poslist[0]);
	i--;
	if(i<=MAX_VISIBLE_ITEMS){
	  XtVaSetValues(combo,DtNvisibleItemCount,i>0?i:1,NULL);
	  XtVaSetValues(list,XmNvisibleItemCount,i>0?i:1,NULL);
	}
	*/
	/*XmListDeselectAllItems(list);*/
	sd=create_session_data(NULL);
	empty_session_data(sd);
	put_session_data(appdata.session,sd,(char)1);
	destroy_session_data(sd);
	write_session_data(session_file,appdata.sdata);
      }
    }
    XtFree(s);
  }
  XmStringFree(xms);
}

static void anon_cb(Widget w,XtPointer app,XtPointer call){
  XmToggleButtonCallbackStruct *cbs;
  Widget passfield;
  char *pass,*save,*anon;
  char* stars;

  cbs=(XmToggleButtonCallbackStruct*)call;
  passfield=(Widget)app;
  XtVaGetValues(passfield,XmNuserData,&pass,NULL);

  if(cbs->set){
    XmTextFieldSetString(passfield,appdata.odata->anonpass);
    passfield=XtNameToWidget(appdata.session,"*user");
    XmTextFieldSetString(passfield,"anonymous");
  } else {
    stars=XtMalloc(strlen(pass)+1);
    memset(stars,'*',strlen(pass));
    stars[strlen(pass)]='\0';
    save=XtNewString(pass);
    anon=XmTextFieldGetString(passfield);
    strcpy(pass,anon);
    XmTextFieldSetString(passfield,stars);
    strcpy(pass,save);
    XtFree(anon);
    XtFree(save);
    XtFree(stars);
  }

}

static void password_cb(Widget w,XtPointer app,XtPointer call){
  XmTextVerifyCallbackStruct *cbs;
  Widget anon;
  char* pass;

  cbs=(XmTextVerifyCallbackStruct*)call;
  anon=(Widget)app;
  XtVaGetValues(w,XmNuserData,&pass,NULL);

  if(XmToggleButtonGetState(anon)){
    return;
  }

  if(cbs->startPos<cbs->endPos){
    memmove(&pass[cbs->startPos],&pass[cbs->endPos],
	strlen(pass)-cbs->endPos+1);
  }
  if(cbs->text->length==0){
    return;
  }
  memmove(&pass[cbs->startPos+cbs->text->length],&pass[cbs->startPos],
	strlen(pass)-cbs->startPos+1);
  memmove(&pass[cbs->startPos],cbs->text->ptr,cbs->text->length);
  memset(cbs->text->ptr,'*',cbs->text->length);
  return;
    
}

Widget create_session_general(Widget parent){
  static String label[]={
    "Profile Name:",
    "Host Name/Address:",
    "User ID:",
    "Password:",
    "Account:",
    "Comment:"
  };
  Widget general;
  Arg args[20];
  Dimension width;
  Cardinal n;
  int i;
  char* passbuf;
  Widget current,child,commentlabel,passfield,anontoggle;
  Pixel white_pixel;
  Pixel black_pixel;

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  general=XmCreateForm(parent,"General",args,n);
  XtManageChild(general);

  /* first column */
  for(i=0;i<XtNumber(label)-1;i++){
    n=0;
    XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
    XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
    XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
    XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
    XtSetArg(args[n],XmNrightPosition,30);n++;
    XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
    XtSetArg(args[n],XmNbottomPosition,100*(i+1)/XtNumber(label));n++;
    current=XmCreateLabel(general,label[i],args,n);
    XtManageChild(current);
  }

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  commentlabel=XmCreateLabel(general,label[i],args,n);
  XtManageChild(commentlabel);

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  /*second column */
  /*
  n=0;
  XtSetArg(args[n],XmNcomboBoxType,XmDROP_DOWN_COMBO_BOX);n++;
  XtSetArg(args[n],XmNselectionPolicy,XmBROWSE_SELECT);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*1/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateComboBox(general,"profile",args,n);
  XtManageChild(current);
  XtAddCallback(current,XmNselectionCallback,listselect_cb,NULL);
  child=XtNameToWidget(current,"*Text");
  XtVaSetValues(child,XmNbackground,white_pixel, NULL);
  child=XtNameToWidget(current,"*List");
  XtVaSetValues(child,XmNbackground,white_pixel,NULL);
  */
  n=0;
  XtSetArg(args[n],DtNcomboBoxType,DtDROP_DOWN_COMBO_BOX);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*1/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=DtCreateComboBox(general,"profile",args,n);
  XtManageChild(current);
  XtAddCallback(current,DtNselectionCallback,listselect_cb,NULL);

  child=XtNameToWidget(current,"*Text");
  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNmarginWidth,3);n++;/* Motif 2.x workaround */
  XtSetArg(args[n],XmNmarginHeight,3);n++;
  XtSetValues(child,args,n);

  child=XtNameToWidget(current,"*List");
  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNselectionPolicy,XmBROWSE_SELECT);n++;
  XtSetValues(child,args,n);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateTextField(general,"host",args,n);
  XtManageChild(current);


  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*3/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateTextField(general,"user",args,n);
  XtManageChild(current);

  passbuf=XtMalloc(MAXPASS+1);
  passbuf[0]='\0';
  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*4/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  XtSetArg(args[n],XmNmaxLength,MAXPASS);n++;
  XtSetArg(args[n],XmNuserData,(XtPointer)passbuf);n++;
  passfield=current=XmCreateTextField(general,"pass",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*5/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateTextField(general,"account",args,n);
  XtManageChild(current);

  n=0; /* comment text field */
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,commentlabel);n++;
  current=XmCreateTextField(general,"comment",args,n);
  XtManageChild(current);

  /* third column */
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*1/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,85);n++;
  current=XmCreatePushButton(general,"New",args,n);
  XtManageChild(current);
  XtAddCallback(current,XmNactivateCallback,listaction_cb,NULL);


  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,85);n++;
  child=XmCreatePushButton(general,"Delete",args,n);
  XtManageChild(child);
  XtAddCallback(child,XmNactivateCallback,listaction_cb,NULL);

  XtVaGetValues(child,XmNwidth,&width,NULL);
  XtVaSetValues(child,XmNleftOffset,-width/2,NULL);
  XtVaSetValues(current,XmNwidth,width,XmNleftOffset,-width/2,NULL);

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*3/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  anontoggle=current=XmCreateToggleButton(general,"Anonymous",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*4/XtNumber(label));n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,70);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  current=XmCreateToggleButton(general,"Save Password",args,n);
  XtManageChild(current);

  XtAddCallback(passfield,XmNmodifyVerifyCallback,password_cb,
      (XtPointer)anontoggle);
  XtAddCallback(anontoggle,XmNvalueChangedCallback,anon_cb,
      (XtPointer)passfield);

  return general;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
