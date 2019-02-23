/* Copyright (c) 1999   Alexander Yukhimets. All rights reserved. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <X11/StringDefs.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include <Xm/DrawingA.h>

#include <XmAxy/NotebookP.h>
#include <XmAxy/Util.h>

#include <Xm/XmP.h>
#include <Xm/DrawP.h>

#define X(w)            (((CoreWidget)(w))->core.x)
#define Y(w)            (((CoreWidget)(w))->core.y)
#define WIDTH(w)        (((CoreWidget)(w))->core.width)
#define HEIGHT(w)       (((CoreWidget)(w))->core.height)
#define DEPTH(w)        (((CoreWidget)(w))->core.depth)
#define BORDER(w)       (((CoreWidget)(w))->core.border_width)
#define COLORMAP(w)     (((CoreWidget)(w))->core.colormap)
#define BACKGROUND(w)   (((CoreWidget)(w))->core.background_pixel)

#define NUM_CHILDREN(w)	(((CompositeWidget)(w))->composite.num_children)
#define CHILD(w,i)      (((CompositeWidget)(w))->composite.children[(i)])

#define CONSTRAINTS(w)  (((CoreWidget)(w))->core.constraints)

#define HAS_FOCUS(w)         (((XmManagerWidget)(w))->manager.has_focus)
#define SHADOW_THICKNESS(w)  (((XmManagerWidget)(w))->manager.shadow_thickness)
#define STRING_DIR(w)        (((XmManagerWidget)(w))->manager.string_direction)
#define FOREGROUND(w)        (((XmManagerWidget)(w))->manager.foreground)
#define HIGHLIGHT_COLOR(w)   (((XmManagerWidget)(w))->manager.highlight_color)
#define HIGHLIGHT_PIXMAP(w)  (((XmManagerWidget)(w))->manager.highlight_pixmap)
#define HIGHLIGHT_GC(w)      (((XmManagerWidget)(w))->manager.highlight_GC)
#define BACKGROUND_GC(w)     (((XmManagerWidget)(w))->manager.background_GC)
#define TOP_SHADOW_GC(w)     (((XmManagerWidget)(w))->manager.top_shadow_GC)
#define BOTTOM_SHADOW_GC(w)  (((XmManagerWidget)(w))->manager.bottom_shadow_GC)


#define ARM_COLOR(w)        (((XmAxyNotebookWidget)(w))->notebook.arm_color)
#define FONT_LIST(w)        (((XmAxyNotebookWidget)(w))->notebook.font_list)
#define SHOW_TABS(w)        (((XmAxyNotebookWidget)(w))->notebook.show_tabs)
#define HIGHLIGHT_THICKNESS(w) \
  (((XmAxyNotebookWidget)(w))->notebook.highlight_thickness)
#define MARGIN_WIDTH(w)     (((XmAxyNotebookWidget)(w))->notebook.margin_width)
#define MARGIN_HEIGHT(w)    (((XmAxyNotebookWidget)(w))->notebook.margin_height)
#define INNER_MARGIN_WIDTH(w) \
  (((XmAxyNotebookWidget)(w))->notebook.inner_margin_width)
#define INNER_MARGIN_HEIGHT(w) \
  (((XmAxyNotebookWidget)(w))->notebook.inner_margin_height)
#define TAB_SPACING(w)      (((XmAxyNotebookWidget)(w))->notebook.tab_spacing)
#define TAB_RAISE(w)        (((XmAxyNotebookWidget)(w))->notebook.tab_raise)
#define TAB_MARGIN_WIDTH(w) \
  (((XmAxyNotebookWidget)(w))->notebook.tab_margin_width)
#define TAB_MARGIN_HEIGHT(w) \
  (((XmAxyNotebookWidget)(w))->notebook.tab_margin_height)
#define LABEL_MARGIN_WIDTH(w) \
  (((XmAxyNotebookWidget)(w))->notebook.label_margin_width)
#define LABEL_MARGIN_HEIGHT(w) \
  (((XmAxyNotebookWidget)(w))->notebook.label_margin_height)
#define PAGE_CHANGED_CB(w) \
  (((XmAxyNotebookWidget)(w))->notebook.page_changed_callback)

#define FONT_FID(w)       (((XmAxyNotebookWidget)(w))->notebook.font_fid)
#define FONT_WIDTH(w)     (((XmAxyNotebookWidget)(w))->notebook.font_width)
#define FONT_HEIGHT(w)    (((XmAxyNotebookWidget)(w))->notebook.font_height)
#define FONT_Y(w)         (((XmAxyNotebookWidget)(w))->notebook.font_y)
#define DRAWING_AREA(w)   (((XmAxyNotebookWidget)(w))->notebook.drawing_area)
#define HIGHLIGHTED_TAB(w) \
  (((XmAxyNotebookWidget)(w))->notebook.highlighted_tab)
#define HIGHLIGHT_DRAWN(w) \
  (((XmAxyNotebookWidget)(w))->notebook.highlight_drawn)
#define CURRENT_WIDGET(w) (((XmAxyNotebookWidget)(w))->notebook.current_widget)
#define CURRENT_PAGE(w)   (((XmAxyNotebookWidget)(w))->notebook.current_page)
#define NUM_PAGES(w)      (((XmAxyNotebookWidget)(w))->notebook.num_pages)
#define TAB_HEIGHT(w)     (((XmAxyNotebookWidget)(w))->notebook.tab_height)
#define CHILD_X(w)        (((XmAxyNotebookWidget)(w))->notebook.child_x)
#define CHILD_Y(w)        (((XmAxyNotebookWidget)(w))->notebook.child_y)
#define CHILD_WIDTH(w)    (((XmAxyNotebookWidget)(w))->notebook.child_width)
#define CHILD_HEIGHT(w)	  (((XmAxyNotebookWidget)(w))->notebook.child_height)
#define NORMAL_GC(w)      (((XmAxyNotebookWidget)(w))->notebook.normal_gc)
#define ARM_GC(w)         (((XmAxyNotebookWidget)(w))->notebook.arm_gc)
#define TAB_CHILD(w)      (((XmAxyNotebookWidget)(w))->notebook.tab_child)

#define CHILD_CONSTR(w,i) CONSTRAINTS(CHILD(w,i))
#define C_TAB_LABEL(r) (((XmAxyNotebookConstraintRec*)r)->notebook.tab_label)
#define C_RESIZABLE(r) (((XmAxyNotebookConstraintRec*)r)->notebook.resizable)
#define C_TAB_RECT(r)  (((XmAxyNotebookConstraintRec*)r)->notebook.tab_rect)
#define C_PAGE_NUM(r)  (((XmAxyNotebookConstraintRec*)r)->notebook.page_number)
#define C_IS_TAB(r)    (((XmAxyNotebookConstraintRec*)r)->notebook.is_tab)
#define C_TAB_CLIP(r)  (((XmAxyNotebookConstraintRec*)r)->notebook.tab_clip)
#define C_TAB_VISIBLE(r) \
  (((XmAxyNotebookConstraintRec*)r)->notebook.tab_visible)

#define TAB_LABEL_I(w,i)   C_TAB_LABEL(CHILD_CONSTR(w,i))
#define RESIZABLE_I(w,i)   C_RESIZABLE(CHILD_CONSTR(w,i))
#define TAB_RECT_I(w,i)    C_TAB_RECT(CHILD_CONSTR(w,i))
#define PAGE_NUM_I(w,i)    C_PAGE_NUM(CHILD_CONSTR(w,i))
#define IS_TAB_I(w,i)      C_IS_TAB(CHILD_CONSTR(w,i))
#define TAB_CLIP_I(w,i)    C_TAB_CLIP(CHILD_CONSTR(w,i))
#define TAB_VISIBLE_I(w,i) C_TAB_VISIBLE(CHILD_CONSTR(w,i))

#define TAB_LABEL(c)       C_TAB_LABEL(CONSTRAINTS(c))
#define RESIZABLE(c)       C_RESIZABLE(CONSTRAINTS(c))
#define TAB_RECT(c)        C_TAB_RECT(CONSTRAINTS(c))
#define PAGE_NUM(c)        C_PAGE_NUM(CONSTRAINTS(c))
#define IS_TAB(c)          C_IS_TAB(CONSTRAINTS(c))
#define TAB_CLIP(c)        C_TAB_CLIP(CONSTRAINTS(c))
#define TAB_VISIBLE(c)     C_TAB_VISIBLE(CONSTRAINTS(c))

#define DEFAULT_FONT_LIST(w) _XmGetDefaultFontList((Widget)w, XmFONT_IS_FONT)
#define EHT(w) \
  ((HIGHLIGHT_THICKNESS(w)<=0)?(Dimension)0:(HIGHLIGHT_THICKNESS(w)+1))


/* class methods */
static void Initialize(Widget,Widget,ArgList,Cardinal*);
static void Destroy(Widget);
static void Resize(Widget);
static void Redisplay(Widget, XEvent*, Region);
static Boolean SetValues(Widget,Widget, Widget,ArgList,Cardinal*);
static XtGeometryResult QueryGeometry(Widget,XtWidgetGeometry*,
                                    XtWidgetGeometry*);

