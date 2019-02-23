/* Copyright (c) 1999   Alexander Yukhimets. All rights reserved. */

#ifndef XM_AXY_LIST_P_H
#define XM_AXY_LIST_P_H

#include <XmAxy/List.h>
#include <Xm/PrimitiveP.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrolledWP.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _XmAxyListClassPart {
    XtPointer extension;
} XmAxyListClassPart;

typedef struct _XmAxyListClassRec {
    CoreClassPart core_class;
    XmPrimitiveClassPart primitive_class;
    XmAxyListClassPart list_class;
} XmAxyListClassRec;

externalref XmAxyListClassRec xmAxyListClassRec;


typedef struct _XmAxyListPart {       
    Dimension			margin_width;
    Dimension			margin_height;
    Dimension			cell_margin_width;
    Dimension			cell_margin_height;
    Dimension			label_margin_width;
    Dimension			label_margin_height;

    XmFontList			font;
    Font			font_fid;
    XFontStruct*		font_struct;
    XFontSet    		font_set;
    Dimension			font_width;
    Dimension			font_height;
    Dimension			font_y;

    XmFontList			label_font;
    Font			label_font_fid;
    XFontStruct*		label_font_struct;
    XFontSet    		label_font_set;
    Dimension			label_font_width;
    Dimension			label_font_height;
    Dimension			label_font_y;

    Boolean			bold_labels;
    Boolean			show_labels;
    Boolean			active_labels;

    Dimension			label_shadow_thickness;
    Pixel			label_background;
    Pixel			label_foreground;
    Pixel			label_top_shadow_color;
    Pixel			label_bottom_shadow_color;
    Pixmap			label_top_shadow_pixmap;
    Pixmap			label_bottom_shadow_pixmap;

    Pixel			cell_background;
    Pixel			cell_foreground;
    Pixel			selected_background;
    Pixel			selected_foreground;

    GC				background_gc;

    GC				label_background_gc;
    GC				label_top_shadow_gc;
    GC				label_bottom_shadow_gc;
    GC				label_draw_gc;

    GC				cell_background_gc;
    GC 				draw_gc;

    GC				selected_background_gc;
    GC				selected_draw_gc;


    XmStringDirection		string_dir;

    int 			row_count;
    int 			visible_row_count;
    Dimension 			row_spacing;
    int				column_count;

    String*			column_labels;
    short*			column_widths;
    unsigned char*		column_alignments;
    unsigned char*		column_label_alignments;
    int*			column_offsets;
  
    unsigned char   		selection_policy;
    unsigned char   		size_policy;
    unsigned char   		sb_display_policy;

    int				vert_shift;
    int				vert_max;
    int				vert_page;
    int				vert_slider;
    int				hor_shift;
    int				hor_max;
    int				hor_page;
    int				hor_slider;

    int				resizing;
    Boolean			resizing_vsb;
    Boolean			resizing_hsb;
    Boolean			resize_timer_set;
    XtIntervalId		resize_timer_id;
    Boolean			resize_timer_started;


    XtCallbackList     		single_callback;
    XtCallbackList     		multiple_callback;
    XtCallbackList     		extended_callback;
    XtCallbackList     		browse_callback;
    XtCallbackList     		default_callback;
    XtCallbackList     		label_callback;
    XtCallbackList     		draw_cell_callback;

    XmScrollBarWidget   	hor_sb;
    XmScrollBarWidget   	vert_sb;

    XmScrolledWindowWidget	scrolled_window;

    Pixmap			buffer;
    Boolean			redraw_buffer;

    Boolean			col_resize_area;
    Boolean			col_resize_underway;
    int 			col_resize_num;
    Position			col_resize_x;
    Cursor 			resize_cursor;

    int*			selected_row;
    int				selected_row_count;
    Boolean*			row_is_selected;
    int				anchor;
    int				last_selected_row;
    XtIntervalId		scroll_timer_id;
    Boolean			scroll_timer_set;
    int				scroll_direction;
    Boolean 			selection_drag;

    Time 			click_time;
    int				click_count;
    int				click_interval;
} XmAxyListPart;

typedef struct _XmAxyListRec {
    CorePart core;
    XmPrimitivePart primitive;
    XmAxyListPart list;
} XmAxyListRec;


#ifdef __cplusplus
}
#endif

#endif /* XM_AXY_LIST_P_H */
