/*
 * SpinButtonP.h, Interleaf, 16aug93 2:37pm Version 1.1.
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
 * (C) Copyright 1991,1992, 1993
 * Interleaf, Inc.
 * 9 Hillside Avenue, Waltham, MA  02154
 *
 * SpinButtonP.h:
 *
 * Private header file for XmSpinButtonWidget.
 */
#ifndef _SpinButtonP_h
#define _SpinButtonP_h

#include <Xm/XmP.h>
#include <Xm/DrawP.h>

#include <Xm/Label.h>
#include <Xm/TextFP.h>
#include <Xm/ArrowB.h>
#include <Xm/ManagerP.h>
#include "SpinButton.h"

/*
 * External definitions of syn_resources for our list widget.
 */
#define SYN_RESOURCE_AA AA((Widget w, int resource_offset, XtArgVal *value))
extern void _XmSpinButtonGetArrowSize	SYN_RESOURCE_AA;

#define ARROW_MULT	   .45
#define ARROW_MIN	    12
#define MAX_FLOAT_DECIMALS  6
#define NUMERIC_LENGTH      128
#define MARGIN		    2
#define LABEL_PADDING	    2
#define LABEL_SHADOW	    2
#define TEXT_FIELD_SHADOW   1

/* 
 * Class Records
 */
typedef struct
{
    Boolean junk;

} XmSpinButtonClassPart;

typedef struct _XmSpinButtonClassRec
{
    CoreClassPart		core_class;
    CompositeClassPart		composite_class;
    ConstraintClassPart		constraint_class;
    XmManagerClassPart		manager_class;
    XmSpinButtonClassPart	spin_button_class;

} XmSpinButtonClassRec;

extern XmSpinButtonClassRec xmSpinButtonClassRec;


/*
 * Instance Record.
 */
typedef struct _XmSpinButtonPart
{
    /* Private data */
    Widget                   label;
    Widget                   up_arrow;
    Widget                   down_arrow;
    unsigned char            which_arrow;
    Boolean                  init_cb;
    Boolean                  grabbed;
    int                      base;
    float                    min, max;
    float                    increment, current;
    char                     float_format[10];
    Dimension                old_width;
    Dimension                old_height;
    Dimension                label_max_length;
    Dimension                label_max_height;
    /* 
     * 11/99: JRA - Added horizontal and vertical spacing members.
     */
    Dimension                horizontal_spacing;
    Dimension                vertical_spacing;

    /* 
     * 11/99: JRA - Added font list members.
     */
    XmFontList               label_font_list;
    XmFontList               text_font_list;

    /* Resource-related data */
    XtCallbackList           activate_callback;
    unsigned char            alignment;
    unsigned char            arrow_layout;
    unsigned char            arrow_sensitivity;
    Dimension                arrow_size;
    unsigned char            child_type;
    short                    text_columns;
    unsigned int             decimal_points;
    Boolean                  editable;
    XtCallbackList           focus_callback;
    int                      numeric_increment;
    unsigned int             initial_delay;
    unsigned int             item_count;
    XmStringTable            items;
    XtCallbackList           losing_focus_callback;
    Dimension                margin_height;
    Dimension                margin_width;
    int                      maximum;
    int                      text_max_length;
    int                      minimum;
    XtCallbackList           modify_verify_callback;
    int                      position;
    Boolean                  recompute_size;
    unsigned int             repeat_delay;
    Widget                   text;
    XtCallbackList           value_changed_callback;
    Boolean                  wrap;

    /* String list related rsources */

    /* Numeric related resources */

    /* TextField resources */

} XmSpinButtonPart;


/* Full instance record declaration */
typedef struct _XmSpinButtonRec
{
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    XmManagerPart	manager;
    XmSpinButtonPart	spin_button;
} XmSpinButtonRec;

/*
 * Error defines.
 * 11/99: JRA - Added SPIN_HORIZONTAL_SPACING and SPIN_VERTICAL_SPACING errors.
 */
#define SPIN_ARROW_SENSITIVE "XmSpinButtonWidget: Invalid arrowSensitivity resource (defaulting to XmARROWS_SENSITIVE)."
#define SPIN_ALIGNMENT "XmSpinButtonWidget: Invalid alignment resource (defaulting to XmALIGNMENT_CENTER)."
#define SPIN_INIT_DELAY "XmSpinButtonWidget: Invalid initialDelay resource (defaulting to 250)."
#define SPIN_MARGIN_HEIGHT "XmSpinButtonWidget: Invalid marginHeight resource (defaulting to 2)."
#define SPIN_MARGIN_WIDTH "XmSpinButtonWidget: Invalid marginWidth resource (defaulting to 2)."
#define SPIN_HORIZONTAL_SPACING "XmSpinButtonWidget: Invalid horizontalSpacing resource (defaulting to 0)."
#define SPIN_VERTICAL_SPACING   "XmSpinButtonWidget: Invalid verticalSpacing resource (defaulting to 0)."
#define SPIN_ARROW_LAYOUT "XmSpinButtonWidget: Invalid arrowLayout resource (defaulting to XmARROWS_BEGINNING)."
#define SPIN_REPEAT_DELAY "XmSpinButtonWidget: Invalid repeatDelay resource (defaulting to 200)."
#define SPIN_ITEM_COUNT "XmSpinButtonWidget: Invalid itemCount resource (defaulting to 0)."
#define SPIN_POSITION_STRING "XmSpinButtonWidget: Invalid position resource (defaulting to 0)."
#define SPIN_POSITION_NUMERIC "XmSpinButtonWidget: Invalid position resource (defaulting to minimum)."
#define SPIN_DECIMAL_POINTS "XmSpinButtonWidget: Invalid decimalPoints resource (defaulting to 0)."
#define SPIN_MIN_MAX "XmSpinButtonWidget: Invalid minimum resource (defaulting to maximum)."
#define SPIN_TEXT "XmSpinButtonWidget: Unable to set textField resource."
#define SPIN_SET_ITEM "XmSpinButtonWidget: Unable to find item to set (XmSpinButtonSetItem)."

#endif /* _SpinButtonP_h */
