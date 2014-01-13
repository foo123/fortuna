/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna_source_timer.

fortuna_source_timer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fortuna_source_timer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fortuna_source_timer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FORTUNA_SOURCE_TIMER_OPTIONS_HPP
#define FORTUNA_SOURCE_TIMER_OPTIONS_HPP

#include <libfortuna_daemon/source.hpp>


namespace fortuna_source_timer {


fortuna_daemon::Source::Config handle_options(int argc, char* argv[]);


} // namespace fortuna_source_timer

#endif // FORTUNA_SOURCE_TIMER_OPTIONS_HPP
