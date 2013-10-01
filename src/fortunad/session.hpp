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

#ifndef FORTUNAD_SESSION_HPP
#define FORTUNAD_SESSION_HPP

#include <memory>

#include <boost/asio/local/stream_protocol.hpp>
#include <boost/noncopyable.hpp>

#include <cryptopp/secblock.h>

namespace fortuna {
    class Accumulator;
}


namespace fortuna_daemon {


class Session
    : public std::enable_shared_from_this<Session>
    , boost::noncopyable
{
private:
    boost::asio::local::stream_protocol::socket socket;
    CryptoPP::SecByteBlock buffer;

    fortuna::Accumulator& accumulator;

public:
    Session(boost::asio::io_service& io_service, fortuna::Accumulator& acc);

    boost::asio::local::stream_protocol::socket& get_socket()
    { return socket; }

    void start();

private:
    /**
     * Starts async read of data from socket to buffer.
     * \param offset offset in buffer.
     * \param length bytes to read.
     * \param handler handler to call when data is read.
     */
    void async_read(std::size_t offset, std::size_t length, void (Session::*handler)(const boost::system::error_code& error));

    /**
     * Starts async write of data from buffer to socket.
     * \param offset offset in buffer.
     * \param length bytes to write.
     * \param handler handler to call when data is written.
     */
    void async_write(std::size_t offset, std::size_t length, void (Session::*handler)(const boost::system::error_code& error));


    /**
     * Read first byte.
     */
    void async_read_command();
    void handle_read_command(const boost::system::error_code& error);

    /**
     * Read pool number, source number, data size.
     */
    void async_read_random_event_header();
    void handle_read_random_event_header(const boost::system::error_code& error);

    /**
     * \pre data size is in buffer[2].
     */
    void async_read_random_event_data();
    void handle_read_random_event_data(const boost::system::error_code& error);

    void async_read_request_length();
    void handle_read_request_length(const boost::system::error_code& error);

    void async_write_random_data(unsigned long size);
    void handle_write_random_data(const boost::system::error_code& error);

    void async_write_generator_not_seeded();
    void handle_write_generator_not_seeded(const boost::system::error_code& error);

    void async_write_request_too_big();
    void handle_write_request_too_big(const boost::system::error_code& error);
};


} // namespace fortuna_daemon

#endif // FORTUNAD_SESSION_HPP
