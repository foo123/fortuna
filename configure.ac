#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

define(LIBFORTUNA_COPYRIGHT, [[
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of libfortuna.

libfortuna is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libfortuna is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libfortuna.  If not, see <http://www.gnu.org/licenses/>.
]])

AC_COPYRIGHT(LIBFORTUNA_COPYRIGHT)
AH_TOP(/*LIBFORTUNA_COPYRIGHT*/)


AC_PREREQ([2.69])
AC_INIT([fortuna], [0.1.0])

LIBFORTUNA_LT_VERSION=0:0:0
LIBFORTUNA_DAEMON_CLIENT_LT_VERSION=0:0:0
LIBFORTUNA_DAEMON_SOURCE_LT_VERSION=0:0:0
AC_SUBST(LIBFORTUNA_LT_VERSION)
AC_SUBST(LIBFORTUNA_DAEMON_CLIENT_LT_VERSION)
AC_SUBST(LIBFORTUNA_DAEMON_SOURCE_LT_VERSION)

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([libfortuna/accumulator.hpp])
AC_CONFIG_HEADERS([config.h])

# Automake
AM_INIT_AUTOMAKE([foreign -Wall no-dist-gzip dist-bzip2 silent-rules])
AM_PROG_AR

# Libtool
LT_INIT([disable-static])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL


AC_LANG([C++])

# Checks for libraries.
AX_BOOST_BASE([], [], AC_MSG_ERROR([Boost not found]))
AX_BOOST_ASIO
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_SYSTEM
AX_BOOST_THREAD

AX_CHECK_LIBRARY([CRYPTOPP], [cryptopp/config.h], [cryptopp], [], [AC_MSG_ERROR([Unable to find CryptoPP])])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AX_CXX_COMPILE_STDCXX_11([noext])

# Checks for library functions.


# Output
AC_CONFIG_FILES([
    Makefile
    pc/fortuna.pc
    pc/fortuna_daemon_client.pc
    pc/fortuna_daemon_source.pc
])
AC_OUTPUT
