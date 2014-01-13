/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna_source_timer.

fortuna_source_timer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fortuna_source_timer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fortuna_source_timer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.hpp"

#include <cstdlib>
#include <iostream>

#include <libfortuna_daemon/source.hpp>

#include "application.hpp"
#include "options.hpp"


volatile
std::sig_atomic_t do_run = 1;

void stop_handler(int) {
    do_run = 0;
}


void die(const std::string& msg, int status_code)
{
    std::cerr << "error: " << msg << std::endl;
    std::exit(status_code);
}


int main(int argc, char* argv[])
{
    using namespace fortuna_source_timer;

    std::signal(SIGINT, stop_handler);
    std::signal(SIGTERM, stop_handler);

    try {
        Application{handle_options(argc, argv)}.run();
    }
    catch (std::exception& e) {
        die(e.what());
    }
    return 0;
}
