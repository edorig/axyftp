/* Copyright (c) 1999   Alexander Yukhimets. All rights reserved. */

#ifndef XM_AXY_NOTEBOOK_H
#define XM_AXY_NOTEBOOK_H

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif

extern WidgetClass xmAxyNotebookWidgetClass;

typedef struct _XmAxyNotebookClassRec *XmAxyNotebookWidgetClass;
typedef struct _XmAxyNotebookRec *XmAxyNotebookWidget;

/* new resources */

#ifndef XmNfontList		
#define XmNfontList 		"fontList"
#endif 

#ifndef XmNhighlightThickness
#define XmNhighlightThickness 	"highlightThickness"
#endif

#ifndef XmNinnerMarginHeight
#define XmNinnerMarginHeight 	"innerMarginHeight"
#endif

#ifndef XmNinnerMarginWidth
#define XmNinnerMarginWidth 	"innerMarginWidth"
#endif

#ifndef XmNlabelMarginHeight
#define XmNlabelMarginHeight 	"labelMarginHeight"
#endif

#ifndef XmNlabelMarginWidth
#define XmNlabelMarginWidth 	"labelMarginWidth"
#endif

#ifndef XmNmarginHeight
#define XmNmarginHeight 	"marginHeight"
#endif

#ifndef XmNmarginWidth
#define XmNmarginWidth 		"marginWidth"
#endif

#ifndef XmNpageChangedCallback
#define XmNpageChangedCallback 	"pageChangedCallback"
#endif

#ifndef XmNselectColor		
#define XmNselectColor 		"selectColor"
#endif 

#ifndef XmNshowTabs
#define XmNshowTabs 		"showTabs"
#endif

#ifndef XmNtabMarginHeight
#define XmNtabMarginHeight 	"tabMarginHeight"
#endif

#ifndef XmNtabMarginWidth
#define XmNtabMarginWidth 	"tabMarginWidth"
#endif

#ifndef XmNtabRaise
#define XmNtabRaise	 	"tabRaise"
#endif

#ifndef XmNtabSpacing
#define XmNtabSpacing 		"tabSpacing"
#endif


/* constraint resources */

#ifndef XmNresizable
#define XmNresizable		"resizable"
#endif

#ifndef XmNtablabel
#define XmNtabLabel 		"tabLabel"
#endif


/* classes */

#ifndef XmCFontList		
#define XmCFontList 		"FontList"
#endif 

#ifndef XmCHighlightThickness
#define XmCHighlightThickness 	"HighlightThickness"
#endif

#ifndef XmCInnerMarginHeight
#define XmCInnerMarginHeight 	"InnerMarginHeight"
#endif

#ifndef XmCInnerMarginWidth
#define XmCInnerMarginWidth 	"InnerMarginWidth"
#endif

#ifndef XmCLabelMarginHeight
#define XmCLabelMarginHeight 	"LabelMarginHeight"
#endif

#ifndef XmCLabelMarginWidth
#define XmCLabelMarginWidth 	"LabelMarginWidth"
#endif

#ifndef XmCMarginHeight
#define XmCMarginHeight 	"MarginHeight"
#endif

#ifndef XmCMarginWidth
#define XmCMarginWidth 		"MarginWidth"
#endif

#ifndef XmCPageChangedCallback
#define XmCPageChangedCallback 	"PageChangedCallback"
#endif

#ifndef XmCSelectColor		
#define XmCSelectColor 		"SelectColor"
#endif 

#ifndef XmCShowTabs
#define XmCShowTabs 		"ShowTabs"
#endif

#ifndef XmCTabMarginHeight
#define XmCTabMarginHeight 	"TabMarginHeight"
#endif

#ifndef XmCTabMarginWidth
#define XmCTabMarginWidth 	"TabMarginWidth"
#endif

#ifndef XmCTabRaise
#define XmCTabRaise 		"TabRaise"
#endif

#ifndef XmCTabSpacing
#define XmCTabSpacing	 	"TabSpacing"
#endif

/* constraint classes */

#ifndef XmCResizable
#define XmCResizable		"Resizable"
#endif

#ifndef XmCTablabel
#define XmCTabLabel 		"TabLabel"
#endif


#ifndef XmUNSPECIFIED_PIXEL
#define XmUNSPECIFIED_PIXEL	(Pixel)(~0)
#endif

#define XmAxyNotebookPageChangedReason	1

typedef struct {
	int reason;
	XEvent *event;
	int	page_number;
	Widget  page_widget;
	XmString tab_label;
	int	prev_page_number;
	Widget  prev_page_widget;
	XmString prev_tab_label;
} XmAxyNotebookCallbackStruct;

extern void XmAxyNotebookSetCurrentWidget(Widget, Widget, Boolean);
extern Widget XmAxyNotebookGetCurrentWidget(Widget);

extern void XmAxyNotebookSetCurrentPage(Widget, int, Boolean);
extern int XmAxyNotebookGetCurrentPage(Widget);

extern int XmAxyNotebookGetNumberOfPages(Widget);

extern Widget XmAxyCreateNotebook(Widget,String,ArgList,Cardinal);

#ifndef XmAxyIsNotebook
#define XmAxyIsNotebook(w) XtIsSubclass((w), xmAxyNotebookWidgetClass)
#endif

#ifdef __cplusplus
} 
#endif

#endif /* XM_AXY_NOTEBOOK_H */

