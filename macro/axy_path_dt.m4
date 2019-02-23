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

