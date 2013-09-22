/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna.

fortuna_client is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fortuna_client is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fortuna_client.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libfortuna_daemon/client_exception.hpp>

#include "application.hpp"
#include "options.hpp"
#include "stdex/cstdlib.hpp"


int main(int argc, char* argv[])
{
    try {
        using namespace fortuna_client;
        Application(handle_options(argc, argv)).run();
    }
    catch (fortuna_daemon::ClientException& e) {
        stdex::die(e.what(), 2+static_cast<int>(e.msg_id()));
    }
    catch (std::exception& e) {
        stdex::die(e.what());
    }
    return 0;
}
