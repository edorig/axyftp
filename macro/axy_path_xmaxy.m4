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

