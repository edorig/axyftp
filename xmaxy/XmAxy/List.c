/* Copyright (c) 1999   Alexander Yukhimets. All rights reserved. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <X11/StringDefs.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include<XmAxy/ListP.h>
#include<XmAxy/Util.h>

#include <Xm/XmP.h>
#include <Xm/DrawP.h>
#include <Xm/DragIcon.h>
#include <Xm/DragC.h>

#define RESIZE_STRIP_WITH_HIDDEN_LABELS 4
#define RESIZE_MARGIN 3
#define COL_MIN_SIZE 8

#define CLICK_INTERVAL(w)		\
  (((XmAxyListWidget)(w))->list.click_interval)
#define CLICK_COUNT(w)		\
  (((XmAxyListWidget)(w))->list.click_count)
#define CLICK_TIME(w)		\
  (((XmAxyListWidget)(w))->list.click_time)
#define SELECTION_DRAG(w)		\
  (((XmAxyListWidget)(w))->list.selection_drag)
#define SCROLL_DIRECTION(w)		\
  (((XmAxyListWidget)(w))->list.scroll_direction)
#define SCROLL_TIMER_ID(w)		\
  (((XmAxyListWidget)(w))->list.scroll_timer_id)
#define SCROLL_TIMER_SET(w)		\
  (((XmAxyListWidget)(w))->list.scroll_timer_set)
#define LAST_SELECTED_ROW(w)		\
  (((XmAxyListWidget)(w))->list.last_selected_row)
#define ANCHOR(w)		\
  (((XmAxyListWidget)(w))->list.anchor)
#define SELECTED_ROWS(w)		\
  (((XmAxyListWidget)(w))->list.selected_row)
#define SELECTED_ROW(w,i)     (SELECTED_ROWS(w)[i])
#define SELECTED_ROW_COUNT(w)			\
  (((XmAxyListWidget)(w))->list.selected_row_count)
#define ROW_IS_SELECTED(w,i)  (ROW_IS_SELECTED_ARRAY(w)[i])
#define ROW_IS_SELECTED_ARRAY(w)		\
  (((XmAxyListWidget)(w))->list.row_is_selected)

#define RESIZE_CURSOR(w)			\
  (((XmAxyListWidget)(w))->list.resize_cursor)
#define COL_RESIZE_AREA(w)			\
  (((XmAxyListWidget)(w))->list.col_resize_area)
#define COL_RESIZE_UNDERWAY(w)			\
  (((XmAxyListWidget)(w))->list.col_resize_underway)
#define COL_RESIZE_NUM(w)			\
  (((XmAxyListWidget)(w))->list.col_resize_num)
#define COL_RESIZE_X(w)			\
  (((XmAxyListWidget)(w))->list.col_resize_x)

#define EHT(w)	(HIGHLIGHT_THICKNESS(w)?(HIGHLIGHT_THICKNESS(w)+(Dimension)1):\
    (Dimension)0)

#define MARGIN_WIDTH(w)			\
  (((XmAxyListWidget)(w))->list.margin_width)
#define MARGIN_HEIGHT(w)		\
  (((XmAxyListWidget)(w))->list.margin_height)
#define CELL_MARGIN_WIDTH(w)    	\
  (((XmAxyListWidget)(w))->list.cell_margin_width)
#define CELL_MARGIN_HEIGHT(w)		\
  (((XmAxyListWidget)(w))->list.cell_margin_height)
#define LABEL_MARGIN_WIDTH(w)		\
  (((XmAxyListWidget)(w))->list.label_margin_width)
#define LABEL_MARGIN_HEIGHT(w)		\
  (((XmAxyListWidget)(w))->list.label_margin_height)

#define FONT(w)				\
  (((XmAxyListWidget)(w))->list.font)
#define FONT_FID(w)			\
  (((XmAxyListWidget)(w))->list.font_fid)
#define FONT_STRUCT(w)			\
  (((XmAxyListWidget)(w))->list.font_struct)
#define FONT_SET(w)			\
  (((XmAxyListWidget)(w))->list.font_set)
#define FONT_WIDTH(w) 			\
  (((XmAxyListWidget)(w))->list.font_width)
#define FONT_HEIGHT(w) 			\
  (((XmAxyListWidget)(w))->list.font_height)
#define FONT_Y(w)			\
  (((XmAxyListWidget)(w))->list.font_y)
#define LABEL_FONT(w)			\
  (((XmAxyListWidget)(w))->list.font)
#define LABEL_FONT_FID(w)		\
  (((XmAxyListWidget)(w))->list.label_font_fid)
#define LABEL_FONT_STRUCT(w)			\
  (((XmAxyListWidget)(w))->list.label_font_struct)
#define LABEL_FONT_SET(w)			\
  (((XmAxyListWidget)(w))->list.label_font_set)
#define LABEL_FONT_WIDTH(w)		\
  (((XmAxyListWidget)(w))->list.label_font_width)
#define LABEL_FONT_HEIGHT(w)		\
  (((XmAxyListWidget)(w))->list.label_font_height)
#define LABEL_FONT_Y(w)		\
  (((XmAxyListWidget)(w))->list.label_font_y)

#define BOLD_LABELS(w)			\
  (((XmAxyListWidget)(w))->list.bold_labels)
#define SHOW_LABELS(w)			\
  (((XmAxyListWidget)(w))->list.show_labels)
#define ACTIVE_LABELS(w)		\
  (((XmAxyListWidget)(w))->list.active_labels)

#define LABEL_SHADOW_THICKNESS(w) 	\
  (((XmAxyListWidget)(w))->list.label_shadow_thickness)
#define LABEL_BACKGROUND(w)		\
  (((XmAxyListWidget)(w))->list.label_background)
#define LABEL_FOREGROUND(w)		\
  (((XmAxyListWidget)(w))->list.label_foreground)
#define LABEL_TOP_SHADOW_COLOR(w)	\
  (((XmAxyListWidget)(w))->list.label_top_shadow_color)
#define LABEL_BOTTOM_SHADOW_COLOR(w)	\
  (((XmAxyListWidget)(w))->list.label_bottom_shadow_color)
#define LABEL_TOP_SHADOW_PIXMAP(w)	\
  (((XmAxyListWidget)(w))->list.label_top_shadow_pixmap)
#define LABEL_BOTTOM_SHADOW_PIXMAP(w)	\
  (((XmAxyListWidget)(w))->list.label_bottom_shadow_pixmap)

#define CELL_BACKGROUND(w)		\
  (((XmAxyListWidget)(w))->list.cell_background)
#define CELL_FOREGROUND(w)		\
  (((XmAxyListWidget)(w))->list.cell_foreground)
#define SELECTED_BACKGROUND(w) 		\
  (((XmAxyListWidget)(w))->list.selected_background)
#define SELECTED_FOREGROUND(w)		\
  (((XmAxyListWidget)(w))->list.selected_foreground)

#define BACKGROUND_GC(w)		\
  (((XmAxyListWidget)(w))->list.background_gc)
#define LABEL_BACKGROUND_GC(w)		\
  (((XmAxyListWidget)(w))->list.label_background_gc)
#define LABEL_TOP_SHADOW_GC(w)		\
  (((XmAxyListWidget)(w))->list.label_top_shadow_gc)
#define LABEL_BOTTOM_SHADOW_GC(w)	\
  (((XmAxyListWidget)(w))->list.label_bottom_shadow_gc)
#define LABEL_DRAW_GC(w)			\
  (((XmAxyListWidget)(w))->list.label_draw_gc)
#define CELL_BACKGROUND_GC(w)		\
  (((XmAxyListWidget)(w))->list.cell_background_gc)
#define DRAW_GC(w)			\
  (((XmAxyListWidget)(w))->list.draw_gc)
#define SELECTED_BACKGROUND_GC(w)		\
  (((XmAxyListWidget)(w))->list.selected_background_gc)
#define SELECTED_DRAW_GC(w)			\
  (((XmAxyListWidget)(w))->list.selected_draw_gc)

#define STRING_DIR(w)			\
  (((XmAxyListWidget)(w))->list.string_dir)

#define ROW_COUNT(w)			\
  (((XmAxyListWidget)(w))->list.row_count)
#define VISIBLE_ROW_COUNT(w)			\
  (((XmAxyListWidget)(w))->list.visible_row_count)
#define ROW_SPACING(w)			\
  (((XmAxyListWidget)(w))->list.row_spacing)
#define ROW_OFFSET(w,i)			\
  (SHADOW_THICKNESS(w)+MARGIN_HEIGHT(w)+\
  (SHOW_LABELS(w)?LABEL_HEIGHT(w):0)+EHT(w)+\
  (ROW_HEIGHT(w)+EHT(w)+ROW_SPACING(w))*(i))
#define ROW_HEIGHT(w)			\
  (CELL_MARGIN_HEIGHT(w)*2+FONT_HEIGHT(w))
#define LABEL_HEIGHT(w)			\
  (LABEL_SHADOW_THICKNESS(w)*2+EHT(w)*2+LABEL_MARGIN_HEIGHT(w)*2+FONT_HEIGHT(w))

#define COLUMN_COUNT(w)			\
  (((XmAxyListWidget)(w))->list.column_count)
#define COLUMN_LABELS(w)		\
  (((XmAxyListWidget)(w))->list.column_labels)
#define COLUMN_LABEL(w,i)		\
  (((XmAxyListWidget)(w))->list.column_labels[(i)])
#define COLUMN_WIDTHS(w)		\
  (((XmAxyListWidget)(w))->list.column_widths)
#define COLUMN_WIDTH(w,i)		\
  (COLUMN_WIDTHS(w)[(i)])
#define COLUMN_ALIGNMENTS(w)		\
  (((XmAxyListWidget)(w))->list.column_alignments)
#define COLUMN_ALIGNMENT(w,i)		\
  (((XmAxyListWidget)(w))->list.column_alignments[(i)])
#define COLUMN_LABEL_ALIGNMENTS(w) 	\
  (((XmAxyListWidget)(w))->list.column_label_alignments)
#define COLUMN_LABEL_ALIGNMENT(w,i) 	\
  (((XmAxyListWidget)(w))->list.column_label_alignments[(i)])
#define COLUMN_OFFSETS(w) 	\
  (((XmAxyListWidget)(w))->list.column_offsets)
#define COLUMN_OFFSET(w,i) 	\
  (((XmAxyListWidget)(w))->list.column_offsets[(i)])

#define SELECTION_POLICY(w)		\
  (((XmAxyListWidget)(w))->list.selection_policy)
#define SIZE_POLICY(w)			\
  (((XmAxyListWidget)(w))->list.size_policy)
#define SB_DISPLAY_POLICY(w)		\
  (((XmAxyListWidget)(w))->list.sb_display_policy)

#define HOR_SHIFT(w)			\
  (((XmAxyListWidget)(w))->list.hor_shift)
#define VERT_SHIFT(w)			\
  (((XmAxyListWidget)(w))->list.vert_shift)
#define HOR_MAX(w)			\
  (((XmAxyListWidget)(w))->list.hor_max)
#define VERT_MAX(w)			\
  (((XmAxyListWidget)(w))->list.vert_max)
#define HOR_PAGE(w)			\
  (((XmAxyListWidget)(w))->list.hor_page)
#define VERT_PAGE(w)			\
  (((XmAxyListWidget)(w))->list.vert_page)
#define HOR_SLIDER(w)			\
  (((XmAxyListWidget)(w))->list.hor_slider)
#define VERT_SLIDER(w)			\
  (((XmAxyListWidget)(w))->list.vert_slider)

#define RESIZING(w)			\
  (((XmAxyListWidget)(w))->list.resizing)
#define RESIZING_VSB(w)			\
  (((XmAxyListWidget)(w))->list.resizing_vsb)
#define RESIZING_HSB(w)			\
  (((XmAxyListWidget)(w))->list.resizing_hsb)
#define RESIZE_TIMER_ID(w)			\
  (((XmAxyListWidget)(w))->list.resize_timer_id)
#define RESIZE_TIMER_STARTED(w)			\
  (((XmAxyListWidget)(w))->list.resize_timer_started)
#define RESIZE_TIMER_SET(w)			\
  (((XmAxyListWidget)(w))->list.resize_timer_set)

#define SINGLE_CB_LIST(w)		\
  (((XmAxyListWidget)(w))->list.single_callback)
#define MULTIPLE_CB_LIST(w)		\
  (((XmAxyListWidget)(w))->list.multiple_callback)
#define EXTENDED_CB_LIST(w)		\
  (((XmAxyListWidget)(w))->list.extended_callback)
#define BROWSE_CB_LIST(w)		\
  (((XmAxyListWidget)(w))->list.browse_callback)
#define DEFAULT_CB_LIST(w)		\
  (((XmAxyListWidget)(w))->list.default_callback)
#define LABEL_CB_LIST(w)		\
  (((XmAxyListWidget)(w))->list.label_callback)
#define DRAW_CB_LIST(w)			\
  (((XmAxyListWidget)(w))->list.draw_cell_callback)

#define VERT_SB(w)			\
  (((XmAxyListWidget)(w))->list.vert_sb)
#define HOR_SB(w)			\
  (((XmAxyListWidget)(w))->list.hor_sb)
#define SCROLLED_WINDOW(w)		\
  (((XmAxyListWidget)(w))->list.scrolled_window)

#define BUFFER(w)			\
  (((XmAxyListWidget)(w))->list.buffer)
#define REDRAW_BUFFER(w)			\
  (((XmAxyListWidget)(w))->list.redraw_buffer)

#define HIGHLIGHTED(w)			\
  (((XmPrimitiveWidget)(w))->primitive.highlighted)
#define HIGHLIGHT_THICKNESS(w)		\
  (((XmPrimitiveWidget)(w))->primitive.highlight_thickness)
#define SHADOW_THICKNESS(w)		\
  (((XmPrimitiveWidget)(w))->primitive.shadow_thickness)
#define FOREGROUND(w)			\
  (((XmPrimitiveWidget)(w))->primitive.foreground)
#define TOP_SHADOW_GC(w)		\
  (((XmPrimitiveWidget)(w))->primitive.top_shadow_GC)
#define BOTTOM_SHADOW_GC(w)		\
  (((XmPrimitiveWidget)(w))->primitive.bottom_shadow_GC)
#define HIGHLIGHT_GC(w)		\
  (((XmPrimitiveWidget)(w))->primitive.highlight_GC)


#define BEING_DESTROYED(w)		\
  (((CoreWidget)(w))->core.being_destroyed)
#define X(w)				\
  (((CoreWidget)(w))->core.x)
#define Y(w)				\
  (((CoreWidget)(w))->core.y)
#define WIDTH(w)			\
  (((CoreWidget)(w))->core.width)
#define HEIGHT(w)			\
  (((CoreWidget)(w))->core.height)
#define BORDER(w)			\
  (((CoreWidget)(w))->core.border_width)
#define DEPTH(w)			\
  (((CoreWidget)(w))->core.depth)
#define COLORMAP(w)			\
  (((CoreWidget)(w))->core.colormap)
#define BACKGROUND(w)			\
  (((CoreWidget)(w))->core.background_pixel)

/* class methods */
static void ClassInitialize(void);
static void ClassPartInitialize(WidgetClass widget_class);
static void Initialize(Widget request, Widget new_w,
		       ArgList args, Cardinal *num_args);
