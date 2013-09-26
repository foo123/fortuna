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

#include <exception>

#include "application.hpp"
#include "options.hpp"
#include "stdex/cstdlib.hpp"


int main(int argc, char* argv[])
{
    using namespace fortuna_daemon;
    try {
        Application(handle_options(argc, argv)).run();
    }
    catch (std::exception& e) {
        stdex::die( e.what() );
    }
    return 0;
}