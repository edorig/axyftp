/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<string.h>

#include"options_display.h"
#include"axyftp.h"
#include"utils.h"
#include"read_init.h"


#ifdef BUILD_GTK

#include<gtk/gtk.h>

static void toggle_cb(GtkWidget* w,gpointer app){
  gtk_widget_set_sensitive(GTK_WIDGET(app),GTK_TOGGLE_BUTTON(w)->active);
}

GtkWidget* create_options_display(GtkWidget *parent){
  GtkWidget *table;
  GtkWidget *current,*child,*save;

  table=gtk_table_new(6,7,TRUE);
  gtk_widget_show(table);
  gtk_container_border_width(GTK_CONTAINER(table),8);

  current=gtk_alignment_new(0,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,0,6,0,1);
  child=gtk_check_button_new_with_label(
      "Show Session Window on startup");
  gtk_widget_set_name(child,"show_session");
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(child),FALSE);
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);
  
  current=gtk_alignment_new(0,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,0,6,2,3);
  save=child=gtk_check_button_new_with_label(
      "Enable automatic column width adjustment");
  gtk_widget_set_name(child,"columnadjustment");
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(child),TRUE);
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);
  
  child=gtk_alignment_new(1,1,0,1);
  gtk_widget_show(child);
  gtk_table_attach_defaults(GTK_TABLE(table),child,1,5,3,4);
  current=gtk_label_new("Maximum column width ");
  gtk_widget_set_name(current,"maxwidth_label");
  gtk_widget_show(current);
  gtk_container_add(GTK_CONTAINER(child),current);

  gtk_signal_connect(GTK_OBJECT(save),"toggled",
      GTK_SIGNAL_FUNC(toggle_cb),(gpointer)current);

  child=gtk_entry_new();
  gtk_widget_set_name(child,"maxwidth");
  gtk_widget_set_usize(child,4*gdk_string_width(mystyle->font,"W"),
      gtkfontheight*2);
  gtk_widget_show(child);
  gtk_table_attach_defaults(GTK_TABLE(table),child,5,6,3,4);

  current=gtk_alignment_new(0,0.5,0,0);
  gtk_widget_show(current);
  gtk_table_attach_defaults(GTK_TABLE(table),current,0,6,5,6);
  save=child=gtk_check_button_new_with_label(
      "Show progress bar");
  gtk_widget_set_name(child,"show_progress");
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(child),TRUE);
  gtk_widget_show(child);
  gtk_container_add(GTK_CONTAINER(current),child);
  
  child=gtk_alignment_new(1,0.5,0,1);
  gtk_widget_show(child);
  gtk_table_attach_defaults(GTK_TABLE(table),child,1,5,6,7);
  current=gtk_label_new(
      "Hide progress bar if file size less than (Kbytes) ");
  gtk_widget_set_name(current,"progress_treshold_label");
  gtk_widget_show(current);
  gtk_container_add(GTK_CONTAINER(child),current);

  gtk_signal_connect(GTK_OBJECT(save),"toggled",
      GTK_SIGNAL_FUNC(toggle_cb),(gpointer)current);

  child=gtk_entry_new();
  gtk_widget_set_name(child,"progress_treshold");
  gtk_widget_set_usize(child,4*gdk_string_width(mystyle->font,"W"),
      gtkfontheight*2);
  gtk_widget_show(child);
  gtk_table_attach_defaults(GTK_TABLE(table),child,5,6,6,7);

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

static void anon_cb(Widget w,XtPointer app,XtPointer call){
  XmToggleButtonCallbackStruct *cbs;

  cbs=(XmToggleButtonCallbackStruct*)call;
  XtSetSensitive((Widget)app,cbs->set);
}

Widget create_options_display(Widget parent){
  Widget general;
  Arg args[20];
  Cardinal n;
  Widget current,save;
  Pixel white_pixel;
  Pixel black_pixel;

  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,4);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,4);n++;
  general=XmCreateForm(parent,"Display",args,n);
  XtManageChild(general);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*1/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*2/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  XtSetArg(args[n],XmNrightOffset,10);n++;
  current=XmCreateToggleButton(general,
      "Show Session Window on startup",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_BEGINNING);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNfillOnSelect,True);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*2/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*3/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  XtSetArg(args[n],XmNrightOffset,10);n++;
  save=current=XmCreateToggleButton(general,
      "Enable automatic column width adjustment",args,n);
  XtManageChild(current);

  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*3/6);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,80);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*4/6);n++;
  current=XmCreateLabel(general,"Maximum column width",args,n);
  XtManageChild(current);

  XtAddCallback(save,XmNvalueChangedCallback,anon_cb,
      (XtPointer)current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,6);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*3/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*4/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,80);n++;
  current=XmCreateTextField(general,"maxwidth",args,n);
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
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNleftOffset,10);n++;
  XtSetArg(args[n],XmNrightOffset,10);n++;
  save=current=XmCreateToggleButton(general,
      "Show progress bar",args,n);
  XtManageChild(current);
  XtVaSetValues(current,XmNset,TRUE,NULL);


  n=0;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_END);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*5/6);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,0);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,80);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNbottomPosition,100*6/6);n++;
  current=XmCreateLabel(general,
      "Hide progress bar if file size less than (Kbytes) ",args,n);
  XtManageChild(current);

  XtAddCallback(save,XmNvalueChangedCallback,anon_cb,
      (XtPointer)current);

  n=0;
  XtSetArg(args[n],XmNbackground,white_pixel);n++;
  XtSetArg(args[n],XmNforeground,black_pixel);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNcolumns,6);n++;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNtopPosition,100*5/6);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftOffset,8);n++;
  XtSetArg(args[n],XmNrightOffset,8);n++;
  XtSetArg(args[n],XmNbottomPosition,100*6/6);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,100);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,80);n++;
  current=XmCreateTextField(general,"progress_treshold",args,n);
  XtManageChild(current);


  return general;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