static void Destroy(Widget w);
static void Resize(Widget w);
static void Redisplay(Widget w, XEvent *event, Region region);
static Boolean SetValues(Widget current, Widget request, Widget new_w,
			  ArgList args, Cardinal *num_args);
static XtGeometryResult QueryGeometry(Widget w,
				       XtWidgetGeometry *proposed,
				       XtWidgetGeometry *answer);

/* action routines */
static void ButtonDown(Widget, XEvent *, String *, Cardinal *);
static void ButtonUp(Widget, XEvent *, String *, Cardinal *);
static void ButtonMotion(Widget, XEvent *, String *, Cardinal *);
static void PointerMotion(Widget, XEvent *, String *, Cardinal *);
static void _XmAxyListBeginSelect(Widget, XEvent *, String *, Cardinal *);
static void _XmAxyListEndSelect(Widget, XEvent *, String *, Cardinal *);
static void _XmAxyListBeginToggle(Widget, XEvent *, String *, Cardinal *);
static void _XmAxyListEndToggle(Widget, XEvent *, String *, Cardinal *);
static void _XmAxyListBeginExtend(Widget, XEvent *, String *, Cardinal *);
static void _XmAxyListEndExtend(Widget, XEvent *, String *, Cardinal *);
static void _XmAxyListButtonMotion(Widget, XEvent *, String *, Cardinal *);

/* local functions */
static Dimension preferredWidth(Widget);
static Dimension preferredHeight(Widget);
static void copyWidths(Widget);
static void copyColumnAlignments(Widget);
static void copyColumnLabelAlignments(Widget);
static void copyColumnLabels(Widget);
static void createBackgroundGC(Widget);
static void createLabelBackgroundGC(Widget);
static void createLabelTopShadowGC(Widget);
static void createLabelBottomShadowGC(Widget);
static void createLabelDrawGC(Widget);
static void createCellBackgroundGC(Widget);
static void createDrawGC(Widget);
static void createSelectedBackgroundGC(Widget);
static void createSelectedDrawGC(Widget);
static void createGCs(Widget);
static void destroy_parent_cb(Widget,XtPointer,XtPointer);
static void hor_sb_cb(Widget,XtPointer,XtPointer);
static void vert_sb_cb(Widget,XtPointer,XtPointer);
static void redrawList(Widget,Drawable,XRectangle*);
static void drawLabels(Widget,Drawable,XRectangle*);
static void drawLabel(Widget,Drawable,XRectangle*,int);
static void drawCells(Widget,Drawable,XRectangle*);
static void drawCell(Widget,Drawable,XRectangle*,int,int);
static void drawResizeLine(Widget,Position);
static void installFont(Widget);
static void installLabelFont(Widget);
static void handleVertSB(Widget,Boolean*);
static void handleHorSB(Widget,Boolean*);
static void resizeLater(XtPointer,XtIntervalId*);
static void scrollTimer(XtPointer,XtIntervalId*);
static int figureColToResize(Widget,Position,Position);
static void doColResize(Widget);
static void select_row(Widget,int);
static void deselect_all(Widget);
static void deselect_row(Widget,int);
static int XYToRow(Widget,int,int);
static int XYToCol(Widget,int,int);
static void drawSelection(Widget,Drawable,XRectangle*,int,int);
static void change_select_range(Widget,int);
static void deselect_current_range(Widget);


static char defaultTranslations[]="\
<Btn1Motion>:          	XmAxyListColumnResizeButtonMotion()\
			XmAxyListButtonMotion()\n\
s ~m ~a <Btn1Down>:	XmAxyListColumnResizeButtonDown()\
                        XmAxyListBeginExtend()\n\
s ~m ~a <Btn1Up>:	XmAxyListColumnResizeButtonUp()\
                        XmAxyListEndExtend()\n\
c ~s ~m ~a <Btn1Down>:  XmAxyListColumnResizeButtonDown()\
                        XmAxyListBeginToggle()\n\
c ~s ~m ~a <Btn1Up>:    XmAxyListColumnResizeButtonUp()\
                        XmAxyListEndToggle()\n\
~c ~s ~m ~a <Btn1Down>: XmAxyListColumnResizeButtonDown()\
                        XmAxyListBeginSelect()\n\
~c ~s ~m ~a <Btn1Up>:   XmAxyListColumnResizeButtonUp()\
                        XmAxyListEndSelect()\n\
<Btn1Down>:		XmAxyListColumnResizeButtonDown()\n\
<Btn1Up>:		XmAxyListColumnResizeButtonUp()\n\
<MotionNotify>:        	XmAxyListColumnResizePointerMotion()\n\
";

static XtActionsRec actions[] = {
    {"XmAxyListColumnResizeButtonDown", ButtonDown},
    {"XmAxyListColumnResizeButtonUp", ButtonUp},
    {"XmAxyListColumnResizeButtonMotion", ButtonMotion},
    {"XmAxyListColumnResizePointerMotion", PointerMotion},
    {"XmAxyListBeginSelect", _XmAxyListBeginSelect},
    {"XmAxyListEndSelect", _XmAxyListEndSelect},
    {"XmAxyListBeginToggle", _XmAxyListBeginToggle},
    {"XmAxyListEndToggle", _XmAxyListEndToggle},
    {"XmAxyListBeginExtend", _XmAxyListBeginExtend},
    {"XmAxyListEndExtend", _XmAxyListEndExtend},
    {"XmAxyListButtonMotion", _XmAxyListButtonMotion},
};


#define Offset(field) XtOffsetOf(XmAxyListRec, list.field)
static XtResource resources[] = {

    {XmNdoubleClickInterval, XmCDoubleClickInterval, 
      XmRInt, sizeof(int), 
      Offset(click_interval),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED},

    {XmNlistMarginWidth, XmCListMarginWidth, 
      XmRHorizontalDimension, sizeof(Dimension), 
      Offset(margin_width),XmRImmediate, 
      (XtPointer)0},

    {XmNlistMarginHeight, XmCListMarginHeight, 
      XmRVerticalDimension, sizeof(Dimension), 
      Offset(margin_height),XmRImmediate, 
      (XtPointer)0},

    {XmNcellMarginWidth, XmCCellMarginWidth, 
      XmRHorizontalDimension, sizeof(Dimension), 
      Offset(cell_margin_width),XmRImmediate, 
      (XtPointer)2},

    {XmNcellMarginHeight, XmCCellMarginHeight, 
      XmRVerticalDimension, sizeof(Dimension), 
      Offset(cell_margin_height),XmRImmediate, 
      (XtPointer)2},

    {XmNlabelMarginWidth, XmCLabelMarginWidth, 
      XmRHorizontalDimension, sizeof(Dimension), 
      Offset(label_margin_width),XmRImmediate, 
      (XtPointer)2},

    {XmNlabelMarginHeight, XmCLabelMarginHeight, 
      XmRVerticalDimension, sizeof(Dimension), 
      Offset(label_margin_height),XmRImmediate, 
      (XtPointer)2},

    {XmNstringDirection, XmCStringDirection, 
      XmRStringDirection, sizeof(XmStringDirection), 
      Offset(string_dir),XmRImmediate, 
      (XtPointer)XmSTRING_DIRECTION_L_TO_R},

    {XmNfontList, XmCFontList, 
      XmRFontList, sizeof(XmFontList), 
      Offset(font),XmRString, 
      (XtPointer)NULL},

    {XmNlabelFontList, XmCFontList, 
      XmRFontList, sizeof(XmFontList), 
      Offset(label_font),XmRImmediate, 
      (XtPointer)NULL},

    {XmNboldLabels, XmCBoldLabels, 
      XmRBoolean, sizeof(Boolean), 
      Offset(bold_labels),XmRImmediate, 
      (XtPointer)FALSE},

    {XmNshowLabels, XmCShowLabels, 
      XmRBoolean, sizeof(Boolean), 
      Offset(show_labels),XmRImmediate, 
      (XtPointer)TRUE},

    {XmNactiveLabels, XmCActiveLabels, 
      XmRBoolean, sizeof(Boolean), 
      Offset(active_labels),XmRImmediate, 
      (XtPointer)FALSE},

    {XmNlabelShadowThickness, XmCShadowThickness, 
      XmRHorizontalDimension, sizeof(Dimension), 
      Offset(label_shadow_thickness),XmRImmediate, 
      (XtPointer)2},

    {XmNlabelBackground, XmCLabelBackground, 
      XmRPixel, sizeof(Pixel), 
      Offset(label_background),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXEL},

    {XmNlabelForeground, XmCLabelForeground, 
      XmRPixel, sizeof(Pixel), 
      Offset(label_foreground),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXEL},

    {XmNlabelBottomShadowColor, XmCLabelBottomShadowColor, 
      XmRPixel, sizeof(Pixel), 
      Offset(label_bottom_shadow_color),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXEL},

    {XmNlabelTopShadowColor, XmCLabelTopShadowColor, 
      XmRPixel, sizeof(Pixel), 
      Offset(label_top_shadow_color),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXEL},

    {XmNlabelBottomShadowPixmap, XmCLabelBottomShadowPixmap, 
      XmRPixmap, sizeof(Pixmap), 
      Offset(label_bottom_shadow_pixmap),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXMAP},

    {XmNlabelTopShadowPixmap, XmCLabelTopShadowPixmap, 
      XmRPixmap, sizeof(Pixmap), 
      Offset(label_top_shadow_pixmap),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXMAP},

    {XmNcellBackground, XmCCellBackground, 
      XmRPixel, sizeof(Pixel), 
      Offset(cell_background),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXEL},

    {XmNcellForeground, XmCCellForeground, 
      XmRPixel, sizeof(Pixel), 
      Offset(cell_foreground),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXEL},

    {XmNselectedBackground, XmCSelectedBackground, 
      XmRPixel, sizeof(Pixel), 
      Offset(selected_background),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXEL},

    {XmNselectedForeground, XmCSelectedForeground, 
      XmRPixel, sizeof(Pixel), 
      Offset(selected_foreground),XmRImmediate, 
      (XtPointer)XmUNSPECIFIED_PIXEL},

    {XmNrowCount, XmCRowCount,
      XmRInt, sizeof(int),
      Offset(row_count), XmRImmediate,
      (XtPointer)0},

    {XmNrowSpacing, XmCRowSpacing,
      XmRVerticalDimension, sizeof(Dimension),
      Offset(row_spacing), XmRImmediate,
      (XtPointer)0},

    {XmNcolumnCount, XmCColumnCount,
      XmRInt, sizeof(int),
      Offset(column_count), XmRImmediate,
      (XtPointer)0},

    {XmNcolumnLabels, XmCColumnLabels,
      XmRStringArray, sizeof(String *),
      Offset(column_labels), XmRImmediate,
      (XtPointer)NULL},

    {XmNcolumnWidths, XmCColumnWidths,
      XmRWidthArray, sizeof(short*),
      Offset(column_widths), XmRImmediate,
      (XtPointer)NULL},

    {XmNcolumnAlignments, XmCColumnAlignments,
      XmRAlignmentArray, sizeof(unsigned char*),
      Offset(column_alignments), XmRImmediate,
      (XtPointer)NULL},

    {XmNcolumnLabelAlignments, XmCColumnAlignments,
      XmRAlignmentArray, sizeof(unsigned char*),
      Offset(column_label_alignments), XmRImmediate,
      (XtPointer)NULL},

    {XmNscrollBarDisplayPolicy, XmCScrollBarDisplayPolicy, 
      XmRScrollBarDisplayPolicy, sizeof(unsigned char), 
      Offset(sb_display_policy),XmRImmediate, 
      (XtPointer)XmAS_NEEDED},

    {XmNselectionPolicy, XmCSelectionPolicy, 
      XmRSelectionPolicy, sizeof(unsigned char), 
      Offset(selection_policy),XmRImmediate, 
      (XtPointer)XmEXTENDED_SELECT},

    {XmNsingleSelectionCallback, XmCCallback, 
      XmRCallback, sizeof(XtCallbackList), 
      Offset(single_callback), XmRCallback, (XtPointer)NULL},

    {XmNmultipleSelectionCallback, XmCCallback, 
      XmRCallback, sizeof(XtCallbackList), 
      Offset(multiple_callback), XmRCallback, (XtPointer)NULL},

    {XmNextendedSelectionCallback, XmCCallback, 
      XmRCallback, sizeof(XtCallbackList), 
      Offset(extended_callback), XmRCallback, (XtPointer)NULL},

    {XmNbrowseSelectionCallback, XmCCallback, 
      XmRCallback, sizeof(XtCallbackList), 
      Offset(browse_callback), XmRCallback, 
      (XtPointer)NULL},

    {XmNdefaultActionCallback, XmCCallback, 
      XmRCallback, sizeof(XtCallbackList), 
      Offset(default_callback), XmRCallback, (XtPointer)NULL},

    {XmNlabelActivateCallback, XmCCallback, 
      XmRCallback, sizeof(XtCallbackList), 
      Offset(label_callback), XmRCallback, (XtPointer)NULL},

    {XmNdrawCellCallback, XmCCallback, 
      XmRCallback, sizeof(XtCallbackList), 
      Offset(draw_cell_callback), XmRCallback, (XtPointer)NULL},

    {XmNhorizontalScrollBar, XmCHorizontalScrollBar, 
      XmRWidget, sizeof(Widget), 
      Offset(hor_sb), XmRImmediate, 
      (XtPointer)NULL},

    {XmNverticalScrollBar, XmCVerticalScrollBar, 
      XmRWidget, sizeof(Widget), 
      Offset(vert_sb), XmRImmediate, 
      (XtPointer)NULL},

};

static XmSyntheticResource syn_resources[] = {

    {XmNrowSpacing, sizeof(Dimension), Offset(row_spacing),
      _XmFromVerticalPixels, _XmToVerticalPixels},
      
    {XmNlistMarginWidth, sizeof(Dimension), Offset(margin_width),
      _XmFromHorizontalPixels, _XmToHorizontalPixels},

    {XmNlistMarginHeight, sizeof(Dimension), Offset(margin_height),
      _XmFromVerticalPixels, _XmToVerticalPixels},
      
    {XmNcellMarginWidth, sizeof(Dimension), Offset(cell_margin_width),
      _XmFromHorizontalPixels, _XmToHorizontalPixels},

    {XmNcellMarginHeight, sizeof(Dimension), Offset(cell_margin_height),
      _XmFromVerticalPixels, _XmToVerticalPixels},

    {XmNlabelMarginWidth, sizeof(Dimension), Offset(label_margin_width),
      _XmFromHorizontalPixels, _XmToHorizontalPixels},

    {XmNlabelMarginHeight, sizeof(Dimension), Offset(label_margin_height),
      _XmFromVerticalPixels, _XmToVerticalPixels},

    {XmNlabelShadowThickness, sizeof(Dimension), Offset(label_shadow_thickness),
      _XmFromHorizontalPixels, _XmToHorizontalPixels},

};
#undef Offset


