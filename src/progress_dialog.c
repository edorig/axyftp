/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<stdio.h>

#include"axyftp.h"
#include"utils.h"
#include"functions.h"
#include"progress_dialog.h"

static void translate_time(char* buf,long tot){
  long hours;
  long mins;
  long secs;

  hours=tot/(60*60);
  tot-=(hours*60*60);
  mins=tot/60;
  tot-=(mins*60);
  secs=tot;

  if(hours){
    sprintf(buf,"%ld:%.2ld:%.2ld",hours,mins,secs);
  } else {
    sprintf(buf,"%ld:%.2ld",mins,secs);
  }
}

#ifdef BUILD_GTK

#include<gtk/gtk.h>

static int xfer_cancel_cb(GtkWidget* w){
  appdata.small_interrupt=1;
  gtk_widget_set_sensitive(WXnameToWidget(w,"Cancel"),FALSE);
  return TRUE;
}

void update_progress_dialog(GtkWidget* w,
    int value,long size,float rate,long elapsed,long left){
  GtkWidget *child;
  char buf[25];

  if(value>=0){
    sprintf(buf,"%d %%",value);
    child=WXnameToWidget(w,"percent");
    gtk_label_set(GTK_LABEL(child),buf);

    child=WXnameToWidget(w,"Scrollbar");
    gtk_progress_bar_update(GTK_PROGRESS_BAR(child),((float)value)/100.);
  }

  if(size>=0){
    child=WXnameToWidget(w,"size");
    sprintf(buf,"%ld",size);
    gtk_label_set(GTK_LABEL(child),buf);
  }
  
  if(rate>=0){
    child=WXnameToWidget(w,"rate");
    sprintf(buf,"%.3f kBps",rate);
    gtk_label_set(GTK_LABEL(child),buf);
  }
  
  if(elapsed>=0){
    child=WXnameToWidget(w,"elapsed");
    translate_time(buf,elapsed);
    gtk_label_set(GTK_LABEL(child),buf);
  }
  
  if(left>=0){
    child=WXnameToWidget(w,"left");
    translate_time(buf,left);
    gtk_label_set(GTK_LABEL(child),buf);
  }
}

void init_progress_dialog(GtkWidget* w,char* name,long size){
  GtkWidget *child;
  char* buf;
  
  child=WXnameToWidget(w,"toptext");
  buf=WXmalloc(strlen(name)+30);
  sprintf(buf,"%s (%ld bytes)",name,size);
  gtk_label_set(GTK_LABEL(child),buf);
  WXfree(buf);

  child=WXnameToWidget(w,"percent");
  gtk_label_set(GTK_LABEL(child),"0");

  child=WXnameToWidget(w,"Scrollbar");
  gtk_progress_bar_update(GTK_PROGRESS_BAR(child),0);

  child=WXnameToWidget(w,"size");
  gtk_label_set(GTK_LABEL(child),"0");
  
  child=WXnameToWidget(w,"rate");
  gtk_label_set(GTK_LABEL(child),"? kBps");
  
  child=WXnameToWidget(w,"elapsed");
  gtk_label_set(GTK_LABEL(child),"0:00");
  
  child=WXnameToWidget(w,"left");
  gtk_label_set(GTK_LABEL(child),"?:??");
  
  child=WXnameToWidget(w,"Cancel");
  gtk_widget_set_sensitive(child,TRUE);
}

void show_progress_dialog(GtkWidget* w){
  gtk_window_position(GTK_WINDOW(w),GTK_WIN_POS_NONE);
  gtk_widget_show(w);
  process_events();
}

void hide_progress_dialog(GtkWidget* w){
  gtk_widget_hide(w);
  process_events();
}

