/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna.

libfortuna_daemon is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libfortuna_daemon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libfortuna_daemon.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "client.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/local/stream_protocol.hpp>

#include <boost/noncopyable.hpp>

#include <cryptopp/secblock.h>


namespace fortuna_daemon {


class Client::Impl
    : boost::noncopyable
{
private:
    boost::asio::io_service io_service;
    boost::asio::local::stream_protocol::socket socket;
    CryptoPP::FixedSizeSecBlock<byte, 1+sizeof(std::size_t)> buffer;

public:
    Impl(Config&& config);

private:
    void connect_socket(const ConnectionInfo& connection_info);
    void prepare_header();

public:
    void get_random_data(byte* data, std::size_t length);

private:
    void send_request(std::size_t length);
    byte receive_status();
    void receive_data(byte* data, std::size_t length);
};


} // namespace fortunad
