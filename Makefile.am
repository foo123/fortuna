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

lib_LTLIBRARIES =
bin_PROGRAMS =

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA =



lib_LTLIBRARIES += libfortuna.la

libfortuna_la_SOURCES = \
    fortuna/accumulator.cpp \
    fortuna/fortuna_exception.cpp \
    fortuna/generator.cpp \
    fortuna/pool.cpp \
    fortuna/stdex/algorithm.hpp

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

pkgconfig_DATA += pc/fortuna.pc



lib_LTLIBRARIES += libfortuna_daemon_client.la

libfortuna_daemon_client_la_SOURCES = \
    fortuna_daemon/client.cpp \
    fortuna_daemon/client_exception.cpp \
    fortuna_daemon/client_impl.hpp \
    fortuna_daemon/client_impl.cpp

libfortuna_daemon_client_la_includedir = $(includedir)/fortuna_daemon
libfortuna_daemon_client_la_include_HEADERS = \
    fortuna_daemon/client.hpp \
    fortuna_daemon/client_exception.hpp \
    fortuna_daemon/connection_info.hpp

libfortuna_daemon_client_la_CPPFLAGS = \
    $(BOOST_CPPFLAGS)

libfortuna_daemon_client_la_LDFLAGS = \
    $(BOOST_LDFLAGS) \
    -version-info $(LIBFORTUNA_DAEMON_CLIENT_LT_VERSION)

libfortuna_daemon_client_la_LIBADD = \
    $(BOOST_ASIO_LIB)

pkgconfig_DATA += pc/fortuna_daemon_client.pc



lib_LTLIBRARIES += libfortuna_daemon_source.la

libfortuna_daemon_source_la_SOURCES = \
    fortuna_daemon/source.cpp \
    fortuna_daemon/source_impl.hpp \
    fortuna_daemon/source_impl.cpp

libfortuna_daemon_source_la_includedir = $(includedir)/fortuna_daemon
libfortuna_daemon_source_la_include_HEADERS = \
    fortuna_daemon/connection_info.hpp \
    fortuna_daemon/source.hpp

libfortuna_daemon_source_la_CPPFLAGS = \
    $(BOOST_CPPFLAGS)

libfortuna_daemon_source_la_LDFLAGS = \
    $(BOOST_LDFLAGS) \
    -version-info $(LIBFORTUNA_DAEMON_SOURCE_LT_VERSION)

libfortuna_daemon_source_la_LIBADD = \
    $(BOOST_ASIO_LIB)

pkgconfig_DATA += pc/fortuna_daemon_source.pc



bin_PROGRAMS += fortunad

fortunad_SOURCES = \
    fortunad/application.cpp \
    fortunad/application.hpp \
    fortunad/main.cpp \
    fortunad/options.cpp \
    fortunad/options.hpp \
    fortunad/server.cpp \
    fortunad/server.hpp \
    fortunad/session.cpp \
    fortunad/session.hpp \
    fortunad/stdex/cstdlib.hpp \
    fortuna_daemon/connection_info.hpp

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
