AC_PREREQ([2.69])
AC_INIT([split698], [0.1.0], [hlqqlm@qq.com])
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_LANG([C],[C++])

# Check for C++11
AC_LANG_PUSH([C++])
AX_CXX_COMPILE_STDCXX_11([noext])
AC_LANG_POP([C++])

# GCC specific configuration (optional)
GCC_WARN_CXXFLAGS="-fdiagnostics-color=auto -Wall -Wextra -Wpedantic -Wfatal-errors"
GCC_WARN_CXXFLAGS_EXTRA="-Wcast-align -Wcast-qual -Wformat -Wpointer-arith -Wwrite-strings"
# GCC_DEBUG_CXXFLAGS="-g -O0 -fno-omit-frame-pointer -fno-inline"
GCC_DEBUG_CXXFLAGS="-ggdb -O0 -fno-omit-frame-pointer -fno-inline"

# Check for GCC warning flags (optional)
AC_MSG_CHECKING([whether to enable the GCC warnings])
AC_ARG_ENABLE([gcc-warnings],
    [AS_HELP_STRING([--enable-gcc-warnings],
                    [turn on lots of GCC warnings (for developers)])],
    [case "$enableval" in
        yes|no) ;;
        *)      AC_MSG_ERROR([bad value ${enableval} for gcc-warnings option]) ;; 
    esac
    gcc_warnings=$enableval],
    [gcc_warnings=no]
)
AC_MSG_RESULT([$gcc_warnings])
if test "$gcc_warnings" = yes; then
    AC_SUBST([WARN_CXXFLAGS],"$GCC_WARN_CXXFLAGS $GCC_WARN_CXXFLAGS_EXTRA")
fi

# Check for GCC debug flags (optional)
AC_MSG_CHECKING([whether to enable the GCC debug build])
AC_ARG_ENABLE([gcc-debug],
    [AS_HELP_STRING([--enable-gcc-debug],
                    [turn on GCC debug flags (for developers)])],
    [case "$enableval" in
        yes|no) ;;
        *)      AC_MSG_ERROR([bad value ${enableval} for gcc-debug option]) ;; 
    esac
    gcc_debug=$enableval],
    [gcc_debug=no]
)
AC_MSG_RESULT([$gcc_debug])
if test "$gcc_debug" = yes; then
    AC_SUBST([CXXFLAGS],"$GCC_DEBUG_CXXFLAGS")
    AC_DEFINE([DEBUG],[],[Debug Mode])
else
    AC_DEFINE([NDEBUG],[],[Release Mode])
fi

# Configure pthreads.
ACX_PTHREAD([have_pthread=yes])

# Define gtest variables 
GTEST_VERSION="1.7.0"
if test "x$have_pthread" = "xyes"; then
  GTEST_CPPFLAGS="-DGTEST_HAS_PTHREAD=1"
  GTEST_CXXFLAGS="$PTHREAD_CFLAGS"
  GTEST_LDFLAGS=
  GTEST_LIBS="$PTHREAD_LIBS"
else
  GTEST_CPPFLAGS="-DGTEST_HAS_PTHREAD=0"
  GTEST_CXXFLAGS=
  GTEST_LDFLAGS=
  GTEST_LIBS=
fi
AC_SUBST([GTEST_VERSION])
AC_SUBST([GTEST_CPPFLAGS])
AC_SUBST([GTEST_CXXFLAGS])
AC_SUBST([GTEST_LDFLAGS])
AC_SUBST([GTEST_LIBS])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 lib/gtest/Makefile
                 src/Makefile
                 tests/Makefile
                 ])
AC_OUTPUT

AC_MSG_RESULT([
split698 $VERSION is now configured
Configure Information:
  C Compiler        : $CC
    DEFS            :   $DEFS
    CPPFLAGS        :   $CPPFLAGS
    CFLAGS          :   $CFLAGS

  C++ Compiler      : $CXX
    DEFS            :   $DEFS
    CPPFLAGS        :   $CPPFLAGS
    CXXFLAGS        :   $CXXFLAGS

  Linker            : $LD
    LDFLAGS         :   $LDFLAGS
    LIBS            :   $LIBS

  Google Test 
    GTEST_CPPFLAGS  : $GTEST_CPPFLAGS
    GTEST_CXXFLAGS  : $GTEST_CXXFLAGS
    GTEST_LDFLAGS   : $GTEST_LDFLAGS
    GTEST_LIBS      : $GTEST_LIBS
])
