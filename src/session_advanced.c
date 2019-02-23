/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"session_advanced.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>

GtkWidget* create_session_advanced(GtkWidget* parent){
  GtkWidget *advanced;
  GtkWidget *current,*child;
  int i;

  advanced=gtk_table_new(1,6,TRUE);
  gtk_container_border_width(GTK_CONTAINER(advanced),8);
  for(i=0;i<0;i++){
    gtk_table_set_row_spacing(GTK_TABLE(advanced),i,4);
  }
  gtk_widget_show(advanced);

  current=gtk_alignment_new(1,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(advanced),current,0,2,0,1);
  child=gtk_label_new("Remote Port ");
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);

  current=gtk_entry_new();
  gtk_widget_set_usize(current,gdk_string_width(mystyle->font,"W")*8,
      gtkfontheight*2);
  gtk_widget_set_name(current,"port");
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(advanced),current,2,3,0,1);
  
  current=gtk_alignment_new(0,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(advanced),current,3,6,0,1);
  child=gtk_label_new(" Remote host control connection port.\nUsually 21.");
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);

  return advanced;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/TextF.h>

Widget create_session_advanced(Widget parent){
  Widget advanced;
  Arg args[15];
  Cardinal n;
  Widget current;
  Pixel white_pixel,black_pixel;
  XmString xms;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  XtSetArg(args[n],XmNrubberPositioning,True);n++;
  advanced=XmCreateForm(parent,"Advanced",args,n);
  XtManageChild(advanced);

  n=0;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,20);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100);n++;
  current=XmCreateLabel(advanced,"Remote Port ",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,10);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,20);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,40);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100);n++;
  current=XmCreateTextField(advanced,"port",args,n);
  XtManageChild(current);

  xms=XmStringCreateSimple("Remote Host control connection port. Usually 21.");
  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,40);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  current=XmCreateLabel(advanced,"port_label",args,n);
  XmStringFree(xms);
  XtManageChild(current);

  return advanced;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
