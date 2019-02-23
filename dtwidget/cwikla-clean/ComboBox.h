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
 * Public header file for DtComboBoxWidget.
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
} DtComboBoxCallbackStruct;

extern WidgetClass dtComboBoxWidgetClass;

typedef struct _DtComboBoxClassRec *DtComboBoxWidgetClass;
typedef struct _DtComboBoxRec      *DtComboBoxWidget;

/*
 * External functions which manipulate the ComboBox list of items.
 */
extern Widget DtCreateComboBox AA((Widget parent, char *name,
				   Arg *arglist, int num_args));
extern void DtComboBoxAddItem AA((DtComboBoxWidget combo, XmString item,
				  int pos, Boolean unique));
extern void DtComboBoxDeletePos AA((DtComboBoxWidget combo, int pos));
extern void DtComboBoxSetItem AA((DtComboBoxWidget combo, XmString item));
extern void DtComboBoxSelectItem AA((DtComboBoxWidget combo, XmString item));


extern int DtComboBoxGetSelectedPosition AA((Widget w));
extern int DtComboBoxItemCount AA((Widget w));
extern char *DtComboBoxGetTextString AA((Widget w));
extern void DtComboBoxSetTextString AA((Widget w, char *_text));
extern void DtComboBoxSelectItemPos AA((XmComboBox combo, int pos));
extern void DtComboBoxDeselectAllItems AA((XmComboBox combo));
extern XmString DtComboBoxGetXmStringAtPos(Widget _w, int _pos);
extern XmString DtComboBoxGetLabelXmString(Widget w);
extern Widget DtComboBoxGetList(Widget _w);
extern void DtComboBoxReplaceItemAtPos(Widget _w, XmString _item, int _pos);
extern Widget DtComboBoxGetTextField(Widget _w);

#define DtIsComboBox(a) XtIsSubclass((a), dtComboBoxWidgetClass)


#endif	/* _ComboBox_h */
