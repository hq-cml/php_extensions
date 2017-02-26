dnl $Id$
dnl config.m4 for extension extskel

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(extskel, for extskel support,
dnl Make sure that the comment is aligned:
dnl [  --with-extskel             Include extskel support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(extskel, whether to enable extskel support,
Make sure that the comment is aligned:
[  --enable-extskel           Enable extskel support])

if test "$PHP_EXTSKEL" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-extskel -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/extskel.h"  # you most likely want to change this
  dnl if test -r $PHP_EXTSKEL/$SEARCH_FOR; then # path given as parameter
  dnl   EXTSKEL_DIR=$PHP_EXTSKEL
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for extskel files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       EXTSKEL_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$EXTSKEL_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the extskel distribution])
  dnl fi

  dnl # --with-extskel -> add include path
  dnl PHP_ADD_INCLUDE($EXTSKEL_DIR/include)

  dnl # --with-extskel -> check for lib and symbol presence
  dnl LIBNAME=extskel # you may want to change this
  dnl LIBSYMBOL=extskel # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $EXTSKEL_DIR/lib, EXTSKEL_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_EXTSKELLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong extskel lib version or lib not found])
  dnl ],[
  dnl   -L$EXTSKEL_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(EXTSKEL_SHARED_LIBADD)

  PHP_NEW_EXTENSION(extskel, extskel.c, $ext_shared)
fi
