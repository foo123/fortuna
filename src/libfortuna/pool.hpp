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

#ifndef FORTUNA_POOL_HPP
#define FORTUNA_POOL_HPP

#include <mutex>

#include <cryptopp/sha3.h>

typedef unsigned char byte;


namespace fortuna {


class Pool
{
private:
    CryptoPP::SHA3_256 hash;
    unsigned long total_length_of_appended_data = 0;

public:
    static constexpr
    const byte hash_length = decltype(hash)::DIGESTSIZE;

    static constexpr
    bool is_event_data_length_invalid(byte length)
    { return length == 0 || length > 32; }

    /**
     * \pre !is_event_data_length_invalid(length)
     */
    void add_random_event(byte source_number, const byte* data, byte length);

    /**
     * \param[out] output must be valid buffer of length hash_length
     */
    void get_hash_and_clear(byte* output);

    unsigned long get_total_length_of_appended_data() const
    { return total_length_of_appended_data; }
};


} // namespace fortuna

#endif // FORTUNA_POOL_HPP