GtkWidget* create_progress_dialog(GtkWidget* parent){
  GtkWidget *progress,*vbox,*hbox,*labels,*al;
  GtkWidget *toptext,*scale,*scroll,*size,*rate,*elapsed,*left,*cancel;
  GtkStyle* ns;

  progress=gtk_window_new(GTK_WINDOW_DIALOG);
  gtk_widget_realize(progress);
  /*
  gdk_window_set_functions(progress->window,0);
  gdk_window_set_decorations(progress->window,0);
  */
  gtk_window_set_title(GTK_WINDOW(progress),"AxY FTP transfer");
  gtk_container_border_width(GTK_CONTAINER(progress),6);
  gtk_widget_set_usize(progress,gdk_string_width(mystyle->font,"Cancel")*15,
      gtkfontheight*7.5);

  vbox=gtk_vbox_new(FALSE,2);
  gtk_container_add(GTK_CONTAINER(progress),vbox);
  gtk_widget_show(vbox);

  toptext=gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(vbox),toptext,TRUE,TRUE,2);
  gtk_widget_show(toptext);
  gtk_widget_set_name(toptext,"toptext");

  scale=gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(vbox),scale,TRUE,TRUE,2);
  gtk_widget_show(scale);
  gtk_widget_set_name(scale,"percent");

  scroll=gtk_progress_bar_new();
  gtk_box_pack_start(GTK_BOX(vbox),scroll,FALSE,FALSE,2);
  gtk_widget_show(scroll);
  gtk_widget_set_name(scroll,"Scrollbar");
  ns=style_full_copy(mystyle);
  ns->bg[GTK_STATE_PRELIGHT].pixel=0;
  ns->bg[GTK_STATE_PRELIGHT].red=12079;
  ns->bg[GTK_STATE_PRELIGHT].green=12079;
  ns->bg[GTK_STATE_PRELIGHT].blue=53199;
  ns->klass->xthickness=1;
  ns->klass->ythickness=1;
  gtk_widget_set_style(scroll,ns);
  gtk_widget_set_usize(scroll,0,gtkfontheight);
  
  hbox=gtk_hbox_new(FALSE,4);
  gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,2);
  gtk_widget_show(hbox);

  al=gtk_alignment_new(1,0.8,0,0);
  gtk_box_pack_end(GTK_BOX(hbox),al,FALSE,FALSE,2);
  gtk_widget_show(al);

  cancel=gtk_button_new_with_label(" Cancel ");
  gtk_container_add(GTK_CONTAINER(al),cancel);
  gtk_widget_show(cancel);
  gtk_widget_set_name(cancel,"Cancel");
  gtk_signal_connect(GTK_OBJECT(cancel),"clicked",
      GTK_SIGNAL_FUNC(xfer_cancel_cb),NULL);
  gtk_signal_connect(GTK_OBJECT(progress),"delete_event",
      GTK_SIGNAL_FUNC(xfer_cancel_cb),NULL);

  al=gtk_alignment_new(0,0,1,0);
  gtk_box_pack_end(GTK_BOX(hbox),al,TRUE,TRUE,2);
  gtk_widget_show(al);

  labels=gtk_hbox_new(TRUE,4);
  gtk_container_add(GTK_CONTAINER(al),labels);
  gtk_widget_show(labels);

  size=gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(labels),size,TRUE,TRUE,2);
  gtk_widget_show(size);
  gtk_widget_set_name(size,"size");

  rate=gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(labels),rate,TRUE,TRUE,2);
  gtk_widget_show(rate);
  gtk_widget_set_name(rate,"rate");

  elapsed=gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(labels),elapsed,TRUE,TRUE,2);
  gtk_widget_show(elapsed);
  gtk_widget_set_name(elapsed,"elapsed");

  left=gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(labels),left,TRUE,TRUE,2);
  gtk_widget_show(left);
  gtk_widget_set_name(left,"left");

  /*if(XAllocNamedColor(XtDisplay(parent),cmap,"#2F2FCF",&color,&exact)){*/

  return progress;
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/Scale.h>
#include<Xm/Form.h>
#include<Xm/Label.h>
#include<Xm/PushB.h>
#include<Xm/ScrollBar.h>

