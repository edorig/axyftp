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

