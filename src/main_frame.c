/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"main_frame.h"
#include"buttonbar.h"
#include"arrows.h"
#include"status.h"
#include"proto.h"
#include"panel.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>

GtkWidget* create_main_frame(GtkWidget* parent){
  GtkWidget *frame;
  GtkWidget *panels,*lbox,*protobox,*proto,*status,*buttonbar;
  GtkWidget *local, *remote, *arrows,*align,*dummy;

  frame=gtk_vbox_new(FALSE,2);
  gtk_container_add(GTK_CONTAINER(parent),frame);
  gtk_widget_show(frame);

  panels=gtk_hpaned_new();
  gtk_box_pack_start(GTK_BOX(frame),panels,TRUE,TRUE,0);
  gtk_widget_show(panels);
  gtk_paned_gutter_size(GTK_PANED(panels),0);

  lbox=gtk_hbox_new(FALSE,2);
  gtk_paned_add1(GTK_PANED(panels),lbox);
  gtk_widget_show(lbox);

  local=create_panel(lbox,LOCAL);
  gtk_box_pack_start(GTK_BOX(lbox),local,TRUE,TRUE,0);
  arrows=create_arrows(lbox);
  gtk_box_pack_start(GTK_BOX(lbox),arrows,FALSE,TRUE,0);
  remote=create_panel(panels,REMOTE);
  gtk_paned_add2(GTK_PANED(panels),remote);
  gtk_widget_show(remote);


  protobox=gtk_hbox_new(TRUE,0);
  gtk_box_pack_start(GTK_BOX(frame),protobox,FALSE,TRUE,0);
  gtk_widget_show(protobox);

  dummy=gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(protobox),dummy,TRUE,TRUE,0);
  gtk_widget_show(dummy);
  proto=create_proto(protobox);
  gtk_box_pack_start(GTK_BOX(protobox),proto,FALSE,TRUE,0);
  appdata.proto=proto;

  align=gtk_alignment_new(1,0.5,0,0);
  gtk_box_pack_end(GTK_BOX(protobox),align,TRUE,TRUE,0);
  gtk_widget_show(align);
  dummy=gtk_label_new("");
  gtk_container_add(GTK_CONTAINER(align),dummy);
  gtk_container_border_width(GTK_CONTAINER(align),8);
  gtk_widget_show(dummy);
  appdata.listprogress=dummy;


  status=create_status(frame);
  gtk_box_pack_start(GTK_BOX(frame),status,FALSE,TRUE,0);

  buttonbar=create_buttonbar(frame);
  gtk_box_pack_start(GTK_BOX(frame),buttonbar,FALSE,TRUE,0);
  

  return frame;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/MainW.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/PanedW.h>


Widget create_main_frame(Widget parent){
  Arg args[10];
  Cardinal n;
  Widget form;
  Widget local,remote,arrows,proto,status,buttonbar,lbox;
  XmString xms;

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  form=XmCreateForm(parent,"form",args,n);
  XtManageChild(form);

  /*
  n=0;
  XtSetArg(args[n],XmNorientation,XmHORIZONTAL);n++;
  XtSetArg(args[n],XmNseparatorOn,FALSE);n++;
  XtSetArg(args[n],XmNspacing,0);n++;
  pane=XmCreatePanedWindow(form,"pane",args,n);
  XtManageChild(pane);
  */

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  /*XtSetArg(args[n],XmNrubberPositioning,TRUE);n++;*/
  lbox=XmCreateForm(form,"lbox",args,n);
  XtManageChild(lbox);
  /*
  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  lbox=XmCreateForm(pane,"lbox",args,n);
  XtManageChild(lbox);
  */

  local=create_panel(lbox,LOCAL);
  arrows=create_arrows(lbox);
  remote=create_panel(lbox,REMOTE);
  /*
  local=create_panel(lbox,LOCAL);
  arrows=create_arrows(lbox);
  remote=create_panel(pane,REMOTE);
  */

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNrightWidget,arrows);n++;
  XtSetValues(local,args,n);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,50);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetValues(arrows,args,n);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftWidget,arrows);n++;
  XtSetValues(remote,args,n);

  proto=create_proto(form);
  status=create_status(form);
  buttonbar=create_buttonbar(form);


  appdata.local.list=NULL;
  appdata.remote.list=NULL;
  
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNbottomWidget,proto);n++;
  XtSetValues(lbox,args,n);


  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,50);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNbottomWidget,status);n++;
  XtSetValues(proto,args,n);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNbottomWidget,buttonbar);n++;
  XtSetValues(status,args,n);
 
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetValues(buttonbar,args,n);

  n=0;
  xms=XmStringCreateLocalized("                    ");
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  XtSetArg(args[n],XmNmarginRight,12);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,remote);n++;
  appdata.listprogress=XmCreateLabel(form,"listprogress",args,n);
  XtManageChild(appdata.listprogress);
  XmStringFree(xms);
  
  return form;
}

int adjust_main_frame(Widget id){
  Widget arrows;
  Dimension d;
  
  arrows=XtNameToWidget(id,"*arrows");
  
  XtVaGetValues(appdata.proto,XmNwidth,&d,NULL);
  XtVaSetValues(appdata.proto,XmNleftOffset,-d/2,NULL);

  XtVaGetValues(arrows,XmNwidth,&d,NULL);
  XtVaSetValues(arrows,XmNleftOffset,-d/2,NULL);

  return 0;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
