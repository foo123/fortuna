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
#include <boost/asio/signal_set.hpp>

#include <libfortuna/accumulator.hpp>

#include <libfortuna_daemon/connection_info.hpp>

#include "connection_manager.hpp"


namespace fortuna_daemon {

class Session;


class Server
{
public:
    struct Config
    {
        ConnectionInfo connection_info;

        Config()
        {}
    };

    struct AllConfig
    {
        Config server;
        fortuna::Accumulator::AllConfig accumulator;

        AllConfig()
        {}
    };

private:
    const Config config;

    boost::asio::io_service io_service;
    boost::asio::signal_set signals;
    boost::asio::local::stream_protocol::acceptor acceptor;
    boost::asio::local::stream_protocol::socket socket;
    ConnectionManager connection_manager;

    fortuna::Accumulator accumulator;

public:
    Server(AllConfig all_config = AllConfig{});
    ~Server() noexcept;

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void run();

private:
    void do_accept();
    void do_await_stop();
};


} // namespace fortuna_daemon

#endif // FORTUNAD_SERVER_HPP
