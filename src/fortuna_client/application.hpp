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

#ifndef FORTUNA_CLIENT_APPLICATION_HPP
#define FORTUNA_CLIENT_APPLICATION_HPP

#include <iosfwd>
#include <string>

#include <cryptopp/secblock.h>

#include <libfortuna_daemon/client.hpp>


namespace fortuna_client {

typedef unsigned long long ullong;


class Application
{
public:
    struct Config
    {
        ullong length;
        std::string outfilename {"-"};

        Config()
        {}
    };

    struct AllConfig
    {
        Config application;
        fortuna_daemon::Client::Config client;

        AllConfig()
        {}
    };

private:
    const Config config;

    fortuna_daemon::Client fortuna;

    CryptoPP::SecByteBlock buffer;

public:
    Application(AllConfig all_config);

    void run();

private:
    void write_data_to_stream(std::ostream& ostream);
    void write_chunk_of_data_to_stream(std::ostream& ostream, std::size_t length);
};


} // namespace fortuna_client

#endif // FORTUNA_CLIENT_APPLICATION_HPP
