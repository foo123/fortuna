/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of libfortuna.

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


const char* FortunaException::what() const noexcept
{
    static const char* messages[] = {
        "request length too big",
        "generator is not seeded",
        "invaild event length"
    };
    return messages[static_cast<byte>(msg_id)];
}


} // namespace fortuna