static void ChangeManaged(Widget);
static XtGeometryResult GeometryManager(Widget,XtWidgetGeometry*,
                                    XtWidgetGeometry*);

static void ConstraintInitialize(Widget,Widget,ArgList,Cardinal*);
static void ConstraintDestroy(Widget);
static Boolean ConstraintSetValues(Widget,Widget, Widget,ArgList,Cardinal*);

/* action routines */
static void DAFocusIn(Widget,XEvent*,String*,Cardinal*);
static void DAFocusOut(Widget,XEvent*,String*,Cardinal*);
static void DAMouseDown(Widget,XEvent*,String*,Cardinal*);
static void DAMoveLeft(Widget,XEvent*,String*,Cardinal*);
static void DAMoveRight(Widget,XEvent*,String*,Cardinal*);
static void DASelect(Widget,XEvent*,String*,Cardinal*);

/* local functions */
static void install_font(Widget);
static void create_gc(Widget);
static void calc_preferred_size(Widget,Dimension*,Dimension*,Widget);
static void do_layout(Widget,Widget);
static void compute_tab_rects(Widget);
static void configure_children(Widget,Widget);
static void draw_shadows(Widget,Region);
static void draw_da_tabs(Widget,Window,XEvent*);
static void draw_da_highlight(Widget);
static Widget search_da_tab(Widget, XEvent*);
static void expose_da_cb(Widget,XtPointer,XtPointer);
static void change_tab(Widget, Widget);

static XtActionsRec actions[] = {
  {"XmAxyNotebookDAMouseDown",DAMouseDown},
  {"XmAxyNotebookDAFocusIn",DAFocusIn},
  {"XmAxyNotebookDAFocusOut",DAFocusOut},
  {"XmAxyNotebookDAMoveRight",DAMoveRight},
  {"XmAxyNotebookDAMoveLeft",DAMoveLeft},
  {"XmAxyNotebookDASelect",DASelect},
};

static char drawingAreaTranslations[]="\
<FocusIn>:		XmAxyNotebookDAFocusIn()\n\
<FocusOut>:		XmAxyNotebookDAFocusOut()\n\
<Btn1Down>:		XmAxyNotebookDAMouseDown()\n\
<Btn1Down>(2+):		XmAxyNotebookDAMouseDown()\n\
:<Key>osfRight:		XmAxyNotebookDAMoveRight()\n\
:<Key>osfLeft:		XmAxyNotebookDAMoveLeft()\n\
:<Key>osfDown:		XmAxyNotebookDAMoveRight()\n\
:<Key>osfUp:		XmAxyNotebookDAMoveLeft()\n\
<Key>Return:		XmAxyNotebookDASelect()\n\
<Key>space:		XmAxyNotebookDASelect()\n\
";

#define Offset(field) XtOffsetOf(XmAxyNotebookRec, notebook.field)
#define offset(field) XtOffsetOf(XmAxyNotebookRec, field)
static XtResource resources[]={

  {XmNselectColor,XmCSelectColor,
    XmRPixel,sizeof(Pixel),
    Offset(arm_color),XmRImmediate,
    (XtPointer)XmUNSPECIFIED_PIXEL},

  {XmNfontList,XmCFontList,
    XmRFontList,sizeof(XmFontList),
    Offset(font_list),XmRImmediate,
    (XtPointer)NULL},

  {XmNshowTabs,XmCShowTabs,
    XmRBoolean,sizeof(Boolean),
    Offset(show_tabs),XmRImmediate,
    (XtPointer)TRUE},

  {XmNhighlightThickness,XmCHighlightThickness,
    XmRHorizontalDimension,sizeof(Dimension),
    Offset(highlight_thickness),XmRImmediate,
    (XtPointer)1},

  {XmNinnerMarginHeight,XmCInnerMarginHeight,
    XmRVerticalDimension,sizeof(Dimension),
    Offset(inner_margin_height),XmRImmediate,
    (XtPointer)4},

  {XmNinnerMarginWidth,XmCInnerMarginWidth,
    XmRHorizontalDimension,sizeof(Dimension),
    Offset(inner_margin_width),XmRImmediate,
    (XtPointer)4},

  {XmNmarginHeight,XmCMarginHeight,
    XmRVerticalDimension,sizeof(Dimension),
    Offset(margin_height),XmRImmediate,
    (XtPointer)6},

  {XmNmarginWidth,XmCMarginWidth,
    XmRHorizontalDimension,sizeof(Dimension),
    Offset(margin_width),XmRImmediate,
    (XtPointer)6},

  {XmNpageChangedCallback,XmCPageChangedCallback,
    XtRCallback,sizeof(XtPointer),
    Offset(page_changed_callback), XtRImmediate,
    (XtPointer)NULL},

  {XmNtabRaise,XmCTabRaise,
    XmRVerticalDimension,sizeof(Dimension),
    Offset(tab_raise), XtRImmediate,
    (XtPointer)2},

  {XmNtabSpacing,XmCTabSpacing,
    XmRHorizontalDimension,sizeof(Dimension),
    Offset(tab_spacing),XmRImmediate,
    (XtPointer)0},

  {XmNtabMarginWidth,XmCTabMarginWidth,
    XmRHorizontalDimension,sizeof(Dimension),
    Offset(tab_margin_width),XmRImmediate,
    (XtPointer)2},

  {XmNtabMarginHeight,XmCTabMarginHeight,
    XmRVerticalDimension,sizeof(Dimension),
    Offset(tab_margin_height),XmRImmediate,
    (XtPointer)2},

  {XmNlabelMarginWidth,XmCLabelMarginWidth,
    XmRHorizontalDimension,sizeof(Dimension),
    Offset(label_margin_width),XmRImmediate,
    (XtPointer)2},

  {XmNlabelMarginHeight,XmCLabelMarginHeight,
    XmRVerticalDimension,sizeof(Dimension),
    Offset(label_margin_height),XmRImmediate,
    (XtPointer)2},

  {XmNshadowThickness,XmCShadowThickness,
    XmRHorizontalDimension,sizeof(Dimension),
    offset(manager.shadow_thickness),XmRImmediate,
    (XtPointer)2},

};

static XmSyntheticResource syn_resources[] = {

    {XmNhighlightThickness, sizeof(Dimension), Offset(highlight_thickness),
      _XmFromHorizontalPixels, _XmToHorizontalPixels},

    {XmNinnerMarginWidth, sizeof(Dimension), Offset(inner_margin_width),
      _XmFromHorizontalPixels, _XmToHorizontalPixels},

    {XmNinnerMarginHeight, sizeof(Dimension), Offset(inner_margin_height),
      _XmFromVerticalPixels, _XmToVerticalPixels},

    {XmNmarginWidth, sizeof(Dimension), Offset(margin_width),
      _XmFromHorizontalPixels, _XmToHorizontalPixels},

    {XmNmarginHeight, sizeof(Dimension), Offset(margin_height),
      _XmFromVerticalPixels, _XmToVerticalPixels},

    {XmNtabRaise, sizeof(Dimension), Offset(tab_raise),
      _XmFromVerticalPixels, _XmToVerticalPixels},

    {XmNtabMarginWidth, sizeof(Dimension), Offset(tab_margin_width),
      _XmFromHorizontalPixels, _XmToHorizontalPixels},

    {XmNtabMarginHeight, sizeof(Dimension), Offset(tab_margin_height),
      _XmFromVerticalPixels, _XmToVerticalPixels},

    {XmNlabelMarginWidth, sizeof(Dimension), Offset(label_margin_width),
      _XmFromHorizontalPixels, _XmToHorizontalPixels},

    {XmNlabelMarginHeight, sizeof(Dimension), Offset(label_margin_height),
      _XmFromVerticalPixels, _XmToVerticalPixels},

};      
#undef offset
#undef Offset

