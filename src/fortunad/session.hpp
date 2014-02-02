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

#include <cstdint>
#include <memory>

#include <boost/asio/local/stream_protocol.hpp>

#include <cryptopp/secblock.h>

namespace fortuna {
    class Accumulator;
}


namespace fortuna_daemon {

class ConnectionManager;


class Session
    : public std::enable_shared_from_this<Session>
{
private:
    boost::asio::local::stream_protocol::socket socket;
    ConnectionManager* connection_manager;
    fortuna::Accumulator* accumulator;
    CryptoPP::SecByteBlock buffer;

public:
    Session(boost::asio::local::stream_protocol::socket&& _socket, ConnectionManager* _connection_manager, fortuna::Accumulator* _accumulator);
    ~Session() noexcept;

    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    void start();
    void stop();

private:
    template <typename F>
    void async_read(std::size_t offset, std::size_t length, F handler);

    template <typename F>
    void async_write(std::size_t offset, std::size_t length, F handler);

    void async_read_command();
    void async_read_random_event_header();
    void async_read_random_event_data();
    void async_read_request_length();
    void async_write_random_data(std::uint32_t length);
    void async_write_request_too_big();
    void async_write_generator_not_seeded();
};

typedef std::shared_ptr<Session> SessionPtr;


} // namespace fortuna_daemon

#endif // FORTUNAD_SESSION_HPP
