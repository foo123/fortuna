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

#ifndef FORTUNA_DAEMON_CLIENT_EXCEPTION_HPP
#define FORTUNA_DAEMON_CLIENT_EXCEPTION_HPP

#include <string>
#include <stdexcept>

typedef unsigned char byte;


namespace fortuna_daemon {


class ClientException
    : public std::exception
{
public:
    enum class msg_id_t : byte {
        request_too_big,
        generator_is_not_seeded,
        illegal_status
    };

private:
    msg_id_t _msg_id;

    explicit
    ClientException(msg_id_t __msg_id)
        : _msg_id{__msg_id}
    {}

public:
    static
    ClientException request_too_big()
    { return ClientException{msg_id_t::request_too_big}; }

    static
    ClientException generator_is_not_seeded()
    { return ClientException{msg_id_t::generator_is_not_seeded}; }

    static
    ClientException illegal_status()
    { return ClientException{msg_id_t::illegal_status}; }


    msg_id_t msg_id() const noexcept
    { return _msg_id; }

    const char* what() const noexcept;
};


} // namespace fortuna_daemon

#endif // FORTUNA_DAEMON_CLIENT_EXCEPTION_HPP
