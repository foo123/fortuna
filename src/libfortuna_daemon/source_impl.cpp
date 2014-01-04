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

#include "source_impl.hpp"

#include <array>

#include <boost/asio/write.hpp>


namespace fortuna_daemon {


Source::Impl::Impl(const Config& config)
    : io_service{}
    , socket{io_service}
    , header{}
{
    socket.connect(boost::asio::local::stream_protocol::endpoint{config.connection_info.socket});
    
    header.BytePtr()[0] = 0x00; // add random event
    header.BytePtr()[1] = 0x00; // pool number
    header.BytePtr()[2] = config.source_number;
}

void Source::Impl::add_random_event(const byte* data, byte length)
{
    if (length == 0 || length > 32)
        throw std::length_error("invalid length");
    
    send_data(data, length);
    update_pool_number();
}

void Source::Impl::send_data(const byte* data, byte length)
{
    header.BytePtr()[3] = length;
    
    std::array<boost::asio::const_buffer,2> buffer = {{
        boost::asio::const_buffer(header, 4),
        boost::asio::const_buffer(data, length)
    }};
    
    boost::asio::write(socket, buffer);
}

void Source::Impl::update_pool_number()
{
    header.BytePtr()[1] = (header.BytePtr()[1] + 1) % 32;
}


} // namespace fortuna_daemon
