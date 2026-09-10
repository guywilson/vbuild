AC_PREREQ([2.69])
AC_INIT([vbuild], [1.0], [https://github.com/guywilson/vbuild/issues])
AC_CONFIG_SRCDIR([src/vbuild.cpp])
AC_PROG_CC
AC_PROG_CXX

AC_PATH_PROG([PANDOC], [pandoc])
AS_IF([test -z "$PANDOC"], [
    AC_MSG_ERROR([pandoc is required to build the manual page])
])

AC_SUBST([MAJOR_VERSION], [1])
AC_SUBST([MINOR_VERSION], [0])
AC_CONFIG_FILES([makefile])
AC_OUTPUT