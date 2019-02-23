/* Copyright (c) 2000   Alexander Yukhimets. All rights reserved. */

#ifndef XM_AXY_NOTEBOOK_P_H
#define XM_AXY_NOTEBOOK_P_H

#include <Xm/XmP.h>
#include <Xm/ManagerP.h>

#include <XmAxy/Notebook.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _XmAxyNotebookClassPart {
	XtPointer extension;
} XmAxyNotebookClassPart;

typedef struct _XmAxyNotebookClassRec {
	CoreClassPart core_class;
	CompositeClassPart composite_class;
	ConstraintClassPart constraint_class;
	XmManagerClassPart manager_class;
	XmAxyNotebookClassPart notebook_class;
} XmAxyNotebookClassRec;

externalref XmAxyNotebookClassRec xmAxyNotebookClassRec;

typedef struct _XmAxyNotebookPart {
        Pixel arm_color;

	XmFontList font_list;

	Boolean	show_tabs;

	Dimension highlight_thickness;
	Dimension margin_width;
	Dimension margin_height;
	Dimension inner_margin_width;
	Dimension inner_margin_height;
	Dimension tab_spacing;
	Dimension tab_raise;
	Dimension tab_margin_width;
	Dimension tab_margin_height;
	Dimension label_margin_width;
	Dimension label_margin_height;

	XtCallbackList page_changed_callback;

	/* private */
	
        Font font_fid;
        Dimension font_width;
        Dimension font_height;
        Dimension font_y;

	Widget drawing_area;
	Widget highlighted_tab;
	Boolean highlight_drawn;
	Widget current_widget;
	int current_page;
	int num_pages;

	Dimension tab_height;
	Dimension child_x;
	Dimension child_y;
	Dimension child_width;
	Dimension child_height;

	GC normal_gc;
	GC arm_gc;

	Boolean tab_child;
} XmAxyNotebookPart;

typedef struct _XmAxyNotebookRec {
	CorePart core;
	CompositePart composite;
	ConstraintPart constraint;
	XmManagerPart manager;
	XmAxyNotebookPart notebook;
} XmAxyNotebookRec;

typedef struct _XmAxyNotebookConstraintPart {
	XmString tab_label;
	Boolean	 resizable;
	/* private */
	XRectangle tab_rect;
	XRectangle tab_clip;
	Boolean	tab_visible;  
	int page_number;
	Boolean is_tab;
} XmAxyNotebookConstraintPart;

typedef struct _XmAxyNotebookConstraintRec {
	XmManagerConstraintPart manager;
	XmAxyNotebookConstraintPart notebook;
} XmAxyNotebookConstraintRec;

#ifdef __cplusplus
}  
#endif

#endif /* XM_AXY_NOTEBOOK_P_H */


