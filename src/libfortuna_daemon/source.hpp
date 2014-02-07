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

#ifndef FORTUNA_DAEMON_SOURCE_HPP
#define FORTUNA_DAEMON_SOURCE_HPP

#include <memory>

#include "connection_info.hpp"

typedef unsigned char byte;


namespace fortuna_daemon {


class Source
{
private:
    class Impl; ///> Hides all the boost::asio and cryptopp stuff.

    std::unique_ptr<Impl> impl;

public:
    struct Config
    {
        byte source_number;
        ConnectionInfo connection_info;

        Config()
        {}
    };


    explicit
    Source(const Config& config);

    Source(const Source&) = delete;
    Source& operator=(const Source&) = delete;

    Source(Source&& other) = default;
    Source& operator=(Source&& other) = default;

    ~Source() noexcept;

    /**
     * \throw std::length_error if length == 0 || length > 32
     * \throw boost::system::system_error on connection failure
     */
    void add_random_event(const byte* data, byte length);
};


} // namespace fortuna_daemon

#endif // FORTUNA_DAEMON_SOURCE_HPP
