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

#include "fortuna_exception.hpp"


namespace fortuna {


static const std::string messages[] = {
    "request length too big",
    "generator is not seeded",
    "invalid pool number",
    "invaild event length",
    "seed file error"
};

FortunaException::FortunaException(msg_id_t _msg_id, std::string info) noexcept
    : msg_id{_msg_id}
    , msg{messages[static_cast<byte>(msg_id)] + ": " + info}
{}


} // namespace fortuna
