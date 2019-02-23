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

