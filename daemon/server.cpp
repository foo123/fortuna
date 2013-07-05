/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna_daemon.

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

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fortuna/accumulator.hpp>

#include "session.hpp"


Server::Server(boost::asio::io_service& ios, fortuna::Accumulator& acc, Config&& conf)
    : config(std::move(conf))
    , accumulator(acc)
    , io_service(ios)
    , acceptor(ios, boost::asio::local::stream_protocol::endpoint(conf.socket_path))
{
    boost::shared_ptr<Session> new_session;
    create_session(new_session);
}

void Server::create_session(boost::shared_ptr<Session>& new_session)
{
    new_session.reset(new Session(io_service, accumulator));
    acceptor.async_accept(
        new_session->get_socket(),
        boost::bind(
            &Server::handle_accept,
            this,
            new_session,
            boost::asio::placeholders::error
        )
    );
}

void Server::handle_accept(boost::shared_ptr<Session> new_session, const boost::system::error_code& error)
{
    if (!error) {
        new_session->start();
        create_session(new_session);
    }
}
