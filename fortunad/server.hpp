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

#ifndef FORTUNAD_SERVER_HPP
#define FORTUNAD_SERVER_HPP

#include <string>
#include <memory>

#include <boost/asio/io_service.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/noncopyable.hpp>

#include <libfortuna/accumulator.hpp>

#include <libfortuna_daemon/connection_info.hpp>


namespace fortuna_daemon {

class Session;


class Server
    : boost::noncopyable
{
public:
    struct Config
    {
        ConnectionInfo connection_info;
    };

    struct AllConfig
    {
        Config server;
        fortuna::Accumulator::Config accumulator;
    };

private:
    const Config config;

    boost::asio::io_service& io_service;

    fortuna::Accumulator accumulator;

    boost::asio::local::stream_protocol::acceptor acceptor;

public:
    Server(boost::asio::io_service& ios, AllConfig&& all_config = AllConfig());

    void run();

private:
    void create_session(std::shared_ptr<Session>& new_session);
    void handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& error);
};


} // namespace fortuna_daemon

#endif // FORTUNAD_SERVER_HPP
