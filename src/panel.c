/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"panel.h"
#include"buttons.h"
#include"dirname.h"
#include"dirlist.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>
GtkWidget* create_panel(GtkWidget* parent,int which){
  GtkWidget *panel,*form,*box,*dirname,*buttons,*buttonsa,*dirlist;

  switch(which){
    case LOCAL:
      panel=gtk_frame_new("Local System");
      break;
    case REMOTE:
      panel=gtk_frame_new("Remote System");
      break;
    default:
      return NULL;
  }

  gtk_frame_set_label_align(GTK_FRAME(panel),0.05,0);
  gtk_container_border_width(GTK_CONTAINER(panel),6);
  gtk_widget_show(panel);
  gtk_frame_set_shadow_type(GTK_FRAME(panel),GTK_SHADOW_ETCHED_IN);

  form=gtk_vbox_new(FALSE,2);
  gtk_container_add(GTK_CONTAINER(panel),form);
  gtk_widget_show(form);
  gtk_container_border_width(GTK_CONTAINER(form),6);

  dirname=create_dirname(form,which);
  gtk_box_pack_start(GTK_BOX(form),dirname,FALSE,TRUE,2);

  box=gtk_hbox_new(FALSE,6);
  gtk_widget_show(box);
  gtk_box_pack_start(GTK_BOX(form),box,TRUE,TRUE,2);
  
  buttonsa=gtk_alignment_new(1,0,1,0);
  gtk_widget_show(buttonsa);
  buttons=create_buttons(buttonsa,which);
  gtk_container_add(GTK_CONTAINER(buttonsa),buttons);
  gtk_box_pack_end(GTK_BOX(box),buttonsa,FALSE,FALSE,0);
  
  dirlist=create_dirlist(box,which);
  gtk_box_pack_end(GTK_BOX(box),dirlist,TRUE,TRUE,0);

  return panel;

}

#elif defined BUILD_MOTIF

#include<Xm/Screen.h>
#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Frame.h>
#include<Xm/Label.h>
#include<Xm/List.h>
#include<Xm/RowColumn.h>

Widget create_panel(Widget parent,int which){
  Arg args[10];
  Cardinal n;
  Widget panel,form,label,dirname,buttons,dirlist;
  XmString l;

  switch(which){
    case LOCAL:
      l=XmStringCreateLocalized("Local System");
      break;
    case REMOTE:
      l=XmStringCreateLocalized("Remote System");
      break;
    default:
      return (Widget)0;
  }

  n=0;
  XtSetArg(args[n],XmNmarginWidth,4);n++;
  XtSetArg(args[n],XmNmarginHeight,4);n++;
  panel=XmCreateFrame(parent,"panel",args,n);
  XtManageChild(panel);

  n=0;
  XtSetArg(args[n],XmNlabelString,l);n++;
  XtSetArg(args[n],XmNchildType,XmFRAME_TITLE_CHILD);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  label=XmCreateLabel(panel,"label",args,n);
  XtManageChild(label);

  n=0;
  XtSetArg(args[n],XmNchildType,XmFRAME_WORKAREA_CHILD);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,2);n++;
  XtSetArg(args[n],XmNverticalSpacing,2);n++;
  form=XmCreateForm(panel,"panel",args,n);
  XtManageChild(form);

  dirname=create_dirname(form,which);
  buttons=create_buttons(form,which);
  dirlist=create_dirlist(form,which);

/*
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetValues(label,args,n);
*/
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
/*  XtSetArg(args[n],XmNtopWidget,label);n++;*/
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetValues(dirname,args,n);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,dirname);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_NONE);n++;
  XtSetValues(buttons,args,n);
  
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,dirname);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNrightWidget,buttons);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetValues(dirlist,args,n);
  
  XmStringFree(l);

  return panel;
}

#else 
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