static XtResource constraints[]={

  {XmNtabLabel,XmCTabLabel,
    XmRXmString,sizeof(XmString),
    XtOffsetOf(XmAxyNotebookConstraintRec, notebook.tab_label),
    XmRImmediate,(XtPointer)NULL},

  {XmNresizable,XmCResizable,
    XmRBoolean,sizeof(Boolean),
    XtOffsetOf(XmAxyNotebookConstraintRec, notebook.resizable),
    XmRImmediate,(XtPointer)TRUE}

};


externaldef (xmaxynotebookclassrec) 
            XmAxyNotebookClassRec xmAxyNotebookClassRec={
    /* Core class part */
    {
      /* superclass */                  (WidgetClass)&xmManagerClassRec,
      /* class_name */                  "XmAxyNotebook",
      /* widget_size */                 sizeof(XmAxyNotebookRec),
      /* class_initialize */            NULL,
      /* class_part_initialize */       NULL,
      /* class_inited */                FALSE,
      /* initialize */                  Initialize,
      /* initialize_hook */             NULL,
      /* realize */                     XtInheritRealize,
      /* actions */                     actions,
      /* num_actions */                 XtNumber(actions),
      /* resources */                   resources,
      /* num_resources */               XtNumber(resources),
      /* xrm_class */                   NULLQUARK,
      /* compress_motion */             TRUE,
      /* compress_exposure */           XtExposeCompressMaximal,
      /* compress_enterleave */         TRUE,
      /* visible_interest */            FALSE,
      /* destroy */                     Destroy,
      /* resize */                      Resize,
      /* expose */                      Redisplay,
      /* set_values */                  SetValues,
      /* set_values_hook */             NULL,
      /* set_values_almost */           XtInheritSetValuesAlmost,
      /* get_values_hook */             NULL,
      /* accept_focus */                NULL,
      /* version */                     XtVersion,
      /* callback_private */            NULL,
      /* tm_table */                    XtInheritTranslations,
      /* query_geometry */              QueryGeometry,
      /* display_accelerator */         NULL,
      /* extension */                   NULL,
    },
    /* Composite Class part */
    {
      /* geometry_manager */            GeometryManager,
      /* change_managed */              ChangeManaged,
      /* insert_child */                XtInheritInsertChild,
      /* delete_child */                XtInheritDeleteChild,
      /* extension */                   NULL,
    },
    /* Constraint Class part */
    {
      /* resources */                   constraints,
      /* num_resources */               XtNumber(constraints),
      /* constraint_size */             sizeof(XmAxyNotebookConstraintRec),
      /* initialize */                  ConstraintInitialize,
      /* destroy */                     ConstraintDestroy,
      /* set_values */                  ConstraintSetValues,
      /* extension */                   NULL,
    },
    /* Manager Class part */
    {
      /* translations */                XtInheritTranslations,
      /* syn_resources */               syn_resources,
      /* num_syn_resources */           XtNumber(syn_resources),
      /* syn_constraint_resources */    NULL,
      /* num_syn_constraint_resources */0,
      /* parent_process */              XmInheritParentProcess,
      /* extension */                   NULL,
    },
    /* Notebook Class part */
    {
      /* extension */                   NULL,
    }
};

externaldef (xmaxynotebookwidgetclass) 
  WidgetClass xmAxyNotebookWidgetClass=(WidgetClass) &xmAxyNotebookClassRec;



/* Core class methods */

static void Initialize(Widget wreq,Widget wnew,ArgList args,Cardinal *nargs){
  Arg arg[10];
  Cardinal n;

  if(ARM_COLOR(wnew)==XmUNSPECIFIED_PIXEL){
    Pixel fg,ts,bs;
    XmGetColors(XtScreen(wnew),COLORMAP(wnew),BACKGROUND(wnew),
	&fg,&ts,&bs,&ARM_COLOR(wnew));
  }
  if(FONT_LIST(wnew)==NULL){
    FONT_LIST(wnew)=XmFontListCopy(
	_XmGetDefaultFontList(wnew,XmLABEL_FONTLIST));
  } else {
    FONT_LIST(wnew)=XmFontListCopy(FONT_LIST(wnew));
  }
  install_font(wnew);
  CURRENT_WIDGET(wnew)=NULL;
  CURRENT_PAGE(wnew)=0;
  NUM_PAGES(wnew)=0;
  TAB_HEIGHT(wnew)=0;
  CHILD_WIDTH(wnew)=0;
  CHILD_HEIGHT(wnew)=0;

  HIGHLIGHTED_TAB(wnew)=0;
  HIGHLIGHT_DRAWN(wnew)=FALSE;

  create_gc(wnew);

  TAB_CHILD(wnew)=TRUE;
  n=0;
  XtSetArg(arg[n],XmNbackground,BACKGROUND(wnew));n++;
  DRAWING_AREA(wnew)=XtCreateWidget("Tabs",xmDrawingAreaWidgetClass,
      wnew,arg,n);

  XtOverrideTranslations(DRAWING_AREA(wnew),
      XtParseTranslationTable(drawingAreaTranslations));
  if(SHOW_TABS(wnew))XtManageChild(DRAWING_AREA(wnew));
  XtAddCallback(DRAWING_AREA(wnew),XmNexposeCallback,
      expose_da_cb,(XtPointer)wnew);
  XtAddCallback(DRAWING_AREA(wnew),XmNresizeCallback,
      expose_da_cb,(XtPointer)wnew);

  XtVaSetValues(wnew,XmNinitialFocus,DRAWING_AREA(wnew),NULL);
}

static void Destroy(Widget w){
  XmFontListFree(FONT_LIST(w));
  XtReleaseGC(w,NORMAL_GC(w));
  XtReleaseGC(w,ARM_GC(w));
}

static void Resize(Widget w){
  do_layout(w,NULL);
  draw_shadows(w,NULL);
}

static void Redisplay(Widget w, XEvent *event, Region region){
  draw_shadows(w,region);
  _XmRedisplayGadgets(w,event,region);
}

#define NE(field) \
(((XmAxyNotebookWidget)wcur)->field != ((XmAxyNotebookWidget)wnew)->field)
#define NB(field) \
((((XmAxyNotebookWidget)wcur)->field && !((XmAxyNotebookWidget)wnew)->field) ||\
(!((XmAxyNotebookWidget)wcur)->field && ((XmAxyNotebookWidget)wnew)->field))
static Boolean SetValues(Widget wcur,Widget wreq, Widget wnew,
				ArgList args,Cardinal *nargs){
  Boolean redraw=FALSE;
  Boolean layout=FALSE;

  if(NE(notebook.arm_color) || NE(manager.foreground) || 
					NE(core.background_pixel)){
    XtReleaseGC(wnew,NORMAL_GC(wnew));
    XtReleaseGC(wnew,ARM_GC(wnew));
    create_gc(wnew);
    XtVaSetValues(DRAWING_AREA(wnew),XmNbackground,BACKGROUND(wnew),NULL);
    redraw=TRUE;
  }
  if(NE(notebook.font_list)){
    XmFontListFree(FONT_LIST(wcur));
    install_font(wnew);
    layout=TRUE;
    redraw=TRUE;
  }
  if(NE(notebook.tab_raise)){
    layout=TRUE;
    redraw=TRUE;
  }
  if(NB(notebook.show_tabs)){
    layout=TRUE;
    redraw=TRUE;
    if(!SHOW_TABS(wnew))XtUnmanageChild(DRAWING_AREA(wnew));
    else XtManageChild(DRAWING_AREA(wnew));
  }
  if(NE(notebook.highlight_thickness) || NE(notebook.margin_width) ||
     NE(notebook.margin_height) || NE(notebook.inner_margin_height) ||
     NE(notebook.inner_margin_width) || NE(notebook.tab_spacing) ||
     NE(manager.shadow_thickness) || NE(manager.string_direction)){
    layout=TRUE;
    redraw=TRUE;
  }

  if(layout) {
    if(HEIGHT(wcur)==HEIGHT(wnew) || WIDTH(wcur)==WIDTH(wnew)){
      Dimension width,height;
      calc_preferred_size(wnew,&width,&height,NULL);
      if(HEIGHT(wcur)==HEIGHT(wnew)) HEIGHT(wnew)=height;
      if(WIDTH(wcur)==WIDTH(wnew)) WIDTH(wnew)=width;
    }
    if(HEIGHT(wcur)==HEIGHT(wnew) && WIDTH(wcur)==WIDTH(wnew)){
      do_layout(wnew,NULL);
      redraw=TRUE;
    }
  }

  return redraw;
}
#undef NE
#undef NB