externaldef (xmaxylistclassrec) XmAxyListClassRec xmAxyListClassRec = {
    /* Core class part */
    {
      /* superclass */			(WidgetClass) &xmPrimitiveClassRec,
      /* class_name */ 			"XmAxyList",
      /* widget_size */ 		sizeof(XmAxyListRec),
      /* class_initialize */ 		ClassInitialize,
      /* class_part_initialize */ 	ClassPartInitialize,
      /* class_inited */ 		FALSE,
      /* initialize */ 			Initialize,
      /* initialize_hook */ 		NULL,
      /* realize */ 			XtInheritRealize,
      /* actions */ 			actions,
      /* num_actions */ 		XtNumber(actions),
      /* resources */ 			resources,
      /* num_resources */ 		XtNumber(resources),
      /* xrm_class */ 			NULLQUARK,
      /* compress_motion */ 		TRUE,
      /* compress_exposure */ 		XtExposeCompressMaximal,
      /* compress_enterleave */ 	TRUE,
      /* visible_interest */ 		FALSE,
      /* destroy */ 			Destroy,
      /* resize */ 			Resize,
      /* expose */ 			Redisplay,
      /* set_values */ 			SetValues,
      /* set_values_hook */ 		NULL,
      /* set_values_almost */ 		XtInheritSetValuesAlmost,
      /* get_values_hook */ 		NULL,
      /* accept_focus */ 		NULL,
      /* version */ 			XtVersion,
      /* callback_private */ 		NULL,
      /* tm_table */ 			defaultTranslations,
      /* query_geometry */ 		QueryGeometry,
      /* display_accelerator */ 	NULL,
      /* extension */ 			NULL,
    },
    /* Primitive Class part */
    {
      /* border_highlight */		NULL,/*XmInheritBorderHighlight,*/
      /* border_unhighlight */		NULL,/*XmInheritBorderUnhighlight,*/
      /* translations */		XtInheritTranslations,
      /* arm_and_activate */		NULL,
      /* syn_resources */		syn_resources,
      /* num_syn_resources */		XtNumber(syn_resources),
      /* extension */			NULL,
    },
    /* List Class part */
    {
      /* extension */ 			NULL,
    }
};

externaldef (xmaxylistwidgetclass) WidgetClass xmAxyListWidgetClass = 
                                  (WidgetClass)&xmAxyListClassRec;

static void ClassInitialize(void) {
}

static void ClassPartInitialize(WidgetClass widget_class) {
}

static void Initialize(Widget req_w, Widget new_w, 
                       ArgList args, Cardinal *num_args) {
  int i;

  RESIZE_CURSOR(new_w)=
    XCreateFontCursor(XtDisplay(new_w), XC_sb_h_double_arrow);
  COL_RESIZE_AREA(new_w)=FALSE;
  COL_RESIZE_UNDERWAY(new_w)=FALSE;
  COL_RESIZE_NUM(new_w)=0;
  COL_RESIZE_X(new_w)=0;


  if(FONT(new_w)==NULL){
    FONT(new_w)=XmFontListCopy(
	_XmGetDefaultFontList(new_w,XmLABEL_FONTLIST));
  } else {
    FONT(new_w)=XmFontListCopy(FONT(new_w));
  }
  installFont(new_w);
  if(LABEL_FONT(new_w)==NULL){
    LABEL_FONT(new_w)=XmFontListCopy(FONT(new_w));
  } else {
    LABEL_FONT(new_w)=XmFontListCopy(LABEL_FONT(new_w));
  }
  installLabelFont(new_w);
  /*
  xms=XmStringCreateSimple("W");
  XmStringExtent(FONT(new_w),xms,
      &FONT_WIDTH(new_w), &FONT_HEIGHT(new_w));
  XmStringFree(xms);
  */

  if(CELL_BACKGROUND(new_w)==XmUNSPECIFIED_PIXEL){
    /*CELL_BACKGROUND(new_w)=BACKGROUND(new_w);*/
    CELL_BACKGROUND(new_w)=WhitePixelOfScreen(XtScreen(new_w));
  }
  if(CELL_FOREGROUND(new_w)==XmUNSPECIFIED_PIXEL){
    Pixel top,bot,sel;
    XmGetColors(XtScreen(new_w),COLORMAP(new_w),CELL_BACKGROUND(new_w),
	&CELL_FOREGROUND(new_w),&top,&bot,&sel);
  }
  if(SELECTED_BACKGROUND(new_w)==XmUNSPECIFIED_PIXEL){
    SELECTED_BACKGROUND(new_w)=CELL_FOREGROUND(new_w);
  }
  if(SELECTED_FOREGROUND(new_w)==XmUNSPECIFIED_PIXEL){
    SELECTED_FOREGROUND(new_w)=CELL_BACKGROUND(new_w);
  }

  if(LABEL_BACKGROUND(new_w)==XmUNSPECIFIED_PIXEL){
    LABEL_BACKGROUND(new_w)=BACKGROUND(new_w);
  }
  if(LABEL_TOP_SHADOW_COLOR(new_w)==XmUNSPECIFIED_PIXEL ||
     LABEL_BOTTOM_SHADOW_COLOR(new_w)==XmUNSPECIFIED_PIXEL ||
     LABEL_FOREGROUND(new_w)==XmUNSPECIFIED_PIXEL){
    Pixel fg,top,bot,sel;
    XmGetColors(XtScreen(new_w),COLORMAP(new_w),LABEL_BACKGROUND(new_w),
	&fg,&top,&bot,&sel);
    if(LABEL_FOREGROUND(new_w)==XmUNSPECIFIED_PIXEL){
      LABEL_FOREGROUND(new_w)=fg;
    }
    if(LABEL_TOP_SHADOW_COLOR(new_w)==XmUNSPECIFIED_PIXEL){
      LABEL_TOP_SHADOW_COLOR(new_w)=top;
    }
    if(LABEL_BOTTOM_SHADOW_COLOR(new_w)==XmUNSPECIFIED_PIXEL){
      LABEL_BOTTOM_SHADOW_COLOR(new_w)=bot;
    }
  }

  if(ROW_COUNT(new_w)<0){
    ROW_COUNT(new_w)=0;
  }
  if(COLUMN_COUNT(new_w)<0){
    COLUMN_COUNT(new_w)=0;
  }

  if(COLUMN_WIDTHS(new_w)==NULL && COLUMN_COUNT(new_w)>0){
   int space=sizeof(short)*COLUMN_COUNT(new_w);
    COLUMN_WIDTHS(new_w)=
      (short*)XtMalloc(space);
    for(i=0;i<COLUMN_COUNT(new_w);i++){
      COLUMN_WIDTHS(new_w)[i]=
	(COLUMN_LABELS(new_w)!=NULL)?(strlen(COLUMN_LABELS(new_w)[i])):3;
    }
  } else {
    copyWidths(new_w);
  }

  if(COLUMN_LABELS(new_w)!=NULL){
    copyColumnLabels(new_w);
  }

  if(COLUMN_ALIGNMENTS(new_w)!=NULL){
    copyColumnAlignments(new_w);
  } else {
    COLUMN_ALIGNMENTS(new_w)=
      (unsigned char*)XtMalloc(sizeof(unsigned char)*COLUMN_COUNT(new_w));
    for(i=0;i<COLUMN_COUNT(new_w);i++) 
      COLUMN_ALIGNMENT(new_w,i)=XmALIGNMENT_BEGINNING;
  }

  if(COLUMN_LABEL_ALIGNMENTS(new_w)!=NULL){
    copyColumnLabelAlignments(new_w);
  } else {
    COLUMN_LABEL_ALIGNMENTS(new_w)=
      (unsigned char*)XtMalloc(sizeof(unsigned char)*COLUMN_COUNT(new_w));
    for(i=0;i<COLUMN_COUNT(new_w);i++) 
      COLUMN_LABEL_ALIGNMENT(new_w,i)=XmALIGNMENT_CENTER;
  }

  BORDER(new_w)=0;

  HOR_SHIFT(new_w)=0;
  VERT_SHIFT(new_w)=0;
  VERT_SLIDER(new_w)=6;
  VERT_MAX(new_w)=VERT_SLIDER(new_w);

  if(CLICK_INTERVAL(new_w)==XmUNSPECIFIED){
    CLICK_INTERVAL(new_w)=XtGetMultiClickTime(XtDisplay(new_w));
  }
  CLICK_COUNT(new_w)=0;
  CLICK_TIME(new_w)=0;

  COLUMN_OFFSETS(new_w)=(int*)XtMalloc(sizeof(int)*(COLUMN_COUNT(new_w)+1));
  COLUMN_OFFSET(new_w,0)=SHADOW_THICKNESS(new_w)+MARGIN_WIDTH(new_w);
  for(i=0;i<COLUMN_COUNT(new_w);i++){
    COLUMN_OFFSET(new_w,i+1)=COLUMN_OFFSET(new_w,i)+
      LABEL_FONT_WIDTH(new_w)*COLUMN_WIDTHS(new_w)[i]+
      LABEL_MARGIN_WIDTH(new_w)*2+LABEL_SHADOW_THICKNESS(new_w)*2+
      +EHT(new_w)*2+(Dimension)(BOLD_LABELS(new_w)?1:0);
  }

  ROW_IS_SELECTED_ARRAY(new_w)=
    (Boolean*)XtMalloc(sizeof(Boolean)*ROW_COUNT(new_w));
  SELECTED_ROWS(new_w)=(int*)XtMalloc(sizeof(int)*ROW_COUNT(new_w));
  for(i=0;i<ROW_COUNT(new_w);i++){
    ROW_IS_SELECTED(new_w,i)=FALSE;
    SELECTED_ROW(new_w,i)=-1;
  }
  SELECTED_ROW_COUNT(new_w)=0;
  ANCHOR(new_w)=-1;
  LAST_SELECTED_ROW(new_w)=-1;
  SCROLL_TIMER_SET(new_w)=FALSE;
  SELECTION_DRAG(new_w)=FALSE;

  if(WIDTH(req_w)==0){
    WIDTH(new_w)=preferredWidth(new_w);
  }
  if(HEIGHT(req_w)==0){
    HEIGHT(new_w)=preferredHeight(new_w);
  }
  VISIBLE_ROW_COUNT(new_w)=(HEIGHT(new_w)-
      (SHOW_LABELS(new_w)?LABEL_HEIGHT(new_w):0)-
      SHADOW_THICKNESS(new_w)*2-MARGIN_HEIGHT(new_w)*2)/(ROW_HEIGHT(new_w)+
      ROW_SPACING(new_w)+EHT(new_w));
  if(VISIBLE_ROW_COUNT(new_w)<0) VISIBLE_ROW_COUNT(new_w)=0;

  if(XmIsScrolledWindow(XtParent(new_w))){
    SCROLLED_WINDOW(new_w)=(XmScrolledWindowWidget)XtParent(new_w);

    VERT_SB(new_w)=(XmScrollBarWidget)XtVaCreateWidget("VertScrollBar", 
	xmScrollBarWidgetClass, (Widget)SCROLLED_WINDOW(new_w), 
	XmNincrement,ROW_HEIGHT(new_w)+ROW_SPACING(new_w)+EHT(new_w),NULL);

    XtSetMappedWhenManaged((Widget)VERT_SB(new_w),TRUE);

    XtAddCallback((Widget)VERT_SB(new_w), XmNdecrementCallback,
		  vert_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)VERT_SB(new_w), XmNdragCallback,
		  vert_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)VERT_SB(new_w), XmNincrementCallback,
		  vert_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)VERT_SB(new_w), XmNpageDecrementCallback,
		  vert_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)VERT_SB(new_w), XmNpageIncrementCallback,
		  vert_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)VERT_SB(new_w), XmNtoBottomCallback,
		  vert_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)VERT_SB(new_w), XmNtoTopCallback,
		  vert_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)VERT_SB(new_w), XmNvalueChangedCallback,
		  vert_sb_cb, (XtPointer)new_w);

    HOR_SB(new_w)=(XmScrollBarWidget)XtVaCreateWidget("HorScrollBar", 
	xmScrollBarWidgetClass, (Widget)SCROLLED_WINDOW(new_w), 
	XmNincrement,ROW_HEIGHT(new_w)+ROW_SPACING(new_w)+EHT(new_w),
	XmNorientation, XmHORIZONTAL, NULL);

    XtSetMappedWhenManaged((Widget)HOR_SB(new_w),TRUE);

    XtAddCallback((Widget)HOR_SB(new_w), XmNdecrementCallback,
		  hor_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)HOR_SB(new_w), XmNdragCallback,
		  hor_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)HOR_SB(new_w), XmNincrementCallback,
		  hor_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)HOR_SB(new_w), XmNpageDecrementCallback,
		  hor_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)HOR_SB(new_w), XmNpageIncrementCallback,
		  hor_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)HOR_SB(new_w), XmNtoBottomCallback,
		  hor_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)HOR_SB(new_w), XmNtoTopCallback,
		  hor_sb_cb, (XtPointer)new_w);
    XtAddCallback((Widget)HOR_SB(new_w), XmNvalueChangedCallback,
		  hor_sb_cb, (XtPointer)new_w);

    XmScrolledWindowSetAreas((Widget)SCROLLED_WINDOW(new_w),
	(Widget)HOR_SB(new_w),(Widget)VERT_SB(new_w),(Widget)new_w);

    /*
    XtAddEventHandler((Widget)SCROLLED_WINDOW(new_w),ExposureMask,FALSE,
	resizeHelper, (XtPointer)new_w);
	*/
  }

  RESIZING(new_w)=0;
  RESIZING_VSB(new_w)=FALSE;
  RESIZING_HSB(new_w)=FALSE;
  RESIZE_TIMER_STARTED(new_w)=FALSE;
  RESIZE_TIMER_SET(new_w)=FALSE;
  createGCs(new_w);
  BUFFER(new_w)=0;
}


