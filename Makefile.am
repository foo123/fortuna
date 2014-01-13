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

AM_CPPFLAGS = -I$(top_srcdir)/src



lib_LTLIBRARIES += libfortuna.la

libfortuna_la_SOURCES = \
    src/libfortuna/accumulator.cpp \
    src/libfortuna/fortuna_exception.cpp \
    src/libfortuna/generator.cpp \
    src/libfortuna/pool.cpp

libfortuna_la_includedir = $(includedir)/fortuna
libfortuna_la_include_HEADERS = \
    src/libfortuna/accumulator.hpp \
    src/libfortuna/fortuna_exception.hpp \
    src/libfortuna/generator.hpp \
    src/libfortuna/monitor.hpp \
    src/libfortuna/pool.hpp


libfortuna_la_CPPFLAGS = \
    $(CRYPTOPP_CPPFLAGS) \
    $(OPENMP_CXXFLAGS) \
    $(AM_CPPFLAGS)

libfortuna_la_LDFLAGS = \
    $(CRYPTOPP_LDFLAGS) \
    $(OPENMP_CXXFLAGS) \
    -version-info $(LIBFORTUNA_LT_VERSION)

libfortuna_la_LIBADD = \
    -lcryptopp

pkgconfig_DATA += pc/fortuna.pc



lib_LTLIBRARIES += libfortuna_daemon_client.la

libfortuna_daemon_client_la_SOURCES = \
    src/libfortuna_daemon/client.cpp \
    src/libfortuna_daemon/client_exception.cpp \
    src/libfortuna_daemon/client_impl.hpp \
    src/libfortuna_daemon/client_impl.cpp

libfortuna_daemon_client_la_includedir = $(includedir)/fortuna_daemon
libfortuna_daemon_client_la_include_HEADERS = \
    src/libfortuna_daemon/client.hpp \
    src/libfortuna_daemon/client_exception.hpp \
    src/libfortuna_daemon/connection_info.hpp

libfortuna_daemon_client_la_CPPFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(AM_CPPFLAGS)

libfortuna_daemon_client_la_LDFLAGS = \
    $(BOOST_LDFLAGS) \
    -version-info $(LIBFORTUNA_DAEMON_CLIENT_LT_VERSION)

libfortuna_daemon_client_la_LIBADD = \
    $(BOOST_ASIO_LIB)

pkgconfig_DATA += pc/fortuna_daemon_client.pc



lib_LTLIBRARIES += libfortuna_daemon_source.la

libfortuna_daemon_source_la_SOURCES = \
    src/libfortuna_daemon/source.cpp \
    src/libfortuna_daemon/source_impl.hpp \
    src/libfortuna_daemon/source_impl.cpp

libfortuna_daemon_source_la_includedir = $(includedir)/fortuna_daemon
libfortuna_daemon_source_la_include_HEADERS = \
    src/libfortuna_daemon/connection_info.hpp \
    src/libfortuna_daemon/source.hpp

libfortuna_daemon_source_la_CPPFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(AM_CPPFLAGS)

libfortuna_daemon_source_la_LDFLAGS = \
    $(BOOST_LDFLAGS) \
    -version-info $(LIBFORTUNA_DAEMON_SOURCE_LT_VERSION)

libfortuna_daemon_source_la_LIBADD = \
    $(BOOST_ASIO_LIB)

pkgconfig_DATA += pc/fortuna_daemon_source.pc



bin_PROGRAMS += fortuna_client

fortuna_client_SOURCES = \
    src/fortuna_client/application.cpp \
    src/fortuna_client/application.hpp \
    src/fortuna_client/main.cpp \
    src/fortuna_client/main.hpp \
    src/fortuna_client/options.cpp \
    src/fortuna_client/options.hpp

fortuna_client_CPPFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(CRYPTOPP_CPPFLAGS) \
    $(AM_CPPFLAGS)

fortuna_client_LDFLAGS = \
    $(BOOST_LDFLAGS) \
    $(CRYPTOPP_LDFLAGS)

fortuna_client_LDADD = \
    $(BOOST_PROGRAM_OPTIONS_LIB) \
    -lcryptopp \
    libfortuna_daemon_client.la



bin_PROGRAMS += fortuna_source_timer

fortuna_source_timer_SOURCES = \
    src/fortuna_source_timer/application.cpp \
    src/fortuna_source_timer/application.hpp \
    src/fortuna_source_timer/main.cpp \
    src/fortuna_source_timer/options.cpp \
    src/fortuna_source_timer/options.hpp \
    src/fortuna_source_timer/sleep_for.hpp

fortuna_source_timer_CPPFLAGS = \
    $(CRYPTOPP_CPPFLAGS) \
    $(BOOST_CPPFLAGS) \
    $(AM_CPPFLAGS)

fortuna_source_timer_LDFLAGS = \
    $(BOOST_LDFLAGS)
#    $(CRYPTOPP_LDFLAGS)

fortuna_source_timer_LDADD = \
    $(BOOST_PROGRAM_OPTIONS_LIB) \
    libfortuna_daemon_source.la
#    -lcryptopp



bin_PROGRAMS += fortunad

fortunad_SOURCES = \
    src/fortunad/application.cpp \
    src/fortunad/application.hpp \
    src/fortunad/main.cpp \
    src/fortunad/main.hpp \
    src/fortunad/options.cpp \
    src/fortunad/options.hpp \
    src/fortunad/server.cpp \
    src/fortunad/server.hpp \
    src/fortunad/session.cpp \
    src/fortunad/session.hpp \
    src/libfortuna_daemon/connection_info.hpp

fortunad_CPPFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(CRYPTOPP_CPPFLAGS) \
    $(AM_CPPFLAGS)

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