static XtGeometryResult QueryGeometry(Widget w, 
			XtWidgetGeometry *proposed,XtWidgetGeometry *answer){
  Dimension width,height;

  calc_preferred_size(w,&width,&height,NULL);
  if(!XtIsRealized(w)){
    if(WIDTH(w)!=0) width=WIDTH(w);
    if(HEIGHT(w)!=0) height=HEIGHT(w);
  }
  if(answer!=NULL){
    answer->request_mode=(CWWidth | CWHeight);
    answer->width=width;
    answer->height=height;
  }

  if(((proposed->request_mode & (CWWidth | CWHeight)) == (CWWidth | CWHeight))
    && proposed->width==width && proposed->height==height)
    return XtGeometryYes;
  else if(width == WIDTH(w) && height == HEIGHT(w))
    return XtGeometryNo;
  else
    return XtGeometryAlmost;
}


/* Composite methods */

static XtGeometryResult GeometryManager(Widget child, 
			XtWidgetGeometry *request,XtWidgetGeometry *reply){
  Widget w=XtParent(child);
  Dimension width,height,saved_width,saved_height;
  XtWidgetGeometry parent;
  XtGeometryResult result;

  if(!RESIZABLE(child)) 
    return XtGeometryNo;


  if((request->request_mode & CWX) || (request->request_mode & CWY))
    return XtGeometryNo;

  saved_width=WIDTH(child);
  saved_height=HEIGHT(child);
  WIDTH(child)=request->width;
  HEIGHT(child)=request->height;
  calc_preferred_size(w,&width,&height,child);
  WIDTH(child)=saved_width;
  HEIGHT(child)=saved_height;

  if(width==WIDTH(w) && height==HEIGHT(w))
    return XtGeometryYes;

  parent.request_mode=(CWWidth|CWHeight);
  parent.width=width;
  parent.height=height;
  if(request->request_mode & XtCWQueryOnly){
    parent.request_mode |= XtCWQueryOnly;
  }
  result=XtMakeGeometryRequest(XtParent(w),&parent,NULL);
  if(result==XtGeometryAlmost) result=XtGeometryNo;

  if(result==XtGeometryYes && !(request->request_mode & XtCWQueryOnly)){
    WIDTH(child)=request->width;
    HEIGHT(child)=request->height;
    do_layout(w,child);
  } else {
    do_layout(w,child);
  }

  return result;
}

static void ChangeManaged(Widget w){
  Dimension width,height;
  int i,page=0;

  for(i=0;i<NUM_CHILDREN(w);i++){
    if(CHILD(w,i)==DRAWING_AREA(w)) continue;
    if(XtIsManaged(CHILD(w,i))) {
      PAGE_NUM_I(w,i)=(++page);
      if(CURRENT_WIDGET(w)==CHILD(w,i)) CURRENT_PAGE(w)=page;
      else if(page>0 && CURRENT_PAGE(w)==page) CURRENT_WIDGET(w)=CHILD(w,i);
    }
  }
  if(CURRENT_WIDGET(w)==NULL || PAGE_NUM(CURRENT_WIDGET(w))!=CURRENT_PAGE(w)){
    for(i=0;i<NUM_CHILDREN(w);i++){
      if(CHILD(w,i)==DRAWING_AREA(w)) continue;
      if(XtIsManaged(CHILD(w,i))){
	CURRENT_PAGE(w)=PAGE_NUM_I(w,i);
	CURRENT_WIDGET(w)=CHILD(w,i);
	break;
      }
    }
  }
  NUM_PAGES(w)=page;
  calc_preferred_size(w,&width,&height,NULL);
  if(!XtIsRealized(w)) {
    if(WIDTH(w)!=0) width=WIDTH(w);
    if(HEIGHT(w)!=0) height=HEIGHT(w);
  }
  while(XtMakeResizeRequest(w,width,height,&width,&height)==XtGeometryAlmost);
  do_layout(w,NULL);
  _XmNavigChangeManaged(w);
}


/* Constraint class methods */

static void ConstraintInitialize(Widget treq,Widget tnew,
			ArgList args,Cardinal *nargs){
  Widget w=XtParent(tnew);

  if(TAB_CHILD(w)){
    TAB_LABEL(tnew)=NULL;
    PAGE_NUM(tnew)=0;
    IS_TAB(tnew)=TRUE;
    TAB_CHILD(w)=FALSE;
  } else {
    if(TAB_LABEL(tnew)){
      TAB_LABEL(tnew)=XmStringCopy(TAB_LABEL(tnew));
    } else {
      TAB_LABEL(tnew)=XmStringCreateLocalized(XtName(tnew));
    }
    PAGE_NUM(tnew)=0;
    IS_TAB(tnew)=FALSE;
  }
}

static void ConstraintDestroy(Widget w){
  if(TAB_LABEL(w))XmStringFree(TAB_LABEL(w));
}

static Boolean ConstraintSetValues(Widget wcur,Widget wreq, Widget wnew,
                                          ArgList args,Cardinal *nargs){
  Boolean redraw = FALSE; 
  Boolean layout = FALSE; 
  Widget w=XtParent(wnew);

  if(!XmStringCompare(TAB_LABEL(wcur),TAB_LABEL(wnew))){
    XmStringFree(TAB_LABEL(wcur));
    TAB_LABEL(wnew)=XmStringCopy(TAB_LABEL(wnew));
    layout=TRUE;
    redraw = TRUE;
  }

  if(layout) {
    Dimension width,height;
    calc_preferred_size(w,&width,&height,NULL);
    HEIGHT(wnew)=height;
    WIDTH(wnew)=width;
    do_layout(wnew,NULL);
    redraw=TRUE;
  }

  return(redraw);
}


/* Actions */

static void DAFocusIn(Widget da,XEvent *event,
                        String *params,Cardinal *nparams){
  Widget w=XtParent(da);
  if(!CURRENT_WIDGET(w)) return;
  HIGHLIGHTED_TAB(w)=CURRENT_WIDGET(w);
  HIGHLIGHT_DRAWN(w)=TRUE;
  draw_da_highlight(da);
}

static void DAFocusOut(Widget da,XEvent *event,
                        String *params,Cardinal *nparams){
  Widget w=XtParent(da);
  if(!HIGHLIGHTED_TAB(w) || !HIGHLIGHT_DRAWN(w)) return;
  HIGHLIGHT_DRAWN(w)=FALSE;
  draw_da_highlight(da);
  HIGHLIGHTED_TAB(w)=0;
}

static void DAMouseDown(Widget da,XEvent *event,
                        String *params,Cardinal *nparams){
  Widget w=XtParent(da);
  Widget child=search_da_tab(da,event);
  XmAxyNotebookCallbackStruct cbs;
  XmString prev,curr;

  if(!child) return;
  XmProcessTraversal(da,XmTRAVERSE_CURRENT);
  if(child==CURRENT_WIDGET(w)) return;

  cbs.reason=XmAxyNotebookPageChangedReason;
  cbs.event=event;

  cbs.prev_page_number=CURRENT_PAGE(w);
  cbs.prev_page_widget=CURRENT_WIDGET(w);
  cbs.prev_tab_label=prev=XmStringCopy(TAB_LABEL(CURRENT_WIDGET(w)));

  change_tab(w,child);

  cbs.page_number=CURRENT_PAGE(w);
  cbs.page_widget=CURRENT_WIDGET(w);
  cbs.tab_label=curr=XmStringCopy(TAB_LABEL(CURRENT_WIDGET(w)));

  XtCallCallbackList(w,PAGE_CHANGED_CB(w),&cbs);

  XmStringFree(prev);
  XmStringFree(curr);
}

