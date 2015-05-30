dnl $Id$
dnl config.m4 for extension dlist

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(dlist, for dlist support,
Make sure that the comment is aligned:
[  --with-dlist             Include dlist support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(dlist, whether to enable dlist support,
dnl Make sure that the comment is aligned:
dnl [  --enable-dlist           Enable dlist support])

if test "$PHP_DLIST" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-dlist -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/dlist.h"  # you most likely want to change this
  dnl if test -r $PHP_DLIST/$SEARCH_FOR; then # path given as parameter
  dnl   DLIST_DIR=$PHP_DLIST
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for dlist files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       DLIST_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$DLIST_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the dlist distribution])
  dnl fi

  dnl # --with-dlist -> add include path
  dnl PHP_ADD_INCLUDE($DLIST_DIR/include)

  dnl # --with-dlist -> check for lib and symbol presence
  dnl LIBNAME=dlist # you may want to change this
  dnl LIBSYMBOL=dlist # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $DLIST_DIR/lib, DLIST_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_DLISTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong dlist lib version or lib not found])
  dnl ],[
  dnl   -L$DLIST_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(DLIST_SHARED_LIBADD)

  PHP_NEW_EXTENSION(dlist, dlist.c, $ext_shared)
fi
