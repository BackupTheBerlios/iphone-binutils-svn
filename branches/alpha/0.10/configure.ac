##
 # Copyright (C) 2007 iZsh 
 # 
 # This software is free software; you can redistribute it and/or
 # modify it under the terms of the GNU General Public
 # License version 2, as published by the Free Software Foundation.
 #
 # This software is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 #
 # See the GNU General Public License version 2 for more details
 ##

#
#  AM_OPTIONS_IPHONEDMG
#  ------------------------------------------------------------------------
#  Adds the --with-heavenly=path option to the configure options
#
AC_DEFUN([AM_OPTIONS_IPHONEDMG],
    [
        AC_ARG_WITH([heavenly], [AC_HELP_STRING([--with-heavenly=path],  [The location of the iphone's Heavenly directory])],
                [with_heavenly=$withval], [with_heavenly="/Heavenly"])
    ])

#
#  AM_PATH_IPHONE([minimum-version], [action-if-found], [action-if-not-found])
#  ------------------------------------------------------------------------
#
#
AC_DEFUN([AM_PATH_IPHONE],
    [
        HEAVENLY_PREFIX="$with_heavenly"

        AC_CHECK_FILE([$HEAVENLY_PREFIX/$1], [HeavenlyFound="OK"])
        AC_MSG_CHECKING([Wether or not the iPhone's Heavenly directory is installed in $HEAVENLY_PREFIX])

        if [[ -z "$HeavenlyFound" ]]; then
            dnl not found !
            AC_MSG_RESULT([no])
            $3
        else
            dnl found !
            AC_MSG_RESULT([yes])
            $2
        fi
    ])

AC_PREREQ(2.59)
AC_INIT(arm-darwin-toolchain, 0.1, foo@bar.com)

AC_PREFIX_DEFAULT([/Developer/SDKs/iPhone])

# Checks for iphone's directory
AM_OPTIONS_IPHONEDMG
AM_PATH_IPHONE(usr/lib/dyld,,AC_MSG_ERROR([The iPhone's directory is missing.]))

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_YACC
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LEX
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_CHECK_PROGS(wget, [wget curl])
if [[ -z "$wget" ]]; then
  AC_MSG_ERROR([Couldn't find either wget or curl.])
fi
if [[ "$wget" = "wget" ]]; then
  wget="$wget -N"
elif [[ "$wget" = "curl" ]]; then
  wget="$wget -O"
fi

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_MAJOR
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(streams/streams.h,,AC_MSG_ERROR([libstreams's header is missing!]))
AC_CHECK_FILE(/usr/include/ar.h,,AC_MSG_ERROR([ar.h is missing!]))

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_SUBST(wget, $wget)
AC_SUBST(heavenly, $with_heavenly)
AC_CONFIG_FILES([Makefile
                 driver/Makefile
                 driver/arm-cc-specs
                 Csu-71/Makefile
                 libarmfp/Makefile])
AC_OUTPUT
