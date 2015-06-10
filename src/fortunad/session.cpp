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

#include "session.hpp"

#include <functional>
#include <iosfwd>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/detail/socket_ops.hpp>

#include <libfortuna/accumulator.hpp>
#include <libfortuna/fortuna_exception.hpp>

#include "connection_manager.hpp"


namespace fortuna_daemon {


static
void log_error(const std::string& msg)
{
    std::cerr << "error: " << msg << std::endl;
}


Session::Session(boost::asio::local::stream_protocol::socket&& _socket, ConnectionManager* _connection_manager, fortuna::Accumulator* _accumulator)
    : std::enable_shared_from_this<Session>{}
    , socket{std::move(_socket)}
    , connection_manager{_connection_manager}
    , accumulator{_accumulator}
    , buffer{}
    , shutting_down{false}
{}

Session::~Session() noexcept
{}


void Session::start()
{
    async_read_command();
}

void Session::stop()
{
    if (socket.is_open()) {
        shutting_down = true;
        socket.shutdown(boost::asio::socket_base::shutdown_both);
        socket.close();
    }
}


template <typename F>
void Session::async_read(std::size_t offset, std::size_t length, F handler)
{
    buffer.Grow(offset + length);
    
    boost::asio::async_read(
        socket,
        boost::asio::buffer(buffer + offset, length),
        [this,handler](boost::system::error_code ec, size_t /*bytes_transferred*/) {
            if (!ec) {
                try {
                    handler();
                } catch (std::exception &e) {
                    log_error(std::string{"unexpected exception thrown by handler: "} + e.what());
                    connection_manager->stop(shared_from_this());
                }
            } else {
                if (ec != boost::asio::error::eof && !(shutting_down && ec == boost::asio::error::operation_aborted))
                    log_error(ec.message());
                if (!shutting_down)
                    connection_manager->stop(shared_from_this());
            }
        }
    );
}

template <typename F>
void Session::async_write(std::size_t offset, std::size_t length, F handler)
{
    boost::asio::async_write(
        socket,
        boost::asio::buffer(buffer + offset, length),
        [this,handler](boost::system::error_code ec, std::size_t /*bytes_transferred*/) {
            if (!ec) {
                try {
                    handler();
                } catch (std::exception &e) {
                    log_error(std::string{"unexpected exception thrown by handler: "} + e.what());
                    connection_manager->stop(shared_from_this());
                }
            } else {
                if (ec != boost::asio::error::eof && !(shutting_down && ec == boost::asio::error::operation_aborted))
                    log_error(ec.message());
                if (!shutting_down)
                    connection_manager->stop(shared_from_this());
            }
        }
    );
}


void Session::async_read_command()
{
    auto self = shared_from_this();
    async_read(0, 1, [this,self]{
        switch (buffer.BytePtr()[0]) {
        case 0x00:
            async_read_random_event_header();
            break;
        case 0x01: // get random data
            async_read_request_length();
            break;
        }
    });
}

void Session::async_read_random_event_header()
{
    auto self = shared_from_this();
    async_read(0, 3, [this,self]{
        if (buffer.BytePtr()[0]/*pool number*/ >= 32
            || buffer.BytePtr()[2]/*data length*/ == 0
            || buffer.BytePtr()[2]/*data length*/ > 32
        ) {
            log_error("header data incorrect");
            connection_manager->stop(self);
            return;
        }
        async_read_random_event_data();
    });
}

void Session::async_read_random_event_data()
{
    auto self = shared_from_this();
    async_read(3, buffer.BytePtr()[2], [this,self]{
        accumulator->add_random_event(
            buffer.BytePtr()[0], /* pool number */
            buffer.BytePtr()[1], /* source number */
            buffer.BytePtr()+3,  /* data */
            buffer.BytePtr()[2]  /* data length */
        );
        async_read_command();
    });
}

void Session::async_read_request_length()
{
    auto self = shared_from_this();
    async_read(0, sizeof(std::uint32_t), [this,self]{
        using boost::asio::detail::socket_ops::network_to_host_long;
        const std::uint32_t length = network_to_host_long(*reinterpret_cast<std::uint32_t*>(buffer.BytePtr()));
        if (length == 0) {
            log_error("length 0 not allowed");
            connection_manager->stop(self);
            return;
        }
        
        const std::size_t blocks_count = fortuna::Accumulator::bytes_to_blocks(length);
        if (fortuna::Generator::is_request_too_big(blocks_count)) {
            async_write_request_too_big();
            return;
        }
        
        buffer.Grow(1 + blocks_count * fortuna::Accumulator::output_block_length);
        try {
            accumulator->get_random_data(buffer + 1, blocks_count);
        } catch (fortuna::FortunaException& e) {
            if (e.get_msg_id() == fortuna::FortunaException::msg_id_t::generator_is_not_seeded) {
                async_write_generator_not_seeded();
                return;
            }
            throw;
        }
        async_write_random_data(length);
    });
}

void Session::async_write_random_data(std::uint32_t length)
{
    buffer.BytePtr()[0] = 0x00;
    
    auto self = shared_from_this();
    async_write(0, 1+length, [this,self]{
        async_read_command();
    });
}

void Session::async_write_request_too_big()
{
    buffer.New(1);
    buffer.BytePtr()[0] = 0x01;
    
    auto self = shared_from_this();
    async_write(0, 1, [this,self]{
        async_read_command();
    });
}

void Session::async_write_generator_not_seeded()
{
    buffer.New(1);
    buffer.BytePtr()[0] = 0x02;
    
    auto self = shared_from_this();
    async_write(0, 1, [this,self]{
        async_read_command();
    });
}


} // namespace fortuna_daemon
