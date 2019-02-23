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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AA
#define AA(args) ()
#endif

#include <Xm/Xm.h>

#ifndef DtNcomboBoxType
#define DtNcomboBoxType		"comboBoxType"
#endif

#ifndef DtNmarginHeight
#define DtNmarginHeight		"marginHeight"
#endif

#ifndef DtNmarginWidth
#define DtNmarginWidth		"marginWidth"
#endif

#ifndef DtNselectedItem
#define DtNselectedItem		"selectedItem"
#endif

#ifndef DtNselectedPosition
#define DtNselectedPosition	"selectedPosition"
#endif

#ifndef DtNselectionCallback
#define DtNselectionCallback	"selectionCallback"
#endif


/* Following resources are supposed to be unsupported in CDE 1.0*/

#ifndef DtNactivateCallback
#define DtNactivateCallback	"activateCallback"
#endif

#ifndef DtNalignment
#define DtNalignment		"alignment"
#endif

#ifndef DtNarrowSize
#define DtNarrowSize		"arrowSize"
#endif

#ifndef DtNarrowSpacing
#define DtNarrowSpacing		"arrowSpacing"
#endif

#ifndef DtNarrowType
#define DtNarrowType		"arrowType"
#endif

#ifndef DtNcolumns
#define DtNcolumns		"columns"
#endif

#ifndef DtNfocusCallback
#define DtNfocusCallback	"focusCallback"
#endif

#ifndef DtNhorizontalSpacing
#define DtNhorizontalSpacing	"horizontalSpacing"
#endif

#ifndef DtNitemCount
#define DtNitemCount		"itemCount"
#endif

#ifndef DtNitems
#define DtNitems		"items"
#endif

#ifndef DtNlabelString
#define DtNlabelString		"labelString"
#endif

#ifndef DtNlist
#define DtNlist			"list"
#endif

#ifndef DtNlistFontList
#define DtNlistFontList		"listFontList"
#endif

#ifndef DtNlistMarginHeight
#define DtNlistMarginHeight	"listMarginHeight"
#endif

#ifndef DtNlistMarginWidth
#define DtNlistMarginWidth	"listMarginWidth"
#endif

#ifndef DtNlistSpacing
#define DtNlistSpacing		"listSpacing"
#endif

#ifndef DtNlosingFocusCallback
#define DtNlosingFocusCallback	"losingFocusCallback"
#endif

#ifndef DtNmaxLength
#define DtNmaxLength		"maxLength"
#endif

#ifndef DtNmenuPostCallback
#define DtNmenuPostCallback	"menuPostCallback"
#endif

#ifndef DtNorientation
#define DtNorientation		"orientation"
#endif

#ifndef DtNpoppedUp
#define DtNpoppedUp		"poppedUp"
#endif

#ifndef DtNrecomputeSize
#define DtNrecomputeSize	"recomputeSize"
#endif

#ifndef DtNtextField
#define DtNtextField		"textField"
#endif

#ifndef DtNtopItemPosition
#define DtNtopItemPosition	"topItemPosition"
#endif

#ifndef DtNupdateLabel
#define DtNupdateLabel		"updateLabel"
#endif

#ifndef	DtNverticalSpacing
#define	DtNverticalSpacing	"verticalSpacing"
#endif

#ifndef DtNvisibleItemCount
#define DtNvisibleItemCount	"visibleItemCount"
#endif

/* End of "unsupported" section */

#ifndef DtCCallback
#define DtCCallback             "Callback"
#endif
#ifndef DtCComboBoxType
#define DtCComboBoxType         "ComboBoxType"
#endif
#ifndef DtCMarginHeight
#define DtCMarginHeight         "MarginHeight"
#endif
#ifndef DtCMarginWidth
#define DtCMarginWidth          "MarginWidth"
#endif
#ifndef DtCSelectedItem
#define DtCSelectedItem         "SelectedItem"
#endif
#ifndef DtCSelectedPosition
#define DtCSelectedPosition     "SelectedPosition"
#endif
#ifndef DtCXmString
#define DtCXmString             "XmString"
#endif


/* "Unsupported" classes */

