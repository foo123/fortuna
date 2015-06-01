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

#include "pool.hpp"


namespace fortuna {


Pool::Pool() = default;

Pool::~Pool() noexcept
{}


void Pool::add_random_event(byte source_number, const byte* data, byte length)
{
    hash.Update(&source_number, 1);
    hash.Update(&length, 1);
    hash.Update(data, length);

    total_length_of_appended_data += length;
}

void Pool::get_hash_and_clear(byte* output)
{
    hash.Final(output);

    total_length_of_appended_data = 0;
}


} // namespace fortuna
