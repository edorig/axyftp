/* $Id: ComboBox.h,v 1.10 1996/08/20 20:01:47 cwikla Exp $ */
/*
 * ComboBox.h, Interleaf, 16aug93 2:37pm Version 1.1.
 */

/***********************************************************
Copyright 1993 Interleaf, Inc.

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

/*
 *  (C) Copyright 1991,1992, 1993
 *  Interleaf, Inc.
 *  Nine Hillside Avenue, Waltham, MA  02154
 *
 * ComboBox.h: 
 * 
 * Public header file for XmComboBoxWidget.
 */
#ifndef _ComboBox_h
#define _ComboBox_h

#ifndef AA
#define AA(args) ()
#endif

#include <Xm/Xm.h>

#define XmNarrowSize		"arrowSize"
#define XmNarrowSpacing		"arrowSpacing"
#define XmNarrowType		"arrowType"
#define XmNlist			"list"
#define XmNlistFontList		"listFontList"
#define XmNpoppedUp		"poppedUp"
#define XmNselectedPosition	"selectedPosition"
#define XmNselectedItem		"selectedItem"
#define XmNtextField		"textField"
#define XmNtype			"type"
#define XmNupdateLabel		"updateLabel"

#define XmCArrowSize		"ArrowSize"
#define XmCArrowSpacing		"ArrowSpacing"
#define XmCArrowType		"ArrowType"
#define XmCHorizontalSpacing	"HorizontalSpacing"
#define XmCList			"List"
#define XmCListFontList		"ListFontList"
#define XmCPoppedUp		"PoppedUp"
#define XmCSelectedPosition	"SelectedPosition"
#define XmCSelectedItem		"SelectedItem"
#define XmCType			"Type"
#define XmCTextField		"TextField"
#define XmCUpdateLabel		"UpdateLabel"
#define XmCVerticalSpacing	"VerticalSpacing"

#define XmRArrowType		"ArrowType"
#define XmRType			"Type"

/* XmNorientation defines */
#define XmLEFT		0
#define XmRIGHT		1

/* ArrowType defines */
#define XmMOTIF		0
#define XmWINDOWS	1

/* XmNtype defines */
#define XmDROP_DOWN_LIST_BOX	0
#define XmDROP_DOWN_COMBO_BOX	1

/* ComboBox callback info */
#define XmNselectionCallback	"selectionCallback"
#define XmNmenuPostCallback	"menuPostCallback"
#define XmCR_SELECT	    128 /* Large #, so no collisions with XM */
#define XmCR_MENU_POST      129 /* Large #, so no collisions with XM */

typedef struct {
   int 	     reason;
   XEvent    *event;
   XmString  item_or_text;
   int       item_position;
} XmComboBoxCallbackStruct;

extern WidgetClass xmComboBoxWidgetClass;

typedef struct _XmComboBoxClassRec *XmComboBoxWidgetClass;
typedef struct _XmComboBoxRec      *XmComboBoxWidget;

/*
 * External functions which manipulate the ComboBox list of items.
 */
extern Widget XmCreateComboBox AA((Widget parent, char *name,
				   Arg *arglist, int num_args));
extern void XmComboBoxAddItem AA((XmComboBoxWidget combo, XmString item,
				  int pos, Boolean unique));
extern void XmComboBoxDeletePos AA((XmComboBoxWidget combo, int pos));
extern int XmComboBoxSetItem AA((XmComboBoxWidget combo, XmString item));
extern void XmComboBoxSelectItem AA((XmComboBoxWidget combo, XmString item));
extern int XmComboBoxGetSelectedPosition AA((Widget w));
extern int XmComboBoxItemCount AA((Widget w));

extern char *XmComboBoxGetTextString AA((Widget w));
extern void XmComboBoxSetTextString AA((Widget w, char *_text));
extern void XmComboBoxSelectItemPos AA((XmComboBox combo, int pos));
extern void XmComboBoxDeselectAllItems AA((XmComboBox combo));
extern XmString XmComboBoxGetXmStringAtPos(Widget _w, int _pos);
extern XmString XmComboBoxGetLabelXmString(Widget w);
extern Widget XmComboBoxGetList(Widget _w);
extern void XmComboBoxReplaceItemAtPos(Widget _w, XmString _item, int _pos);
extern Widget XmComboBoxGetTextField(Widget _w);


#define XmIsComboBox(a) XtIsSubclass((a), xmComboBoxWidgetClass)


#endif	/* _ComboBox_h */