static void Destroy(Widget w) {
  int i;
  
  XmFontListFree(FONT(w));
  XmFontListFree(LABEL_FONT(w));

  XFreeGC(XtDisplay(w),BACKGROUND_GC(w));
  XFreeGC(XtDisplay(w),LABEL_BACKGROUND_GC(w));
  XFreeGC(XtDisplay(w),LABEL_TOP_SHADOW_GC(w));
  XFreeGC(XtDisplay(w),LABEL_BOTTOM_SHADOW_GC(w));
  XFreeGC(XtDisplay(w),LABEL_DRAW_GC(w));
  XFreeGC(XtDisplay(w),CELL_BACKGROUND_GC(w));
  XFreeGC(XtDisplay(w),DRAW_GC(w));
  XFreeGC(XtDisplay(w),SELECTED_BACKGROUND_GC(w));
  XFreeGC(XtDisplay(w),SELECTED_DRAW_GC(w));
  
  XtFree((char*)COLUMN_WIDTHS(w));
  XtFree((char*)COLUMN_OFFSETS(w));
  XtFree((char*)COLUMN_ALIGNMENTS(w));
  XtFree((char*)COLUMN_LABEL_ALIGNMENTS(w));
  XtFree((char*)SELECTED_ROWS(w));
  XtFree((char*)ROW_IS_SELECTED_ARRAY(w));

  for(i=0;i<COLUMN_COUNT(w);i++){
    XtFree((char*)COLUMN_LABELS(w)[i]);
  }
  XtFree((char*)COLUMN_LABELS(w));

  if(RESIZE_TIMER_SET(w)){
    XtRemoveTimeOut(RESIZE_TIMER_ID(w));
  }

  if(BUFFER(w)){
    XFreePixmap(XtDisplay(w),BUFFER(w));
  }

  if(SCROLLED_WINDOW(w) && (!BEING_DESTROYED(SCROLLED_WINDOW(w)))){
    XtDestroyWidget((Widget)SCROLLED_WINDOW(w));
  }
}

static void Resize(Widget w) {
  Boolean hor_mc,vert_mc;

  hor_mc=FALSE;
  vert_mc=FALSE;

  /*printf("In Resize() with %dx%d\n",WIDTH(w),HEIGHT(w));*/
  /*
  if(RESIZING(w)==0 && 
      (XtIsManaged((Widget)HOR_SB(w)) || XtIsManaged((Widget)VERT_SB(w)))){
    XtUnmanageChild((Widget)HOR_SB(w));
    XtUnmanageChild((Widget)VERT_SB(w));
    SCROLLED_WINDOW(w)->core.widget_class->core_class.resize(
	(Widget)SCROLLED_WINDOW(w));
  }
  */
  RESIZING(w)++;
  /*printf("RECURSIVITY=%d\n",RESIZING(w));*/


  handleHorSB(w,&hor_mc);
  handleVertSB(w,&vert_mc);

  /*printf("In Resize: hor_mc=%d vert_mc=%d\n",hor_mc,vert_mc);*/

  if(!RESIZE_TIMER_SET(w) || ((hor_mc || vert_mc) && 
	RESIZE_TIMER_STARTED(w))){
    RESIZE_TIMER_SET(w)=TRUE;
    /*printf("Setting Time Out\n");*/
    RESIZE_TIMER_ID(w)=XtAppAddTimeOut(XtWidgetToApplicationContext(w),0,resizeLater,
	(XtPointer)w);
  }

  if(BUFFER(w)){
    XFreePixmap(XtDisplay(w),BUFFER(w));
    BUFFER(w)=0;
  }
  RESIZING(w)--;
}

static void Redisplay(Widget w, XEvent *event, Region region) {
  XRectangle cr;

  if(!XtIsRealized(w)) return;

  if(!BUFFER(w)){
    BUFFER(w)=XCreatePixmap(XtDisplay(w),RootWindowOfScreen(XtScreen(w)),
	WIDTH(w),HEIGHT(w),DEPTH(w));
    REDRAW_BUFFER(w)=TRUE;
  }

  if(REDRAW_BUFFER(w)) {
    redrawList(w,BUFFER(w),NULL);
    REDRAW_BUFFER(w)=FALSE;
  }

  if(region) XClipBox(region,&cr);
  else {cr.x=0;cr.y=0;cr.width=WIDTH(w);cr.height=HEIGHT(w);}

  XCopyArea(XtDisplay(w),BUFFER(w),XtWindow(w),BACKGROUND_GC(w),cr.x,cr.y,
      cr.width,cr.height,cr.x,cr.y);

}

#define NE(field) \
(((XmAxyListWidget)wcur)->field != ((XmAxyListWidget)wnew)->field)
#define NB(field) \
((((XmAxyListWidget)wcur)->field && !((XmAxyListWidget)wnew)->field) ||\
(!((XmAxyListWidget)wcur)->field && ((XmAxyListWidget)wnew)->field))
static Boolean SetValues(Widget wcur, Widget wreq, Widget wnew,
	                  ArgList args, Cardinal *num_args) {
  Boolean redraw=FALSE;
  Boolean layout=FALSE;

  if(NE(list.font)){
    FONT(wnew)=XmFontListCopy(FONT(wnew));
    installFont(wnew);
    layout=TRUE;
    redraw=TRUE;
  }
  if(NE(list.label_font)){
    LABEL_FONT(wnew)=XmFontListCopy(LABEL_FONT(wnew));
    installLabelFont(wnew);
    layout=TRUE;
    redraw=TRUE;
  }

  if(NE(primitive.highlight_GC) || NE(primitive.top_shadow_GC) ||
     NE(primitive.bottom_shadow_GC)){
    redraw=TRUE;
  }
  if(NE(core.background_pixel)){
    XFreeGC(XtDisplay(wnew),BACKGROUND_GC(wnew));
    createBackgroundGC(wnew);
    redraw=TRUE;
  }
  if(NE(list.label_background)){
    XFreeGC(XtDisplay(wnew),LABEL_BACKGROUND_GC(wnew));
    createLabelBackgroundGC(wnew);
    redraw=TRUE;
  }
  if(NE(list.label_background) || NE(list.label_top_shadow_color) ||
     NE(list.label_top_shadow_pixmap)){
    XFreeGC(XtDisplay(wnew),LABEL_TOP_SHADOW_GC(wnew));
    createLabelTopShadowGC(wnew);
    redraw=TRUE;
  }
  if(NE(list.label_background) || NE(list.label_bottom_shadow_color) ||
     NE(list.label_bottom_shadow_pixmap)){
    XFreeGC(XtDisplay(wnew),LABEL_BOTTOM_SHADOW_GC(wnew));
    createLabelBottomShadowGC(wnew);
    redraw=TRUE;
  }
  if(NE(list.label_background) || NE(list.label_foreground) ||
     NE(list.label_font_fid)){
    XFreeGC(XtDisplay(wnew),LABEL_DRAW_GC(wnew));
    createLabelDrawGC(wnew);
    redraw=TRUE;
  }
  if(NE(list.cell_background)){
    XFreeGC(XtDisplay(wnew),CELL_BACKGROUND_GC(wnew));
    createCellBackgroundGC(wnew);
    redraw=TRUE;
  }
  if(NE(list.cell_background) || NE(list.cell_foreground) ||
     NE(list.font_fid)){
    XFreeGC(XtDisplay(wnew),DRAW_GC(wnew));
    createDrawGC(wnew);
    redraw=TRUE;
  }
  if(NE(list.selected_background)){
    XFreeGC(XtDisplay(wnew),SELECTED_BACKGROUND_GC(wnew));
    createSelectedBackgroundGC(wnew);
    redraw=TRUE;
  }
  if(NE(list.selected_background) || NE(list.selected_foreground) ||
     NE(list.font_fid)){
    XFreeGC(XtDisplay(wnew),SELECTED_DRAW_GC(wnew));
    createSelectedDrawGC(wnew);
    redraw=TRUE;
  }

  if(NE(list.margin_width) || NE(list.margin_height) || 
     NE(list.cell_margin_width) || NE(list.cell_margin_height) ||
     NE(list.label_margin_width) || NE(list.label_margin_height) ||
     NE(list.label_shadow_thickness) || NE(list.row_spacing) ||
     NE(primitive.highlight_thickness) || NE(primitive.shadow_thickness)){
    layout=TRUE;
    redraw=TRUE;
  }
  if(NE(list.string_dir)){
    redraw=TRUE;
  }
  if(NB(list.bold_labels) || NB(list.show_labels)){
    layout=TRUE;
    redraw=TRUE;
  }
  if(NE(list.row_count)){
    int i;

    XtFree((char*)SELECTED_ROWS(wnew));
    XtFree((char*)ROW_IS_SELECTED_ARRAY(wnew));

    ROW_IS_SELECTED_ARRAY(wnew)=
      (Boolean*)XtMalloc(sizeof(Boolean)*ROW_COUNT(wnew));
    SELECTED_ROWS(wnew)=(int*)XtMalloc(sizeof(int)*ROW_COUNT(wnew));

    for(i=0;i<ROW_COUNT(wnew);i++){
      ROW_IS_SELECTED(wnew,i)=FALSE;
      SELECTED_ROW(wnew,i)=-1;
    }
    
    SELECTED_ROW_COUNT(wnew)=0;
    ANCHOR(wnew)=-1;
    LAST_SELECTED_ROW(wnew)=-1;
    SELECTION_DRAG(wnew)=FALSE;

    layout=TRUE;
    redraw=TRUE;
  }
  if(NE(list.column_widths)){
    copyWidths(wnew);
    layout=TRUE;
    redraw=TRUE;
  }
  if(NE(list.column_labels)){
    copyColumnLabels(wnew);
    redraw=TRUE;
  }
  if(NE(list.column_alignments)){
    copyColumnAlignments(wnew);
    redraw=TRUE;
  }
  if(NE(list.column_label_alignments)){
    copyColumnLabelAlignments(wnew);
    redraw=TRUE;
  }
  if(NE(list.column_count)){
    XtFree((char*)COLUMN_OFFSETS(wnew));
    COLUMN_OFFSETS(wnew)=(int*)XtMalloc(sizeof(int)*(COLUMN_COUNT(wnew)+1));
    layout=TRUE;
    redraw=TRUE;
  }

  BORDER(wnew)=0;

  if(layout){
    int i;
    int width=WIDTH(wnew),height=HEIGHT(wnew);
    if(HEIGHT(wcur)==HEIGHT(wnew)) {
      height=preferredHeight(wnew);
    }
    if(WIDTH(wcur)==WIDTH(wnew)) width=preferredWidth(wnew);
    HOR_SHIFT(wnew)=0;
    VERT_SHIFT(wnew)=0;
    VISIBLE_ROW_COUNT(wnew)=(HEIGHT(wnew)-
      (SHOW_LABELS(wnew)?LABEL_HEIGHT(wnew):0)-
      SHADOW_THICKNESS(wnew)*2-MARGIN_HEIGHT(wnew)*2)/(ROW_HEIGHT(wnew)+
      ROW_SPACING(wnew)+EHT(wnew));
    if(VISIBLE_ROW_COUNT(wnew)<0) VISIBLE_ROW_COUNT(wnew)=0;
    COLUMN_OFFSET(wnew,0)=SHADOW_THICKNESS(wnew)+MARGIN_WIDTH(wnew);
    for(i=0;i<COLUMN_COUNT(wnew);i++){
      COLUMN_OFFSET(wnew,i+1)=COLUMN_OFFSET(wnew,i)+
	LABEL_FONT_WIDTH(wnew)*COLUMN_WIDTHS(wnew)[i]+
	LABEL_MARGIN_WIDTH(wnew)*2+LABEL_SHADOW_THICKNESS(wnew)*2+
	+EHT(wnew)*2+(Dimension)(BOLD_LABELS(wnew)?1:0);
    }
    if(VERT_SB(wnew))
      XtVaSetValues((Widget)VERT_SB(wnew),
	XmNincrement,ROW_HEIGHT(wnew)+ROW_SPACING(wnew)+EHT(wnew),NULL);
    if(HOR_SB(wnew))
      XtVaSetValues((Widget)HOR_SB(wnew),
	XmNincrement,ROW_HEIGHT(wnew)+ROW_SPACING(wnew)+EHT(wnew),NULL);

    WIDTH(wnew)=WIDTH(wcur);
    HEIGHT(wnew)=HEIGHT(wcur);
    Resize(wnew);
    WIDTH(wnew)=width;
    HEIGHT(wnew)=height;
    redraw=TRUE;
  }

  if(redraw){
    REDRAW_BUFFER(wnew)=TRUE;
  }
  
  return redraw;
}
#undef NE
#undef NB

static XtGeometryResult QueryGeometry(Widget w, XtWidgetGeometry *proposed, 
                                       XtWidgetGeometry *answer) {
  Dimension ret_width,pref_width;

  answer->request_mode = (CWWidth | CWHeight);

  ret_width=WIDTH(w);
  pref_width=preferredWidth(w);

  if(SIZE_POLICY(w) == XmRESIZE_IF_POSSIBLE ||
     (SIZE_POLICY(w) == XmVARIABLE && pref_width>ret_width)){
    ret_width=pref_width;
  }

  answer->height=preferredHeight(w);
  answer->width=ret_width;

  if(((proposed->request_mode & (CWWidth | CWHeight)) == (CWWidth | CWHeight))
    && proposed->width==answer->width && proposed->height==answer->height)
    return XtGeometryYes;
  else if(answer->width == WIDTH(w) &&
          answer->height == HEIGHT(w))
    return XtGeometryNo;
  else
    return XtGeometryAlmost;
}

static int figureColToResize(Widget w,Position x,Position y){
  Dimension label_height;
  int i,col_x;

  label_height=SHOW_LABELS(w)?LABEL_HEIGHT(w):RESIZE_STRIP_WITH_HIDDEN_LABELS;

  if(y<SHADOW_THICKNESS(w)+MARGIN_HEIGHT(w)) return -1;
  if(y>SHADOW_THICKNESS(w)+MARGIN_HEIGHT(w)+label_height) return -1;

  if(x<SHADOW_THICKNESS(w)+MARGIN_WIDTH(w)) return -1;
  if(x>WIDTH(w)-MARGIN_WIDTH(w)-SHADOW_THICKNESS(w)) return -1;

  for(i=0;i<COLUMN_COUNT(w);i++){
    col_x=COLUMN_OFFSET(w,i+1)-HOR_SHIFT(w);
    if(Abs(x-col_x)<=RESIZE_MARGIN) return i;
  }
  return -1;
}

static int XYToRow(Widget w,int x,int y){
  int row;

  if(y<(int)ROW_OFFSET(w,0)-(int)EHT(w) || 
   y>=(int)HEIGHT(w)-(int)SHADOW_THICKNESS(w)-(int)MARGIN_HEIGHT(w))return -1;
  if(x<(int)SHADOW_THICKNESS(w)+(int)MARGIN_WIDTH(w) || 
     x>(int)WIDTH(w)-(int)SHADOW_THICKNESS(w)-(int)MARGIN_WIDTH(w) ||
     x>(int)COLUMN_OFFSET(w,COLUMN_COUNT(w))-(int)HOR_SHIFT(w)) return -1;
  y-=(ROW_OFFSET(w,-1)+ROW_HEIGHT(w)+EHT(w));
  y+=VERT_SHIFT(w);

  row=y/(ROW_HEIGHT(w)+ROW_SPACING(w)+EHT(w));
  if(row<0) row=0;

  if(row>=ROW_COUNT(w)) return -1;
  return row;
}

