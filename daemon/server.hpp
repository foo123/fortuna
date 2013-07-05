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

#ifndef FORTUNAD_SERVER_HPP
#define FORTUNAD_SERVER_HPP

#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace fortuna { class Accumulator; }

class Session;


class Server
    : boost::noncopyable
{
public:
    struct Config
    {
        std::string socket_path {"./fortunad.socket"};
    };

private:
    const Config config;

    fortuna::Accumulator& accumulator;

    boost::asio::io_service& io_service;
    boost::asio::local::stream_protocol::acceptor acceptor;

public:
    Server(boost::asio::io_service& ios, fortuna::Accumulator& acc, Config&& conf);

private:
    void create_session(boost::shared_ptr<Session>& new_session);
    void handle_accept(boost::shared_ptr<Session> new_session, const boost::system::error_code& error);
};

#endif // FORTUNAD_SERVER_HPP
