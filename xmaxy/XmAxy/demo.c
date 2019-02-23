/* Copyright (c) 1999   Alexander Yukhimets. All rights reserved. */

#define XM_NOTEBOOK 0

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

#include<Xm/Xm.h>
#include<X11/IntrinsicP.h>
#include<X11/Core.h>
#include<X11/CoreP.h>
#include<Xm/PushB.h>
#include<Xm/Label.h>
#include<Xm/List.h>
#include<Xm/Form.h>
#include<Xm/Frame.h>
#include<Xm/PushBG.h>
#if XM_NOTEBOOK
#include<Xm/Notebook.h>
#endif
#include<XmAxy/Notebook.h>
#include<XmAxy/List.h>

#include<X11/xpm.h>
#include"link.xpm"

Pixmap trans,tranm;

XtAppContext appcontext;
Widget toplevel;
String labels[4]={"^","Name","Date","Size"};
short widths[4]={4,8,8,8};

static void button_cb(Widget w,XtPointer app,XtPointer call){
  CoreWidget xm,axy;

  xm=(CoreWidget)XtParent(XtNameToWidget((Widget)app,"*Xm"));
  axy=(CoreWidget)XtParent(XtNameToWidget((Widget)app,"*Axy"));
  
  printf("%s:  w=%d h=%d b=%d x=%d y=%d managed=%s\n",xm->core.name,
      xm->core.width,xm->core.height, xm->core.border_width,xm->core.x,
      xm->core.y, XtIsManaged((Widget)xm)?"yes":"no");
  printf("%s: w=%d h=%d b=%d x=%d y=%d managed=%s\n",axy->core.name,
      axy->core.width,axy->core.height, axy->core.border_width,axy->core.x,
      axy->core.y, XtIsManaged((Widget)axy)?"yes":"no");



  /*
  if(XmAxyNotebookGetCurrentPage((Widget)app)==1){
    XmAxyNotebookSetCurrentPage((Widget)app,2,FALSE);
  } else {
    XmAxyNotebookSetCurrentPage((Widget)app,1,FALSE);
  }

  XtVaGetValues(XtNameToWidget((Widget)app,"*Xm"),
	XmNvisibleItemCount,&count,NULL);
  XtVaSetValues(XtNameToWidget((Widget)app,"*Xm"),
	XmNvisibleItemCount,count+1,NULL);
	*/

}

static void default_cb(Widget w,XtPointer app,XtPointer call){
  XmAxyListDefaultActionCallbackStruct *cbs=
    (XmAxyListDefaultActionCallbackStruct*)call;
  printf("Default Action on row=%d column=%d\n",cbs->row,cbs->column);
}

XmString xxx;

static void draw_cell_cb(Widget w,XtPointer app,XtPointer call){
  XmAxyListDrawCellCallbackStruct* cbs=(XmAxyListDrawCellCallbackStruct*)call;

  if(cbs->column==0){
    cbs->type=XmPIXMAP;
    cbs->pixmap=trans;
    cbs->pixmask=tranm;
    cbs->pixwidth=0;
    cbs->pixheight=0;
    cbs->pixdepth=0;
  } else {
    cbs->type=XmSTRING;
    /*cbs->string="Hello, Worlds!";*/
    cbs->is_xm_string=TRUE;
    cbs->string=(String)xxx;
  }
}