static int XYToCol(Widget w,int x,int y){
  int col;

  if(y<(int)ROW_OFFSET(w,0)-(int)EHT(w) || 
   y>=(int)HEIGHT(w)-(int)SHADOW_THICKNESS(w)-(int)MARGIN_HEIGHT(w))return -1;
  if(x<(int)SHADOW_THICKNESS(w)+(int)MARGIN_WIDTH(w) || 
     x>(int)WIDTH(w)-(int)SHADOW_THICKNESS(w)-(int)MARGIN_WIDTH(w) ||
     x>(int)COLUMN_OFFSET(w,COLUMN_COUNT(w))-(int)HOR_SHIFT(w)) return -1;

  x+=HOR_SHIFT(w);

  for(col=0;col<COLUMN_COUNT(w);col++){
    if(x<COLUMN_OFFSET(w,col+1)) return col;
  }

  return -1;
}

static void _XmAxyListBeginSelect(Widget w,XEvent *event,
                                  String *parmas, Cardinal *num_params){
  XButtonEvent *be=(XButtonEvent*)event;
  int row;

  if((SHOW_LABELS(w) && COLUMN_COUNT(w)>0) || 
     (ROW_COUNT(w)>0 && COLUMN_COUNT(w)>0)){
    XmProcessTraversal(w,XmTRAVERSE_CURRENT);
  }
  if(ROW_COUNT(w)==0 || COLUMN_COUNT(w)==0) return;

  row=XYToRow(w,be->x,be->y);

  if(row<0) return;

  switch(SELECTION_POLICY(w)){
    case XmEXTENDED_SELECT:
      deselect_all(w);
      select_row(w,row);
      ANCHOR(w)=row;
      break;

    default:
      return;
  }

  /*printf("Anchor=%d Last=%d\n",ANCHOR(w),LAST_SELECTED_ROW(w));*/
  Redisplay(w,NULL,NULL);

  return;
}

static void _XmAxyListEndSelect(Widget w,XEvent *event,
                                  String *parmas, Cardinal *num_params){
  XButtonEvent *be=(XButtonEvent*)event;
  int row,col;

  SELECTION_DRAG(w)=FALSE;

  if(SCROLL_TIMER_SET(w)){
    XtRemoveTimeOut(SCROLL_TIMER_ID(w));
    SCROLL_TIMER_SET(w)=FALSE;
  }

  if((row=XYToRow(w,be->x,be->y))<0) return;
  if((col=XYToCol(w,be->x,be->y))<0) return;

  if(CLICK_COUNT(w)>0 && be->time<CLICK_TIME(w)+CLICK_INTERVAL(w)){
    XmAxyListDefaultActionCallbackStruct cbs;

    cbs.reason=XmAxyListDefaultActionReason;
    cbs.event=event;
    cbs.row=row;
    cbs.column=col;

    XtCallCallbackList(w,DEFAULT_CB_LIST(w),&cbs);

    CLICK_COUNT(w)=0;
  } else {
    CLICK_TIME(w)=be->time;
    CLICK_COUNT(w)=1;
  }

}

static void _XmAxyListBeginToggle(Widget w,XEvent *event,
                                  String *parmas, Cardinal *num_params){
  XButtonEvent *be=(XButtonEvent*)event;
  int row;

  if((SHOW_LABELS(w) && COLUMN_COUNT(w)>0) || 
     (ROW_COUNT(w)>0 && COLUMN_COUNT(w)>0)){
    XmProcessTraversal(w,XmTRAVERSE_CURRENT);
  }
  if(ROW_COUNT(w)==0 || COLUMN_COUNT(w)==0) return;

  if(SELECTION_POLICY(w)==XmEXTENDED_SELECT){
    row=XYToRow(w,be->x,be->y);
    if(row<0) return;
    if(ROW_IS_SELECTED(w,row)){
      deselect_row(w,row);
    } else {
      select_row(w,row);
    }
    ANCHOR(w)=row;
    /*printf("Anchor=%d Last=%d\n",ANCHOR(w),LAST_SELECTED_ROW(w));*/
    Redisplay(w,NULL,NULL);
  }
  return;
}

static void _XmAxyListEndToggle(Widget w,XEvent *event, 
                                  String *parmas, Cardinal *num_params){
  SELECTION_DRAG(w)=FALSE;
}

static void _XmAxyListBeginExtend(Widget w,XEvent *event,
                                  String *parmas, Cardinal *num_params){
  XButtonEvent *be=(XButtonEvent*)event;
  int row;

  if((SHOW_LABELS(w) && COLUMN_COUNT(w)>0) || 
     (ROW_COUNT(w)>0 && COLUMN_COUNT(w)>0)){
    XmProcessTraversal(w,XmTRAVERSE_CURRENT);
  }
  if(ROW_COUNT(w)==0 || COLUMN_COUNT(w)==0) return;

  if(SELECTION_POLICY(w)==XmEXTENDED_SELECT){
    row=XYToRow(w,be->x,be->y);
    if(row<0) return;
    if(ANCHOR(w)>=0) {
      change_select_range(w,row);
      /*printf("Anchor=%d Last=%d\n",ANCHOR(w),LAST_SELECTED_ROW(w));*/
      Redisplay(w,NULL,NULL);
    }
  }
  return;
}

static void _XmAxyListEndExtend(Widget w,XEvent *event,
                                  String *parmas, Cardinal *num_params){
  SELECTION_DRAG(w)=FALSE;
}

#define SCROLL_DELAY 100
static void _XmAxyListButtonMotion(Widget w,XEvent *event,
                                  String *parmas, Cardinal *num_params){
  XButtonEvent *be=(XButtonEvent*)event;
  int row;

  if((SHOW_LABELS(w) && COLUMN_COUNT(w)>0) || 
     (ROW_COUNT(w)>0 && COLUMN_COUNT(w)>0)){
    XmProcessTraversal(w,XmTRAVERSE_CURRENT);
  }
  if(ROW_COUNT(w)==0 || COLUMN_COUNT(w)==0) return;

  if(SELECTION_POLICY(w)==XmEXTENDED_SELECT){
    if(be->y>=(int)HEIGHT(w)-(int)SHADOW_THICKNESS(w)-(int)MARGIN_HEIGHT(w)){
      if(SELECTION_DRAG(w) && !SCROLL_TIMER_SET(w)){
	SCROLL_DIRECTION(w)=1;
	SCROLL_TIMER_SET(w)=TRUE;
	SCROLL_TIMER_ID(w)=XtAppAddTimeOut(XtWidgetToApplicationContext(w),
	    SCROLL_DELAY,scrollTimer,(XtPointer)w);
      }
    } else if(be->y<(int)ROW_OFFSET(w,0)-(int)EHT(w)){
      if(SELECTION_DRAG(w) && !SCROLL_TIMER_SET(w)){
	SCROLL_DIRECTION(w)=-1;
	SCROLL_TIMER_SET(w)=TRUE;
	SCROLL_TIMER_ID(w)=XtAppAddTimeOut(XtWidgetToApplicationContext(w),
	    SCROLL_DELAY,scrollTimer,(XtPointer)w);
      }
    } else {
      if(SCROLL_TIMER_SET(w)){
	XtRemoveTimeOut(SCROLL_TIMER_ID(w));
	SCROLL_TIMER_SET(w)=FALSE;
      }
      row=XYToRow(w,be->x,be->y);
      if(row<0) return;
      SELECTION_DRAG(w)=TRUE;
      if(LAST_SELECTED_ROW(w)!=row){
	change_select_range(w,row);
	Redisplay(w,NULL,NULL);
      }
    }
  }
  return;
}

static void scrollTimer(XtPointer w,XtIntervalId* id){
  int shift;
  XmScrollBarCallbackStruct cbs;

  if((SCROLL_DIRECTION(w)==1 && VERT_SHIFT(w)==VERT_MAX(w)-VERT_SLIDER(w)) ||
     (SCROLL_DIRECTION(w)==-1 && VERT_SHIFT(w)==0)){
    SCROLL_TIMER_SET(w)=FALSE;
    return;
  }
  if(SCROLL_DIRECTION(w)==1){
    shift=VERT_SHIFT(w)+(ROW_HEIGHT(w)+EHT(w)+ROW_SPACING(w));
    if(shift>VERT_MAX(w)-VERT_SLIDER(w)){
      shift=VERT_MAX(w)-VERT_SLIDER(w);
    }
    change_select_range((Widget)w,LAST_SELECTED_ROW(w)+1);
  } else {
    shift=VERT_SHIFT(w)-(ROW_HEIGHT(w)+EHT(w)+ROW_SPACING(w));
    if(shift<0){
      shift=0;
    }
    change_select_range((Widget)w,LAST_SELECTED_ROW(w)-1);
  }
  XtVaSetValues((Widget)VERT_SB(w),XmNvalue,shift,NULL);
  cbs.value=shift;
  vert_sb_cb(NULL,(XtPointer)w,(XtPointer)&cbs);
  SCROLL_TIMER_ID(w)=XtAppAddTimeOut(XtWidgetToApplicationContext((Widget)w),
      SCROLL_DELAY,scrollTimer,(XtPointer)w);
  return;
}

static void deselect_current_range(Widget w){
  int i,last=LAST_SELECTED_ROW(w);

  if(LAST_SELECTED_ROW(w)<0 || ANCHOR(w)<0) return;

  if(LAST_SELECTED_ROW(w)>ANCHOR(w)){
    for(i=ANCHOR(w)+1;i<=last && i<ROW_COUNT(w);i++){
      deselect_row(w,i);
    }
  } else {
    for(i=ANCHOR(w)-1;i>=last && i>=0;i--){
      deselect_row(w,i);
    }
  }
}

static void change_select_range(Widget w,int row){
  int i;

  deselect_current_range(w);

  if(row>ANCHOR(w)){
    for(i=ANCHOR(w)+1;i<=row && i<ROW_COUNT(w);i++){
      if(ROW_IS_SELECTED(w,ANCHOR(w))) select_row(w,i);
      else deselect_row(w,i);
    }
  } else {
    for(i=ANCHOR(w)-1;i>=row && i>=0;i--){
      if(ROW_IS_SELECTED(w,ANCHOR(w))) select_row(w,i);
      else deselect_row(w,i);
    }
  }
}

static void select_row(Widget w,int row){
  int i;

  if(row<0 || row>=ROW_COUNT(w)) return;

  LAST_SELECTED_ROW(w)=row;

  if(ROW_IS_SELECTED(w,row)) return;

  ROW_IS_SELECTED(w,row)=TRUE;

  if(SELECTION_POLICY(w)==XmSINGLE_SELECT || 
     SELECTION_POLICY(w)==XmBROWSE_SELECT){
    
    if(SELECTED_ROW_COUNT(w)>0){
      ROW_IS_SELECTED(w,SELECTED_ROW(w,0))=FALSE;
    }
    SELECTED_ROW(w,0)=row;
    SELECTED_ROW_COUNT(w)=1;
  } else {
    if(SELECTED_ROW_COUNT(w)==ROW_COUNT(w)) return;
    for(i=0;i<SELECTED_ROW_COUNT(w);i++){
      if(SELECTED_ROW(w,i)==row) return;
    }
    SELECTED_ROW(w,SELECTED_ROW_COUNT(w))=row;
    SELECTED_ROW_COUNT(w)++;
  }
  REDRAW_BUFFER(w)=TRUE;
}

static void deselect_all(Widget w){
  int i;

  for(i=0;i<ROW_COUNT(w);i++){
    ROW_IS_SELECTED(w,i)=FALSE;
    SELECTED_ROW(w,i)=-1;
  }
  SELECTED_ROW_COUNT(w)=0;
  LAST_SELECTED_ROW(w)=-1;
  REDRAW_BUFFER(w)=TRUE;
}

static void deselect_row(Widget w,int row){
  int i;

  if(row<0 || row>=ROW_COUNT(w)) return;

  if(!ROW_IS_SELECTED(w,row)) return;

  ROW_IS_SELECTED(w,row)=FALSE;

  if(SELECTION_POLICY(w)==XmSINGLE_SELECT || 
     SELECTION_POLICY(w)==XmBROWSE_SELECT){
    SELECTED_ROW(w,0)=-1;
    SELECTED_ROW_COUNT(w)=0;
    LAST_SELECTED_ROW(w)=-1;
  } else {
    for(i=0;i<SELECTED_ROW_COUNT(w);i++){
      if(SELECTED_ROW(w,i)==row){
	for(i++;i<SELECTED_ROW_COUNT(w);i++){
	  SELECTED_ROW(w,i-1)=SELECTED_ROW(w,i);
	}
	SELECTED_ROW_COUNT(w)--;
	if(LAST_SELECTED_ROW(w)==row) LAST_SELECTED_ROW(w)=-1;
	break;
      }
    }
  }
  REDRAW_BUFFER(w)=TRUE;
}

static void PointerMotion(Widget w, XEvent *event, 
                           String *params, Cardinal *num_params) {
  int col;
  XMotionEvent *me;

  if (event->type != MotionNotify) return;
  me = (XMotionEvent *)event;
  
  if(COL_RESIZE_UNDERWAY(w)) return;
  
  /* just changing cursor ... */
  col=figureColToResize(w,me->x,me->y);

  if(col<0){
    if(COL_RESIZE_AREA(w)){
      COL_RESIZE_AREA(w)=FALSE;
      XUndefineCursor(XtDisplay(w),XtWindow(w));
    }
  } else {
    COL_RESIZE_NUM(w)=col;
    if(!COL_RESIZE_AREA(w)){
      COL_RESIZE_AREA(w)=TRUE;
      XDefineCursor(XtDisplay(w),XtWindow(w),RESIZE_CURSOR(w));
    }
  }
  return;
}

static void ButtonMotion(Widget w, XEvent *event, 
                           String *params, Cardinal *num_params) {

  XMotionEvent *me;

  if (event->type != MotionNotify) return;
  me = (XMotionEvent *)event;

  if(!COL_RESIZE_UNDERWAY(w)) return;

  drawResizeLine(w,me->x);

  return;
}

static void ButtonUp(Widget w, XEvent *event, 
                           String *params, Cardinal *num_params) {
  XButtonEvent *be;

  if (event->type != ButtonRelease) return;
  be = (XButtonEvent *)event;

  if(!COL_RESIZE_UNDERWAY(w)) return;

  COL_RESIZE_UNDERWAY(w)=FALSE;
  COL_RESIZE_AREA(w)=FALSE;
  XUndefineCursor(XtDisplay(w),XtWindow(w));

  /* do real resize stuff */
  doColResize(w);

  drawResizeLine(w,-1);

  return;
}

