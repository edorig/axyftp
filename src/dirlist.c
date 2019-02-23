/* Copyright (c) 1998   Alexander Yukhimets. All rights reserved. */
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"axyftp.h"
#include"utils.h"
#include"functions.h"
#include"little_dialogs.h"
#include"dirinfo.h"
#include"dirlist.h"
#include"pixmaps.h"

#define NUMFIELDS 5

#ifdef BUILD_GTK

#include<gtk/gtk.h>

static int widths[NUMFIELDS];

static void doubleclick_cb(GtkWidget* w,gint row,gint column,
    GdkEventButton *e,gpointer data){
  int which,ret;

  which=(int)data;
  if(e && (e->type==GDK_2BUTTON_PRESS || e->type==GDK_3BUTTON_PRESS)){
    switch(which){
      case LOCAL:
	busy_cursor(TRUE);
	ret=chg_local_dir(row);
	if(!ret){
	   char* mask=gtk_entry_get_text(GTK_ENTRY(appdata.local.text));
	   update_local(mask);
	}
	if(!appdata.job)busy_cursor(FALSE);
	break;
      case REMOTE:
	if(appdata.job)break;
	if(!appdata.connected){
	  (void)popup_warning_dialog(NULL,"Not connected");
	  break;
	}
	appdata.job=5;
	busy_cursor(TRUE);
	ret=chg_remote_dir(row);
	if(!ret){
	   char* mask=gtk_entry_get_text(GTK_ENTRY(appdata.remote.text));
	   update_remote(mask);
	}
	busy_cursor(FALSE);
	appdata.job=0;
	break;
    }

      /*g_print("%s row: %d\n",(which==LOCAL)?"Local":"Remote",row);*/
  }
}

int* get_selected_rows(GtkWidget* table){
  GList *list;
  int* retval;
  int num,j;

  list=GTK_CLIST(table)->selection;
  if(list==NULL)return NULL;
  if((num=g_list_length(list))<1)return NULL;
  retval=(int*)WXmalloc(sizeof(int)*(num+1));
  retval[num]=-1;
  for(j=0;list!=NULL;list=list->next){
    retval[j++]=(int)list->data;
  }
  return retval;
}

int get_selected_row(GtkWidget* table){
  GList *list;

  list=GTK_CLIST(table)->selection;
  if(list==NULL)return NONE;
  if(list->next!=NULL)return MANY;
  return (int)list->data;
}

void cancel_table_update(GtkWidget*table ){
  gtk_clist_freeze(GTK_CLIST(table));
}

void restore_table_update(GtkWidget* table){
  gtk_clist_thaw(GTK_CLIST(table));
}

void clear_dirlist(GtkWidget* table){
  int i;

  gtk_clist_clear(GTK_CLIST(table));
  for(i=0;i<NUMFIELDS;i++){
    gtk_clist_set_column_width(GTK_CLIST(table),i,widths[i]);
  }
}

