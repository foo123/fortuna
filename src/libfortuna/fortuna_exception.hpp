/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna.

libfortuna is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libfortuna is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libfortuna.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FORTUNA_EXCEPTION_HPP
#define FORTUNA_EXCEPTION_HPP

#include <stdexcept>

typedef unsigned char byte;


namespace fortuna {


class FortunaException
    : public std::exception
{
public:
    enum class msg_id_t : byte {
        request_length_too_big,
        generator_is_not_seeded,
        invaild_event_length
    };

private:
    msg_id_t msg_id;

    explicit
    FortunaException(msg_id_t _msg_id) noexcept
        : msg_id{_msg_id}
    {}

public:
#define FORTUNA_EXCEPTION_NAMED_CONSTRUCTOR(id) \
    static \
    FortunaException id() noexcept \
    { return FortunaException{msg_id_t::id}; }

    FORTUNA_EXCEPTION_NAMED_CONSTRUCTOR(request_length_too_big)
    FORTUNA_EXCEPTION_NAMED_CONSTRUCTOR(generator_is_not_seeded)
    FORTUNA_EXCEPTION_NAMED_CONSTRUCTOR(invaild_event_length)
#undef FORTUNA_EXCEPTION_NAMED_CONSTRUCTOR

    msg_id_t get_msg_id() const noexcept
    { return msg_id; }

    const char* what() const noexcept;
};


} // namespace fortuna

#endif // FORTUNA_EXCEPTION_HPP