static void DAMoveLeft(Widget da,XEvent *event,
                        String *params,Cardinal *nparams){
  Widget w=XtParent(da);
  int i,page;

  if(!HIGHLIGHTED_TAB(w)) return;

  if(PAGE_NUM(HIGHLIGHTED_TAB(w))==1) page=NUM_PAGES(w);
  else page=PAGE_NUM(HIGHLIGHTED_TAB(w))-1;

  for(i=0;i<NUM_CHILDREN(w);i++){
    if(PAGE_NUM_I(w,i)==page){
      if(HIGHLIGHT_DRAWN(w) && HIGHLIGHTED_TAB(w)){
	HIGHLIGHT_DRAWN(w)=FALSE;
	draw_da_highlight(DRAWING_AREA(w));
      }
      HIGHLIGHTED_TAB(w)=CHILD(w,i);
      HIGHLIGHT_DRAWN(w)=TRUE;
      draw_da_highlight(DRAWING_AREA(w));
      return;
    }
  }
}

static void DAMoveRight(Widget da,XEvent *event,
                        String *params,Cardinal *nparams){
  Widget w=XtParent(da);
  int i,page;

  if(!HIGHLIGHTED_TAB(w)) return;

  if(PAGE_NUM(HIGHLIGHTED_TAB(w))==NUM_PAGES(w)) page=1;
  else page=PAGE_NUM(HIGHLIGHTED_TAB(w))+1;

  for(i=0;i<NUM_CHILDREN(w);i++){
    if(/*TAB_VISIBLE_I(w,i) && */PAGE_NUM_I(w,i)==page){
      if(HIGHLIGHT_DRAWN(w) && HIGHLIGHTED_TAB(w)){
	HIGHLIGHT_DRAWN(w)=FALSE;
	draw_da_highlight(DRAWING_AREA(w));
      }
      HIGHLIGHTED_TAB(w)=CHILD(w,i);
      HIGHLIGHT_DRAWN(w)=TRUE;
      draw_da_highlight(DRAWING_AREA(w));
      return;
    }
  }
}

static void DASelect(Widget da,XEvent *event,
                        String *params,Cardinal *nparams){
  Widget w=XtParent(da);

  if(HIGHLIGHTED_TAB(w)) change_tab(w,HIGHLIGHTED_TAB(w));
}


/* public methods */

extern void XmAxyNotebookSetCurrentPage(Widget w,int page,Boolean notify){
  int i;
#ifdef XTHREADS
  XtAppContext app=XtWidgetToApplicationContext(w);
  XtAppLock(app);
#endif

  if(page<=0) {
#ifdef XTHREADS
    XtAppUnlock(app);
#endif
    return;
  }

  for(i=0;i<NUM_CHILDREN(w);i++){
    if(XtIsManaged(CHILD(w,i)) && PAGE_NUM_I(w,i)==page) {
      XmAxyNotebookSetCurrentWidget(w,CHILD(w,i),notify);
#ifdef XTHREADS
      XtAppUnlock(app);
#endif
      return;
    }
  }

  if(!XtIsRealized(w)){
    CURRENT_PAGE(w)=page;
    CURRENT_WIDGET(w)=NULL;
#ifdef XTHREADS
    XtAppUnlock(app);
#endif
    return;
  }
#ifdef XTHREADS
  XtAppUnlock(app);
#endif
}

extern Widget XmAxyNotebookGetCurrentWidget(Widget w){
  Widget cw;
#ifdef XTHREADS
  XtAppContext app=XtWidgetToApplicationContext(w);
  XtAppLock(app);
#endif

  cw=CURRENT_WIDGET(w);

#ifdef XTHREADS
  XtAppUnlock(app);
#endif

  return cw;
}

extern void XmAxyNotebookSetCurrentWidget(Widget w,Widget child,Boolean notify){
  XmAxyNotebookCallbackStruct cbs;
  XmString prev=NULL,curr;
#ifdef XTHREADS
  XtAppContext app=XtWidgetToApplicationContext(w);
  XtAppLock(app);
#endif

  if(XtParent(child)!=w || !XtIsManaged(child)) {
#ifdef XTHREADS
    XtAppUnlock(app);
#endif
    return;
  }

  if(!XtIsRealized(w)){
    CURRENT_WIDGET(w)=child;
    CURRENT_PAGE(w)=0;
#ifdef XTHREADS
    XtAppUnlock(app);
#endif
    return;
  }

  if(notify){
    cbs.reason=XmAxyNotebookPageChangedReason;
    cbs.event=NULL;

    cbs.prev_page_number=CURRENT_PAGE(w);
    cbs.prev_page_widget=CURRENT_WIDGET(w);
    cbs.prev_tab_label=prev=XmStringCopy(TAB_LABEL(CURRENT_WIDGET(w)));
  }

  change_tab(w,child);

  if(notify){
    cbs.page_number=CURRENT_PAGE(w);
    cbs.page_widget=CURRENT_WIDGET(w);
    cbs.tab_label=curr=XmStringCopy(TAB_LABEL(CURRENT_WIDGET(w)));

    XtCallCallbackList(w,PAGE_CHANGED_CB(w),&cbs);

    XmStringFree(prev);
    XmStringFree(curr);
  }

#ifdef XTHREADS
  XtAppUnlock(app);
#endif
}

extern int XmAxyNotebookGetCurrentPage(Widget w){
  int cp;
#ifdef XTHREADS
  XtAppContext app=XtWidgetToApplicationContext(w);
  XtAppLock(app);
#endif

  cp=CURRENT_PAGE(w);

#ifdef XTHREADS
  XtAppUnlock(app);
#endif

  return cp;
}

extern int XmAxyNotebookGetNumberOfPages(Widget w){
  int tp;
#ifdef XTHREADS
  XtAppContext app=XtWidgetToApplicationContext(w);
  XtAppLock(app);
#endif

  tp=NUM_PAGES(w);

#ifdef XTHREADS
  XtAppUnlock(app);
#endif

  return tp;
}

extern Widget XmAxyCreateNotebook(Widget parent,String name,ArgList args,
                                                                 Cardinal n){
  return XtCreateWidget(name,xmAxyNotebookWidgetClass,parent,args,n);
}


/* local functions */

static void install_font(Widget w){
  XmFontContext context;
  XFontStruct *font;
  XmFontListEntry font_list_entry;
  XmFontType type;
  XFontSetExtents *extents;
  XFontStruct **fonts;
  char **font_names;

  XmFontListInitFontContext(&context, FONT_LIST(w));
  font_list_entry = XmFontListNextEntry(context);
  font = (XFontStruct*)XmFontListEntryGetFont(font_list_entry, &type);
  if (type == XmFONT_IS_FONTSET){
      extents = XExtentsOfFontSet((XFontSet)font);
      FONT_WIDTH(w) = extents->max_logical_extent.width;
      FONT_HEIGHT(w) = extents->max_logical_extent.height;
      FONT_Y(w) = extents->max_logical_extent.y;
      XFontsOfFontSet((XFontSet)font, &fonts, &font_names);
      FONT_FID(w) = fonts[0]->fid;
  } else {
      FONT_WIDTH(w) = 
	(font->max_bounds.width + font->min_bounds.width) /2;
      FONT_HEIGHT(w) = 
	  (font->max_bounds.descent + font->max_bounds.ascent);
      FONT_Y(w) = font->max_bounds.ascent;
      FONT_FID(w) = font->fid;
  }
  XmFontListFreeFontContext(context);
}

static void create_gc(Widget w){
  XGCValues values;

 
  values.foreground = FOREGROUND(w);
  values.background = BACKGROUND(w);
  values.font=FONT_FID(w);

  NORMAL_GC(w) = XtAllocateGC(w,DEPTH(w),
   GCForeground|GCBackground|GCFont,&values,
   GCClipXOrigin|GCClipYOrigin|GCPlaneMask|GCForeground|GCBackground|GCFont,
   GCLineWidth|GCLineStyle|GCCapStyle|GCJoinStyle|GCGraphicsExposures|
   GCDashOffset|GCArcMode);

  values.foreground=ARM_COLOR(w);

  ARM_GC(w) = XtAllocateGC(w,DEPTH(w),
   GCForeground|GCBackground,&values,
   GCForeground|GCBackground|GCClipXOrigin|GCClipYOrigin|GCPlaneMask,
   GCLineWidth|GCLineStyle|GCCapStyle|GCJoinStyle|GCGraphicsExposures|
   GCDashOffset|GCArcMode);
   
}