static void xfer_cancel_cb(Widget w,XtPointer app,XtPointer call){
  appdata.small_interrupt=1;
  XtSetSensitive(w,False);
}

void update_progress_dialog(Widget w,
  int value,long size,float rate,long elapsed,long left){
  Widget child;
  char buf[25];
  XmString xms;


  if(value>=0){
    child=XtNameToWidget(w,"*scale");
    XtVaSetValues(child,XmNvalue,0,XmNsliderSize,(value==0)?1:value,NULL);

    child=XtNameToWidget(w,"*percent");
    sprintf(buf,"%d %%",value);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }

  
  if(size>=0){
    child=XtNameToWidget(w,"*size");
    sprintf(buf,"%ld",size);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }
  
  if(rate>=0){
    child=XtNameToWidget(w,"*rate");
    sprintf(buf,"%.3f kBps",rate);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }
  
  if(elapsed>=0){
    child=XtNameToWidget(w,"*elapsed");
    translate_time(buf,elapsed);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }
  
  if(left>=0){
    child=XtNameToWidget(w,"*left");
    translate_time(buf,left);
    xms=XmStringCreateLocalized(buf);
    XtVaSetValues(child,XmNlabelString,xms,NULL);
    XmStringFree(xms);
  }
}

void init_progress_dialog(Widget w,char* name,long size){
  Widget child;
  char* buf;
  XmString xms;
  
  child=XtNameToWidget(w,"*toptext");
  buf=XtMalloc(strlen(name)+30);
  sprintf(buf,"%s (%ld bytes)",name,size);
  xms=XmStringCreateLocalized(buf);
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  XtFree(buf);

  child=XtNameToWidget(w,"*percent");
  xms=XmStringCreateLocalized("0 %");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);

  child=XtNameToWidget(w,"*scale");
  XtVaSetValues(child,XmNvalue,0,XmNsliderSize,1,NULL);

  child=XtNameToWidget(w,"*size");
  xms=XmStringCreateLocalized("0");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  
  child=XtNameToWidget(w,"*rate");
  xms=XmStringCreateLocalized("? kBps");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  
  child=XtNameToWidget(w,"*elapsed");
  xms=XmStringCreateLocalized("0:00");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  
  child=XtNameToWidget(w,"*left");
  xms=XmStringCreateLocalized("?:??");
  XtVaSetValues(child,XmNlabelString,xms,NULL);
  XmStringFree(xms);
  
  child=XtNameToWidget(w,"*Cancel");
  XtSetSensitive(child,True);
}

void show_progress_dialog(Widget w){
  XtManageChild(w);
  XmUpdateDisplay(toplevel);
  process_events();
}

void hide_progress_dialog(Widget w){
  XtUnmanageChild(w);
  XmUpdateDisplay(toplevel);
  XmUpdateDisplay(w);
  process_events();
/*  XtDestroyWidget(w);*/
}

static void slider_cb(Widget w,XtPointer app,XtPointer call){
  XtVaSetValues(w,XmNvalue,0,NULL);
  return;
}

