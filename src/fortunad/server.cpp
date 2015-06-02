/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna.

fortuna_daemon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fortuna_daemon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fortuna_daemon.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "server.hpp"

#include <unistd.h>
#include <utility>

#include "session.hpp"


namespace fortuna_daemon {


Server::LocalEndpoint::~LocalEndpoint() noexcept
{
    ::unlink(path().c_str());
}


Server::Server(AllConfig all_config)
    : config{std::move(all_config.server)}
    , io_service{}
    , signals{io_service, SIGINT, SIGTERM}
    , endpoint{config.connection_info.socket}
    , acceptor{io_service, endpoint}
    , socket{io_service}
    , accumulator{std::move(all_config.accumulator)}
{
    do_await_stop();
    do_accept();
}

Server::~Server() noexcept
{}


void Server::run()
{
    #pragma omp parallel
    io_service.run();
}

void Server::do_accept()
{
    acceptor.async_accept(
        socket,
        [this](boost::system::error_code ec) {
            if (!acceptor.is_open())
                return;
            if (!ec)
                connection_manager.start(std::make_shared<Session>(std::move(socket), &connection_manager, &accumulator));
            do_accept();
        }
    );
}

void Server::do_await_stop()
{
    signals.async_wait([this](boost::system::error_code /*ec*/, int /*signo*/) {
        acceptor.close();
        connection_manager.stop_all();
    });
}

} // namespace fortuna_daemon
