dnl aclocal.m4 generated automatically by aclocal 1.4-p6

dnl Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl	AXY_LIB_XTHREADS
dnl	(borrowed from Xbae distribution and modified)
dnl

AC_DEFUN(AXY_LIB_XTHREADS,
[AC_REQUIRE([AC_PATH_X])
AC_CACHE_CHECK(whether libXt was compiled with -DXTHREADS, axy_cv_xthreads,
[axy_save_CPPFLAGS="$CPPFLAGS"
axy_save_LDFLAGS="$LDFLAGS"
axy_save_LIBS="$LIBS"
CPPFLAGS="-I$ac_x_includes"
LDFLAGS="-L$ac_x_libraries"
LIBS="-lXt -lSM -lICE -lX11"
AC_TRY_LINK(,[_XtProcessLock()],
axy_cv_xthreads=yes,
axy_cv_xthreads=no)
])
if test x$axy_cv_xthreads = xyes; then
  AC_DEFINE(XTHREADS)
fi
CPPFLAGS="$axy_save_CPPFLAGS"
LDFLAGS="$axy_save_LDFLAGS"
LIBS="$axy_save_LIBS"
])

dnl
dnl	AXY_PATH_DT
dnl	(borrowed from Xbae distribution and modified)
dnl

AC_DEFUN(AXY_PATH_DT,
[AC_REQUIRE_CPP()dnl

AC_MSG_CHECKING(for Dt)

no_dt=
AC_ARG_WITH(dt,
   [  --with-dt      enable use of already installed Dt library],
   if test "x$with_dt" = xno ; then
     no_dt=yes
   fi
)dnl

if test "x$no_dt" != xyes ; then 

  AC_ARG_WITH(dt-includes, 
     [  --with-dt-includes=DIR     Dt include files are in DIR])
  if test  -n "$with_dt_includes" ; then
    dt_includes=$with_dt_includes
  fi
  AC_ARG_WITH(dt-libraries, 
     [  --with-dt-libraries=DIR    Dt library files are in DIR])
  if test -n "$with_dt_libraries" ; then
    dt_libraries=$with_dt_libraries
  fi


  AC_CACHE_VAL(ac_cv_path_dt, [
    no_dt=yes

    test -z "$dt_direct_test_library" && 
	 dt_direct_test_library=DtWidget
    test -z "$dt_direct_test_function" && 
	 dt_direct_test_function=DtCreateComboBox
    test -z "$dt_direct_test_include" && 
	 dt_direct_test_include=Dt/ComboBox.h

    if test -z "$dt_libraries" || test -z "$dt_includes" ; then
      ac_save_libs="$LIBS"
      ac_save_cflags="$CFLAGS"
      ac_save_cppflags="$CPPFLAGS"
      LIBS="-lDtWidget $MOTIF_LIBS $X_LIBS -lXt $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
      CFLAGS="$MOTIF_CFLAGS $X_CFLAGS $CFLAGS"
      CPPFLAGS="$MOTIF_CFLAGS $X_CFLAGS $CPPFLAGS"
      AC_TRY_LINK([#include <Dt/ComboBox.h>],
	[Widget w; DtCreateComboBox(w, "", NULL, 0);], 
	[no_dt=  ac_dt_includes="" ac_dt_libraries=""])
      if test -n "$no_dt" ; then
        LIBS="-lDtWidget -lDtSvc -ltt $MOTIF_LIBS $X_LIBS -lXt $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
	AC_TRY_LINK([#include <Dt/ComboBox.h>],
	  [Widget w; DtCreateComboBox(w, "", NULL, 0);], 
	  [no_dt=  ac_dt_includes="" ac_dt_libraries=""])
      fi
      if test -n "$no_dt" ; then
        LIBS="-lDtWidget -lDtSvc -ltt -lgen $MOTIF_LIBS $X_LIBS -lXt $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
	AC_TRY_LINK([#include <Dt/ComboBox.h>],
	  [Widget w; DtCreateComboBox(w, "", NULL, 0);], 
	  [no_dt=  ac_dt_includes="" ac_dt_libraries=""])
      fi
      LIBS="$ac_save_libs"
      CFLAGS="$ac_save_cflags"
      CPPFLAGS="$ac_save_cppflags"
    else
      no_dt=
      ac_dt_includes="$dt_includes" ac_dt_libraries="$dt_libraries"
    fi

    if test "x$no_dt" = xyes ; then

      if test -n "$dt_includes" ; then
	ac_dt_includes="$dt_includes"
      else
	ac_dt_includes=
      fi
      if test -n "$dt_libraries" ; then
	ac_dt_libraries="$dt_libraries"
      else
	ac_dt_libraries=
      fi

      if test -z "$ac_dt_includes" ; then
	for ac_dir in               \
	  /usr/include              \
	  /usr/include/Motif1.2     \
	  /usr/include/X11R6        \
	  /usr/include/X11R5        \
	  /usr/include/X11          \
	  /usr/openwin/include      \
	  /usr/dt/include           \
	  /usr/motif/include        \
	  /usr/Motif1.2/include     \
	  /usr/X11R6/include        \
	  /usr/X11R5/include        \
	  /usr/X11/include          \
	  /usr/X386/include         \
	  /usr/x386/include         \
	  /usr/XFree86/include/X11  \
	  /usr/athena/include       \
	  /opt/openwin/include      \
	  /usr/unsupported/include  \
	  /usr/lpp/Xamples/include  \
	  /usr/local/include        \
	  /usr/local/include/X11R6  \
	  /usr/local/include/X11R5  \
	  /usr/local/include/X11    \
	  /usr/local/X11R6/include  \
	  /usr/local/X11R5/include  \
	  /usr/local/X11/include    \
	  /usr/local/x11r5/include  \
	  ; \
	do
	  if test -r "$ac_dir/$dt_direct_test_include"; then
	    ac_dt_includes=$ac_dir
	    break
	  fi
	done
      fi

      if test -z "$ac_dt_libraries" ; then
	for ac_dir in `echo "$ac_dt_includes" | sed s/include/lib/g` \
	  /usr/lib              \
	  /usr/lib/Motif1.2     \
	  /usr/lib/X11R6        \
	  /usr/lib/X11R5        \
	  /usr/lib/X11          \
	  /usr/openwin/lib      \
	  /usr/dt/lib           \
	  /usr/motif/lib        \
	  /usr/Motif1.2/lib     \
	  /usr/X11R6/lib        \
	  /usr/X11R5/lib        \
	  /usr/X11/lib          \
	  /usr/X386/lib         \
	  /usr/x386/lib         \
	  /usr/XFree86/lib/X11  \
	  /usr/athena/lib       \
	  /opt/openwin/lib      \
	  /usr/unsupported/lib  \
	  /usr/lpp/Xamples/lib  \
	  /usr/local/lib        \
	  /usr/local/lib/X11R6  \
	  /usr/local/lib/X11R5  \
	  /usr/local/lib/X11    \
	  /usr/local/X11R6/lib  \
	  /usr/local/X11R5/lib  \
	  /usr/local/X11/lib    \
	  /usr/local/x11r5/lib  \
	  ; \
	do
	  for ac_extension in a so sl; do
	    if test -r $ac_dir/lib${dt_direct_test_library}.$ac_extension; then
	      ac_dt_libraries=$ac_dir
	      break 2
	    fi
	  done
	done
      fi

      if test -n "$ac_dt_libraries" && test -n "ac_dt_includes" ; then
	ac_cv_path_dt="no_dt= ac_dt_includes=$ac_dt_includes ac_dt_libraries=$ac_dt_libraries"
      else
	ac_cv_path_dt="no_dt=yes"
      fi
    
    else 

      ac_cv_path_dt="no_dt= ac_dt_includes=$ac_dt_includes ac_dt_libraries=$ac_dt_libraries"
    fi
  ])dnl

  eval "$ac_cv_path_dt"

fi

if test "x$no_dt" = xyes ; then
  AC_MSG_RESULT(no)
else
  dt_includes=$ac_dt_includes
  dt_libraries=$ac_dt_libraries
  if test "x$dt_libraries" = x ; then
    DT_LIBS="-lDtWidget"
  else
    DT_LIBS="-L$dt_libraries -lDtWidget"
  fi
  if test "x$dt_includes" = x ; then
    DT_CFLAGS=""
  else
    DT_CFLAGS="-I$dt_includes"
  fi

  axy_dt_link_ok=no
  ac_save_libs="$LIBS"
  ac_save_cflags="$CFLAGS"
  ac_save_cppflags="$CPPFLAGS"
  LIBS="$DT_LIBS $MOTIF_LIBS $X_LIBS -lXt $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
  CFLAGS="$DT_CFLAGS $MOTIF_CFLAGS $X_CFLAGS $CFLAGS"
  CPPFLAGS="$DT_CFLAGS $MOTIF_CFLAGS $X_CFLAGS $CPPFLAGS"
  AC_TRY_LINK([#include <Dt/ComboBox.h>],
    [Widget w; DtCreateComboBox(w, "", NULL, 0);], 
    [axy_dt_link_ok=yes])
  if test "x$axy_dt_link_ok" = xno ; then
    DT_LIBS="$DT_LIBS -lDtSvc -ltt"
    LIBS="$DT_LIBS $MOTIF_LIBS $X_LIBS -lXt $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
    AC_TRY_LINK([#include <Dt/ComboBox.h>],
      [Widget w; DtCreateComboBox(w, "", NULL, 0);], 
      [axy_dt_link_ok=yes])
  fi
  if test "x$axy_dt_link_ok" = xno ; then
    DT_LIBS="$DT_LIBS -lgen"
    LIBS="$DT_LIBS $MOTIF_LIBS $X_LIBS -lXt $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
    AC_TRY_LINK([#include <Dt/ComboBox.h>],
      [Widget w; DtCreateComboBox(w, "", NULL, 0);], 
      [axy_dt_link_ok=yes])
  fi
  LIBS="$ac_save_libs"
  CFLAGS="$ac_save_cflags"
  CPPFLAGS="$ac_save_cppflags"

  if test "x$axy_dt_link_ok" = xno ; then
    no_dt=yes
    DT_LIBS=
    DT_CFLAGS=
    AC_MSG_RESULT(no)
  else
    AC_MSG_RESULT([libraries $dt_libraries, headers $dt_includes])
  fi

fi

AC_SUBST(DT_LIBS)
AC_SUBST(DT_CFLAGS)
])dnl

dnl
dnl	AXY_PATH_MOTIF
dnl	(borrowed from Xbae distribution and modified)
dnl

AC_DEFUN(AXY_PATH_MOTIF,
[AC_REQUIRE_CPP()dnl

AC_MSG_CHECKING(for Motif)

AC_ARG_WITH(motif-includes, 
   [  --with-motif-includes=DIR     Motif include files are in DIR])
if test  -n "$with_motif_includes" ; then
  motif_includes=$with_motif_includes
fi
AC_ARG_WITH(motif-libraries, 
   [  --with-motif-libraries=DIR    Motif library files are in DIR])
if test -n "$with_motif_libraries" ; then
  motif_libraries=$with_motif_libraries
fi


AC_CACHE_VAL(ac_cv_path_motif, [
  no_motif=yes

  test -z "$motif_direct_test_library" && 
       motif_direct_test_library=Xm
  test -z "$motif_direct_test_function" && 
       motif_direct_test_function=XmCreateDrawingArea
  test -z "$motif_direct_test_include" && 
       motif_direct_test_include=Xm/Xm.h

  if test -z "$motif_libraries" || test -z "$motif_includes" ; then
    ac_save_libs="$LIBS"
    ac_save_cflags="$CFLAGS"
    ac_save_cppflags="$CPPFLAGS"
    LIBS="-lXm $X_LIBS -lXt $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
    CFLAGS="$X_CFLAGS $CFLAGS"
    CPPFLAGS="$X_CFLAGS $CPPFLAGS"
    AC_TRY_LINK([#include <Xm/Label.h>],
      [Widget w; XmCreateLabel(w, "", NULL, 0);], 
      [no_motif=  ac_motif_includes="" ac_motif_libraries=""])
    LIBS="$ac_save_libs"
    CFLAGS="$ac_save_cflags"
    CPPFLAGS="$ac_save_cppflags"
  else
    no_motif=
    ac_motif_includes="$motif_includes" ac_motif_libraries="$motif_libraries"
  fi

  if test "x$no_motif" = xyes ; then

    if test -n "$motif_includes" ; then
      ac_motif_includes="$motif_includes"
    else
      ac_motif_includes=
    fi
    if test -n "$motif_libraries" ; then
      ac_motif_libraries="$motif_libraries"
    else
      ac_motif_libraries=
    fi

    if test -z "$ac_motif_includes" ; then
      for ac_dir in               \
	/usr/include              \
	/usr/include/Motif1.2     \
	/usr/include/X11R6        \
	/usr/include/X11R5        \
	/usr/include/X11          \
	/usr/openwin/include      \
	/usr/dt/include           \
	/usr/motif/include        \
	/usr/Motif1.2/include     \
	/usr/X11R6/include        \
	/usr/X11R5/include        \
	/usr/X11/include          \
	/usr/X386/include         \
	/usr/x386/include         \
	/usr/XFree86/include/X11  \
	/usr/athena/include       \
	/opt/openwin/include      \
	/usr/unsupported/include  \
	/usr/lpp/Xamples/include  \
	/opt/motif21/include      \
	/opt/motif20/include      \
	/opt/motif12/include      \
	/usr/local/include        \
	/usr/local/include/X11R6  \
	/usr/local/include/X11R5  \
	/usr/local/include/X11    \
	/usr/local/X11R6/include  \
	/usr/local/X11R5/include  \
	/usr/local/X11/include    \
	/usr/local/x11r5/include  \
	; \
      do
	if test -r "$ac_dir/$motif_direct_test_include"; then
	  ac_motif_includes=$ac_dir
	  break
	fi
      done
    fi

    if test -z "$ac_motif_libraries" ; then
      ac_save_libs="$LIBS"
      LIBS="-l$motif_direct_test_library $LIBS"
      for ac_dir in `echo "$ac_motif_includes" | sed s/include/lib/g` \
	/usr/lib              \
	/usr/lib/Motif1.2     \
	/usr/lib/X11R6        \
	/usr/lib/X11R5        \
	/usr/lib/X11          \
	/usr/openwin/lib      \
	/usr/dt/lib           \
	/usr/motif/lib        \
	/usr/Motif1.2/lib     \
	/usr/X11R6/lib        \
	/usr/X11R5/lib        \
	/usr/X11/lib          \
	/usr/X386/lib         \
	/usr/x386/lib         \
	/usr/XFree86/lib/X11  \
	/usr/athena/lib       \
	/opt/openwin/lib      \
	/usr/unsupported/lib  \
	/usr/lpp/Xamples/lib  \
	/opt/motif21/lib      \
	/opt/motif20/lib      \
	/opt/motif12/lib      \
	/usr/local/lib        \
	/usr/local/lib/X11R6  \
	/usr/local/lib/X11R5  \
	/usr/local/lib/X11    \
	/usr/local/X11R6/lib  \
	/usr/local/X11R5/lib  \
	/usr/local/X11/lib    \
	/usr/local/x11r5/lib  \
	; \
      do
	for ac_extension in a so sl; do
	  if test -r $ac_dir/lib${motif_direct_test_library}.$ac_extension; then
	    ac_motif_libraries=$ac_dir
	    break 2
	  fi
	done
      done
      LIBS="$ac_save_libs"
    fi

    if test -n "$ac_motif_libraries" && test -n "ac_motif_includes" ; then
      ac_cv_path_motif="no_motif= ac_motif_includes=$ac_motif_includes ac_motif_libraries=$ac_motif_libraries"
    else
      ac_cv_path_motif="no_motif=yes"
    fi
  
  else 

    ac_cv_path_motif="no_motif= ac_motif_includes=$ac_motif_includes ac_motif_libraries=$ac_motif_libraries"
  fi
])dnl

eval "$ac_cv_path_motif"

if test "x$no_motif" = xyes; then
  AC_MSG_RESULT(no)
else
  motif_includes=$ac_motif_includes
  motif_libraries=$ac_motif_libraries
  if test "x$motif_libraries" = x ; then
    MOTIF_LIBS="-lXm"
  else
    MOTIF_LIBS="-L$motif_libraries -lXm"
  fi
  if test "x$motif_includes" = x ; then
    MOTIF_CFLAGS=""
  else
    MOTIF_CFLAGS="-I$motif_includes"
  fi
  AC_MSG_RESULT([libraries $motif_libraries, headers $motif_includes])
fi

AC_SUBST(MOTIF_LIBS)
AC_SUBST(MOTIF_CFLAGS)
])dnl

dnl
dnl	AXY_PATH_XMAXY
dnl	(borrowed from Xbae distribution and modified)
dnl

AC_DEFUN(AXY_PATH_XMAXY,
[AC_REQUIRE_CPP()dnl

AC_MSG_CHECKING(for XmAxy)

no_xmaxy=
AC_ARG_WITH(xmaxy,
   [  --with-xmaxy      enable use of already installed XmAxy library],
   if test "x$with_xmaxy" = xno ; then
     no_xmaxy=yes
   fi
)dnl

if test "x$no_xmaxy" != xyes ; then 

  AC_ARG_WITH(xmaxy-includes, 
     [  --with-xmaxy-includes=DIR     XmAxy include files are in DIR])
  if test  -n "$with_xmaxy_includes" ; then
    xmaxy_includes=$with_xmaxy_includes
  fi
  AC_ARG_WITH(xmaxy-libraries, 
     [  --with-xmaxy-libraries=DIR    XmAxy library files are in DIR])
  if test -n "$with_xmaxy_libraries" ; then
    xmaxy_libraries=$with_xmaxy_libraries
  fi


  AC_CACHE_VAL(ac_cv_path_xmaxy, [
    no_xmaxy=yes

    test -z "$xmaxy_direct_test_library" && 
	 xmaxy_direct_test_library=XmAxy
    test -z "$xmaxy_direct_test_function" && 
	 xmaxy_direct_test_function=XmAxyCreateNotebook
    test -z "$xmaxy_direct_test_include" && 
	 xmaxy_direct_test_include=XmAxy/Notebook.h

    if test -z "$xmaxy_libraries" || test -z "$xmaxy_includes" ; then
      ac_save_libs="$LIBS"
      ac_save_cflags="$CFLAGS"
      ac_save_cppflags="$CPPFLAGS"
      LIBS="-lXmAxy $MOTIF_LIBS $X_LIBS -lXt $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
      CFLAGS="$MOTIF_CFLAGS $X_CFLAGS $CFLAGS"
      CPPFLAGS="$MOTIF_CFLAGS $X_CFLAGS $CPPFLAGS"
      AC_TRY_LINK([#include <XmAxy/Notebook.h>],
	[Widget w; XmAxyCreateNotebook(w, "", NULL, 0);], 
	[no_xmaxy=  ac_xmaxy_includes="" ac_xmaxy_libraries=""])
      LIBS="$ac_save_libs"
      CFLAGS="$ac_save_cflags"
      CPPFLAGS="$ac_save_cppflags"
    else
      no_xmaxy=
      ac_xmaxy_includes="$xmaxy_includes" ac_xmaxy_libraries="$xmaxy_libraries"
    fi

    if test "x$no_xmaxy" = xyes ; then

      if test -n "$xmaxy_includes" ; then
	ac_xmaxy_includes="$xmaxy_includes"
      else
	ac_xmaxy_includes=
      fi
      if test -n "$xmaxy_libraries" ; then
	ac_xmaxy_libraries="$xmaxy_libraries"
      else
	ac_xmaxy_libraries=
      fi

      if test -z "$ac_xmaxy_includes" ; then
	for ac_dir in               \
	  /usr/include              \
	  /usr/include/Motif1.2     \
	  /usr/include/X11R6        \
	  /usr/include/X11R5        \
	  /usr/include/X11          \
	  /usr/openwin/include      \
	  /usr/dt/include           \
	  /usr/motif/include        \
	  /usr/Motif1.2/include     \
	  /usr/X11R6/include        \
	  /usr/X11R5/include        \
	  /usr/X11/include          \
	  /usr/X386/include         \
	  /usr/x386/include         \
	  /usr/XFree86/include/X11  \
	  /usr/athena/include       \
	  /opt/openwin/include      \
	  /usr/unsupported/include  \
	  /usr/lpp/Xamples/include  \
	  /usr/local/include        \
	  /usr/local/include/X11R6  \
	  /usr/local/include/X11R5  \
	  /usr/local/include/X11    \
	  /usr/local/X11R6/include  \
	  /usr/local/X11R5/include  \
	  /usr/local/X11/include    \
	  /usr/local/x11r5/include  \
	  /usr/local/xmaxy/include  \
	  /opt/xmaxy/include        \
	  ; \
	do
	  if test -r "$ac_dir/$xmaxy_direct_test_include"; then
	    ac_xmaxy_includes=$ac_dir
	    break
	  fi
	done
      fi

      if test -z "$ac_xmaxy_libraries" ; then
	ac_save_libs="$LIBS"
	LIBS="-l$xmaxy_direct_test_library $LIBS"
	for ac_dir in `echo "$ac_xmaxy_includes" | sed s/include/lib/g` \
	  /usr/lib              \
	  /usr/lib/Motif1.2     \
	  /usr/lib/X11R6        \
	  /usr/lib/X11R5        \
	  /usr/lib/X11          \
	  /usr/openwin/lib      \
	  /usr/dt/lib           \
	  /usr/motif/lib        \
	  /usr/Motif1.2/lib     \
	  /usr/X11R6/lib        \
	  /usr/X11R5/lib        \
	  /usr/X11/lib          \
	  /usr/X386/lib         \
	  /usr/x386/lib         \
	  /usr/XFree86/lib/X11  \
	  /usr/athena/lib       \
	  /opt/openwin/lib      \
	  /usr/unsupported/lib  \
	  /usr/lpp/Xamples/lib  \
	  /usr/local/lib        \
	  /usr/local/lib/X11R6  \
	  /usr/local/lib/X11R5  \
	  /usr/local/lib/X11    \
	  /usr/local/X11R6/lib  \
	  /usr/local/X11R5/lib  \
	  /usr/local/X11/lib    \
	  /usr/local/x11r5/lib  \
	  /usr/local/xmaxy/lib  \
	  /opt/xmaxy/include    \
	  ; \
	do
	  for ac_extension in a so sl; do
	    if test -r $ac_dir/lib${xmaxy_direct_test_library}.$ac_extension; then
	      ac_xmaxy_libraries=$ac_dir
	      break 2
	    fi
	  done
	done
	LIBS="$ac_save_libs"
      fi

      if test -n "$ac_xmaxy_libraries" && test -n "ac_xmaxy_includes" ; then
	ac_cv_path_xmaxy="no_xmaxy= ac_xmaxy_includes=$ac_xmaxy_includes ac_xmaxy_libraries=$ac_xmaxy_libraries"
      else
	ac_cv_path_xmaxy="no_xmaxy=yes"
      fi
    
    else 

      ac_cv_path_xmaxy="no_xmaxy= ac_xmaxy_includes=$ac_xmaxy_includes ac_xmaxy_libraries=$ac_xmaxy_libraries"
    fi
  ])dnl

  eval "$ac_cv_path_xmaxy"

fi

if test "x$no_xmaxy" = xyes; then
  AC_MSG_RESULT(no)
else
  xmaxy_includes=$ac_xmaxy_includes
  xmaxy_libraries=$ac_xmaxy_libraries
  if test "x$xmaxy_libraries" = x ; then
    XMAXY_LIBS="-lXmAxy"
  else
    XMAXY_LIBS="-L$xmaxy_libraries -lXmAxy"
  fi
  if test "x$xmaxy_includes" = x ; then
    XMAXY_CFLAGS=""
  else
    XMAXY_CFLAGS="-I$xmaxy_includes"
  fi
  AC_MSG_RESULT([libraries $xmaxy_libraries, headers $xmaxy_includes])
fi

AC_SUBST(XMAXY_LIBS)
AC_SUBST(XMAXY_CFLAGS)
])dnl

dnl
dnl	AXY_PATH_XPM
dnl	(borrowed from Xbae distribution and modified)
dnl

AC_DEFUN(AXY_PATH_XPM,
[AC_REQUIRE_CPP()dnl

AC_MSG_CHECKING(for XPM)

AC_ARG_WITH(xpm-includes, 
   [  --with-xpm-includes=DIR     XPM include files are in DIR])
if test  -n "$with_xpm_includes" ; then
  xpm_includes=$with_xpm_includes
fi
AC_ARG_WITH(xpm-libraries, 
   [  --with-xpm-libraries=DIR    XPM library files are in DIR])
if test -n "$with_xpm_libraries" ; then
  xpm_libraries=$with_xpm_libraries
fi


AC_CACHE_VAL(ac_cv_path_xpm, [
  no_xpm=yes

  test -z "$xpm_direct_test_library" && 
       xpm_direct_test_library=Xpm
  test -z "$xpm_direct_test_function" && 
       xpm_direct_test_function=XpmCreatePixmapFromData
  test -z "$xpm_direct_test_include" && 
       xpm_direct_test_include=X11/xpm.h

  if test -z "$xpm_libraries" || test -z "$xpm_includes" ; then
    ac_save_libs="$LIBS"
    ac_save_cflags="$CFLAGS"
    ac_save_cppflags="$CPPFLAGS"
    LIBS="-lXpm $X_LIBS -lXt $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $LIBS"
    CFLAGS="$X_CFLAGS $CFLAGS"
    CPPFLAGS="$X_CFLAGS $CPPFLAGS"
    AC_TRY_LINK([#include <X11/xpm.h>],
      [XpmCreatePixmapFromData(NULL,0,NULL,NULL,NULL,NULL);], 
      [no_xpm=  ac_xpm_includes="" ac_xpm_libraries=""])
    LIBS="$ac_save_libs"
    CFLAGS="$ac_save_cflags"
    CPPFLAGS="$ac_save_cppflags"
  else
    no_xpm=
    ac_xpm_includes="$xpm_includes" ac_xpm_libraries="$xpm_libraries"
  fi

  if test "x$no_xpm" = xyes ; then

    if test -n "$xpm_includes" ; then
      ac_xpm_includes="$xpm_includes"
    else
      ac_xpm_includes=
    fi
    if test -n "$xpm_libraries" ; then
      ac_xpm_libraries="$xpm_libraries"
    else
      ac_xpm_libraries=
    fi

    if test -z "$ac_xpm_includes" ; then
      for ac_dir in               \
	/usr/include              \
	/usr/include/X11R6        \
	/usr/include/X11R5        \
	/usr/include/X11          \
	/usr/openwin/include      \
	/usr/dt/include           \
	/usr/X11R6/include        \
	/usr/X11R5/include        \
	/usr/X11/include          \
	/usr/X386/include         \
	/usr/x386/include         \
	/usr/XFree86/include/X11  \
	/usr/athena/include       \
	/opt/openwin/include      \
	/usr/unsupported/include  \
	/usr/lpp/Xamples/include  \
	/usr/local/include        \
	/usr/local/include/X11R6  \
	/usr/local/include/X11R5  \
	/usr/local/include/X11    \
	/usr/local/X11R6/include  \
	/usr/local/X11R5/include  \
	/usr/local/X11/include    \
	/usr/local/x11r5/include  \
	; \
      do
	if test -r "$ac_dir/$xpm_direct_test_include"; then
	  ac_xpm_includes=$ac_dir
	  break
	fi
      done
    fi

    if test -z "$ac_xpm_libraries" ; then
      ac_save_libs="$LIBS"
      LIBS="-l$xpm_direct_test_library $LIBS"
      for ac_dir in `echo "$ac_xpm_includes" | sed s/include/lib/g` \
	/usr/lib              \
	/usr/lib/X11R6        \
	/usr/lib/X11R5        \
	/usr/lib/X11          \
	/usr/openwin/lib      \
	/usr/dt/lib           \
	/usr/X11R6/lib        \
	/usr/X11R5/lib        \
	/usr/X11/lib          \
	/usr/X386/lib         \
	/usr/x386/lib         \
	/usr/XFree86/lib/X11  \
	/usr/athena/lib       \
	/opt/openwin/lib      \
	/usr/unsupported/lib  \
	/usr/lpp/Xamples/lib  \
	/usr/local/lib        \
	/usr/local/lib/X11R6  \
	/usr/local/lib/X11R5  \
	/usr/local/lib/X11    \
	/usr/local/X11R6/lib  \
	/usr/local/X11R5/lib  \
	/usr/local/X11/lib    \
	/usr/local/x11r5/lib  \
	; \
      do
	for ac_extension in a so sl; do
	  if test -r $ac_dir/lib${xpm_direct_test_library}.$ac_extension; then
	    ac_xpm_libraries=$ac_dir
	    break 2
	  fi
	done
      done
      LIBS="$ac_save_libs"
    fi

    if test -n "$ac_xpm_libraries" && test -n "ac_xpm_includes" ; then
      ac_cv_path_xpm="no_xpm= ac_xpm_includes=$ac_xpm_includes ac_xpm_libraries=$ac_xpm_libraries"
    else
      ac_cv_path_xpm="no_xpm=yes"
    fi
  
  else 

    ac_cv_path_xpm="no_xpm= ac_xpm_includes=$ac_xpm_includes ac_xpm_libraries=$ac_xpm_libraries"
  fi
])dnl

eval "$ac_cv_path_xpm"

if test "x$no_xpm" = xyes; then
  AC_MSG_RESULT(no)
else
  xpm_includes=$ac_xpm_includes
  xpm_libraries=$ac_xpm_libraries
  if test "x$xpm_libraries" = x ; then
    XPM_LIBS="-lXpm"
  else
    XPM_LIBS="-L$xpm_libraries -lXpm"
  fi
  if test "x$xpm_includes" = x ; then
    XPM_CFLAGS=""
  else
    XPM_CFLAGS="-I$xpm_includes"
  fi
  AC_MSG_RESULT([libraries $xpm_libraries, headers $xpm_includes])
fi

AC_SUBST(XPM_LIBS)
AC_SUBST(XPM_CFLAGS)
])dnl


# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN([AM_INIT_AUTOMAKE],
[AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
AC_REQUIRE([AC_PROG_INSTALL])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package]))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal-${am__api_version}, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake-${am__api_version}, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_REQUIRE([AC_PROG_MAKE_SET])])

# Copyright 2002  Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA

# AM_AUTOMAKE_VERSION(VERSION)
# ----------------------------
# Automake X.Y traces this macro to ensure aclocal.m4 has been
# generated from the m4 files accompanying Automake X.Y.
AC_DEFUN([AM_AUTOMAKE_VERSION],[am__api_version="1.4"])

# AM_SET_CURRENT_AUTOMAKE_VERSION
# -------------------------------
# Call AM_AUTOMAKE_VERSION so it can be traced.
# This function is AC_REQUIREd by AC_INIT_AUTOMAKE.
AC_DEFUN([AM_SET_CURRENT_AUTOMAKE_VERSION],
	 [AM_AUTOMAKE_VERSION([1.4-p6])])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN([AM_SANITY_CHECK],
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN([AM_MISSING_PROG],
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])


# serial 1

# @defmac AC_PROG_CC_STDC
# @maindex PROG_CC_STDC
# @ovindex CC
# If the C compiler in not in ANSI C mode by default, try to add an option
# to output variable @code{CC} to make it so.  This macro tries various
# options that select ANSI C on some system or another.  It considers the
# compiler to be in ANSI C mode if it handles function prototypes correctly.
#
# If you use this macro, you should check after calling it whether the C
# compiler has been set to accept ANSI C; if not, the shell variable
# @code{am_cv_prog_cc_stdc} is set to @samp{no}.  If you wrote your source
# code in ANSI C, you can make an un-ANSIfied copy of it by using the
# program @code{ansi2knr}, which comes with Ghostscript.
# @end defmac

AC_DEFUN([AM_PROG_CC_STDC],
[AC_REQUIRE([AC_PROG_CC])
AC_BEFORE([$0], [AC_C_INLINE])
AC_BEFORE([$0], [AC_C_CONST])
dnl Force this before AC_PROG_CPP.  Some cpp's, eg on HPUX, require
dnl a magic option to avoid problems with ANSI preprocessor commands
dnl like #elif.
dnl FIXME: can't do this because then AC_AIX won't work due to a
dnl circular dependency.
dnl AC_BEFORE([$0], [AC_PROG_CPP])
AC_MSG_CHECKING(for ${CC-cc} option to accept ANSI C)
AC_CACHE_VAL(am_cv_prog_cc_stdc,
[am_cv_prog_cc_stdc=no
ac_save_CC="$CC"
# Don't try gcc -ansi; that turns off useful extensions and
# breaks some systems' header files.
# AIX			-qlanglvl=ansi
# Ultrix and OSF/1	-std1
# HP-UX			-Aa -D_HPUX_SOURCE
# SVR4			-Xc -D__EXTENSIONS__
for ac_arg in "" -qlanglvl=ansi -std1 "-Aa -D_HPUX_SOURCE" "-Xc -D__EXTENSIONS__"
do
  CC="$ac_save_CC $ac_arg"
  AC_TRY_COMPILE(
[#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
/* Most of the following tests are stolen from RCS 5.7's src/conf.sh.  */
struct buf { int x; };
FILE * (*rcsopen) (struct buf *, struct stat *, int);
static char *e (p, i)
     char **p;
     int i;
{
  return p[i];
}
static char *f (char * (*g) (char **, int), char **p, ...)
{
  char *s;
  va_list v;
  va_start (v,p);
  s = g (p, va_arg (v,int));
  va_end (v);
  return s;
}
int test (int i, double x);
struct s1 {int (*f) (int a);};
struct s2 {int (*f) (double a);};
int pairnames (int, char **, FILE *(*)(struct buf *, struct stat *, int), int, int);
int argc;
char **argv;
], [
return f (e, argv, 0) != argv[0]  ||  f (e, argv, 1) != argv[1];
],
[am_cv_prog_cc_stdc="$ac_arg"; break])
done
CC="$ac_save_CC"
])
if test -z "$am_cv_prog_cc_stdc"; then
  AC_MSG_RESULT([none needed])
else
  AC_MSG_RESULT($am_cv_prog_cc_stdc)
fi
case "x$am_cv_prog_cc_stdc" in
  x|xno) ;;
  *) CC="$CC $am_cv_prog_cc_stdc" ;;
esac
])

# Configure paths for GTK+
# Owen Taylor     97-11-3

dnl AM_PATH_GTK([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl Test for GTK, and define GTK_CFLAGS and GTK_LIBS
dnl
AC_DEFUN(AM_PATH_GTK,
[dnl 
dnl Get the cflags and libraries from the gtk-config script
dnl
AC_ARG_WITH(gtk-prefix,[  --with-gtk-prefix=PFX   Prefix where GTK is installed (optional)],
            gtk_config_prefix="$withval", gtk_config_prefix="")
AC_ARG_WITH(gtk-exec-prefix,[  --with-gtk-exec-prefix=PFX Exec prefix where GTK is installed (optional)],
            gtk_config_exec_prefix="$withval", gtk_config_exec_prefix="")
AC_ARG_ENABLE(gtktest, [  --disable-gtktest       Do not try to compile and run a test GTK program],
		    , enable_gtktest=yes)

  for module in . $4
  do
      case "$module" in
         gthread) 
             gtk_config_args="$gtk_config_args gthread"
         ;;
      esac
  done

  if test x$gtk_config_exec_prefix != x ; then
     gtk_config_args="$gtk_config_args --exec-prefix=$gtk_config_exec_prefix"
     if test x${GTK_CONFIG+set} != xset ; then
        GTK_CONFIG=$gtk_config_exec_prefix/bin/gtk-config
     fi
  fi
  if test x$gtk_config_prefix != x ; then
     gtk_config_args="$gtk_config_args --prefix=$gtk_config_prefix"
     if test x${GTK_CONFIG+set} != xset ; then
        GTK_CONFIG=$gtk_config_prefix/bin/gtk-config
     fi
  fi

  AC_PATH_PROG(GTK_CONFIG, gtk-config, no)
  min_gtk_version=ifelse([$1], ,0.99.7,$1)
  AC_MSG_CHECKING(for GTK - version >= $min_gtk_version)
  no_gtk=""
  if test "$GTK_CONFIG" = "no" ; then
    no_gtk=yes
  else
    GTK_CFLAGS=`$GTK_CONFIG $gtk_config_args --cflags`
    GTK_LIBS=`$GTK_CONFIG $gtk_config_args --libs`
    gtk_config_major_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    gtk_config_minor_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    gtk_config_micro_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_gtktest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $GTK_CFLAGS"
      LIBS="$GTK_LIBS $LIBS"
dnl
dnl Now check if the installed GTK is sufficiently new. (Also sanity
dnl checks the results of gtk-config to some extent
dnl
      rm -f conf.gtktest
      AC_TRY_RUN([
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int 
main ()
{
  int major, minor, micro;
  char *tmp_version;

  system ("touch conf.gtktest");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = g_strdup("$min_gtk_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_gtk_version");
     exit(1);
   }

  if ((gtk_major_version != $gtk_config_major_version) ||
      (gtk_minor_version != $gtk_config_minor_version) ||
      (gtk_micro_version != $gtk_config_micro_version))
    {
      printf("\n*** 'gtk-config --version' returned %d.%d.%d, but GTK+ (%d.%d.%d)\n", 
             $gtk_config_major_version, $gtk_config_minor_version, $gtk_config_micro_version,
             gtk_major_version, gtk_minor_version, gtk_micro_version);
      printf ("*** was found! If gtk-config was correct, then it is best\n");
      printf ("*** to remove the old version of GTK+. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If gtk-config was wrong, set the environment variable GTK_CONFIG\n");
      printf("*** to point to the correct copy of gtk-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    } 
#if defined (GTK_MAJOR_VERSION) && defined (GTK_MINOR_VERSION) && defined (GTK_MICRO_VERSION)
  else if ((gtk_major_version != GTK_MAJOR_VERSION) ||
	   (gtk_minor_version != GTK_MINOR_VERSION) ||
           (gtk_micro_version != GTK_MICRO_VERSION))
    {
      printf("*** GTK+ header files (version %d.%d.%d) do not match\n",
	     GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
      printf("*** library (version %d.%d.%d)\n",
	     gtk_major_version, gtk_minor_version, gtk_micro_version);
    }
#endif /* defined (GTK_MAJOR_VERSION) ... */
  else
    {
      if ((gtk_major_version > major) ||
        ((gtk_major_version == major) && (gtk_minor_version > minor)) ||
        ((gtk_major_version == major) && (gtk_minor_version == minor) && (gtk_micro_version >= micro)))
      {
        return 0;
       }
     else
      {
        printf("\n*** An old version of GTK+ (%d.%d.%d) was found.\n",
               gtk_major_version, gtk_minor_version, gtk_micro_version);
        printf("*** You need a version of GTK+ newer than %d.%d.%d. The latest version of\n",
	       major, minor, micro);
        printf("*** GTK+ is always available from ftp://ftp.gtk.org.\n");
        printf("***\n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the gtk-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of GTK+, but you can also set the GTK_CONFIG environment to point to the\n");
        printf("*** correct copy of gtk-config. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
  return 1;
}
],, no_gtk=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_gtk" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$GTK_CONFIG" = "no" ; then
       echo "*** The gtk-config script installed by GTK could not be found"
       echo "*** If GTK was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the GTK_CONFIG environment variable to the"
       echo "*** full path to gtk-config."
     else
       if test -f conf.gtktest ; then
        :
       else
          echo "*** Could not run GTK test program, checking why..."
          CFLAGS="$CFLAGS $GTK_CFLAGS"
          LIBS="$LIBS $GTK_LIBS"
          AC_TRY_LINK([
#include <gtk/gtk.h>
#include <stdio.h>
],      [ return ((gtk_major_version) || (gtk_minor_version) || (gtk_micro_version)); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding GTK or finding the wrong"
          echo "*** version of GTK. If it is not finding GTK, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***"
          echo "*** If you have a RedHat 5.0 system, you should remove the GTK package that"
          echo "*** came with the system with the command"
          echo "***"
          echo "***    rpm --erase --nodeps gtk gtk-devel" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means GTK was incorrectly installed"
          echo "*** or that you have moved GTK since it was installed. In the latter case, you"
          echo "*** may want to edit the gtk-config script: $GTK_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     GTK_CFLAGS=""
     GTK_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(GTK_CFLAGS)
  AC_SUBST(GTK_LIBS)
  rm -f conf.gtktest
])