static void calc_preferred_size(Widget w,Dimension *width,Dimension *height,
                                                                 Widget in){
  Dimension tab_height=0,tab_width=0,page_height=0,page_width=0;
  Dimension str_width,str_height;
  int i;
  Widget child;
  XtWidgetGeometry request,reply;

  for(i=0;i<NUM_CHILDREN(w);i++){
    child=CHILD(w,i);
    if(!XtIsManaged(child)) continue;

    if(SHOW_TABS(w)){
      XmStringExtent(FONT_LIST(w),TAB_LABEL(child),&str_width,&str_height);
      tab_width+=(Dimension)(str_width+SHADOW_THICKNESS(w)*2+
	  TAB_MARGIN_WIDTH(w)*2+(HIGHLIGHT_THICKNESS(w)+1)*2+
	  LABEL_MARGIN_WIDTH(w)*2+TAB_SPACING(w));
      if(str_height>tab_height) tab_height=str_height;
    }

    if(child==in){
      if(WIDTH(child)>page_width) page_width=WIDTH(child);
      if(HEIGHT(child)>page_height) page_height=HEIGHT(child);
    } else {
      request.request_mode=(CWWidth | CWHeight);
      request.width=0;
      request.height=0;
      XtQueryGeometry(child,NULL,&reply);

      if(!(reply.request_mode & CWWidth)) reply.width=WIDTH(child);
      if(!(reply.request_mode & CWHeight)) reply.height=HEIGHT(child);

      if(reply.width>page_width) page_width=reply.width;
      if(reply.height>page_height) page_height=reply.height;
    }
  }
  if(SHOW_TABS(w)){
    tab_height+=(Dimension)(SHADOW_THICKNESS(w)+TAB_MARGIN_HEIGHT(w)*2+
	EHT(w)*2+LABEL_MARGIN_HEIGHT(w)*2+TAB_RAISE(w));
    tab_width-=(Dimension)(TAB_SPACING(w));
  }
  tab_width=Max(tab_width,page_width+INNER_MARGIN_WIDTH(w)*2+
      SHADOW_THICKNESS(w)*2);
  *width=(Dimension)(tab_width+MARGIN_WIDTH(w)*2);
  *height=(Dimension)(page_height+SHADOW_THICKNESS(w)*2+tab_height+
    INNER_MARGIN_HEIGHT(w)*2+MARGIN_HEIGHT(w)*2);
}

static void do_layout(Widget w,Widget instigator){
  compute_tab_rects(w);
  configure_children(w,instigator);
}

static void compute_tab_rects(Widget w){
  Position left_x;
  Dimension greater_height,str_width,str_height;
  Widget child=0,prev=0;
  int i;
  Dimension width, height;

  left_x=0;
  greater_height = 0;

  if(SHOW_TABS(w)){
    TAB_RECT(DRAWING_AREA(w)).x=MARGIN_WIDTH(w);
    TAB_RECT(DRAWING_AREA(w)).y=MARGIN_HEIGHT(w);
    TAB_RECT(DRAWING_AREA(w)).width=i=WIDTH(w)-MARGIN_WIDTH(w)*2;
    if(i<=0) TAB_RECT(DRAWING_AREA(w)).width=1;

    for(i=0;i<NUM_CHILDREN(w);i++){
      child=CHILD(w,i);
      if(child==DRAWING_AREA(w)) continue;
      if(!XtIsManaged(child)) continue;

      XmStringExtent(FONT_LIST(w),TAB_LABEL(child),
	  &str_width,&str_height);
      width=(Dimension)(str_width+SHADOW_THICKNESS(w)*2+
	  TAB_MARGIN_WIDTH(w)*2+EHT(w)*2+LABEL_MARGIN_WIDTH(w)*2);
      height=(Dimension)(str_height+SHADOW_THICKNESS(w)+
	  TAB_MARGIN_HEIGHT(w)*2+EHT(w)*2+LABEL_MARGIN_HEIGHT(w)*2+TAB_RAISE(w));
      if(height>greater_height)greater_height = height;
      TAB_RECT(child).x=left_x;
      TAB_RECT(child).y=0;
      TAB_RECT(child).width = width;

      if(!prev || TAB_RECT(prev).x+TAB_RECT(prev).width<=TAB_RECT(child).x){
	TAB_CLIP(child).x=TAB_RECT(child).x;
	TAB_CLIP(child).y=TAB_RECT(child).y;
	TAB_CLIP(child).width=TAB_RECT(child).width;
      } else {
	TAB_CLIP(child).x=TAB_RECT(prev).x+TAB_RECT(prev).width;
	TAB_CLIP(child).y=TAB_RECT(child).y;
	TAB_CLIP(child).width=
	  TAB_RECT(child).x+TAB_RECT(child).width-TAB_CLIP(child).x;
      }

      if(TAB_RECT(child).x+TAB_RECT(child).width >
	  TAB_RECT(DRAWING_AREA(w)).width){
	TAB_VISIBLE(child)=FALSE;
      } else {
	TAB_VISIBLE(child)=TRUE;
      }

      left_x+=(Dimension)(width+TAB_SPACING(w)-SHADOW_THICKNESS(w));
      prev=child;
    }
    for(i=0;i<NUM_CHILDREN(w);i++){
      child=CHILD(w,i);
      if(child==DRAWING_AREA(w)) continue;
      if(!XtIsManaged(child)) continue;
      TAB_RECT(child).height=greater_height;
      TAB_CLIP(child).height=TAB_RECT(child).height;
      prev=child;
    }

    TAB_RECT(DRAWING_AREA(w)).height=greater_height;
    if(greater_height<=0) TAB_RECT(DRAWING_AREA(w)).height=1;
    if(prev)
      TAB_RECT(DRAWING_AREA(w)).width=Min(TAB_RECT(DRAWING_AREA(w)).width,
	  TAB_RECT(prev).x+TAB_RECT(prev).width);

  }

  TAB_HEIGHT(w)=greater_height;

  CHILD_WIDTH(w)=WIDTH(w)-MARGIN_WIDTH(w)*2-SHADOW_THICKNESS(w)*2-
    INNER_MARGIN_WIDTH(w)*2;

  CHILD_HEIGHT(w)=HEIGHT(w)-MARGIN_HEIGHT(w)*2-SHADOW_THICKNESS(w)*2-
    INNER_MARGIN_HEIGHT(w)*2-TAB_HEIGHT(w);

  CHILD_X(w)=MARGIN_WIDTH(w)+SHADOW_THICKNESS(w)+INNER_MARGIN_WIDTH(w);
  CHILD_Y(w)=MARGIN_HEIGHT(w)+TAB_HEIGHT(w)+SHADOW_THICKNESS(w)+
    INNER_MARGIN_HEIGHT(w);
}

static void configure_children(Widget w,Widget instigator){
  int i;
  Widget child;

  for(i=0;i<NUM_CHILDREN(w);i++){
    XRectangle *rect;

    child=CHILD(w,i);
    if(!XtIsManaged(child)) continue;

    if(child==DRAWING_AREA(w)){
      rect=&TAB_RECT(child);
      if(child==instigator){
	X(child)=rect->x;
	Y(child)=rect->y;
	WIDTH(child)=rect->width;
	HEIGHT(child)=rect->height;
	BORDER(child)=0;
      } else {
	_XmConfigureObject(child,rect->x,rect->y,rect->width,rect->height,0);
      }
    } else {
      if(child==CURRENT_WIDGET(w)){
	if(child==instigator){
	  X(child)=CHILD_X(w);
	  Y(child)=CHILD_Y(w);
	  WIDTH(child)=CHILD_WIDTH(w);
	  HEIGHT(child)=CHILD_HEIGHT(w);
	  BORDER(child)=0;
	} else {
	  _XmConfigureObject(child,CHILD_X(w),CHILD_Y(w),
	      CHILD_WIDTH(w),CHILD_HEIGHT(w),0);
	}
      } else {
	if(child==instigator){
	  X(child)=X(w)-CHILD_WIDTH(w);
	  Y(child)=Y(w)-CHILD_HEIGHT(w);
	  WIDTH(child)=CHILD_WIDTH(w);
	  HEIGHT(child)=CHILD_HEIGHT(w);
	  BORDER(child)=0;
	} else {
	  _XmConfigureObject(child,X(w)-CHILD_WIDTH(w),Y(w)-CHILD_HEIGHT(w),
	      CHILD_WIDTH(w),CHILD_HEIGHT(w),0);
	}
      }
    }
  }
}

