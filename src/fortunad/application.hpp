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

#ifndef FORTUNAD_APPLICATION_HPP
#define FORTUNAD_APPLICATION_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>

#include "server.hpp"


namespace fortuna_daemon {


class Application
    : boost::noncopyable
{
public:
    struct Config
    {
        bool default_threads_num = true;
        unsigned custom_threads_num = 0;

        constexpr
        Config()
        {}
    };

    struct AllConfig
        : public Server::AllConfig
    {
        Config application;

        AllConfig()
        {}
    };

private:
    const Config config;

    boost::asio::io_service io_service;
    boost::asio::signal_set signals;
    boost::thread_group threads;

    Server server;

public:
    Application(AllConfig all_config = AllConfig{});

    void run();
};


} // namespace fortuna_daemon

#endif // FORTUNAD_APPLICATION_HPP