void update_dirlist(GtkWidget *table,dirinfo* di){
  int i,j,temp;
  GdkPixmap* pix;
  GdkBitmap* bit;
  int max[NUMFIELDS];
  GtkStyle *style;
  gchar* dummy[NUMFIELDS];

  for(i=0;i<NUMFIELDS;i++)dummy[i]="";

  style=gtk_widget_get_style(table);

  gtk_clist_freeze(GTK_CLIST(table));
  gtk_clist_clear(GTK_CLIST(table));

  max[0]=3;for(i=1;i<NUMFIELDS;i++)max[i]=6;
  for(i=0;i<di->total;i++){
    gtk_clist_append(GTK_CLIST(table),dummy);
    for(j=0;j<NUMFIELDS;j++){

      switch(j){

	case 0:
	  switch(di->files[i+1]->perms[0]){

	    case 'd':
	      if(strcmp(di->files[i+1]->name,"..")==0){
		pix=pixmap[0];
		bit=mask[0];
	      } else {
		pix=pixmap[1];
		bit=mask[1];
	      }
	      break;

	    case 'l':
	      pix=pixmap[3];
	      bit=mask[3];
	      break;

	    default:
	      pix=pixmap[2];
	      bit=mask[2];
	      break;
	  }
	  gtk_clist_set_pixmap(GTK_CLIST(table),i,j,pix,bit);
	  break;
		  
	case 1:
	  gtk_clist_set_text(GTK_CLIST(table),i,j,di->files[i+1]->name);
	  break;

	case 2:
	  gtk_clist_set_text(GTK_CLIST(table),i,j,di->files[i+1]->date);
	  break;

	case 3:
	  gtk_clist_set_text(GTK_CLIST(table),i,j,di->files[i+1]->time);
	  break;

	case 4:
	  gtk_clist_set_text(GTK_CLIST(table),i,j,di->files[i+1]->size_str);
	  break;

	default:
	  break;
      }
    }
    if(max[1]<(temp=strlen(di->files[i+1]->name)))max[1]=temp;
    if(max[2]<(temp=2+strlen(di->files[i+1]->date)))max[2]=temp;
    if(max[3]<(temp=2+strlen(di->files[i+1]->time)))max[3]=temp;
    if(max[4]<(temp=strlen(di->files[i+1]->size_str)))max[4]=temp;
    gtk_clist_set_foreground(GTK_CLIST(table),i,&style->black);
    gtk_clist_set_background(GTK_CLIST(table),i,&style->white);
  }


  if(appdata.odata->columnadjustment){
    temp=gdk_string_width(mystyle->font,"w");
    j=atoi(appdata.odata->maxwidth);
    if(j>0)if(max[1]>j)max[1]=j;
    for(i=0;i<NUMFIELDS;i++){
      gtk_clist_set_column_width(GTK_CLIST(table),i,temp*max[i]);
    }
  }
  gtk_clist_thaw(GTK_CLIST(table));
}

static void clist_select_row(GtkCList *w,int row,GdkEventButton* e){
    gtk_signal_emit_by_name(GTK_OBJECT(w),"select_row",row,-1,e);
}

static void clist_unselect_row(GtkCList *w,int row,GdkEventButton* e){
    gtk_signal_emit_by_name(GTK_OBJECT(w),"unselect_row",row,-1,e);
}
static void clist_unselect_all(GtkCList *w,GdkEventButton* e){
  GList *l;
  while(1){
    l=w->selection;
    if(l==NULL)break;
    clist_unselect_row(w,(int)(l->data),e);
  }
}
static void clist_unselect_rows(GtkCList* w,int start,int end,GdkEventButton* e){
  if(start<end){
    for(;start<=end;end--){
      clist_unselect_row(w,end,e);
    }
  } else {
    for(;end<=start;end++){
      clist_unselect_row(w,end,e);
    }
  }
}
static void clist_select_rows(GtkCList* w,int start,int end,GdkEventButton* e){
  if(start<end){
    for(;start<=end;end--){
      clist_select_row(w,end,e);
    }
  } else {
    for(;end<=start;end++){
      clist_select_row(w,end,e);
    }
  }
}

struct _select {
  int anchor;
  int extend;
  int select;
  int down;
};

