/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"session_startup.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>

GtkWidget* create_session_startup(GtkWidget* parent){
  GtkWidget *startup;
  GtkWidget *current,*child;
  int i;

  startup=gtk_table_new(8,6,TRUE);
  gtk_container_border_width(GTK_CONTAINER(startup),8);
  for(i=0;i<7;i++){
    gtk_table_set_row_spacing(GTK_TABLE(startup),i,4);
  }
  gtk_widget_show(startup);

  current=gtk_alignment_new(0,1,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,0,6,0,1);
  child=gtk_label_new(
      "Initial Command: (use ';' to separate multiple commands)");
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);

  current=gtk_entry_new();
  gtk_widget_set_usize(current,0,gtkfontheight*2);
  gtk_widget_set_name(current,"initcom");
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,0,6,1,2);
  
  current=gtk_alignment_new(0,1,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,0,6,2,3);
  child=gtk_label_new("Initial Remote Directory:");
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);

  current=gtk_entry_new();
  gtk_widget_set_usize(current,0,gtkfontheight*2);
  gtk_widget_set_name(current,"remdir");
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,0,6,3,4);
  
  current=gtk_alignment_new(0,1,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,0,6,4,5);
  child=gtk_label_new("Initial Local Directory:");
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);

  current=gtk_entry_new();
  gtk_widget_set_usize(current,0,gtkfontheight*2);
  gtk_widget_set_name(current,"locdir");
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,0,6,5,6);
  
  current=gtk_alignment_new(1,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,0,2,6,7);
  child=gtk_label_new("Remote File Mask: ");
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);

  current=gtk_entry_new();
  gtk_widget_set_usize(current,gdk_string_width(mystyle->font,"W")*8,
      gtkfontheight*2);
  gtk_widget_set_name(current,"remmask");
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,2,3,6,7);
  
  current=gtk_alignment_new(1,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,0,2,7,8);
  child=gtk_label_new("Local File Mask: ");
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);

  current=gtk_entry_new();
  gtk_widget_set_usize(current,gdk_string_width(mystyle->font,"W")*8,
      gtkfontheight*2);
  gtk_widget_set_name(current,"locmask");
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(startup),current,2,3,7,8);

  return startup;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/TextF.h>

Widget create_session_startup(Widget parent){
  Widget startup;
  Arg args[15];
  Cardinal n;
  Widget current;
  Pixel white_pixel,black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  XtSetArg(args[n],XmNrubberPositioning,True);n++;
  startup=XmCreateForm(parent,"Startup",args,n);
  XtManageChild(startup);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*1);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  current=XmCreateLabel(startup,
      "Initial Command: (use ';' to separate multiple commands)",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*2);n++;
  current=XmCreateTextField(startup,"initcom",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*3);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  current=XmCreateLabel(startup,"Initial Remote Directory:",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*4);n++;
  current=XmCreateTextField(startup,"remdir",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*5);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  current=XmCreateLabel(startup,"Initial Local Directory:",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*6);n++;
  current=XmCreateTextField(startup,"locdir",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,30);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*8);n++;
  current=XmCreateLabel(startup,"Remote File Mask:",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,10);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,50);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomOffset,4);n++;
  XtSetArg(args[n],XmNbottomPosition,100/9*8);n++;
  current=XmCreateTextField(startup,"remmask",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,30);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  current=XmCreateLabel(startup,"Local File Mask:",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,10);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,50);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  current=XmCreateTextField(startup,"locmask",args,n);
  XtManageChild(current);


  return startup;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
