/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include"axyftp.h"
#include"pixmaps.h"
#include"about_dialog.h"

#ifdef BUILD_GTK

#include<gtk/gtk.h>

static int hide_about_cb(GtkWidget *w){
  gtk_widget_hide(appdata.about);
  return TRUE;
}

GtkWidget* create_about_dialog(GtkWidget *parent){
  GtkWidget *about,*hbox,*vbox,*picture,*version,*copyright,*form,*button;
  char version_str[64];

  about=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_realize(about);

  gtk_signal_connect(GTK_OBJECT(about),"delete_event",
      GTK_SIGNAL_FUNC(hide_about_cb),NULL);
  gtk_window_set_title(GTK_WINDOW(about),"About AxY FTP");
  gtk_container_border_width(GTK_CONTAINER(about),6);
  /*
  gtk_window_position(GTK_WINDOW(about),GTK_WIN_POS_NONE);
  */

  hbox=gtk_hbox_new(FALSE,4);
  gtk_container_add(GTK_CONTAINER(about),hbox);
  gtk_widget_show(hbox);

  picture=gtk_pixmap_new(portret,portmask);
  gtk_box_pack_start(GTK_BOX(hbox),picture,FALSE,FALSE,0);
  gtk_widget_show(picture);

  vbox=gtk_vbox_new(FALSE,2);
  gtk_box_pack_start(GTK_BOX(hbox),vbox,TRUE,TRUE,0);
  gtk_widget_show(vbox);

  sprintf(version_str,
   "AxY FTP  GTK+ %d.%d version %d.%d.%d",
   gtk_major_version,gtk_minor_version,
   AXYFTP_MAJOR,AXYFTP_MINOR,AXYFTP_MICRO);
  version=gtk_label_new(version_str);
  gtk_box_pack_start(GTK_BOX(vbox),version,TRUE,TRUE,4);
  gtk_widget_show(version);

  copyright=gtk_label_new("Copyright (c) 1999  Alexander Yukhimets."
      " All rights reserved.");
  gtk_box_pack_start(GTK_BOX(vbox),copyright,TRUE,TRUE,4);
  gtk_widget_show(copyright);

  form=gtk_alignment_new(0.5,0.5,0,0);
  gtk_box_pack_start(GTK_BOX(vbox),form,FALSE,FALSE,4);
  gtk_widget_show(form);

  button=gtk_button_new_with_label(" OK ");
  gtk_container_add(GTK_CONTAINER(form),button);
  gtk_widget_show(button);
  
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
      GTK_SIGNAL_FUNC(hide_about_cb),NULL);

  return about;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/PushB.h>

void hide_about_cb(Widget w,XtPointer app,XtPointer call){
  XtUnmanageChild(appdata.about);
}

Widget create_about_dialog(Widget parent){
  Widget about,picture,version,copyright,form,button;
  char version_str[64];
  Arg args[10];
  Cardinal n;
  Dimension d;

  n=0;
  XtSetArg(args[n],XmNtitle,"About AxY FTP");n++;
  XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;
  XtSetArg(args[n],XmNverticalSpacing,6);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,6);n++;
  XtSetArg(args[n],XmNnoResize,True);n++;
  about=XmCreateFormDialog(parent,"about",args,n);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNlabelType,XmPIXMAP);n++;
  if(portret){
    XtSetArg(args[n],XmNlabelPixmap,portret);n++;
  }
  picture=XmCreateLabel(about,"picture",args,n);n++;
  XtManageChild(picture);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNtopOffset,20);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,picture);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  sprintf(version_str,"AxY FTP  Motif %d.%d version %d.%d.%d",
   XmVERSION,XmREVISION,AXYFTP_MAJOR,AXYFTP_MINOR,AXYFTP_MICRO);
  version=XmCreateLabel(about,version_str,args,n);n++;
  XtManageChild(version);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,version);n++;
  XtSetArg(args[n],XmNtopOffset,20);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,picture);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  copyright=XmCreateLabel(about,"Copyright (c) 1999  Alexander Yukhimets."
      " All rights reserved.",args,n);n++;
  XtManageChild(copyright);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,copyright);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNleftWidget,picture);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  form=XmCreateForm(about,"form",args,n);
  XtManageChild(form);
  
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,50);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  button=XmCreatePushButton(form," OK ",args,n);n++;
  XtManageChild(button);

  XtVaGetValues(button,XmNwidth,&d,NULL);
  XtVaSetValues(button,XmNleftOffset,-d/2,NULL);


  XtAddCallback(button,XmNactivateCallback,hide_about_cb,NULL);

  return about;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
