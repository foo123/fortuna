/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna_client.

fortuna_client is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fortuna_client is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fortuna_client.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FORTUNA_DAEMON_DIE_HPP
#define FORTUNA_DAEMON_DIE_HPP

#include <iosfwd>


namespace fortuna_daemon {


void die(const std::string& msg, int status_code = 1);


} // fortuna_daemon

#endif // FORTUNA_DAEMON_DIE_HPP
