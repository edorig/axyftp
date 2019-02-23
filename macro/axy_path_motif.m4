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