static int button_up_cb(GtkWidget* w,GdkEventButton* e,gpointer app){
  struct _select *s;
  /*
  gint x,y,row,col;
  GdkModifierType m;
  */
  
  s=(struct _select*)app;
  s->down=0;
  /*
  gdk_window_get_pointer(e->window,&x,&y,&m);

  if(m&GDK_BUTTON1_MASK){
    if(gtk_clist_get_selection_info(GTK_CLIST(w),x,y,&row,&col)){
    }
  }
  */
  return TRUE;
}
static int button_down_cb(GtkWidget* w,GdkEventButton* e,gpointer app){
  struct _select *s;
  gint x,y,row,col;
  GdkModifierType m;
  
  s=(struct _select*)app;
  if(GTK_CLIST(w)->clist_window!=e->window)return FALSE;
  gdk_window_get_pointer(e->window,&x,&y,&m);

  if(m&GDK_BUTTON1_MASK){
    if(gtk_clist_get_selection_info(GTK_CLIST(w),x,y,&row,&col)){
      s->down=1;
      if(!(m&GDK_SHIFT_MASK) && !(m&GDK_CONTROL_MASK)){
	clist_unselect_all(GTK_CLIST(w),e);
	s->anchor=row;
	s->extend=-1;
	s->select=1;
	clist_select_row(GTK_CLIST(w),s->anchor,e);
      } else if(!(m&GDK_SHIFT_MASK) && (m&GDK_CONTROL_MASK)){
	GList *l;
	l=GTK_CLIST(w)->selection;
	if(g_list_find(l,(gpointer)row)!=NULL){
	  clist_unselect_row(GTK_CLIST(w),row,e);
	  s->select=-1;
	} else {
	  clist_select_row(GTK_CLIST(w),row,e);
	  s->select=1;
	}
	s->anchor=row;
        s->extend=-1;
      } else if((m&GDK_SHIFT_MASK) && !(m&GDK_CONTROL_MASK)){
	if(s->extend>=0){
	  clist_unselect_rows(GTK_CLIST(w),s->extend,s->anchor,e);
	}
	s->extend=row;
	if(s->select==1){
	  clist_select_rows(GTK_CLIST(w),row,s->anchor,e);
	} else {
	  clist_unselect_rows(GTK_CLIST(w),row,s->anchor,e);
	}
      }
    }
  }
  return TRUE;
}

static int motion_cb(GtkWidget* w,GdkEventMotion* e,gpointer app){
  struct _select *s;
  gint x,y,row,col;
  GdkModifierType m;
  
  s=(struct _select*)app;
  if(GTK_CLIST(w)->clist_window!=e->window)return FALSE;
  gdk_window_get_pointer(e->window,&x,&y,&m);

  if(m&GDK_BUTTON1_MASK){
    if(gtk_clist_get_selection_info(GTK_CLIST(w),x,y,&row,&col)){
      if(s->extend!=row && s->down!=0){
	gfloat down=1.;
	if(s->extend>=0){
	  if(s->select==1){
	    clist_unselect_rows(GTK_CLIST(w),s->extend,s->anchor,
		(GdkEventButton*)e);
	  } else {
	    clist_select_rows(GTK_CLIST(w),s->extend,s->anchor,
		(GdkEventButton*)e);
	  }
	}
	if(row<s->extend)down=0.;
	s->extend=row;
	if(s->select==1){
	  clist_select_rows(GTK_CLIST(w),row,s->anchor,(GdkEventButton*)e);
	} else {
	  clist_unselect_rows(GTK_CLIST(w),row,s->anchor,(GdkEventButton*)e);
	}
	if(gtk_clist_row_is_visible(GTK_CLIST(w),row)!=GTK_VISIBILITY_FULL){
          gtk_clist_moveto(GTK_CLIST(w),row,-1,down,0);
	}
      }
    }
  }
  return FALSE;
}

