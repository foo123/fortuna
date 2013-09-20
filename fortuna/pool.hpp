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

#include <atomic>
#include <mutex>

#include <cryptopp/sha3.h>

#include "noncopyable.hpp"

typedef unsigned char byte;


namespace fortuna {


class Pool
    : noncopyable
{
private:
    CryptoPP::SHA3_256 hash;
    std::mutex hash_access;
    std::atomic_ulong total_length_of_appended_data {0ul};

public:
    static constexpr const byte hash_length = CryptoPP::SHA3_256::DIGESTSIZE;

    /**
     * \throw FortunaException if length == 0 || length > 32
     */
    void add_random_event(byte source_number, const byte* data, byte length);

    /**
     * \param output must be valid buffer of length hash_length
     */
    void get_hash_and_clear(byte* output);

    unsigned long get_total_length_of_appended_data() const noexcept
    { return total_length_of_appended_data; }
};


} // namespace fortuna

#endif // FORTUNA_POOL_HPP