static void ButtonDown(Widget w, XEvent *event, 
                           String *params, Cardinal *num_params) {
  XButtonEvent *be;

  if (event->type != ButtonPress) return;
  be = (XButtonEvent *)event;

  if(!COL_RESIZE_AREA(w)) return;

  COL_RESIZE_UNDERWAY(w)=TRUE;
  COL_RESIZE_X(w)=-1;
  drawResizeLine(w,be->x);
  
  return;
}

static void doColResize(Widget w){
  int col,i,delta;

  col=COL_RESIZE_NUM(w);
  delta=COL_RESIZE_X(w)-COLUMN_OFFSET(w,col+1)+HOR_SHIFT(w);


  if(delta<0){
    int max_offset=COL_RESIZE_X(w)+HOR_SHIFT(w)-COL_MIN_SIZE;
    for(i=col;i>0;i--){
      if(COLUMN_OFFSET(w,i)>max_offset){
	COLUMN_OFFSET(w,i)=max_offset;
	max_offset-=COL_MIN_SIZE;
	if(max_offset<COLUMN_OFFSET(w,0)){
	  for(i=0;i<col;i++){
	    COLUMN_OFFSET(w,i+1)=COLUMN_OFFSET(w,i)+COL_MIN_SIZE;
	  }
	  delta=COLUMN_OFFSET(w,col)+COL_MIN_SIZE-COLUMN_OFFSET(w,col+1);
	  break;
	}
      } else {
	break;
      }
    }
  } 
  for(i=col;i<COLUMN_COUNT(w);i++){
    COLUMN_OFFSET(w,i+1)+=delta;
  }
  REDRAW_BUFFER(w)=TRUE;
  Resize(w);
  Redisplay(w,NULL,NULL);
}

static void drawResizeLine(Widget w,Position x){
  XRectangle r;
  Region region;
  if(COL_RESIZE_X(w)>=0 && COL_RESIZE_X(w)<WIDTH(w)){
    /* erasing previous thing */
    r.x=COL_RESIZE_X(w);
    r.y=0;
    r.width=1;
    r.height=HEIGHT(w);
    region=XCreateRegion();
    XUnionRectWithRegion(&r,region,region);
    Redisplay(w,NULL,region);
    XDestroyRegion(region);
  }
  COL_RESIZE_X(w)=x;
  if(COL_RESIZE_X(w)>=0 && COL_RESIZE_X(w)<WIDTH(w)){
    XDrawLine(XtDisplay(w),XtWindow(w),HIGHLIGHT_GC(w),x,0,x,HEIGHT(w));
  }
}

static void destroy_parent_cb(Widget w, XtPointer app,XtPointer call){
  XtDestroyWidget(XtParent(w));
}


Widget XmAxyCreateList(Widget parent, char *name, 
                       Arg *arglist, Cardinal argCount) {
    return XtCreateWidget(name,xmAxyListWidgetClass,parent,arglist,argCount);
}

Widget XmAxyCreateScrolledList(Widget parent, char *name, 
                               Arg *arglist, Cardinal argCount) {
    Widget sw, list;
    char *sn;
    int n;
    Arg *args;

    sn = XtMalloc(strlen(name) + 3);
    strcpy(sn, name);
    strcat(sn, "SW");

    args = (Arg *)XtCalloc(argCount + 4, sizeof(Arg));
    for(n = 0;n<argCount;n++){
      args[n].name=arglist[n].name;
      args[n].value=arglist[n].value;
    }

    XtSetArg(args[n],XmNscrollingPolicy,XmAPPLICATION_DEFINED);n++;
    XtSetArg(args[n],XmNvisualPolicy,XmVARIABLE);n++;
    XtSetArg(args[n],XmNscrollBarDisplayPolicy,XmSTATIC);n++;
    XtSetArg(args[n],XmNshadowThickness,0);n++;

    sw=XtCreateManagedWidget(sn,xmScrolledWindowWidgetClass,parent,args,n);
    XtFree((XtPointer)args);
    XtFree((XtPointer)sn);

    list=XtCreateWidget(name,xmAxyListWidgetClass,sw,arglist,argCount);
    
    XtAddCallback(list,XmNdestroyCallback,destroy_parent_cb,(XtPointer)list);
	
    if(XtIsRealized(parent)){
      XtRealizeWidget(sw);
    }

    return list;
}

void XmAxyListRefresh(Widget w){
  REDRAW_BUFFER(w)=TRUE;
  Redisplay(w,NULL,NULL);
}

void XmAxyListDeselectAllRows(Widget w){
  deselect_all(w);
  Redisplay(w,NULL,NULL);
}

void XmAxyListDeselectRow(Widget w,int row){
  deselect_row(w,row);
  Redisplay(w,NULL,NULL);
}

void XmAxyListSelectRow(Widget w,int row){
  select_row(w,row);
  Redisplay(w,NULL,NULL);
}

Boolean XmAxyListRowSelected(Widget w,int row){
  if(row<0 || row>ROW_COUNT(w)) return FALSE;

  return ROW_IS_SELECTED(w,row);
}

Boolean XmAxyListGetSelectedRows(Widget w,int **selection,int* num){
  int i;

  *num=SELECTED_ROW_COUNT(w);
  if(*num==0) return FALSE;

  *selection=(int*)XtMalloc(sizeof(int)*SELECTED_ROW_COUNT(w));

  for(i=0;i<*num;i++){
    (*selection)[i]=SELECTED_ROW(w,i);
  }

  return TRUE;
}

static Dimension preferredWidth(Widget w){
  return (Dimension)(COLUMN_OFFSET(w,COLUMN_COUNT(w))+
      SHADOW_THICKNESS(w)+MARGIN_WIDTH(w));
}

static Dimension preferredHeight(Widget w){
  Dimension height=0;

  height=SHADOW_THICKNESS(w)*2+MARGIN_HEIGHT(w)*2;

  if(COLUMN_LABELS(w)!=NULL && SHOW_LABELS(w)){
    height+=LABEL_HEIGHT(w);
  }
    
  if(ROW_COUNT(w)>0){
    height+=(EHT(w)+(ROW_HEIGHT(w)+EHT(w))*ROW_COUNT(w));
    if(ROW_COUNT(w)>1){
      height+=(ROW_SPACING(w)*(ROW_COUNT(w)-1));
    }
  }

  return height;
}

static void copyWidths(Widget w){
  short *copy=(short*)XtMalloc(sizeof(short)*COLUMN_COUNT(w));
  int i;
  
  for(i=0;i<COLUMN_COUNT(w);i++){
    copy[i]=COLUMN_WIDTHS(w)[i];
  }
  COLUMN_WIDTHS(w)=copy;
}

static void copyColumnAlignments(Widget w){
  unsigned char *copy=
    (unsigned char*)XtMalloc(sizeof(unsigned char)*COLUMN_COUNT(w));
  int i;
  
  for(i=0;i<COLUMN_COUNT(w);i++){
    copy[i]=COLUMN_ALIGNMENT(w,i);
  }
  COLUMN_ALIGNMENTS(w)=copy;
}

static void copyColumnLabelAlignments(Widget w){
  unsigned char *copy=
    (unsigned char*)XtMalloc(sizeof(unsigned char)*COLUMN_COUNT(w));
  int i;
  
  for(i=0;i<COLUMN_COUNT(w);i++){
    copy[i]=COLUMN_LABEL_ALIGNMENT(w,i);
  }
  COLUMN_LABEL_ALIGNMENTS(w)=copy;
}

static void copyColumnLabels(Widget w){
  String *copy=(String*)XtMalloc(sizeof(String)*COLUMN_COUNT(w));
  int i;

  for(i=0;i<COLUMN_COUNT(w);i++){
    copy[i]=XtNewString(COLUMN_LABELS(w)[i]);
  }
  COLUMN_LABELS(w)=copy;
}

static void createBackgroundGC(Widget w){
  XGCValues values;

  values.foreground=BACKGROUND(w);
  BACKGROUND_GC(w)=XCreateGC(XtDisplay(w),
      RootWindowOfScreen(XtScreen(w)),
      GCForeground, &values);
}

static void createLabelBackgroundGC(Widget w){
  XGCValues values;

  values.foreground=LABEL_BACKGROUND(w);
  LABEL_BACKGROUND_GC(w)=XCreateGC(XtDisplay(w),
      RootWindowOfScreen(XtScreen(w)),
      GCForeground, &values);
}

static void createLabelTopShadowGC(Widget w){
  XGCValues values;

  values.foreground=LABEL_TOP_SHADOW_COLOR(w);
  values.background=LABEL_BACKGROUND(w);
  values.line_width=1;
  values.line_style=LineSolid;
  values.join_style=JoinMiter;
  values.cap_style=CapButt;
  if(LABEL_TOP_SHADOW_PIXMAP(w)!=None && 
     LABEL_TOP_SHADOW_PIXMAP(w)!=XmUNSPECIFIED_PIXMAP){
    values.tile=LABEL_TOP_SHADOW_PIXMAP(w);
    values.fill_style=FillTiled;
    LABEL_TOP_SHADOW_GC(w)=XCreateGC(XtDisplay(w),
	RootWindowOfScreen(XtScreen(w)),
	GCForeground | GCBackground | GCLineWidth | GCLineStyle | GCCapStyle | 
	GCJoinStyle | GCTile | GCFillStyle, &values);
  } else {
    LABEL_TOP_SHADOW_GC(w)=XCreateGC(XtDisplay(w),
	RootWindowOfScreen(XtScreen(w)),
	GCForeground | GCBackground | GCLineWidth | GCLineStyle | GCCapStyle | 
	GCJoinStyle, &values);
  }
}

static void createLabelBottomShadowGC(Widget w){
  XGCValues values;

  values.foreground=LABEL_BOTTOM_SHADOW_COLOR(w);
  values.background=LABEL_BACKGROUND(w);
  values.line_width=1;
  values.line_style=LineSolid;
  values.join_style=JoinMiter;
  values.cap_style=CapButt;
  if(LABEL_BOTTOM_SHADOW_PIXMAP(w)!=None && 
     LABEL_BOTTOM_SHADOW_PIXMAP(w)!=XmUNSPECIFIED_PIXMAP){
    values.tile=LABEL_BOTTOM_SHADOW_PIXMAP(w);
    values.fill_style=FillTiled;
    LABEL_BOTTOM_SHADOW_GC(w)=XCreateGC(XtDisplay(w),
	RootWindowOfScreen(XtScreen(w)),
	GCForeground | GCBackground | GCLineWidth | GCLineStyle | GCCapStyle | 
	GCJoinStyle | GCTile | GCFillStyle, &values);
  } else {
    LABEL_BOTTOM_SHADOW_GC(w)=XCreateGC(XtDisplay(w),
	RootWindowOfScreen(XtScreen(w)),
	GCForeground | GCBackground | GCLineWidth | GCLineStyle | GCCapStyle | 
	GCJoinStyle, &values);
  }
}

static void createLabelDrawGC(Widget w){
  XGCValues values;

  values.foreground=LABEL_FOREGROUND(w);
  values.background=LABEL_BACKGROUND(w);
  values.font=LABEL_FONT_FID(w);
  LABEL_DRAW_GC(w)=XCreateGC(XtDisplay(w),
      RootWindowOfScreen(XtScreen(w)),
      GCForeground | GCBackground | GCFont, &values);
}

static void createCellBackgroundGC(Widget w){
  XGCValues values;

  values.foreground=CELL_BACKGROUND(w);
  CELL_BACKGROUND_GC(w)=XCreateGC(XtDisplay(w),
      RootWindowOfScreen(XtScreen(w)),
      GCForeground, &values);
}

static void createDrawGC(Widget w){
  XGCValues values;

  values.foreground=CELL_FOREGROUND(w);
  values.background=CELL_BACKGROUND(w);
  values.font=FONT_FID(w);
  DRAW_GC(w)=XCreateGC(XtDisplay(w),
      RootWindowOfScreen(XtScreen(w)),
      GCForeground | GCBackground | GCFont, &values);
}

static void createSelectedBackgroundGC(Widget w){
  XGCValues values;

  values.foreground=SELECTED_BACKGROUND(w);
  SELECTED_BACKGROUND_GC(w)=XCreateGC(XtDisplay(w),
      RootWindowOfScreen(XtScreen(w)),
      GCForeground, &values);
}

static void createSelectedDrawGC(Widget w){
  XGCValues values;

  values.foreground=SELECTED_FOREGROUND(w);
  values.background=SELECTED_BACKGROUND(w);
  values.font=FONT_FID(w);
  SELECTED_DRAW_GC(w)=XCreateGC(XtDisplay(w),
      RootWindowOfScreen(XtScreen(w)),
      GCForeground | GCBackground | GCFont, &values);
}

static void createGCs(Widget w){
  createBackgroundGC(w);
  createLabelBackgroundGC(w);
  createLabelTopShadowGC(w);
  createLabelBottomShadowGC(w);
  createLabelDrawGC(w);
  createCellBackgroundGC(w);
  createDrawGC(w);
  createSelectedBackgroundGC(w);
  createSelectedDrawGC(w);
}

static void hor_sb_cb(Widget w,XtPointer app,XtPointer call){
  Widget list=(Widget)app;
  XmScrollBarCallbackStruct *cbs=(XmScrollBarCallbackStruct*)call;
  int delta=cbs->value-HOR_SHIFT(list),x;
  XRectangle cr,r;

  if(delta==0) {
    return;
  }

  if(!XtIsRealized(list) || !BUFFER(list) || REDRAW_BUFFER(list)){
    HOR_SHIFT(list)=cbs->value;
    Redisplay(list,NULL,NULL);
    return;
  }

  cr.x=SHADOW_THICKNESS(list)+MARGIN_WIDTH(list)+EHT(list);
  cr.y=SHADOW_THICKNESS(list)+MARGIN_HEIGHT(list);
  cr.width=x=WIDTH(list)-SHADOW_THICKNESS(list)*2-
    MARGIN_WIDTH(list)*2-EHT(list)*2;
  if(x<0)cr.width=0;
  cr.height=x=HEIGHT(list)-SHADOW_THICKNESS(list)*2-MARGIN_HEIGHT(list)*2;
  if(x<0)cr.height=0;

  if(delta>0){
      if(delta<cr.width){
      XCopyArea(XtDisplay(list),BUFFER(list),BUFFER(list),BACKGROUND_GC(list),
	  cr.x+delta,cr.y,cr.width-delta,cr.height,cr.x,cr.y);
      r.x=cr.x+cr.width-delta;
      r.y=cr.y;
      r.width=delta;
      r.height=cr.height;
    } else {
      r.x=cr.x;r.y=cr.y;r.width=cr.width;r.height=cr.height;
    }
  } else {
    if(-delta<cr.width){
      XCopyArea(XtDisplay(list),BUFFER(list),BUFFER(list),BACKGROUND_GC(list),
	  cr.x,cr.y,cr.width+delta,cr.height,cr.x-delta,cr.y);
      r.x=cr.x;
      r.y=cr.y;
      r.width=-delta;
      r.height=cr.height;
    } else {
      r.x=cr.x;r.y=cr.y;r.width=cr.width;r.height=cr.height;
    }
  }
      
  HOR_SHIFT(list)=cbs->value;
  redrawList(list,BUFFER(list),&r);

  XCopyArea(XtDisplay(list),BUFFER(list),XtWindow(list),BACKGROUND_GC(list),
      cr.x,cr.y,cr.width,cr.height,cr.x,cr.y);

  return;
}