static void draw_shadows(Widget w,Region region){
  XRectangle *rect;
  XRectangle cr;
#define OPTIMIZE_SHADOW_DRAWING 0
#if OPTIMIZE_SHADOW_DRAWING
  Boolean shadow_intersect;
  XRectangle r[4];
#endif

  if(!XtIsRealized(w)) return;

  if(region) XClipBox(region,&cr);
  else { cr.x=0;cr.y=0;cr.width=WIDTH(w);cr.height=HEIGHT(w);}

  XClearArea(XtDisplay(w),XtWindow(w),cr.x,cr.y,cr.width,cr.height,FALSE);

#if OPTIMIZE_SHADOW_DRAWING
  /* left */
  r[0].x=MARGIN_WIDTH(w);
  r[0].y=MARGIN_HEIGHT(w)+TAB_HEIGHT(w);
  r[0].width=SHADOW_THICKNESS(w);
  r[0].height=HEIGHT(w)-MARGIN_HEIGHT(w)*2-TAB_HEIGHT(w);

  /* bottom */
  r[1].x=r[0].x;
  r[1].y=r[0].y+r[0].height-r[0].width;
  r[1].width=WIDTH(w)-MARGIN_WIDTH(w)*2;
  r[1].height=r[0].width;

  /* top */
  r[2].x=r[1].x;
  r[2].y=r[0].y;
  r[2].width=r[1].width;
  r[2].height=r[1].height;

  /* right */
  r[3].x=r[0].x+r[1].width-r[0].width;
  r[3].y=r[0].y;
  r[3].width=r[0].width;
  r[3].height=r[0].height;

  shadow_intersect=axyRectanglesIntersect(&r[0],&cr) | 
    axyRectanglesIntersect(&r[1],&cr) |
    axyRectanglesIntersect(&r[2],&cr) |
    axyRectanglesIntersect(&r[3],&cr);

  if(shadow_intersect) {
#endif
      XSetClipRectangles(XtDisplay(w),TOP_SHADOW_GC(w),0,0,&cr,1,Unsorted);
      XSetClipRectangles(XtDisplay(w),BOTTOM_SHADOW_GC(w),0,0,&cr,1,Unsorted);

    _XmDrawShadows(XtDisplay(w),XtWindow(w),
	  TOP_SHADOW_GC(w),BOTTOM_SHADOW_GC(w),
	  MARGIN_WIDTH(w),MARGIN_HEIGHT(w)+TAB_HEIGHT(w),
	  WIDTH(w)-MARGIN_WIDTH(w)*2,HEIGHT(w)-MARGIN_HEIGHT(w)*2-TAB_HEIGHT(w),
	  SHADOW_THICKNESS(w),XmSHADOW_OUT);
      
      XSetClipMask(XtDisplay(w),TOP_SHADOW_GC(w),None);
      XSetClipMask(XtDisplay(w),BOTTOM_SHADOW_GC(w),None);
#if OPTIMIZE_SHADOW_DRAWING
  }
#endif

  rect=&TAB_RECT(CURRENT_WIDGET(w));
  if(SHOW_TABS(w) && 
      (int)rect->x+(int)rect->width<=(int)WIDTH(w)-(int)MARGIN_WIDTH(w)*2){

#if OPTIMIZE_SHADOW_DRAWING
    if(shadow_intersect) {
#endif
      XClearArea(XtDisplay(w),XtWindow(w),
	  rect->x+SHADOW_THICKNESS(w)+MARGIN_WIDTH(w),
	  MARGIN_HEIGHT(w)+TAB_HEIGHT(w),
	  rect->width-SHADOW_THICKNESS(w)*2,SHADOW_THICKNESS(w),FALSE);
#if OPTIMIZE_SHADOW_DRAWING
    }
#endif
  }
}

static void draw_da_tabs(Widget da,Window window,XEvent *event){
  Widget w=XtParent(da);
  XExposeEvent *e=(XExposeEvent*)event;
  int i,tmp;
  XRectangle *tab_rect,*tab_clip,tab_clip_modified;
  XRectangle cr,clip;

  if(window==0) return;

  if(e==NULL){
    cr.x=0;cr.y=0;cr.width=WIDTH(da);cr.height=HEIGHT(da);
  } else {
    cr.x=e->x;cr.y=e->y;cr.width=e->width;cr.height=e->height;
  }

  XClearArea(XtDisplay((Widget)w),window,
      cr.x,cr.y,cr.width,cr.height,FALSE);

  if(SHOW_TABS(w)){
    for(i=NUM_CHILDREN(w)-1;i>=0;i--){

      if(CHILD(w,i)==da || !XtIsManaged(CHILD(w,i))) continue;
      if(CURRENT_WIDGET(w)==CHILD(w,i)) continue;
      if(!TAB_VISIBLE_I(w,i)) {
	continue;
      }

      tab_rect=&TAB_RECT_I(w,i);
      tab_clip=&TAB_CLIP_I(w,i);

      if(TAB_VISIBLE(CURRENT_WIDGET(w)) &&
	  PAGE_NUM(CURRENT_WIDGET(w))==PAGE_NUM_I(w,i)+1){
	tab_clip_modified.x=tab_clip->x;
	tab_clip_modified.y=tab_clip->y;
	tab_clip_modified.width=tmp=
	  (int)TAB_RECT(CURRENT_WIDGET(w)).x-(int)tab_clip->x;
	if(tmp<0) tab_clip_modified.width=0;
	tab_clip_modified.height=tab_clip->height;
      } else {
	tab_clip_modified.x=tab_clip->x;
	tab_clip_modified.y=tab_clip->y;
	tab_clip_modified.width=tab_clip->width;
	tab_clip_modified.height=tab_clip->height;
      }

      axyRectanglesIntersection(&cr,&tab_clip_modified,&clip);
      if(axyRectangleIsEmpty(&clip)) {
	continue;
      }

      XSetClipRectangles(XtDisplay((Widget)w),ARM_GC(w),0,0,&clip,1,Unsorted);

      XFillRectangle(XtDisplay((Widget)w),window,ARM_GC(w),
	  tab_rect->x+SHADOW_THICKNESS(w),
	  tab_rect->y+TAB_RAISE(w)+SHADOW_THICKNESS(w),
	  tab_rect->width-SHADOW_THICKNESS(w)*2,
	  tab_rect->height-TAB_RAISE(w)-SHADOW_THICKNESS(w));

      XSetClipMask(XtDisplay((Widget)w),ARM_GC(w),None);


      XSetClipRectangles(XtDisplay(w),NORMAL_GC(w),0,0,&clip,1,Unsorted);

      XmStringDraw(XtDisplay(w),window,FONT_LIST(w),
	  TAB_LABEL_I(w,i),NORMAL_GC(w),
	  tab_rect->x+SHADOW_THICKNESS(w)+TAB_MARGIN_WIDTH(w)+
	  EHT(w)+LABEL_MARGIN_WIDTH(w),
	  tab_rect->y+SHADOW_THICKNESS(w)+TAB_MARGIN_HEIGHT(w)+
	  EHT(w)+LABEL_MARGIN_HEIGHT(w)+(Dimension)TAB_RAISE(w),
	  tab_rect->width-SHADOW_THICKNESS(w)*2-TAB_MARGIN_WIDTH(w)*2-
	  EHT(w)*2-LABEL_MARGIN_WIDTH(w)*2,
	  XmALIGNMENT_CENTER,STRING_DIR(w),&clip);

      XSetClipMask(XtDisplay(w),NORMAL_GC(w),None);


      XSetClipRectangles(XtDisplay(w),TOP_SHADOW_GC(w),0,0,&clip,1,Unsorted);
      XSetClipRectangles(XtDisplay(w),BOTTOM_SHADOW_GC(w),0,0,&clip,1,Unsorted);

      _XmDrawShadows(XtDisplay(w),window,
	  TOP_SHADOW_GC(w),BOTTOM_SHADOW_GC(w),
	  tab_rect->x,tab_rect->y+(Dimension)TAB_RAISE(w),
	  tab_rect->width,tab_rect->height+SHADOW_THICKNESS(w)-
	   (Dimension)TAB_RAISE(w),
	  SHADOW_THICKNESS(w),XmSHADOW_OUT);
      
      XSetClipMask(XtDisplay(w),TOP_SHADOW_GC(w),None);
      XSetClipMask(XtDisplay(w),BOTTOM_SHADOW_GC(w),None);
    }

    tab_rect=&TAB_RECT(CURRENT_WIDGET(w));
    axyRectanglesIntersection(&cr,tab_rect,&clip);

    if(TAB_VISIBLE(CURRENT_WIDGET(w)) && !axyRectangleIsEmpty(&clip)){

      XClearArea(XtDisplay(w),window,clip.x,clip.y,
	  clip.width,clip.height,FALSE);

      XSetClipRectangles(XtDisplay(w),NORMAL_GC(w),0,0,&clip,1,Unsorted);

      XmStringDraw(XtDisplay(w),window,FONT_LIST(w),
	  TAB_LABEL(CURRENT_WIDGET(w)),NORMAL_GC(w),
	  tab_rect->x+SHADOW_THICKNESS(w)+TAB_MARGIN_WIDTH(w)+
	  EHT(w)+LABEL_MARGIN_WIDTH(w),
	  tab_rect->y+SHADOW_THICKNESS(w)+TAB_MARGIN_HEIGHT(w)+
	  EHT(w)+LABEL_MARGIN_HEIGHT(w)+
	  (Dimension)(TAB_RAISE(w)/2),
	  tab_rect->width-SHADOW_THICKNESS(w)*2-TAB_MARGIN_WIDTH(w)*2-
	  EHT(w)*2-LABEL_MARGIN_WIDTH(w)*2,
	  XmALIGNMENT_CENTER,STRING_DIR(w),&clip);

      XSetClipMask(XtDisplay(w),NORMAL_GC(w),None);


      XSetClipRectangles(XtDisplay(w),TOP_SHADOW_GC(w),0,0,&clip,1,Unsorted);
      XSetClipRectangles(XtDisplay(w),BOTTOM_SHADOW_GC(w),0,0,&clip,1,Unsorted);

      _XmDrawShadows(XtDisplay(w),window,
	  TOP_SHADOW_GC(w),BOTTOM_SHADOW_GC(w),
	  tab_rect->x, tab_rect->y,
	  tab_rect->width,tab_rect->height+SHADOW_THICKNESS(w),
	  SHADOW_THICKNESS(w),XmSHADOW_OUT);

      XSetClipMask(XtDisplay(w),TOP_SHADOW_GC(w),None);
      XSetClipMask(XtDisplay(w),BOTTOM_SHADOW_GC(w),None);
    }

    if(HIGHLIGHT_DRAWN(w)) draw_da_highlight(da);
  }

}

