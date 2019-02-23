/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */

/******************************************************************
Permission to use, copy, modify, and distribute this software
and its documentation for any purpose without fee is granted,
provided that the above copyright notice appear in all copies
and that both copyright notice and this permission notice appear
in supporting documentation, and that the name of Interleaf not
be used in advertising or publicly pertaining to distribution of
the software without specific written prior permission.

Interleaf makes no representation about the suitability of this
software for any purpose. It is provided "AS IS" without any
express or implied warranty. 
******************************************************************/


#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

#include<Xm/Xm.h>
#include<Xm/TextF.h>

#include<Dt/ComboBox.h>

XtAppContext appcontext;
Widget toplevel;


void activate_cb(Widget w,XtPointer app,XtPointer call){
  DtComboBoxCallbackStruct *cbs=(DtComboBoxCallbackStruct*)call;
  printf("ACTIVATE %d\n",cbs->reason);
}

void menu_cb(Widget w,XtPointer app,XtPointer call){
  DtComboBoxCallbackStruct *cbs=(DtComboBoxCallbackStruct*)call;
  printf("MENU %d\n",cbs->reason);
}

void select_cb(Widget w,XtPointer app,XtPointer call){
  DtComboBoxCallbackStruct *cbs=(DtComboBoxCallbackStruct*)call;
  printf("SELECT %d\n",cbs->reason);
}

void focus_cb(Widget w,XtPointer app,XtPointer call){
  DtComboBoxCallbackStruct *cbs=(DtComboBoxCallbackStruct*)call;
  printf("FOCUS %d\n",cbs->reason);
}

void losing_cb(Widget w,XtPointer app,XtPointer call){
  DtComboBoxCallbackStruct *cbs=(DtComboBoxCallbackStruct*)call;
  printf("LOSING %d\n",cbs->reason);
}

int main(int argc, char* argv[]){
  Widget mainwin;
  Arg args[10];
  Cardinal n;


  toplevel=XtAppInitialize(&appcontext,"Test",
      NULL,0,&argc,argv,NULL,NULL,0);

  XtVaSetValues(toplevel,XmNforeground,BlackPixelOfScreen(XtScreen(toplevel)),
     XmNtitle,"ComboBox",NULL);

  n=0;
  XtSetArg(args[n],XmNhighlightThickness,4);n++;
  XtSetArg(args[n],DtNcomboBoxType,DtDROP_DOWN_COMBO_BOX);n++;
  XtSetArg(args[n],DtNarrowType,DtMOTIF);n++;
  mainwin=DtCreateComboBox(toplevel, "Hello",args,n);
  DtComboBoxAddItem(mainwin,XmStringCreateLocalized("Item 1"),1,FALSE);
  DtComboBoxAddItem(mainwin,XmStringCreateLocalized("Item 2"),2,FALSE);

  XtAddCallback(mainwin,DtNactivateCallback,activate_cb,NULL);
  XtAddCallback(mainwin,DtNselectionCallback,select_cb,NULL);
  XtAddCallback(mainwin,DtNfocusCallback,focus_cb,NULL);
  XtAddCallback(mainwin,DtNlosingFocusCallback,losing_cb,NULL);
  XtAddCallback(mainwin,DtNmenuPostCallback,menu_cb,NULL);
  XtManageChild(mainwin);
  XtRealizeWidget(toplevel);

  XtAppMainLoop(appcontext);

  return 0;
}

