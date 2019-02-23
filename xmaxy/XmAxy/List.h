/* Copyright (c) 1999   Alexander Yukhimets. All rights reserved. */

#ifndef XM_AXY_LIST_H
#define XM_AXY_LIST_H

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif

externalref WidgetClass xmAxyListWidgetClass;

typedef struct _XmAxyListClassRec *XmAxyListWidgetClass;
typedef struct _XmAxyListRec *XmAxyListWidget;

/* New resources */
#ifndef XmNactiveLabels
#define XmNactiveLabels			"activeLabels"
#endif

#ifndef XmNboldLabels
#define XmNboldLabels			"boldLabels"
#endif

#ifndef XmNcellBackground
#define XmNcellBackground		"cellBackground"
#endif

#ifndef XmNcellForeground
#define XmNcellForeground		"cellForeground"
#endif

#ifndef XmNcellMarginHeight
#define XmNcellMarginHeight		"cellMarginHeight"
#endif

#ifndef XmNcellMarginWidth
#define XmNcellMarginWidth		"cellMarginWidth"
#endif

#ifndef XmNcolumnAlignments
#define XmNcolumnAlignments		"columnAlignments"
#endif

#ifndef XmNcolumnCount
#define XmNcolumnCount			"columnCount"
#endif

#ifndef XmNcolumnLabelAlignments
#define XmNcolumnLabelAlignments 	"columnLabelAlignments"
#endif
#ifndef XmNcolumnLabels
#define XmNcolumnLabels			"columnLabels"
#endif

#ifndef XmNcolumnWidths
#define XmNcolumnWidths			"columnWidths"
#endif

#ifndef XmNdoubleClickInterval
#define XmNdoubleClickInterval	 	"doubleClickInterval"
#endif

#ifndef XmNdrawCellCallback
#define XmNdrawCellCallback	 	"drawCellCallback"
#endif

#ifndef XmNfontList
#define XmNfontList			"fontList"
#endif

#ifndef XmNshowLabels
#define XmNshowLabels			"showLabels"
#endif

#ifndef XmNlabelActivateCallback
#define XmNlabelActivateCallback 	"labelActivateCallback"
#endif

#ifndef XmNlabelBackground
#define XmNlabelBackground		"labelBackground"
#endif

#ifndef XmNlabelBottomShadowColor
#define XmNlabelBottomShadowColor	"labelBottomShadowColor"
#endif

#ifndef XmNlabelBottomShadowPixmap
#define XmNlabelBottomShadowPixmap	"labelBottomShadowPixmap"
#endif

#ifndef XmNlabelFontList
#define XmNlabelFontList		"labelFontList"
#endif

#ifndef XmNlabelForeground
#define XmNlabelForeground		"labelForeground"
#endif

#ifndef XmNlabelMarginHeight
#define XmNlabelMarginHeight		"labelMarginHeight"
#endif

#ifndef XmNlabelMarginWidth
#define XmNlabelMarginWidth		"labelMarginWidth"
#endif

#ifndef XmNlabelShadowThickness
#define XmNlabelShadowThickness		"labelShadowThickness"
#endif

#ifndef XmNlabelTopShadowColor
#define XmNlabelTopShadowColor		"labelTopShadowColor"
#endif

#ifndef XmNlabelTopShadowPixmap
#define XmNlabelTopShadowPixmap		"labelTopPixmap"
#endif

#ifndef XmNlistMarginHeight
#define XmNlistMarginHeight		"listMarginHeight"
#endif

#ifndef XmNlistMarginWidth
#define XmNlistMarginWidth		"listMarginWidth"
#endif

#ifndef XmNrowCount
#define XmNrowCount			"rowCount"
#endif

#ifndef XmNrowSpacing
#define XmNrowSpacing			"rowSpacing"
#endif

#ifndef XmNselectedBackground
#define XmNselectedBackground		"selectBackground"
#endif

#ifndef XmNselectedForeground
#define XmNselectedForeground		"selectForeground"
#endif

#ifndef XmNstringDirection
#define XmNstringDirection		"stringDirection"
#endif

/* classes */
#ifndef XmCActiveLabels
#define XmCActiveLabels			"ActiveLabels"
#endif

#ifndef XmCBoldLabels
#define XmCBoldLabels			"BoldLabels"
#endif

#ifndef XmCCellBackground
#define XmCCellBackground		"CellBackground"
#endif

#ifndef XmCCellForeground
#define XmCCellForeground		"CellForeground"
#endif

#ifndef XmCCellMarginHeight
#define XmCCellMarginHeight		"CellMarginHeight"
#endif

#ifndef XmCCellMarginWidth
#define XmCCellMarginWidth		"CellMarginWidth"
#endif

#ifndef XmCColumnAlignments
#define XmCColumnAlignments		"ColumnAlignments"
#endif

#ifndef XmCColumnCount
#define XmCColumnCount			"ColumnCount"
#endif