static void draw_da_highlight(Widget da){
  Widget w=XtParent(da);
  XRectangle *rect,*clip,clip_modified;
  GC gc=HIGHLIGHT_DRAWN(w)?HIGHLIGHT_GC(w):
      (HIGHLIGHTED_TAB(w)==CURRENT_WIDGET(w)?BACKGROUND_GC(w):ARM_GC(w));
  int tmp;

  if(HIGHLIGHT_THICKNESS(w)==0) return;
  if(!XtIsRealized(da) || !HIGHLIGHTED_TAB(w) || 
     !TAB_VISIBLE(HIGHLIGHTED_TAB(w))) return;

  rect=&TAB_RECT(HIGHLIGHTED_TAB(w));
  clip=&TAB_CLIP(HIGHLIGHTED_TAB(w));

  if(HIGHLIGHTED_TAB(w)!=CURRENT_WIDGET(w)){
    if(TAB_VISIBLE(CURRENT_WIDGET(w)) && 
	PAGE_NUM(CURRENT_WIDGET(w))==PAGE_NUM(HIGHLIGHTED_TAB(w))+1){
      clip_modified.x=clip->x;
      clip_modified.y=clip->y;
      clip_modified.width=tmp=
	(int)TAB_RECT(CURRENT_WIDGET(w)).x-(int)clip->x;
      if(tmp<0) clip_modified.width=0;
      clip_modified.height=clip->height;
    } else {
      clip_modified.x=clip->x;
      clip_modified.y=clip->y;
      clip_modified.width=clip->width;
      clip_modified.height=clip->height;
    }
    XSetClipRectangles(XtDisplay(w),gc,0,0,&clip_modified,1,Unsorted);
  }

  _XmDrawSimpleHighlight(XtDisplay(da),XtWindow(da),gc,
      rect->x+SHADOW_THICKNESS(w)+TAB_MARGIN_WIDTH(w),
      rect->y+SHADOW_THICKNESS(w)+TAB_MARGIN_HEIGHT(w)+
      (HIGHLIGHTED_TAB(w)==CURRENT_WIDGET(w)?
       (Dimension)(TAB_RAISE(w)/2):(Dimension)TAB_RAISE(w)),
      rect->width-SHADOW_THICKNESS(w)*2-TAB_MARGIN_WIDTH(w)*2,
      rect->height-SHADOW_THICKNESS(w)-TAB_MARGIN_HEIGHT(w)*2-TAB_RAISE(w),
      HIGHLIGHT_THICKNESS(w));

  XSetClipMask(XtDisplay(w),gc,None);
}

static Widget search_da_tab(Widget da, XEvent *event){
  Widget w=XtParent(da);
  int i;
  XRectangle *rect;
  Widget child;
  Position mx,my;
	
  if(!SHOW_TABS(w)) return 0;
  mx=event->xbutton.x;
  my=event->xbutton.y;

  for(i=NUM_CHILDREN(w)-1;i>=0;i--){
    child=CHILD(w,i);
    if(child==da) continue;
    if(!XtIsManaged(child)) continue;
    if(!TAB_VISIBLE(child)) continue;

    if(child==CURRENT_WIDGET(w)) {
      rect=&TAB_RECT(child);
      if(mx>=rect->x && mx<rect->x+rect->width) return child;
    } else {
      rect=&TAB_CLIP(child);
      if(mx>=rect->x && mx<rect->x+rect->width){
	if(my<=TAB_RAISE(w)) return 0;
	else return child;
      }
    }
  }
  return 0;
}

static void expose_da_cb(Widget da,XtPointer w,XtPointer call){
  XmDrawingAreaCallbackStruct *cbs=(XmDrawingAreaCallbackStruct*)call;

  if(cbs->window==0) return;

  draw_da_tabs(da,cbs->window,cbs->event);
}

static void change_tab(Widget w,Widget child){
  _XmConfigureObject(CURRENT_WIDGET(w),X(w)-CHILD_WIDTH(w),Y(w)-CHILD_HEIGHT(w),
      CHILD_WIDTH(w),CHILD_HEIGHT(w),0);
  if(HIGHLIGHTED_TAB(w) && HIGHLIGHT_DRAWN(w)){
    HIGHLIGHT_DRAWN(w)=FALSE;
    draw_da_highlight(DRAWING_AREA(w));
    HIGHLIGHT_DRAWN(w)=TRUE;
  }
  CURRENT_PAGE(w)=PAGE_NUM(child);
  CURRENT_WIDGET(w)=child;
  _XmConfigureObject(CURRENT_WIDGET(w),CHILD_X(w),CHILD_Y(w),
      CHILD_WIDTH(w),CHILD_HEIGHT(w),0);
  HIGHLIGHTED_TAB(w)=child;
  HIGHLIGHT_DRAWN(w)=TRUE;
  draw_shadows(w,NULL);
  draw_da_tabs(DRAWING_AREA(w),XtWindow(DRAWING_AREA(w)),NULL);
}