GtkWidget* create_dirlist(GtkWidget* parent,int which){
  struct _select *select_struct;
  GtkWidget* list;
#ifndef OLD_GTK
  GtkWidget* sw;
#endif
  static gchar* titles[NUMFIELDS]={"^","Name","Date","Time","Size"};
  static GtkJustification justs[NUMFIELDS+1]={GTK_JUSTIFY_CENTER,
    GTK_JUSTIFY_LEFT,GTK_JUSTIFY_CENTER,GTK_JUSTIFY_CENTER,GTK_JUSTIFY_RIGHT};
  int i,w,t;

#ifndef OLD_GTK
  sw=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_set_border_width(GTK_CONTAINER(sw),0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
   GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
#endif

  list=gtk_clist_new_with_titles(NUMFIELDS,titles);

#ifndef OLD_GTK
  gtk_container_add(GTK_CONTAINER(sw),list);
#endif

  gtk_clist_set_selection_mode(GTK_CLIST(list),-1);

  /*gtk_clist_set_policy(GTK_CLIST(list),
      GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);*/

#ifndef OLD_GTK
  gtk_clist_set_shadow_type(GTK_CLIST(list),GTK_SHADOW_IN);
#else
  gtk_clist_set_border(GTK_CLIST(list),GTK_SHADOW_IN);
#endif

  gtk_clist_set_row_height(GTK_CLIST(list),gtkfontheight*1.6);

#ifdef OLD_GTK
  GTK_CLIST_CLASS(GTK_OBJECT(list)->klass)->scrollbar_spacing=0;
#endif



  w=0;
  for(i=0;i<NUMFIELDS;i++){
    t=gdk_string_width(mystyle->font,titles[i])+8;
    gtk_clist_set_column_width(GTK_CLIST(list),i,t);
    widths[i]=t;
    w+=t;
    gtk_clist_set_column_justification(GTK_CLIST(list),i,justs[i]);
  }
  gtk_widget_set_usize(list,w+NUMFIELDS*8,0);
  gtk_signal_connect_after(GTK_OBJECT(list),"select_row",
      GTK_SIGNAL_FUNC(doubleclick_cb),(gpointer)which);
  gtk_widget_show(list);

  select_struct=WXmalloc(sizeof(struct _select));
  select_struct->down=0;
  gtk_signal_connect(GTK_OBJECT(list),"button_press_event",
      GTK_SIGNAL_FUNC(button_down_cb),(gpointer)select_struct);
  gtk_signal_connect(GTK_OBJECT(list),"button_release_event",
      GTK_SIGNAL_FUNC(button_up_cb),(gpointer)select_struct);
  gtk_signal_connect(GTK_OBJECT(list),"motion_notify_event",
      GTK_SIGNAL_FUNC(motion_cb),(gpointer)select_struct);
  gtk_widget_set_events(list,GDK_BUTTON_PRESS_MASK|
      GDK_POINTER_MOTION_MASK);

  switch(which){
    case LOCAL:
      appdata.local.table=list;
      break;
    case REMOTE:
      appdata.remote.table=list;
      break;
    default:
      break;
  }

#ifndef OLD_GTK
  gtk_widget_show(list);
  gtk_widget_show(sw);
  return sw;
#else
  return list;
#endif
}

#elif defined BUILD_MOTIF

#include<Xm/Xm.h>
#include<Xm/TextF.h>
#include<XmAxy/List.h>


static void doubleclick_cb(Widget,XtPointer,XtPointer);

void doubleclick_cb(Widget w,XtPointer app,XtPointer call){
  int which;
  XmAxyListDefaultActionCallbackStruct *cbs;
  int ret;

  which=(int)app;
  cbs=(XmAxyListDefaultActionCallbackStruct*)call;

  switch(which){
    case LOCAL:
      busy_cursor(True);
      ret=chg_local_dir(cbs->row);
      if(!ret){
         char* mask=XmTextFieldGetString(appdata.local.text);
	 update_local(mask);
	 XtFree(mask);
      }
      if(!appdata.job)busy_cursor(False);
      break;
    case REMOTE:
      if(appdata.job)break;
      if(!appdata.connected){
	(void)popup_warning_dialog(toplevel,"Not connected");
        break;
      }
      appdata.job=5;
      busy_cursor(True);
      ret=chg_remote_dir(cbs->row);
      if(!ret){
         char* mask=XmTextFieldGetString(appdata.remote.text);
	 update_remote(mask);
	 XtFree(mask);
      }
      busy_cursor(False);
      appdata.job=0;
      break;
  }
  /*
  printf("DOUBLECLICK for %s row #%d\n",(which==REMOTE)?"remote":"local",
      cbs->row);
      */
  return;
}

void cancel_table_update(Widget w){
/*  XbaeMatrixDisableRedisplay(w);*/
}

void restore_table_update(Widget w){
  XmAxyListRefresh(w);
}

int* get_selected_rows(Widget w){
  int num,*list;
  int *retval;

  if(!XmAxyListGetSelectedRows(w,&list,&num)){
    return NULL;
  }

  retval=(int*)XtRealloc((char*)list,sizeof(int)*(num+1));
  retval[num]=-1;
  return retval;
}

int get_selected_row(Widget w){
  int num,*list;
  int retval;
  if(!XmAxyListGetSelectedRows(w,&list,&num)){
    return NONE;
  }
  if(num>1) retval=MANY;
  else retval=list[0];
  XtFree((char*)list);
  return retval;
}

static void redraw_cb(Widget,XtPointer,XtPointer);
/*
static void enter_cb(Widget,XtPointer,XtPointer);
static void select_cb(Widget,XtPointer,XtPointer);
*/

void update_dirlist(Widget table,dirinfo* di){
  XtCallbackRec rw[2];
  int i,temp;
  short max[NUMFIELDS];
  Pixel **colors;
  Pixel white_pixel;
  
  rw[1].callback=NULL;
  rw[1].closure=NULL;
  rw[0].callback=redraw_cb;
  rw[0].closure=di;
  
  white_pixel=WhitePixelOfScreen(XtScreen(table));
  colors=(Pixel**)XtMalloc(di->total*sizeof(Pixel*));
  for(i=0;i<di->total;i++){
    colors[i]=(Pixel*)XtMalloc(NUMFIELDS*sizeof(Pixel));
    for(temp=0;temp<NUMFIELDS;temp++)colors[i][temp]=white_pixel;
  }

  max[0]=4;for(i=1;i<NUMFIELDS;i++)max[i]=6;
  for(i=1;i<=di->total;i++){
    if(max[1]<(temp=strlen(di->files[i]->name)))max[1]=temp;
    if(max[2]<(temp=2+strlen(di->files[i]->date)))max[2]=temp;
    if(max[3]<(temp=2+strlen(di->files[i]->time)))max[3]=temp;
    if(max[4]<(temp=strlen(di->files[i]->size_str)))max[4]=temp;
  } 

  XtVaSetValues(table,
      XmNrowCount,di->total,
      /*XmNcellBackgrounds,colors,*/
      XmNdrawCellCallback,rw,
      NULL);

  if(appdata.odata->columnadjustment){
    temp=atoi(appdata.odata->maxwidth);
    if(temp>0)if(max[1]>temp)max[1]=temp;
    XtVaSetValues(table,XmNcolumnWidths,max,NULL);
  }

  /*XbaeMatrixDeselectAll(table);*/
  for(i=0;i<di->total;i++) XtFree((char*)colors[i]);
  XtFree((char*)colors);

  return;
}

void redraw_cb(Widget w,XtPointer app,XtPointer call){
  dirinfo *di;
  XmAxyListDrawCellCallbackStruct *cs;
  
  cs=(XmAxyListDrawCellCallbackStruct*)call;
  di=(dirinfo*)app;
  cs->type=XmSTRING;
  switch(cs->column){
    case 0:
      switch(di->files[cs->row+1]->perms[0]){
        case 'd':
	  if(strcmp(di->files[cs->row+1]->name,"..")==0){
	    cs->pixmap=pixmap[0];
	    cs->pixmask=mask[0];
	  } else {
	    cs->pixmap=pixmap[1];
	    cs->pixmask=mask[1];
	  }
	  cs->string=" D";
	  break;

	case 'l':
	  cs->pixmap=pixmap[3];
	  cs->pixmask=mask[3];
	  cs->string=" L";
	  break;

	default:
	  cs->pixmap=pixmap[2];
	  cs->pixmask=mask[2];
	  cs->string=" F";
	  break;
      }
      if(cs->pixmap && cs->pixmask){
	cs->type=XmPIXMAP;
      } else {
	cs->type=XmSTRING;
      }
      break;
	      
    case 1:
      cs->string=di->files[cs->row+1]->name;
      break;

    case 2:
      cs->string=di->files[cs->row+1]->date;
      break;

    case 3:
      cs->string=di->files[cs->row+1]->time;
      break;

    case 4:
      cs->string=di->files[cs->row+1]->size_str;
      break;

    default:
      break;
  }
  return;
}

void clear_dirlist(Widget table){
  static short mw[NUMFIELDS]={4,6,6,6,6};

  XtVaSetValues(table,XmNrowCount,0,XmNcolumnWidths,mw,
      XmNdrawCellCallback,NULL,NULL);
}

struct _select {
  int anchor;
  int extend;
  int select;
};

Widget create_dirlist(Widget parent,int which){
  Widget list;
  Arg args[20];
  Cardinal n;
  static short mw[NUMFIELDS]={4,6,6,6,6};
  static unsigned char  ca[NUMFIELDS]=
   {XmALIGNMENT_CENTER,XmALIGNMENT_BEGINNING,
    XmALIGNMENT_CENTER,XmALIGNMENT_CENTER,XmALIGNMENT_END};
  static unsigned char  la[NUMFIELDS]=
   {XmALIGNMENT_CENTER,XmALIGNMENT_CENTER,XmALIGNMENT_CENTER,
    XmALIGNMENT_CENTER,XmALIGNMENT_CENTER};
    /*
  Pixel white_pixel,black_pixel;
  Colormap cmap;
  XColor color,exact;
  */

  static String labels[NUMFIELDS]={"^","Name","Date","Time","Size"};

  /*
  white_pixel=WhitePixelOfScreen(XtScreen(parent));
  black_pixel=BlackPixelOfScreen(XtScreen(parent));
  */

  n=0;
  XtSetArg(args[n],XmNrowCount,0);n++;
  XtSetArg(args[n],XmNselectionPolicy,XmEXTENDED_SELECT);n++;
  /*XtSetArg(args[n],XmNselectedForeground,white_pixel);n++;*/
  /*XtSetArg(args[n],XmNforeground,black_pixel);n++;*/
  XtSetArg(args[n],XmNcolumnWidths,mw);n++;
  /*XtSetArg(args[n],XmNbuttonLabels,True);n++;*/
  /*XtSetArg(args[n],XmNcellHighlightThickness,0);n++;*/
  XtSetArg(args[n],XmNcellMarginWidth,2);n++;
  XtSetArg(args[n],XmNcolumnCount,NUMFIELDS);n++;
  XtSetArg(args[n],XmNcolumnLabels,labels);n++;
  XtSetArg(args[n],XmNcolumnLabelAlignments,la);n++;
  XtSetArg(args[n],XmNcolumnAlignments,ca);n++;
  XtSetArg(args[n],XmNrowSpacing,1);n++;
  XtSetArg(args[n],XmNcellMarginHeight,4);n++;
  XtSetArg(args[n],XmNlabelMarginHeight,3);n++;
  XtSetArg(args[n],XmNhighlightThickness,0);n++;
  /*XtSetArg(args[n],XmNfill,True);n++;*/
  /*XtSetArg(args[n],XmNspace,0);n++;*/
  /*XtSetArg(args[n],XmNshowArrows,True);n++;*/
  /*XtSetArg(args[n],XmNgridType,XmGRID_NONE);n++;*/
  /*XtSetArg(args[n],XmNallowColumnResize,True);n++;*/
  list=XmAxyCreateScrolledList(parent,"dirlist",args,n);
  XtManageChild(list);

  /*
  XtVaGetValues(list,XmNcolormap,&cmap,NULL);
  if(XAllocNamedColor(XtDisplay(list),cmap,"#2F2FCF",&color,&exact)){
    XtVaSetValues(list,XmNselectedBackground,color.pixel,NULL);
  }
  */

  /*XtOverrideTranslations(list,XtParseTranslationTable(translations));*/
  
  /*XtAddCallback(list,XmNenterCellCallback,enter_cb,NULL);*/
  /*xp=(XtPointer)XtNew(struct _select);*/
  /*XtAddCallback(list,XmNselectCellCallback,select_cb,xp);*/

  XtAddCallback(list,XmNdefaultActionCallback,doubleclick_cb,(XtPointer)which);

  switch(which){
    case LOCAL:
      appdata.local.table=list;
      break;
    case REMOTE:
      appdata.remote.table=list;
      break;
    default:
      break;
  }
  
  return XtParent(list);
}



  /*
void deselect_rows(Widget w,int start,int end){
  if(start<end){
    for(;start<=end;end--){
      XbaeMatrixDeselectRow(w,end);
    }
  } else {
    for(;end<=start;end++){
      XbaeMatrixDeselectRow(w,end);
    }
  }
}

  */
  /*
void select_rows(Widget w,int start,int end){
  if(start<end){
    for(;start<=end;end--){
      XbaeMatrixSelectRow(w,end);
    }
  } else {
    for(;end<=start;end++){
      XbaeMatrixSelectRow(w,end);
    }
  }
}

  */
#if 0
void select_cb(Widget w,XtPointer app,XtPointer call){
  int top;
  struct _select * s;
  XbaeMatrixSelectCellCallbackStruct *cbs;
  cbs=(XbaeMatrixSelectCellCallbackStruct *) call;
  s=(struct _select *)app;

  /*
  printf("Row/Column: %d/%d \n%d params: ",
      cbs->row,cbs->column,cbs->num_params);
  for(i=0;i<cbs->num_params;i++)printf("%s ",cbs->params[i]);
  printf("\n");
  */


  XtVaGetValues(w,XmNrows,&top,NULL);
  if(top<=cbs->row)return;

  switch(cbs->params[0][0]){
    case 'S': 
      XbaeMatrixDeselectAll(w);
      s->anchor=cbs->row;
      s->extend=-1;
      s->select=1;
      XbaeMatrixSelectRow(w,s->anchor);
      break;

    case 'T':
      if(XbaeMatrixIsRowSelected(w,cbs->row)){
	XbaeMatrixDeselectRow(w,cbs->row);
	s->select=-1;
      } else {
	XbaeMatrixSelectRow(w,cbs->row);
	s->select=1;
      }
      s->anchor=cbs->row;
      s->extend=-1;
      break;

    case 'E':
      XbaeMatrixDisableRedisplay(w);
      XtVaGetValues(w,XmNtopRow,&top,NULL);
      if(s->extend>=0){
	deselect_rows(w,s->extend,s->anchor);
      }
      s->extend=cbs->row;
      if(s->select==1){
	select_rows(w,cbs->row,s->anchor);
      } else {
	deselect_rows(w,cbs->row,s->anchor);
      }
      XtVaSetValues(w,XmNtopRow,top,NULL);
      XbaeMatrixEnableRedisplay(w,True);
      break;

    case 'e':
      XbaeMatrixDisableRedisplay(w);
      if(s->extend>=0){
        if(s->select==1){
	  deselect_rows(w,s->extend,s->anchor);
	} else {
	  select_rows(w,s->extend,s->anchor);
	}
      }
      s->extend=cbs->row;
      if(s->select==1){
	select_rows(w,cbs->row,s->anchor);
      } else {
	deselect_rows(w,cbs->row,s->anchor);
      }
      XbaeMatrixEnableRedisplay(w,True);
      break;

    case 'U':
      XbaeMatrixDisableRedisplay(w);
      XtVaGetValues(w,XmNtopRow,&top,NULL);
      XbaeMatrixUnhighlightAll(w);
      XbaeMatrixHighlightRow(w,cbs->row);
      XtVaSetValues(w,XmNtopRow,top,NULL);
      XbaeMatrixEnableRedisplay(w,True);
      break;
  }
  return; 
}

void enter_cb(Widget w,XtPointer app,XtPointer call){
  /*
  XbaeMatrixEnterCellCallbackStruct *cbs=
   (XbaeMatrixEnterCellCallbackStruct*) call;
  cbs->map=False;
  cbs->doit=False;
  */
}
#endif  

#else
#error Either BUILD_GTK or BUILD_MOTIF should be defined
#endif
