### Copyright (C) 2012 Juan Pablo Carbajal
###
### This file is part of Dwengo.
###
### Octave is free software; you can redistribute it and/or modify it
### under the terms of the GNU General Public License as published by the
### Free Software Foundation; either version 3 of the License, or (at
### your option) any later version.
###
### Octave is distributed in the hope that it will be useful, but WITHOUT
### ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
### FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
### for more details.
###
### You should have received a copy of the GNU General Public License
### along with Octave; see the file COPYING.  If not, see
### <http://www.gnu.org/licenses/>.

AC_PREREQ([2.65])

AC_INIT([libdwengo], [1.0.0], [info@dwengo.org],, [www.dwengo.org])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([lib/dwengoBoard.c])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_LIBOBJ_DIR([lib])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

### Configure variables: MCC_BIN and MCC_INCLUDE.
AC_ARG_VAR([MCC_BIN], [The PATH wherein mcc18 and mplib can be found.])
AC_ARG_VAR([MCC_INCLUDE], [The PATH wherein p18f4550.h can be found.])

# Ensure that both or neither are specified.
if (test -n "$MCC_BIN" && test -z "$MCC_INCLUDE") || \
   (test -n "$MCC_INCLUDE" && test -z "$MCC_BIN"); then
  AC_MSG_ERROR([Specify both MCC18 and include paths, or neither.])
fi

#Search for location of mcc18
if test -z "$MCC_BIN"; then
  true
  # TODO: Search for mcc18, mplib and includes
fi

AC_ARG_VAR([MCCFLAGS], [The FLAGS for mcc18.])
if test -z "$MCCFLAGS"; then
  AC_SUBST([MCCFLAGS],["-ls --no-extended -ml -p=18f4550"])
fi

# Checks for programs.
AC_PROG_INSTALL
AC_PROG_RANLIB

AC_CHECK_PROG([MCC_C], [mcc18], [${MCC_BIN}/mcc18],[no],[${MCC_BIN}])
### Check version number of mcc18.and database
MCCC_VERSION=
if test "$MCC_C" != no; then
  mcc18_version=`$MCC_C -v 2>&1 | grep "^MPLAB C18" | \
    sed -e 's/^MPLAB C18 v*//' -e 's/Copy.*$//'`

  devicedb_version=`$MCC_C -v 2>&1 | grep "^Device Database" | \
    sed -e 's/^Device Database version //'`

  AS_VERSION_COMPARE([$mcc18_version], [3.8],
    [AC_MSG_ERROR([mcc18 version $mcc18_version will probably fail to compile libdwengo])])

  AS_VERSION_COMPARE([$devicedb_version], [1.3],
    [AC_MSG_ERROR([Device database version $devicedb_version will probably not work with libdwengo])])

  MCCC_VERSION=$mcc18_version
  DEVDB_VERSION=$devicedb_version
fi
AC_SUBST(MCCC_VERSION)
AC_SUBST(DEVDB_VERSION)

AC_CHECK_PROG([MCC_L], [mplib], [${MCC_BIN}/mplib],[no],[${MCC_BIN}])
### Check version number of mplib.
MCCL_VERSION=
if test "$MCC_L" != no; then
  mplib_version=`$MCC_L -v 2>&1 | grep "^MPLIB " | \
    sed -e 's/^MPLIB //' -e 's/, Librarian$//'`

  AS_VERSION_COMPARE([$mplib_version], [4.40],
    [AC_MSG_ERROR([mplib version $mplib_version will probably fail to compile libdwengo])])

  MCCL_VERSION=$mplib_version
fi
AC_SUBST(MCCL_VERSION)

# Define library construction
AC_CONFIG_FILES([Makefile lib/Makefile])

AC_OUTPUT

echo \
"-------------------------------------------------
 ${PACKAGE_NAME} Version ${PACKAGE_VERSION}
 Prefix........: $prefix
 Include folder: $MCC_INCLUDE

 Compiler........: $MCC_C
 Compiler flags..: $MCCFLAGS
 Compiler.version: $MCCC_VERSION

 Librarian........: $MCC_L
 Librarian.version: $MCCL_VERSION

 Device database version: $DEVDB_VERSION

 Now type 'make @<:@<target>@:>@'
   where the optional <target> is:
     all                - build all binaries
     install            - install everything
--------------------------------------------------"
