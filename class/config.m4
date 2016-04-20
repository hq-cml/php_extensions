PHP_ARG_ENABLE(hq,
    [Whether to enable the "hq" extension],
    [  enable-hq        Enable "hq" extension support])

if test $PHP_HQ != "no"; then
    PHP_SUBST(HQ_SHARED_LIBADD)
    PHP_NEW_EXTENSION(hq, hq.c, $ext_shared)
fi
