# Copyright 2013 Adam Mizerski <adam@mizerski.pl>
# 
# This file is part of libfortuna.
# 
# libfortuna is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# libfortuna is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with libfortuna.  If not, see <http://www.gnu.org/licenses/>.

ACLOCAL_AMFLAGS = -I m4


stdex_SOURCES = \
    stdex/algorithm.hpp \
    stdex/cstdlib.hpp



lib_LTLIBRARIES = libfortuna.la

libfortuna_la_SOURCES = \
    fortuna/accumulator.cpp \
    fortuna/fortuna_exception.cpp \
    fortuna/generator.cpp \
    fortuna/pool.cpp

libfortuna_la_includedir = $(includedir)/fortuna
libfortuna_la_include_HEADERS = \
    fortuna/accumulator.hpp \
    fortuna/fortuna_exception.hpp \
    fortuna/generator.hpp \
    fortuna/noncopyable.hpp \
    fortuna/pool.hpp

libfortuna_la_CPPFLAGS = \
    $(CRYPTOPP_CPPFLAGS)

libfortuna_la_LDFLAGS = \
    $(CRYPTOPP_LDFLAGS) \
    -version-info $(LIBFORTUNA_LT_VERSION)

libfortuna_la_LIBADD = \
    -lcryptopp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pc/fortuna.pc



bin_PROGRAMS = fortunad

fortunad_SOURCES = \
    daemon/application.cpp \
    daemon/application.hpp \
    daemon/main.cpp \
    daemon/server.cpp \
    daemon/server.hpp \
    daemon/session.cpp \
    daemon/session.hpp

fortunad_CPPFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(CRYPTOPP_CPPFLAGS)

fortunad_LDFLAGS = \
    $(BOOST_LDFLAGS) \
    $(CRYPTOPP_LDFLAGS)

fortunad_LDADD = \
    $(BOOST_ASIO_LIB) \
    $(BOOST_PROGRAM_OPTIONS_LIB) \
    $(BOOST_SYSTEM_LIB) \
    $(BOOST_THREAD_LIB) \
    -lcryptopp \
    libfortuna.la
