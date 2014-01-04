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

#include "client_impl.hpp"

#include <boost/asio/detail/socket_ops.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include "client_exception.hpp"


namespace fortuna_daemon {


Client::Impl::Impl(const Config& config)
    : io_service{}
    , socket{io_service}
    , buffer{}
{
    socket.connect(boost::asio::local::stream_protocol::endpoint{config.connection_info.socket});
    
    buffer.BytePtr()[0] = 0x01; // get random data
}


void Client::Impl::get_random_data(byte* data, std::uint32_t length)
{
    send_request(length);
    byte status = receive_status();
    
    switch (status) {
    case 0x00:
        receive_data(data, length);
        break;
    case 0x01:
        throw ClientException::request_too_big();
    case 0x02:
        throw ClientException::generator_is_not_seeded();
    default:
        throw ClientException::illegal_status();
    }
}

void Client::Impl::send_request(std::uint32_t length)
{
    using boost::asio::detail::socket_ops::host_to_network_long;
    
    *reinterpret_cast<std::uint32_t*>(buffer.BytePtr()+1) = host_to_network_long(length);
    boost::asio::write(socket, boost::asio::buffer(buffer, buffer.SizeInBytes()));
}

byte Client::Impl::receive_status()
{
    byte status;
    boost::asio::read(socket, boost::asio::buffer(&status, 1));
    return status;
}

void Client::Impl::receive_data(byte* data, std::uint32_t length)
{
    boost::asio::read(socket, boost::asio::buffer(data, length));
}


} // namespace fortuna_daemon
