/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>

#include"options_sound.h"
#include"axyftp.h"
#include"utils.h"
#include"read_init.h"


#ifdef BUILD_GTK

#include<gtk/gtk.h>


GtkWidget* create_options_sound(GtkWidget *parent){
  GtkWidget *table;
  GtkWidget *current,*child;

  table=gtk_table_new(6,4,TRUE);
  gtk_widget_show(table);
  gtk_container_border_width(GTK_CONTAINER(table),8);

  current=gtk_alignment_new(0,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,0,6,1,2);
  child=gtk_check_button_new_with_label("Beep at the end of download/upload");
  gtk_widget_set_name(child,"xferbeep");
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(child),TRUE);
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

Widget create_options_sound(Widget parent){
  Widget general;
  Arg args[20];
  Cardinal n;
  Widget current;
  Pixel white_pixel;
  Pixel black_pixel;

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  general=XmCreateForm(parent,"Sound",args,n);
  XtManageChild(general);

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*1/4);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/4);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  XtSetArg(args[n],XmNrightOffset,10);n++;
  current=XmCreateToggleButton(general,
      "Beep at the end of download/upload",args,n);
  XtManageChild(current);

  return general;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