int main(int argc, char* argv[]){
  Widget form,button,frame,axylist,xmlist;
  Arg args[80];
  Cardinal n;
  XpmAttributes attr;



  toplevel=XtAppInitialize(&appcontext,"XmAxyListTest",
      NULL,0,&argc,argv,NULL,NULL,0);

  XtVaSetValues(toplevel,XmNforeground,BlackPixelOfScreen(XtScreen(toplevel)),
     XmNtitle,"XmAxyListTest",NULL);

  attr.valuemask=XpmExactColors|XpmCloseness;
  attr.exactColors=FALSE;
  attr.closeness=10000;
  XpmCreatePixmapFromData(XtDisplay(toplevel),
                          DefaultRootWindow(XtDisplay(toplevel)),
                          link_p,&trans,&tranm,&attr);

  xxx=XmStringCreateSimple("Hello, Worlds!");

  n=0;
  XtSetArg(args[n],XmNverticalSpacing,10);n++;
  form=XmCreateForm(toplevel,"Form",args,n);
  XtManageChild(form);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_NONE);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_NONE);n++;
  button=XmCreatePushButton(form,"Switch",args,n);
  XtManageChild(button);

  n=0;
  XtSetArg(args[n],XmNtopAttachment,XmATTACH_WIDGET);n++;
  XtSetArg(args[n],XmNtopWidget,button);n++;
  XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM);n++;
  XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM);n++;
  /*
  XtSetArg(args[n],XmNshadowThickness,0);n++;
  XtSetArg(args[n],XmNmarginWidth,0);n++;
  XtSetArg(args[n],XmNmarginHeight,0);n++;
  XtSetArg(args[n],XmNinnerMarginWidth,0);n++;
  XtSetArg(args[n],XmNinnerMarginHeight,0);n++;
  */
#if !XM_NOTEBOOK
  /*XtSetArg(args[n],XmNbackground,WhitePixelOfScreen(XtScreen(form)));n++;*/
  /*XtSetArg(args[n],XmNhideTabs,TRUE);n++;*/
  /*
  XtSetArg(args[n],XmNtabRaise,2);n++;
  XtSetArg(args[n],XmNtabSpacing,0);n++;
  XtSetArg(args[n],XmNshadowThickness,2);n++;
  XtSetArg(args[n],XmNhighlightThickness,1);n++;
  */
  /*
  XtSetArg(args[n],XmNlabelMarginHeight,5);n++;
  XtSetArg(args[n],XmNlabelMarginWidth,5);n++;
  XtSetArg(args[n],XmNtextMarginHeight,0);n++;
  XtSetArg(args[n],XmNtextMarginWidth,0);n++;
  */
  /*XtSetArg(args[n],XmNbackground,WhitePixelOfScreen(XtScreen(form)));n++;*/
  /*XtSetArg(args[n],XmNbackgroundPixmap,trans);n++;*/
  frame=XmAxyCreateNotebook(form,"Card",args,n);
  XtManageChild(frame);
  XtAddCallback(button,XmNactivateCallback,button_cb,frame);
#else
  XtSetArg(args[n],XmNbackPagePlacement,XmTOP_RIGHT);n++;
  XtSetArg(args[n],XmNmajorTabSpacing,0);n++;
  XtSetArg(args[n],XmNorientation,XmVERTICAL);n++;
  XtSetArg(args[n],XmNbindingType,XmNONE);n++;
  XtSetArg(args[n],XmNbackPageNumber,1);n++;
  XtSetArg(args[n],XmNbackPageSize,0);n++;
  frame=XmCreateNotebook(form,"Card",args,n);
  XtManageChild(frame);
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmPAGE_SCROLLER);n++;
  XtManageChild(XmCreateLabel(frame,"",args,n));
  XtAddCallback(button,XmNactivateCallback,button_cb,frame);
#endif
  /*
  frame=XmCreateFrame(form,"Card",args,n);
  XtManageChild(frame);
  XtAddCallback(button,XmNactivateCallback,button_cb,frame);
  */

  n=0;
  XtManageChild(XmCreatePushButton(frame,"Bgutton",args,n));

  n=0;
#if XM_NOTEBOOK
  XtSetArg(args[n],XmNnotebookChildType,XmPAGE);n++;
