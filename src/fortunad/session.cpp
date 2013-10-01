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

#include <iosfwd>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <libfortuna/accumulator.hpp>
#include <libfortuna/fortuna_exception.hpp>


namespace fortuna_daemon {


static inline
void log_error(const std::string& msg)
{
    std::cerr << "error: " << msg << std::endl;
}


Session::Session(boost::asio::io_service& io_service, fortuna::Accumulator& acc)
    : std::enable_shared_from_this<Session>{}
    , socket{io_service}
    , buffer{}
    , accumulator(acc)
{}

void Session::start()
{
    async_read_command();
}


void Session::async_read(std::size_t offset, std::size_t length, void (Session::*handler)(const boost::system::error_code& error))
{
    if (offset == 0)
        buffer.New(length);
    else
        buffer.Grow(offset + length);
    
    boost::asio::async_read(
        socket,
        boost::asio::buffer(buffer + offset, length),
        boost::bind(
            handler,
            shared_from_this(),
            boost::asio::placeholders::error
        )
    );
}

void Session::async_write(std::size_t offset, std::size_t length, void (Session::*handler)(const boost::system::error_code& error))
{
    boost::asio::async_write(
        socket,
        boost::asio::buffer(buffer + offset, length),
        boost::bind(
            handler,
            shared_from_this(),
            boost::asio::placeholders::error
        )
    );
}


void Session::async_read_command()
{
    async_read(0, 1, &Session::handle_read_command);
}

void Session::handle_read_command(const boost::system::error_code& error)
{
    if (error) {
        log_error(error.message());
        return;
    }
    
    switch (buffer.BytePtr()[0]) {
    case 0x00:
        async_read_random_event_header();
        break;
    case 0x01: // get random data
        async_read_request_length();
        break;
    }
}


void Session::async_read_random_event_header()
{
    async_read(0, 3, &Session::handle_read_random_event_header);
}

void Session::handle_read_random_event_header(const boost::system::error_code& error)
{
    if (error) {
        log_error(error.message());
        return;
    }
    if (buffer.BytePtr()[0]/*pool number*/ >= 32
        || buffer.BytePtr()[2]/*data length*/ == 0
        || buffer.BytePtr()[2]/*data length*/ > 32
    ) {
        log_error("header data incorrect");
        return;
    }
    
    async_read_random_event_data();
}


void Session::async_read_random_event_data()
{
    async_read(3, buffer.BytePtr()[2], &Session::handle_read_random_event_data);
}

void Session::handle_read_random_event_data(const boost::system::error_code& error)
{
    if (error) {
        log_error(error.message());
        return;
    }
    
    accumulator.add_random_event(
        buffer.BytePtr()[0], /* pool number */
        buffer.BytePtr()[1], /* source number */
        buffer.BytePtr()+3,  /* data */
        buffer.BytePtr()[2]  /* data length */
    );
    
    async_read_command();
}


void Session::async_read_request_length()
{
    async_read(0, sizeof(std::size_t), &Session::handle_read_request_length);
}

void Session::handle_read_request_length(const boost::system::error_code& error)
{
    if (error) {
        log_error(error.message());
        return;
    }
    
    const unsigned long length = *reinterpret_cast<std::size_t*>(buffer.BytePtr());
    if (length == 0) {
        log_error("length 0 not allowed");
        return;
    }
    
    try {
        async_write_random_data(length);
    } catch (fortuna::FortunaException& e) {
        if (e.get_msg_id() == fortuna::FortunaException::msg_id_t::request_length_too_big)
            async_write_request_too_big();
        if (e.get_msg_id() == fortuna::FortunaException::msg_id_t::generator_is_not_seeded)
            async_write_generator_not_seeded();
    }
}


void Session::async_write_random_data(unsigned long length)
{
    const unsigned long blocks_count = fortuna::Accumulator::bytes_to_blocks(length);
    buffer.New(1 + blocks_count * fortuna::Accumulator::output_block_length);
    buffer.BytePtr()[0] = 0x00;
    accumulator.get_random_data(buffer + 1, blocks_count);
    
    async_write(0, 1+length, &Session::handle_write_random_data);
}

void Session::handle_write_random_data(const boost::system::error_code& error)
{
    if (error) {
        log_error(error.message());
        return;
    }
    
    async_read_command();
}

void Session::async_write_request_too_big()
{
    buffer.New(1);
    buffer.BytePtr()[0] = 0x01;
    async_write(0, 1, &Session::handle_write_request_too_big);
}

void Session::handle_write_request_too_big(const boost::system::error_code& error)
{
    if (error) {
        log_error(error.message());
        return;
    }
    
    async_read_command();
}

void Session::async_write_generator_not_seeded()
{
    buffer.New(1);
    buffer.BytePtr()[0] = 0x02;
    async_write(0, 1, &Session::handle_write_generator_not_seeded);
}

void Session::handle_write_generator_not_seeded(const boost::system::error_code& error)
{
    if (error) {
        log_error(error.message());
        return;
    }
    
    async_read_command();
}


} // namespace fortuna_daemon
