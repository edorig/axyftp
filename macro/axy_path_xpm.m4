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

