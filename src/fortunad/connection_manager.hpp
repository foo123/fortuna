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

#ifndef FORTUNAD_CONNECTION_MANAGER_HPP
#define FORTUNAD_CONNECTION_MANAGER_HPP

#include <set>

#include "session.hpp"


namespace fortuna_daemon {


class ConnectionManager
{
private:
    std::set<SessionPtr> sessions;

public:
    ConnectionManager() = default;

    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    void start(SessionPtr session);
    void stop(SessionPtr session);
    void stop_all();
};


} // namespace fortuna_daemon

#endif // FORTUNAD_CONNECTION_MANAGER_HPP
