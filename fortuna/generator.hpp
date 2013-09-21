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

#ifndef FORTUNA_GENERATOR_HPP
#define FORTUNA_GENERATOR_HPP

#include <mutex>

#include <cryptopp/secblock.h>
#include <cryptopp/aes.h>

#include "counter.hpp"
#include "fortuna_exception.hpp"
#include "noncopyable.hpp"


namespace fortuna {


class Generator
    : noncopyable
{
private:
    static const std::size_t key_length = 32; // 256 bit
    static const std::size_t counter_length = 16; // 128 bit

    CryptoPP::FixedSizeSecBlock<byte, key_length> key;
    Counter<byte, counter_length> counter;
    mutable std::mutex key_and_counter_access;

public:
    static constexpr const std::size_t output_block_length = CryptoPP::AES::BLOCKSIZE;

    Generator();

    bool is_seeded() const;

    void reseed(const byte* seed, std::size_t length);

private:
    void compute_new_key(const byte* seed, std::size_t length);

public:
    /**
     * \throw FortunaException if blocks_count is too big.
     * \throw FortunaException if generator is not seeded.
     */
    void get_pseudo_random_data(byte* output, std::size_t blocks_count);

private:
    /**
     * \todo non-inline static constexpr noexcept works in g++ >= 4.8 and clang++ >= 3.3
     */
    static constexpr
    bool is_request_too_big(std::size_t blocks_count) /*noexcept*/;

    void generate_blocks(byte* output, std::size_t blocks_count);
};


} // namespace fortuna

#endif // FORTUNA_GENERATOR_HPP