#ifndef DtCAlignment
#define DtCAlignment		"Alignment"
#endif
#ifndef DtCArrowSize
#define DtCArrowSize		"ArrowSize"
#endif
#ifndef DtCArrowSpacing
#define DtCArrowSpacing		"ArrowSpacing"
#endif
#ifndef DtCArrowType
#define DtCArrowType            "ArrowType"
#endif
#ifndef DtCColumns
#define DtCColumns              "Columns"
#endif
#ifndef DtCHorizontalSpacing
#define DtCHorizontalSpacing    "HorizontalSpacing"
#endif
#ifndef DtCItemCount
#define DtCItemCount            "ItemCount"
#endif
#ifndef DtCItems
#define DtCItems                "Items"
#endif
#ifndef DtCList
#define DtCList                 "List"
#endif
#ifndef DtCListFontList
#define DtCListFontList         "ListFontList"
#endif
#ifndef DtCListMarginHeight
#define DtCListMarginHeight     "ListMarginHeight"
#endif
#ifndef DtCListMarginWidth
#define DtCListMarginWidth      "ListMarginWidth"
#endif
#ifndef DtCListSpacing
#define DtCListSpacing          "ListSpacing"
#endif
#ifndef DtCMaxLength
#define DtCMaxLength            "MaxLength"
#endif
#ifndef DtCOrientation
#define DtCOrientation          "Orientation"
#endif
#ifndef DtCPoppedUp
#define DtCPoppedUp             "PoppedUp"
#endif
#ifndef DtCRecomputeSize
#define DtCRecomputeSize        "RecomputeSize"
#endif
#ifndef DtCTextField
#define DtCTextField            "TextField"
#endif
#ifndef DtCTopItemPosition
#define DtCTopItemPosition      "TopItemPosition"
#endif
#ifndef DtCUpdateLabel
#define DtCUpdateLabel          "UpdateLabel"
#endif
#ifndef DtCVerticalSpacing
#define DtCVerticalSpacing      "VerticalSpacing"
#endif
#ifndef DtCVisibleItemCount
#define DtCVisibleItemCount     "VisibleItemCount"
#endif

/* End of "unsupported" classes */


#ifndef	DtRArrowType
#define DtRArrowType		"ArrowType"
#endif
#ifndef DtRComboBoxType
#define DtRComboBoxType		"ComboBoxType"
#endif

/* Orientation defines */
#ifndef DtLEFT
#define DtLEFT		1
#endif
#ifndef DtRIGHT
#define DtRIGHT		2
#endif

/* ArrowType defines */
#ifndef DtMOTIF
#define DtMOTIF		0
#endif
#ifndef DtWINDOWS
#define DtWINDOWS	1
#endif

/* ComboBoxType defines */
#ifndef DtDROP_DOWN_LIST
#define DtDROP_DOWN_LIST	0
#endif
#ifndef DtDROP_DOWN_COMBO_BOX
#define DtDROP_DOWN_COMBO_BOX	1
#endif

/* Alignment values */
#ifndef DtALIGNMENT_BEGINNING
#define DtALIGNMENT_BEGINNING   XmALIGNMENT_BEGINNING
#endif
#ifndef DtALIGNMENT_CENTER
#define DtALIGNMENT_CENTER      XmALIGNMENT_CENTER
#endif
#ifndef DtALIGNMENT_END
#define DtALIGNMENT_END         XmALIGNMENT_END
#endif

/* ComboBox callback info */
#ifndef DtCR_SELECT
#define DtCR_SELECT	    57 /* Large #, so no collisions with XM */
#endif
#ifndef DtCR_MENU_POST
#define DtCR_MENU_POST      129 /* Large #, so no collisions with XM */
#endif

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
extern Widget DtCreateComboBox (Widget parent, char *name,
				   Arg *arglist, Cardinal num_args);
extern void DtComboBoxAddItem (Widget combo, XmString item,
				  int pos, Boolean unique);
extern void DtComboBoxDeletePos (Widget combo, int pos);
extern void DtComboBoxSelectItem (Widget combo, XmString item);
extern void DtComboBoxSetItem (Widget combo, XmString item);

#ifndef DtIsComboBox
#define DtIsComboBox(a) XtIsSubclass((a), dtComboBoxWidgetClass)
#endif

#ifdef __cplusplus
}
#endif

#endif	/* _ComboBox_h */
