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

#include "application.hpp"

#include <utility>


namespace fortuna_daemon {


Application::Application(Application::AllConfig&& all_config)
    : config(std::move(all_config.application))
    , io_service()
    , signals(io_service, SIGINT, SIGTERM)
    , threads()
    , server(io_service, std::move(static_cast<Server::AllConfig>(all_config)))
{
    signals.async_wait([&](const boost::system::error_code& /*error*/, int /*signal_number*/){
        io_service.stop();
    });
}

void Application::run()
{
    const unsigned threads_num =
        config.default_threads_num
            ? boost::thread::hardware_concurrency()
            : config.custom_threads_num
    ;
    for (unsigned i = 0; i < threads_num; ++i)
        threads.create_thread([&]{
            io_service.run();
        });
    
    threads.join_all();
}


} // namespace fortuna_daemon