#endif
  XtSetArg(args[n],XmNshadowThickness,2);n++;
  XtSetArg(args[n],XmNborderWidth,0);n++;
  XtSetArg(args[n],XmNlistMarginWidth,15);n++;
  XtSetArg(args[n],XmNlistMarginHeight,30);n++;
  XtSetArg(args[n],XmNhighlightThickness,20);n++;
  XtSetArg(args[n],XmNlistSpacing,10);n++;
  XtSetArg(args[n],XmNvisibleItemCount,4);n++;
  XtSetArg(args[n],XmNlistSizePolicy,XmRESIZE_IF_POSSIBLE);n++;
  XtSetArg(args[n],XmNscrollBarDisplayPolicy,XmAS_NEEDED);n++;
  XtSetArg(args[n],XmNselectionPolicy,XmEXTENDED_SELECT);n++;
  xmlist=XmCreateScrolledList(frame,"Xm",args,n);/*frame*/
  XmListAddItem(xmlist, XmStringCreateLocalized("ITEM 1 XXXXXXXXXXXX"),1);
  XmListAddItem(xmlist, XmStringCreateLocalized("ITEM 2XXXXXXXXXXXXX"),2);
  XmListAddItem(xmlist, XmStringCreateLocalized("ITEM 3YYYYYYYYYY"),3);
  XmListAddItem(xmlist, XmStringCreateLocalized("ITEM 4"),4);
  XmListAddItem(xmlist, XmStringCreateLocalized("ITEM 5"),5);
  XmListAddItem(xmlist, XmStringCreateLocalized("ITEM 6DDDDDDDDDDDDD"),6);
  XmListAddItem(xmlist, XmStringCreateLocalized("ITEM 7"),7);
  XmListAddItem(xmlist, XmStringCreateLocalized("ITEM 8"),8);
  XmListAddItem(xmlist, XmStringCreateLocalized("ITEM 9"),9);
  XtManageChild(xmlist);
  /*XtUnmanageChild(XtParent(xmlist));*/
#if XM_NOTEBOOK
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(frame,"XmP",args,n));
#endif


  n=0;

#if XM_NOTEBOOK
  XtSetArg(args[n],XmNnotebookChildType,XmPAGE);n++;
#endif
  XtSetArg(args[n],XmNshowLabels,TRUE);n++;
  XtSetArg(args[n],XmNborderWidth,20);n++;
  XtSetArg(args[n],XmNshadowThickness,4);n++;
  XtSetArg(args[n],XmNhighlightThickness,10);n++;
  XtSetArg(args[n],XmNlistMarginWidth,15);n++;
  XtSetArg(args[n],XmNlistMarginHeight,30);n++;
  XtSetArg(args[n],XmNcellMarginWidth,6);n++;
  XtSetArg(args[n],XmNcellMarginHeight,6);n++;
  XtSetArg(args[n],XmNlabelMarginWidth,6);n++;
  XtSetArg(args[n],XmNlabelMarginHeight,6);n++;
  XtSetArg(args[n],XmNcolumnWidths,widths);n++;
  XtSetArg(args[n],XmNcolumnLabels,labels);n++;
  XtSetArg(args[n],XmNcolumnCount,4);n++;
  /*XtSetArg(args[n],XmNcellBackground,WhitePixelOfScreen(XtScreen(form)));n++;*/
  /*XtSetArg(args[n],XmNlabelBackground,WhitePixelOfScreen(XtScreen(form)));n++;*/
  XtSetArg(args[n],XmNrowCount,5);n++;
  XtSetArg(args[n],XmNrowSpacing,15);n++;
  XtSetArg(args[n],XmNlistSizePolicy,XmRESIZE_IF_POSSIBLE);n++;
  XtSetArg(args[n],XmNscrollBarDisplayPolicy,XmAS_NEEDED);n++;
  XtSetArg(args[n],XmNselectionPolicy,XmEXTENDED_SELECT);n++;
  axylist=XmAxyCreateScrolledList(frame, "Axy",args,n);
  XtAddCallback(axylist,XmNdrawCellCallback,draw_cell_cb,(XtPointer)NULL);
  XtAddCallback(axylist,XmNdefaultActionCallback,default_cb,(XtPointer)NULL);
  XtManageChild(axylist);
#if XM_NOTEBOOK
  n=0;
  XtSetArg(args[n],XmNnotebookChildType,XmMAJOR_TAB);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  XtManageChild(XmCreatePushButton(frame,"AxyP",args,n));
#endif

#if !XM_NOTEBOOK
  n=0;
  XtManageChild(XmCreatePushButtonGadget(frame,"Gadget",args,n));

  XmAxyNotebookSetCurrentPage(frame,3,FALSE);
#endif

  XtRealizeWidget(toplevel);

  XtAppMainLoop(appcontext);

  return 0;
}