static void vert_sb_cb(Widget w,XtPointer app,XtPointer call){
  Widget list=(Widget)app;
  XmScrollBarCallbackStruct *cbs=(XmScrollBarCallbackStruct*)call;
  int x,delta=cbs->value-VERT_SHIFT(list);
  XRectangle cr,r;

  if(delta==0) {
    return;
  }

  if(!XtIsRealized(list) || !BUFFER(list) || REDRAW_BUFFER(list)){
    VERT_SHIFT(list)=cbs->value;
    Redisplay(list,NULL,NULL);
    return;
  }

  cr.x=SHADOW_THICKNESS(list)+MARGIN_WIDTH(list);
  cr.y=SHADOW_THICKNESS(list)+MARGIN_HEIGHT(list)+
    (SHOW_LABELS(list)?LABEL_HEIGHT(list):(Dimension)0);
  cr.width=x=WIDTH(list)-SHADOW_THICKNESS(list)*2-MARGIN_WIDTH(list)*2;
  if(x<0)cr.width=0;
  cr.height=x=HEIGHT(list)-SHADOW_THICKNESS(list)-cr.y-MARGIN_WIDTH(list)*2;
  if(x<0)cr.height=0;

  if(delta>0){
    if(delta<cr.height){
      XCopyArea(XtDisplay(list),BUFFER(list),BUFFER(list),BACKGROUND_GC(list),
	  cr.x,cr.y+delta,cr.width,cr.height-delta,cr.x,cr.y);
      r.x=cr.x;
      r.y=cr.y+cr.height-delta;
      r.width=cr.width;
      r.height=delta;
    } else {
      r.x=cr.x;r.y=cr.y;r.width=cr.width;r.height=cr.height;
    }
  } else {
    if(-delta<cr.height){
      XCopyArea(XtDisplay(list),BUFFER(list),BUFFER(list),BACKGROUND_GC(list),
	  cr.x,cr.y,cr.width,cr.height+delta,cr.x,cr.y-delta);
      r.x=cr.x;
      r.y=cr.y;
      r.width=cr.width;
      r.height=-delta;
    } else {
      r.x=cr.x;r.y=cr.y;r.width=cr.width;r.height=cr.height;
    }
  }
      
  VERT_SHIFT(list)=cbs->value;
  redrawList(list,BUFFER(list),&r);

  XCopyArea(XtDisplay(list),BUFFER(list),XtWindow(list),BACKGROUND_GC(list),
      cr.x,cr.y,cr.width,cr.height,cr.x,cr.y);

  return;
}

static void redrawList(Widget w,Drawable d,XRectangle *cr){
  XRectangle bcr,margin;
  int x;

  if(cr==NULL){
    cr=&bcr;cr->x=0;cr->y=0;cr->width=WIDTH(w);cr->height=HEIGHT(w);
  }

  XFillRectangle(XtDisplay(w),d,BACKGROUND_GC(w),
      cr->x,cr->y,cr->width,cr->height);

  margin.x=SHADOW_THICKNESS(w)+MARGIN_WIDTH(w);
  margin.y=SHADOW_THICKNESS(w)+MARGIN_HEIGHT(w);
  margin.width=x=WIDTH(w)-SHADOW_THICKNESS(w)*2-MARGIN_WIDTH(w)*2;
  if(x<0)margin.width=0;
  margin.height=x=HEIGHT(w)-SHADOW_THICKNESS(w)*2-MARGIN_HEIGHT(w)*2;
  if(x<0)margin.height=0;

  axyRectanglesIntersection(cr,&margin,&margin);

  drawLabels(w,d,&margin);
  drawCells(w,d,&margin);

  if(cr->x>SHADOW_THICKNESS(w) && 
     cr->x+cr->width<WIDTH(w)-SHADOW_THICKNESS(w) &&
     cr->y>SHADOW_THICKNESS(w) &&
     cr->y+cr->height<HEIGHT(w)-SHADOW_THICKNESS(w)) return;
  else 
    _XmDrawShadows(XtDisplay(w),d,
	  TOP_SHADOW_GC(w),BOTTOM_SHADOW_GC(w),
	  0,0,WIDTH(w),HEIGHT(w),
	  SHADOW_THICKNESS(w),XmSHADOW_IN);

}

static void installFont(Widget new_w){
  XmFontContext context;
  XFontStruct *font;
  XmFontListEntry font_list_entry;
  XmFontType type;
  XFontSetExtents *extents;
  XFontStruct **fonts;
  char **font_names;

  XmFontListInitFontContext(&context, LABEL_FONT(new_w));
  font_list_entry = XmFontListNextEntry(context);
  font = (XFontStruct*)XmFontListEntryGetFont(font_list_entry, &type);
  if(type == XmFONT_IS_FONTSET){
    extents = XExtentsOfFontSet((XFontSet)font);
    FONT_WIDTH(new_w) = extents->max_logical_extent.width;
    FONT_HEIGHT(new_w) = extents->max_logical_extent.height;
    FONT_Y(new_w) = -extents->max_logical_extent.y;
    XFontsOfFontSet((XFontSet)font, &fonts, &font_names);
    FONT_FID(new_w) = fonts[0]->fid;
    FONT_STRUCT(new_w) = fonts[0];
    FONT_SET(new_w) = (XFontSet)font;
  } else {
    FONT_WIDTH(new_w) = 
      (font->max_bounds.width + font->min_bounds.width) /2;
    FONT_HEIGHT(new_w) = 
	(font->max_bounds.descent + font->max_bounds.ascent);
    FONT_Y(new_w) = font->max_bounds.ascent;
    FONT_FID(new_w) = font->fid;
    FONT_STRUCT(new_w) = font;
    FONT_SET(new_w) = (XFontSet)NULL;
  }
  XmFontListFreeFontContext(context);
}

static void installLabelFont(Widget new_w){
  XmFontContext context;
  XFontStruct *font;
  XmFontListEntry font_list_entry;
  XmFontType type;
  XFontSetExtents *extents;
  XFontStruct **fonts;
  char **font_names;

  XmFontListInitFontContext(&context, LABEL_FONT(new_w));
  font_list_entry = XmFontListNextEntry(context);
  font = (XFontStruct*)XmFontListEntryGetFont(font_list_entry, &type);
  if (type == XmFONT_IS_FONTSET){
    extents = XExtentsOfFontSet((XFontSet)font);
    LABEL_FONT_WIDTH(new_w) = extents->max_logical_extent.width;
    LABEL_FONT_HEIGHT(new_w) = extents->max_logical_extent.height;
    LABEL_FONT_Y(new_w) = -extents->max_logical_extent.y;
    XFontsOfFontSet((XFontSet)font, &fonts, &font_names);
    LABEL_FONT_FID(new_w) = fonts[0]->fid;
    LABEL_FONT_STRUCT(new_w) = fonts[0];
    LABEL_FONT_SET(new_w) = (XFontSet)font;
  } else {
    LABEL_FONT_WIDTH(new_w) = 
      (font->max_bounds.width + font->min_bounds.width) /2;
    LABEL_FONT_HEIGHT(new_w) = 
	(font->max_bounds.descent + font->max_bounds.ascent);
    LABEL_FONT_Y(new_w) = font->max_bounds.ascent;
    LABEL_FONT_FID(new_w) = font->fid;
    LABEL_FONT_STRUCT(new_w) = font;
    LABEL_FONT_SET(new_w) = (XFontSet)NULL;
  }
  XmFontListFreeFontContext(context);
}

static void drawLabels(Widget w,Drawable d,XRectangle *cr){
  int i,i0,i1,x;
  XRectangle r,bcr;

  if(!SHOW_LABELS(w)) return;

  if(cr==NULL){
    cr=&bcr; cr->x=0; cr->y=0; cr->width=WIDTH(w); cr->height=HEIGHT(w);
  }

  r.x=SHADOW_THICKNESS(w)+MARGIN_WIDTH(w)-HOR_SHIFT(w);
  r.y=SHADOW_THICKNESS(w)+MARGIN_HEIGHT(w);
  r.width=x=
    COLUMN_OFFSET(w,COLUMN_COUNT(w))-SHADOW_THICKNESS(w)-MARGIN_WIDTH(w);
  if(x<0)r.width=0;
  r.height=x=LABEL_HEIGHT(w);
  if(x<0)r.height=0;

  axyRectanglesIntersection(cr,&r,&r);
  if(axyRectangleIsEmpty(&r)) return;

  for(i=0;i<COLUMN_COUNT(w);i++){
    if((int)COLUMN_OFFSET(w,i+1)-(int)HOR_SHIFT(w)>(int)r.x){
      break;
    }
  }
  i0=i;
  for(i=i0;i<COLUMN_COUNT(w);i++){
    if((int)COLUMN_OFFSET(w,i)-(int)HOR_SHIFT(w)>=(int)r.x+(int)r.width){
      break;
    }
  }
  i1=i;

  XFillRectangle(XtDisplay(w),d,LABEL_BACKGROUND_GC(w),
      r.x,r.y,r.width,r.height);

  for(i=i0;i<i1;i++){
    drawLabel(w,d,&r,i);
  }
}

static void drawLabel(Widget w,Drawable d,XRectangle *cr,int i){
  XRectangle r,clip;
  int width,x;

  r.x=COLUMN_OFFSET(w,i)+LABEL_SHADOW_THICKNESS(w)+EHT(w)+
    LABEL_MARGIN_WIDTH(w)-HOR_SHIFT(w);
  r.y=SHADOW_THICKNESS(w)+MARGIN_HEIGHT(w)+LABEL_SHADOW_THICKNESS(w)+EHT(w)+
    LABEL_MARGIN_HEIGHT(w);
  r.width=x=COLUMN_OFFSET(w,i+1)-COLUMN_OFFSET(w,i)-
    LABEL_SHADOW_THICKNESS(w)*2-EHT(w)*2-LABEL_MARGIN_WIDTH(w)*2;
  if(x<0)r.width=0;
  r.height=x=LABEL_HEIGHT(w)-LABEL_SHADOW_THICKNESS(w)*2-EHT(w)*2-
    LABEL_MARGIN_HEIGHT(w)*2;
  if(x<0)r.height=0;

  axyRectanglesIntersection(cr,&r,&clip);

  if(!axyRectangleIsEmpty(&clip)){

    width=XTextWidth(LABEL_FONT_STRUCT(w),
	COLUMN_LABEL(w,i),strlen(COLUMN_LABEL(w,i)));

    if(COLUMN_LABEL_ALIGNMENT(w,i)==XmALIGNMENT_END){
      x=(int)r.x+(int)r.width-width;
    } else if(COLUMN_LABEL_ALIGNMENT(w,i)==XmALIGNMENT_CENTER){
      x=(int)r.x+(int)r.width/2-width/2;
    } else {
      x=(int)r.x;
    }

    XSetClipRectangles(XtDisplay(w),LABEL_DRAW_GC(w),0,0,&clip,1,Unsorted);
    XDrawString(XtDisplay(w),d,LABEL_DRAW_GC(w),
	x,r.y+LABEL_FONT_Y(w),
	COLUMN_LABELS(w)[i],strlen(COLUMN_LABELS(w)[i]));
    XSetClipMask(XtDisplay(w),LABEL_DRAW_GC(w),None);
  }

  XSetClipRectangles(XtDisplay(w),LABEL_TOP_SHADOW_GC(w),
      0,0,cr,1,Unsorted);
  XSetClipRectangles(XtDisplay(w),LABEL_BOTTOM_SHADOW_GC(w),
      0,0,cr,1,Unsorted);
  _XmDrawShadows(XtDisplay(w),d,
      LABEL_TOP_SHADOW_GC(w),LABEL_BOTTOM_SHADOW_GC(w),
      COLUMN_OFFSET(w,i)-HOR_SHIFT(w),
      SHADOW_THICKNESS(w)+MARGIN_HEIGHT(w),
      COLUMN_OFFSET(w,i+1)-COLUMN_OFFSET(w,i),
      LABEL_HEIGHT(w),
      LABEL_SHADOW_THICKNESS(w),XmSHADOW_OUT);
  XSetClipMask(XtDisplay(w),LABEL_TOP_SHADOW_GC(w),None);
  XSetClipMask(XtDisplay(w),LABEL_BOTTOM_SHADOW_GC(w),None);
}

static void drawCells(Widget w,Drawable d,XRectangle *cr){
  int i0,i1,j0,j1,i,j,x,y;
  XRectangle r,bcr;

  if(ROW_COUNT(w)<=0 || COLUMN_COUNT(w)<=0) return;
  
  if(cr==NULL){
    cr=&bcr; cr->x=0; cr->y=0; cr->width=WIDTH(w); cr->height=HEIGHT(w);
  }

  /*printf("ClipBox=%dx%d+%d+%d\n",cr->width,cr->height,cr->x,cr->y);*/
  
  r.x=SHADOW_THICKNESS(w)+MARGIN_WIDTH(w);
  r.y=ROW_OFFSET(w,0)-EHT(w);
  x=COLUMN_OFFSET(w,COLUMN_COUNT(w))-HOR_SHIFT(w);
  y=ROW_OFFSET(w,ROW_COUNT(w))-ROW_SPACING(w)-VERT_SHIFT(w);

  x=Min(x,cr->x+cr->width);
  y=Min(y,cr->y+cr->height);

  r.width=x-r.x;
  r.height=y-r.y;

  axyRectanglesIntersection(cr,&r,&r);
  if(axyRectangleIsEmpty(&r)) return;

  /*printf("CellClip=%dx%d+%d+%d\n",rect.width,rect.height,rect.x,rect.y);*/

  for(i=0;i<COLUMN_COUNT(w);i++){
    if((int)COLUMN_OFFSET(w,i+1)-(int)HOR_SHIFT(w)>(int)r.x){
      break;
    }
  }
  i0=i;
  for(i=i0;i<COLUMN_COUNT(w);i++){
    if((int)COLUMN_OFFSET(w,i)-(int)HOR_SHIFT(w)>=(int)r.x+(int)r.width){
      break;
    }
  }
  i1=i;

  for(j=0;j<ROW_COUNT(w);j++){
    if((int)ROW_OFFSET(w,j+1)-(int)VERT_SHIFT(w)>(int)r.y){
      break;
    }
  }
  j0=j;
  for(j=j0;j<ROW_COUNT(w);j++){
    if((int)ROW_OFFSET(w,j)-(int)VERT_SHIFT(w)>=(int)r.y+(int)r.height){
      break;
    }
  }
  j1=j;

  /*printf("COL: %d - %d  ROW: %d - %d \n",i0,i1,j0,j1);*/

  XFillRectangle(XtDisplay(w),d,CELL_BACKGROUND_GC(w),
      r.x,r.y,r.width,r.height);

  drawSelection(w,d,&r,j0,j1);

  for(i=i0;i<i1;i++){
    for(j=j0;j<j1;j++){
      drawCell(w,d,&r,i,j);
    }
  }
}