Widget create_progress_dialog(Widget parent){
  Widget progress;
  Widget toptext,scale,size,rate,elapsed,left,cancel,percent;
  Arg args[15];
  Cardinal n;
  Dimension d;
  XmString xms;
  Colormap cmap;
  XColor color,exact;

  n=0;
  XtSetArg(args[n],XmNtitle,"AxY FTP transfer");n++;
  /*XtSetArg(args[n],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);n++;*/
  XtSetArg(args[n],XmNverticalSpacing,6);n++;
  XtSetArg(args[n],XmNhorizontalSpacing,6);n++;
  XtSetArg(args[n],XmNresizePolicy,XmRESIZE_NONE);n++;
  XtSetArg(args[n],XmNinput,False);n++;
  progress=XmCreateFormDialog(parent,"progress",args,n);

  xms=XmStringCreateLocalized("|");
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  toptext=XmCreateLabel(progress,"toptext",args,n);
  XtManageChild(toptext);
  
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,toptext);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginTop,10);n++;
  XtSetArg(args[n],XmNalignment,XmALIGNMENT_CENTER);n++;
  XtSetArg(args[n],XmNrecomputeSize,FALSE);n++;
  percent=XmCreateLabel(progress,"percent",args,n);
  XtManageChild(percent);

  /*
  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,percent);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNorientation,XmHORIZONTAL);n++;
  XtSetArg(args[n],XmNprocessingDirection,XmMAX_ON_RIGHT);n++;
  XtSetArg(args[n],XmNslidingMode,XmTHERMOMETER);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtSetArg(args[n],XmNsliderVisual,XmFOREGROUND_COLOR);n++;
  XtSetArg(args[n],XmNshowValue,FALSE);n++;
  scale=XmCreateScale(progress,"scale",args,n);
  XtManageChild(scale);

  XtVaGetValues(parent,XmNcolormap,&cmap,NULL);
  if(XAllocNamedColor(XtDisplay(parent),cmap,"#2F2FCF",&color,&exact)){
    scroll=XtNameToWidget(scale,"*Scrollbar");
    XtVaSetValues(scroll,XmNforeground,color.pixel,NULL);
  }
  */

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,percent);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;

  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  /*XtSetArg(args[n],XmNsensitive,FALSE);n++;*/
  XtSetArg(args[n],XmNminimum,0);n++;
  XtSetArg(args[n],XmNmaximum,100);n++;
  XtSetArg(args[n],XmNvalue,0);n++;
  XtSetArg(args[n],XmNorientation,XmHORIZONTAL);n++;
  XtSetArg(args[n],XmNprocessingDirection,XmMAX_ON_RIGHT);n++;
  XtSetArg(args[n],XmNshowArrows,FALSE);n++;
  XtSetArg(args[n],XmNsliderSize,1);n++;
  XtVaGetValues(parent,XmNcolormap,&cmap,NULL);
  if(XAllocNamedColor(XtDisplay(parent),cmap,"#2F2FCF",&color,&exact)){
    XtSetArg(args[n],XmNtroughColor,color.pixel);n++;
  }
  scale=XmCreateScrollBar(progress,"scale",args,n);
  XtManageChild(scale);
  XtAddCallback(scale,XmNvalueChangedCallback,slider_cb,NULL);
  XtAddCallback(scale,XmNdragCallback,slider_cb,NULL);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,1*100/5);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  size=XmCreateLabel(progress,"size",args,n);
  XtManageChild(size);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,1*100/5);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,2*100/5);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  rate=XmCreateLabel(progress,"rate",args,n);
  XtManageChild(rate);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,2*100/5);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,3*100/5);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  elapsed=XmCreateLabel(progress,"elapsed",args,n);
  XtManageChild(elapsed);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNleftPosition,3*100/5);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_POSITION);n++;
  XtSetArg(args[n],XmNrightPosition,4*100/5);n++;
  XtSetArg(args[n],XmNresizable,True);n++;
  XtSetArg(args[n],XmNlabelString,xms);n++;
  XtSetArg(args[n],XmNmarginLeft,12);n++;
  XtSetArg(args[n],XmNrecomputeSize,True);n++;
  left=XmCreateLabel(progress,"left",args,n);
  XtManageChild(left);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,scale);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  cancel=XmCreatePushButton(progress,"Cancel",args,n);
  XtManageChild(cancel);
  XtAddCallback(cancel,XmNactivateCallback,xfer_cancel_cb,NULL);
  XtAddCallback(cancel,XmNdisarmCallback,xfer_cancel_cb,NULL);

  XtVaGetValues(cancel,XmNwidth,&d,NULL);
  XtVaSetValues(progress,XmNwidth,d*10,NULL);

  XmStringFree(xms);

  return progress;
}

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
