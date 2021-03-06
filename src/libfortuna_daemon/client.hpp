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

#ifndef FORTUNA_DAEMON_CLIENT_HPP
#define FORTUNA_DAEMON_CLIENT_HPP

#include <cstdint>
#include <memory>
#include <string>

#include "connection_info.hpp"

typedef unsigned char byte;


namespace fortuna_daemon {


class Client
{
private:
    class Impl; ///> Hides all the boost::asio and cryptopp stuff.

    std::unique_ptr<Impl> impl;

public:
    static constexpr
    const std::size_t max_request_length = /*2^20*/ 1048576;

    struct Config
    {
        ConnectionInfo connection_info;

        Config()
        {}
    };


    explicit
    Client(const Config& config);

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    Client(Client&& other) = default;
    Client& operator=(Client&& other) = default;

    ~Client() noexcept;

    /**
     * \throw boost::system::system_error on connection failure
     * \throw ClientException if length is too big (max is 2^20 == 1048576)
     * \throw ClientException if generator is not seeded
     */
    void get_random_data(byte* data, std::uint32_t length);

    /**
     * It's like get_random_data, but instead of throwing ClientException if
     * length is too big, it makes more calls to get_random_data.
     *
     * \throw boost::system::system_error on connection failure
     * \throw ClientException if generator is not seeded
     */
    void get_long_random_data(byte* data, unsigned long long length);
};


} // namespace fortuna_daemon

#endif // FORTUNA_DAEMON_CLIENT_HPP