static void drawSelection(Widget w,Drawable d,XRectangle *cr,int j0,int j1){
  int j,tmp;
  XRectangle r;

  for(j=j0;j<j1;j++){
    if(!ROW_IS_SELECTED(w,j)) continue;
    r.y=ROW_OFFSET(w,j)-VERT_SHIFT(w);
    r.height=ROW_HEIGHT(w);
    tmp=ROW_OFFSET(w,0)-EHT(w);
    if(r.y<tmp) {
      tmp-=r.y;
      r.y+=tmp;
      if(tmp<r.height) r.height-=tmp;
      else r.height=0;
    }
    tmp=(int)HEIGHT(w)-(int)SHADOW_THICKNESS(w)-(int)MARGIN_HEIGHT(w);
    if((int)r.y+(int)r.height>=tmp){
      tmp-=((int)r.y+(int)r.height);
      if(tmp<r.height) r.height-=tmp;
      else r.height=0;

    }
    r.x=SHADOW_THICKNESS(w)+MARGIN_WIDTH(w)+EHT(w);
    tmp=Min(COLUMN_OFFSET(w,COLUMN_COUNT(w))-HOR_SHIFT(w),
	(int)WIDTH(w)-(int)SHADOW_THICKNESS(w)-(int)MARGIN_WIDTH(w))-
      (int)EHT(w);
    tmp-=(int)r.x;
    if(tmp<0)tmp=0;
    r.width=tmp;

    axyRectanglesIntersection(cr,&r,&r);
    if(axyRectangleIsEmpty(&r)) continue;

    XFillRectangle(XtDisplay(w),d,SELECTED_BACKGROUND_GC(w),
	r.x,r.y,r.width,r.height);
  }
}

static void drawCell(Widget w,Drawable d,XRectangle *cr,int i,int j){
  XmAxyListDrawCellCallbackStruct cbs;
  Window root;
  int x,y,width;
  unsigned int border;
  XRectangle r,clip,pixr,pixr_trans,helper;

  helper.y=0;
  helper.height=HEIGHT(w);
  helper.x=SHADOW_THICKNESS(w)+MARGIN_WIDTH(w)+EHT(w);
  width=WIDTH(w)-SHADOW_THICKNESS(w)*2-MARGIN_WIDTH(w)*2-EHT(w)*2;
  if(width>0) helper.width=width;
  else helper.width=0;

  axyRectanglesIntersection(cr,&helper,&helper);
  cr=&helper;

  r.x=COLUMN_OFFSET(w,i)+EHT(w)-HOR_SHIFT(w);
  r.y=ROW_OFFSET(w,j)-VERT_SHIFT(w);
  r.width=x=COLUMN_OFFSET(w,i+1)-COLUMN_OFFSET(w,i)-EHT(w)*2;
  if(x<0)r.width=0;
  r.height=ROW_HEIGHT(w);

  if(!axyRectanglesIntersect(cr,&r)) return;

  cbs.reason=XmAxyListDrawCellReason;
  cbs.event=NULL;
  cbs.column=i;
  cbs.row=j;
  cbs.is_xm_string=FALSE;
  cbs.width=r.width;
  cbs.height=r.height;
  cbs.type=0;
  cbs.string=NULL;
  cbs.pixmap=(Pixmap)0;
  cbs.pixmask=(Pixmap)0;
  cbs.pixwidth=0;
  cbs.pixheight=0;
  cbs.pixdepth=0;
  if(ROW_IS_SELECTED(w,j)){
    cbs.foreground=SELECTED_FOREGROUND(w);
    cbs.background=SELECTED_BACKGROUND(w);
  } else {
    cbs.foreground=CELL_FOREGROUND(w);
    cbs.background=CELL_BACKGROUND(w);
  }

  XtCallCallbackList(w,DRAW_CB_LIST(w),(XtPointer)&cbs);

  switch(cbs.type){

    case XmSTRING:

      r.x+=CELL_MARGIN_WIDTH(w);
      r.y+=CELL_MARGIN_HEIGHT(w);
      if(r.width<CELL_MARGIN_WIDTH(w)*2) r.width=0;
      else r.width-=CELL_MARGIN_WIDTH(w)*2;
      if(r.height<CELL_MARGIN_HEIGHT(w)*2) r.height=0;
      else r.height-=CELL_MARGIN_HEIGHT(w)*2;

      axyRectanglesIntersection(cr,&r,&clip);
      if(axyRectangleIsEmpty(&clip)) break;

      if(!cbs.is_xm_string){

	width=XTextWidth(FONT_STRUCT(w),cbs.string,strlen(cbs.string));
	if(COLUMN_ALIGNMENT(w,i)==XmALIGNMENT_END){
	  x=(int)r.width-width;
	} else if(COLUMN_ALIGNMENT(w,i)==XmALIGNMENT_CENTER){
	  x=(int)r.width/2-width/2;
	} else {
	  x=0;
	}


	XSetClipRectangles(XtDisplay(w),
	    ROW_IS_SELECTED(w,j)?SELECTED_DRAW_GC(w):DRAW_GC(w),
	    0,0,&clip,1,Unsorted);
	XDrawString(XtDisplay(w),d,
	  ROW_IS_SELECTED(w,j)?SELECTED_DRAW_GC(w):DRAW_GC(w),
	  r.x+x,r.y+FONT_Y(w),
	  cbs.string,strlen(cbs.string));
	XSetClipMask(XtDisplay(w),
	    ROW_IS_SELECTED(w,j)?SELECTED_DRAW_GC(w):DRAW_GC(w),None);

      } else {

	XSetClipRectangles(XtDisplay(w),
	    ROW_IS_SELECTED(w,j)?SELECTED_DRAW_GC(w):DRAW_GC(w),
	    0,0,&clip,1,Unsorted);
	XmStringDraw(XtDisplay(w),d,FONT(w),(XmString)cbs.string,
	    ROW_IS_SELECTED(w,j)?SELECTED_DRAW_GC(w):DRAW_GC(w),
	    r.x,r.y,r.width,
	    COLUMN_ALIGNMENT(w,i),STRING_DIR(w),&clip);
	XSetFont(XtDisplay(w),DRAW_GC(w),FONT_FID(w));
	XSetClipMask(XtDisplay(w),
	    ROW_IS_SELECTED(w,j)?SELECTED_DRAW_GC(w):DRAW_GC(w),None);

      }

      break;


    case XmPIXMAP:

      axyRectanglesIntersection(cr,&r,&clip);
      if(axyRectangleIsEmpty(&clip)) break;

      if(cbs.pixwidth<=0 || cbs.pixdepth<=0 || cbs.pixheight<=0){
	XGetGeometry(XtDisplay(w),cbs.pixmap,&root,&x,&y,
	    &cbs.pixwidth,&cbs.pixheight,&border,&cbs.pixdepth);
      }

      /* x,y are coords of the pixmap relative to the upper left corner 
       * of the cell */
      if(COLUMN_ALIGNMENT(w,i)==XmALIGNMENT_END){
	x=(int)r.width-(int)cbs.pixwidth;
      } else if(COLUMN_ALIGNMENT(w,i)==XmALIGNMENT_CENTER){
	x=(int)r.width/2-(int)cbs.pixwidth/2;
      } else {
	x=0;
      }
      y=(int)r.height/2-(int)cbs.pixheight/2;

      /* pixr rectangle of the pixmap to copy after clipping on the cell */
      pixr.x=(x<0)?(-x):0;
      pixr.y=(y<0)?(-y):0;
      pixr.width=Min(r.width,cbs.pixwidth);
      pixr.height=Min(r.height,cbs.pixheight);

      /* pixr_trans are the same rectangle, only in the "absolute" coords */
      pixr_trans.x=pixr.x+r.x+x;
      pixr_trans.y=pixr.y+r.y+y;
      pixr_trans.width=pixr.width;
      pixr_trans.height=pixr.height;

      /* Now clip() it */
      axyRectanglesIntersection(&pixr_trans,&clip,&clip);
      /* so clip is the rect we want to put _part_ of the pixmap */

      /* Now modify the rectangle in pixmap coords */
      pixr.x+=(clip.x-pixr_trans.x);
      pixr.y+=(clip.y-pixr_trans.y);
      pixr.width=clip.width;
      pixr.height=clip.height;

      if(cbs.pixdepth>1){
	if(cbs.pixmask){
	  XSetClipMask(XtDisplay(w),DRAW_GC(w),cbs.pixmask);
	  XSetClipOrigin(XtDisplay(w),DRAW_GC(w),r.x+x,r.y+y);
	} 
	XCopyArea(XtDisplay(w),cbs.pixmap,d,DRAW_GC(w),
	      pixr.x,pixr.y,
	      pixr.width,pixr.height,
	      clip.x,clip.y);
      } else {
	XCopyPlane(XtDisplay(w),cbs.pixmap,d,DRAW_GC(w),
	      pixr.x,pixr.y,
	      pixr.width,pixr.height,
	      clip.x,clip.y,1L);
      }
      XSetClipMask(XtDisplay(w),DRAW_GC(w),None);

      break;


    default:

      break;
  }
}

static void handleVertSB(Widget w,Boolean* mc){
  int ph;

  /*if(RESIZING_VSB(w)) return;*/

  RESIZING_VSB(w)=TRUE;
  
  ph=(int)preferredHeight(w);

  if(ph>HEIGHT(w)){
    if(!XtIsManaged((Widget)VERT_SB(w))){
      XtManageChild((Widget)VERT_SB(w));
      *mc=TRUE;
    }
    VISIBLE_ROW_COUNT(w)=(HEIGHT(w)-(SHOW_LABELS(w)?LABEL_HEIGHT(w):0)-
	SHADOW_THICKNESS(w)*2)/(ROW_HEIGHT(w)+ROW_SPACING(w)+EHT(w));
    if(VISIBLE_ROW_COUNT(w)<0) VISIBLE_ROW_COUNT(w)=0;
    VERT_MAX(w)=ph-HEIGHT(w);
    VERT_SLIDER(w)=HEIGHT(w);
    if(VERT_SLIDER(w)<6)VERT_SLIDER(w)=6;
    VERT_MAX(w)=VERT_MAX(w)+VERT_SLIDER(w);
    if(VISIBLE_ROW_COUNT(w)>1){
      VERT_PAGE(w)=
	(VISIBLE_ROW_COUNT(w)-1)*(ROW_HEIGHT(w)+ROW_SPACING(w)+EHT(w));
    } else {
      VERT_PAGE(w)=ROW_HEIGHT(w)+ROW_SPACING(w)+EHT(w);
    }
    if(VERT_PAGE(w)<1)VERT_PAGE(w)=1;
    if(VERT_SHIFT(w)>VERT_MAX(w)-VERT_SLIDER(w)) 
      VERT_SHIFT(w)=VERT_MAX(w)-VERT_SLIDER(w);
    XtVaSetValues((Widget)VERT_SB(w),
	XmNminimum,0,
	XmNmaximum,VERT_MAX(w),
	XmNsliderSize,VERT_SLIDER(w),
	XmNpageIncrement,VERT_PAGE(w),
	XmNvalue,VERT_SHIFT(w),
	NULL);
  } else {
    VERT_SHIFT(w)=0;
    VERT_SLIDER(w)=6;
    VERT_MAX(w)=VERT_SLIDER(w);
    VISIBLE_ROW_COUNT(w)=ROW_COUNT(w);
    if(XtIsManaged((Widget)VERT_SB(w))){
      XtUnmanageChild((Widget)VERT_SB(w));
      *mc=TRUE;
    }
  }
  /*
  xmScrolledWindowWidgetClass->core_class.resize(
      (Widget)SCROLLED_WINDOW(w));
      */
  RESIZING_VSB(w)=FALSE;
}

static void handleHorSB(Widget w,Boolean* mc){
  int pw;

  /*if(RESIZING_HSB(w)) return;*/

  RESIZING_HSB(w)=TRUE;

  pw=(int)preferredWidth(w);

  if(pw>WIDTH(w)){
    if(!XtIsManaged((Widget)HOR_SB(w))){
      XtManageChild((Widget)HOR_SB(w));
      *mc=TRUE;
    }
    HOR_MAX(w)=pw-WIDTH(w);
    HOR_SLIDER(w)=WIDTH(w);
    if(HOR_SLIDER(w)<6)HOR_SLIDER(w)=6;
    HOR_MAX(w)+=HOR_SLIDER(w);
    HOR_PAGE(w)=WIDTH(w)-32;
    if(HOR_PAGE(w)<1)HOR_PAGE(w)=1;
    if(HOR_SHIFT(w)>HOR_MAX(w)-HOR_SLIDER(w)) 
      HOR_SHIFT(w)=HOR_MAX(w)-HOR_SLIDER(w);
    /*printf("Setting HOR_MAX=%d HOR_SLIDER=%d\n",HOR_MAX(w),HOR_SLIDER(w));*/
    XtVaSetValues((Widget)HOR_SB(w),XmNmaximum,HOR_MAX(w),
	XmNminimum,0,
	XmNmaximum,HOR_MAX(w),
	XmNsliderSize,HOR_SLIDER(w),
	XmNpageIncrement,HOR_PAGE(w),
	XmNvalue,HOR_SHIFT(w),
	NULL);
  } else {
    HOR_SHIFT(w)=0;
    HOR_SLIDER(w)=6;
    HOR_MAX(w)=HOR_SLIDER(w);
    if(XtIsManaged((Widget)HOR_SB(w))){
      XtUnmanageChild((Widget)HOR_SB(w));
      *mc=TRUE;
    }
  }
  /*
  xmScrolledWindowWidgetClass->core_class.resize(
      (Widget)SCROLLED_WINDOW(w));
      */
  RESIZING_HSB(w)=FALSE;
}

static void resizeLater(XtPointer w,XtIntervalId* id){
  RESIZE_TIMER_STARTED(w)=TRUE;
  RESIZING(w)++;
  SCROLLED_WINDOW(w)->core.widget_class->core_class.resize(
      (Widget)SCROLLED_WINDOW(w));
  RESIZE_TIMER_SET(w)=FALSE;
  RESIZING(w)--;
  RESIZE_TIMER_STARTED(w)=FALSE;
}
