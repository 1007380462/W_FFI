dnl $Id$
dnl config.m4 for extension W_FFI

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(W_FFI, for W_FFI support,
dnl Make sure that the comment is aligned:
dnl [  --with-W_FFI             Include W_FFI support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(W_FFI, whether to enable W_FFI support,
 [  --enable-W_FFI           Enable W_FFI support])

if test "$PHP_W_FFI" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-W_FFI -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/W_FFI.h"  # you most likely want to change this
  dnl if test -r $PHP_W_FFI/$SEARCH_FOR; then # path given as parameter
  dnl   W_FFI_DIR=$PHP_W_FFI
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for W_FFI files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       W_FFI_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$W_FFI_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the W_FFI distribution])
  dnl fi

  dnl # --with-W_FFI -> add include path
  dnl PHP_ADD_INCLUDE($W_FFI_DIR/include)

  dnl # --with-W_FFI -> check for lib and symbol presence
  dnl LIBNAME=W_FFI # you may want to change this
  dnl LIBSYMBOL=W_FFI # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $W_FFI_DIR/$PHP_LIBDIR, W_FFI_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_W_FFILIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong W_FFI lib version or lib not found])
  dnl ],[
  dnl   -L$W_FFI_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(W_FFI_SHARED_LIBADD)

  PHP_NEW_EXTENSION(W_FFI,W_MAIN.c W_FFI.c W_FUNC.c, $ext_shared)
fi
