/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>

#include"options_advanced.h"
#include"axyftp.h"
#include"utils.h"
#include"read_init.h"


#ifdef BUILD_GTK

#include<gtk/gtk.h>


GtkWidget* create_options_advanced(GtkWidget *parent){
  GtkWidget *table;
  GtkWidget *current,*child;

  table=gtk_table_new(6,4,TRUE);
  gtk_container_border_width(GTK_CONTAINER(table),8);
  gtk_widget_show(table);

  current=gtk_alignment_new(1,1,0,1);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,1,5,1,2);
  child=gtk_label_new("Number of redial attempts: ");
  gtk_container_add(GTK_CONTAINER(current),child);
  gtk_widget_show(child);

  current=gtk_entry_new();
  gtk_widget_set_name(current,"redial");
  gtk_widget_set_usize(current,gdk_string_width(mystyle->font,"W")*4,
      gtkfontheight*2);
  gtk_table_attach_defaults(GTK_TABLE(table),current,5,6,1,2);
  gtk_widget_show(current);
  
  current=gtk_alignment_new(1,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,1,5,2,3);
  child=gtk_label_new("Delay between attempts (sec): ");
  gtk_container_add(GTK_CONTAINER(current),child);
  gtk_widget_show(child);

  current=gtk_entry_new();
  gtk_widget_set_name(current,"delay");
  gtk_widget_set_usize(current,gdk_string_width(mystyle->font,"W")*4,
      gtkfontheight*2);
  gtk_table_attach_defaults(GTK_TABLE(table),current,5,6,2,3);
  gtk_widget_show(current);
  
  return table;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/PushB.h>
#include<Xm/ToggleB.h>
#include<Xm/TextF.h>
#include<Xm/Label.h>
#include<Xm/List.h>

Widget create_options_advanced(Widget parent){
  Widget general;
  Arg args[20];
  Cardinal n;
  Widget current;
  Pixel white_pixel;
  Pixel black_pixel;

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  general=XmCreateForm(parent,"Advanced",args,n);
  XtManageChild(general);

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*1/6);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,17);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/6);n++;
  current=XmCreateLabel(general,"Number of redial attempts: ",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,10);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*1/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,70);n++;
  current=XmCreateTextField(general,"redial",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*2/6);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,17);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,70);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*3/6);n++;
  current=XmCreateLabel(general,"Delay between attempts (sec): ",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,10);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*2/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*3/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,70);n++;
  current=XmCreateTextField(general,"delay",args,n);
  XtManageChild(current);

  return general;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
