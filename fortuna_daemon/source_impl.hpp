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

#include "source.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/local/stream_protocol.hpp>

#include <cryptopp/secblock.h>


namespace fortuna_daemon {


class Source::Impl
    : boost::noncopyable
{
private:
    boost::asio::io_service io_service;
    boost::asio::local::stream_protocol::socket socket;
    CryptoPP::FixedSizeSecBlock<byte,4> header;

public:
    explicit
    Impl(Config&& config);

    /**
     * \throw std::length_error if length == 0 || length > 32
     */
    void add_random_event(const byte* data, byte length);

private:
    void send_data(const byte* data, byte length);
    void update_pool_number();
};


} // namespace fortuna_daemon
