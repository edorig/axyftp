/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>

#include"options_general.h"
#include"axyftp.h"
#include"utils.h"
#include"read_init.h"


#ifdef BUILD_GTK

#include<gtk/gtk.h>


GtkWidget* create_options_general(GtkWidget *parent){
  GtkWidget *table;
  GtkWidget *current,*child;

  table=gtk_table_new(6,4,TRUE);
  gtk_widget_show(table);
  gtk_container_border_width(GTK_CONTAINER(table),8);

  gtk_table_set_col_spacing(GTK_TABLE(table),1,8);
  /*gtk_table_set_row_spacing(GTK_TABLE(table),0,8);*/

  child=gtk_alignment_new(1,1,0,1);
  gtk_widget_show(child);
  gtk_table_attach_defaults(GTK_TABLE(table),child,0,2,1,2);
  current=gtk_label_new("Anonymous password:");
  gtk_widget_show(current);
  gtk_container_add(GTK_CONTAINER(child),current);

  child=gtk_entry_new();
  gtk_widget_set_name(child,"anonpass");
  gtk_widget_set_usize(child,0,gtkfontheight*2);
  gtk_widget_show(child);
  gtk_table_attach_defaults(GTK_TABLE(table),child,2,5,1,2);

  current=gtk_alignment_new(0,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,0,3,3,4);
  child=gtk_check_button_new_with_label("Verify deletions ");
  gtk_widget_set_name(child,"deletions");
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(child),TRUE);
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);

  current=gtk_alignment_new(0,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,0,3,4,5);
  child=gtk_check_button_new_with_label("Recurse deletions");
  gtk_widget_set_name(child,"recurse_del");
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(child),FALSE);
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);
  
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

Widget create_options_general(Widget parent){
  Widget general;
  Arg args[20];
  Cardinal n;
  Widget current;
  Pixel white_pixel;
  Pixel black_pixel;

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  general=XmCreateForm(parent,"General",args,n);
  XtManageChild(general);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*1/6);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,30);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/6);n++;
  current=XmCreateLabel(general,"Anonymous password:",args,n);
  XtManageChild(current);

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*1/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,30);n++;
  current=XmCreateTextField(general,"anonpass",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*3/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*4/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,50);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  XtSetArg(args[n],XmNrightOffset,10);n++;
  current=XmCreateToggleButton(general,"Verify deletions",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*4/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*5/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,50);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  XtSetArg(args[n],XmNrightOffset,10);n++;
  current=XmCreateToggleButton(general,"Recurse deletions",args,n);
  XtManageChild(current);

  return general;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
