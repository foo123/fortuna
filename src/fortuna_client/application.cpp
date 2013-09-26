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

#include "application.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>


namespace fortuna_client {


Application::Application(AllConfig&& all_config)
    : config{std::move(all_config.application)}
    , fortuna{std::move(all_config.client)}
    , buffer{std::min(static_cast<ullong>(fortuna_daemon::Client::max_request_length), config.length)}
{}

void Application::run()
{
    if (config.outfilename == "-")
        write_data_to_stream(std::cout);
    else {
        std::ofstream outfile{config.outfilename, std::ios_base::binary|std::ios_base::trunc};
        write_data_to_stream(outfile);
    }
}


void Application::write_data_to_stream(std::ostream& ostream)
{
    ullong data_left_to_write = config.length;
    while (data_left_to_write) {
        std::size_t data_to_write_now = std::min(static_cast<ullong>(fortuna_daemon::Client::max_request_length), data_left_to_write);
        write_chunk_of_data_to_stream(ostream, data_to_write_now);
        data_left_to_write -= data_to_write_now;
    }
}

void Application::write_chunk_of_data_to_stream(std::ostream& ostream, std::size_t length)
{
    fortuna.get_random_data(buffer, length);
    ostream.write(reinterpret_cast<char*>(buffer.BytePtr()), length);
}


} // namespace fortuna_client
