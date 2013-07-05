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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include <fortuna/accumulator.hpp>

#include "config.hpp"
#include "server.hpp"
#include "options.hpp"


class Application
    : boost::noncopyable
{
private:
    Config config;

    fortuna::Accumulator accumulator;

    boost::asio::io_service io_service;
    boost::asio::signal_set signals;

    Server server;

    boost::thread_group threads;

public:
    Application(int argc, char* argv[], Config&& default_config = Config())
        : config(handle_options(argc, argv, std::move(default_config)))
        , accumulator(std::move(config.fortuna))
        , io_service()
        , signals(io_service)
        , server(io_service, accumulator, std::move(config.server))
        , threads()
    {
        signals.add(SIGINT);
        signals.add(SIGTERM);
        signals.async_wait(boost::bind(&boost::asio::io_service::stop, &io_service));
    }

    void run()
    {
        for (std::size_t i = 0; i < config.threads_count; ++i)
            threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
        threads.join_all();
    }
};


int main(int argc, char* argv[])
{
    try {
        Application app(argc, argv);
        app.run();
    }
    catch (std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