#ifndef XmCColumnLabelAlignments
#define XmCColumnLabelAlignments	"ColumnLabelAlignments"
#endif

#ifndef XmCColumnLabels
#define XmCColumnLabels			"ColumnLabels"
#endif

#ifndef XmCColumnWidths
#define XmCColumnWidths			"ColumnWidths"
#endif

#ifndef XmCDoubleClickInterval
#define XmCDoubleClickInterval	 	"DoubleClickInterval"
#endif

#ifndef XmCFontList
#define XmCFontList			"FontList"
#endif

#ifndef XmCShowLabels
#define XmCShowLabels			"ShowLabels"
#endif

#ifndef XmCLabelBackground
#define XmCLabelBackground		"LabelBackground"
#endif

#ifndef XmCLabelBottomShadowColor
#define XmCLabelBottomShadowColor	"LabelBottomShadowColor"
#endif

#ifndef XmCLabelBottomShadowPixmap
#define XmCLabelBottomShadowPixmap	"LabelBottomShadowPixmap"
#endif

#ifndef XmCLabelForeground
#define XmCLabelForeground		"LabelForeground"
#endif

#ifndef XmCLabelMarginHeight
#define XmCLabelMarginHeight		"LabelMarginHeight"
#endif

#ifndef XmCLabelMarginWidth
#define XmCLabelMarginWidth		"LabelMarginWidth"
#endif

#ifndef XmCLabelTopShadowColor
#define XmCLabelTopShadowColor		"LabelTopShadowColor"
#endif

#ifndef XmCLabelTopShadowPixmap
#define XmCLabelTopShadowPixmap		"LabelTopShadowPixmap"
#endif

#ifndef XmCListMarginHeight
#define XmCListMarginHeight		"ListMarginHeight"
#endif

#ifndef XmCListMarginWidth
#define XmCListMarginWidth		"ListMarginWidth"
#endif

#ifndef XmCRowCount
#define XmCRowCount			"RowCount"
#endif

#ifndef XmCRowSpacing
#define XmCRowSpacing			"RowSpacing"
#endif

#ifndef XmCSelectedBackground
#define XmCSelectedBackground		"SelectedBackground"
#endif

#ifndef XmCSelectedForeground
#define XmCSelectedForeground		"SelectedForeground"
#endif

#ifndef XmCStringDirection
#define XmCStringDirection		"StringDirection"
#endif


/* representation types */
#ifndef XmRAlignmentArray
#define XmRAlignmentArray		"AlignmentArray"
#endif

#ifndef XmRWidthArray
#define XmRWidthArray			"WidthArray"
#endif


#ifndef XmUNSPECIFIED
#define XmUNSPECIFIED			(~0)
#endif

#ifndef XmUNSPECIFIED_PIXEL
#define XmUNSPECIFIED_PIXEL		(Pixel)(~0)
#endif

#ifndef XmUNSPECIFIED_PIXMAP
#define XmUNSPECIFIED_PIXMAP		(Pixmap)(~0)
#endif

typedef enum { XmAxyListDrawCellReason=1, 
               XmAxyListDefaultActionReason,
	       XmAxyListLabelActivateReason
} XmAxyReasonType;

typedef struct {

  XmAxyReasonType 	reason;
  XEvent*		event;
  int			row;
  int 			column;

} XmAxyListAnyCallbackStruct;

typedef struct {

  XmAxyReasonType 	reason;
  XEvent*		event;
  int			row;
  int 			column;

} XmAxyListDefaultActionCallbackStruct;

typedef struct {

  XmAxyReasonType 	reason;
  XEvent*		event;
  int			row;
  int 			column;
  int			type;

  Dimension		width;
  Dimension		height;
  
  String		string;
  Boolean		is_xm_string;

  Pixmap		pixmap;
  Pixmap		pixmask;
  unsigned int		pixwidth;
  unsigned int		pixheight;
  unsigned int		pixdepth;

  Pixel			foreground;
  Pixel			background;

} XmAxyListDrawCellCallbackStruct;

extern Widget XmAxyCreateList(Widget parent, char *name,
                              Arg *arglist, Cardinal n);
extern Widget XmAxyCreateScrolledList(Widget parent, char *name,
                                      Arg *arglist, Cardinal n);

#ifndef XmAxyIsList
#define XmAxyIsList(w) XtIsSubclass((w), xmAxyListWidgetClass)
#endif

extern void XmAxyListRefresh(Widget list);
extern void XmAxyListDeselectAllRows(Widget list);
extern void XmAxyListDeselectRow(Widget list,int row);
extern void XmAxyListSelectRow(Widget list,int row);
extern Boolean XmAxyListGetSelectedRows(Widget list,
    int** selected_rows_list,int* selected_rows_count);
extern Boolean XmAxyListRowSelected(Widget list,int row);

#ifdef __cplusplus
}
#endif

#endif /* XM_AXY_LIST_H */
