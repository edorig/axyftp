/*
 * demo.c - simple spin button demo.
 *
 * Jerry Adair
 * adair@iglou.com
 * 
 */
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include "SpinButton.h"

int  main(    int,    char **                                 );
void test(    Widget, XtPointer, XmSpinButtonCallbackStruct * );
void test1(   Widget, XtPointer, XmSpinButtonCallbackStruct * );
void GetText( Widget, Widget,    XtPointer                    );
void DoQuit(  Widget, XtPointer, XtPointer                    );

Widget           topLevel;
Widget           form;
Widget           quitButton;
Widget           separator;
Widget           spinButton1;
Widget           spinButton2;
Widget           getTextButton;
XtAppContext     appContext;


int main( int argc, char **argv )
{
   char             text[20];
   unsigned         i;
   XmString         str;
   Boolean          Trav;
   XFontStruct     *localFont;
   XmFontList       fontList;


   topLevel = XtVaAppInitialize( &appContext, "Demo", NULL, 0,
                                 &argc, argv, NULL,
                                 XmNtitle, "Spin Button", NULL );

   form = XtVaCreateManagedWidget(
                      "form",
                      xmFormWidgetClass,
                      topLevel,
                      NULL );

   str = XmStringCreateLtoR( "Quit Demo", XmFONTLIST_DEFAULT_TAG  );
   quitButton = XtVaCreateManagedWidget(
                      "quitButton",
                      xmPushButtonWidgetClass,
                      form, 
                      XmNlabelString, str, 
                      XmNleftAttachment, XmATTACH_FORM, 
                      XmNleftOffset, 8, 
                      XmNtopAttachment,  XmATTACH_FORM, 
                      XmNtopOffset, 8, 
                      NULL);
   XmStringFree(str);
   XtAddCallback( quitButton,
                  XmNactivateCallback, ( XtCallbackProc ) DoQuit,
                  NULL );

   separator = XtVaCreateManagedWidget(
                      "separator1",
                      xmSeparatorWidgetClass,
                      form, 
                      XmNleftAttachment,  XmATTACH_FORM, 
                      XmNrightAttachment, XmATTACH_FORM,
                      XmNtopAttachment,   XmATTACH_WIDGET, 
                      XmNtopWidget,       quitButton, 
                      NULL ); 

   if( ( localFont = XLoadQueryFont( XtDisplay(form), "-adobe-times-medium-r-normal--18-180-75-75-p-94-iso8859-1\0") ) != NULL )
     fontList  = XmFontListCreate( localFont, XmFONTLIST_DEFAULT_TAG );
   else
   {
     localFont = XLoadQueryFont( XtDisplay(form), "8x13\0");
     fontList  = XmFontListCreate( localFont, XmFONTLIST_DEFAULT_TAG );

   } /* end else can't load that font */

   spinButton1 = XtVaCreateManagedWidget( "spinButton2",
                      xmSpinButtonWidgetClass,
                      form, 
                      XmNcolumns,                  4,
                      XmNmaxLength,                4,
                      XmNmaximum,                  900,
                      XmNminimum,                  100,
                      XmNposition,                 100,
                      XmNarrowLayout,              XmARROWS_END,
                      XmNinitialDelay,             500,
                      XmNrepeatDelay,              50,
                      XmNspinButtonLabelFontList,  fontList,
                      XmNspinButtonTextFontList,   fontList,
                      XmNdecimalPoints,            2,
                      XmNverticalSpacing,          10,
                      XmNchildType,                XmNUMERIC,
                      XmNleftAttachment,           XmATTACH_FORM, 
                      XmNrightAttachment,          XmATTACH_FORM,
                      XmNtopAttachment,            XmATTACH_WIDGET, 
                      XmNtopWidget,                separator, 
                      NULL );

   XtAddCallback( spinButton1, XmNvalueChangedCallback, test, NULL );

   spinButton2 = XtVaCreateManagedWidget( "spinButton2",
                      xmSpinButtonWidgetClass,
                      form, 
                      XmNcolumns,                  10,
                      XmNmaxLength,                3,
                      XmNarrowLayout,              XmARROWS_END,
                      XmNinitialDelay,             500,
                      XmNrepeatDelay,              50,
                      XmNminimum,                  1,
                      XmNmaximum,                  100,
                      XmNspinButtonLabelFontList,  fontList,
                      XmNspinButtonTextFontList,   fontList,
                      XmNchildType,                XmSTRING,
                      XmNleftAttachment,           XmATTACH_FORM, 
                      XmNrightAttachment,          XmATTACH_FORM,
                      XmNtopAttachment,            XmATTACH_WIDGET, 
                      XmNtopWidget,                spinButton1, 
                      NULL );

   for( i=1 ; i<100 ; i++ )
   {
      sprintf( text, "%d", i );
      str = XmStringCreateLtoR( text, XmFONTLIST_DEFAULT_TAG );
      XmSpinButtonAddItem( spinButton2, str, i, 1 );
      XmStringFree( str );

   } /* end for */

   XtAddCallback( spinButton2, XmNvalueChangedCallback, test1, NULL );

   str = XmStringCreateLtoR( "Get String in SpinButton1", XmFONTLIST_DEFAULT_TAG );
   getTextButton = XtVaCreateManagedWidget(
                      "getTextButton",
                      xmPushButtonWidgetClass,
                      form, 
                      XmNlabelString,              str, 
                      XmNleftAttachment,           XmATTACH_FORM, 
                      XmNtopAttachment,            XmATTACH_WIDGET, 
                      XmNtopWidget,                spinButton2, 
                      NULL );
    XmStringFree( str );
    XtAddCallback( getTextButton,
                   XmNactivateCallback, ( XtCallbackProc ) GetText,
                   spinButton1 );

    XtRealizeWidget( topLevel   );

    XtAppMainLoop(   appContext );

} /* end main() */


void test( Widget widget, XtPointer client_data, XmSpinButtonCallbackStruct *call_data )
{
   char   *text;

   XmStringGetLtoR( call_data->value, XmFONTLIST_DEFAULT_TAG, &text );
   printf( "value = %s\n", text );
   XtFree( text );

}


void test1( Widget widget, XtPointer client_data, XmSpinButtonCallbackStruct *call_data )
{
   char   *text;

   XmStringGetLtoR( call_data->value, XmFONTLIST_DEFAULT_TAG, &text );
   printf( "value = %s\n", text );
   XtFree( text );

}


void GetText( Widget widget, Widget client_data, XtPointer call_data )
{
   char   *text;

   text = XmSpinButtonGetTextString( client_data );
   printf( "Retrieving text; value = >%s<\n", text );
   XtFree( text );

}


void DoQuit( Widget widget, XtPointer client_data, XtPointer call_data )
{
   XmString   str;
   char      *text;

   exit( 0 );

}
