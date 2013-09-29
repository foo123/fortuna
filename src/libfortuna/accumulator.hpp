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

#ifndef FORTUNA_ACCUMULATOR_HPP
#define FORTUNA_ACCUMULATOR_HPP

#include <cstdint>
#include <array>
#include <mutex>
#include <chrono>

#include "generator.hpp"
#include "noncopyable.hpp"
#include "pool.hpp"

#include "stdex/monitor.hpp"

typedef unsigned char byte;


namespace fortuna {


class Accumulator
    : noncopyable
{
public:
    struct Config
    {
        unsigned long min_pool_size = 64;

        // It seems useless, but without it compilation fails. See http://stackoverflow.com/q/17430377/952263
        constexpr
        Config()
        {}
    };

private:
    const Config config;

    std::array<stdex::monitor<Pool>, 32> pools;
    Generator generator;

    std::mutex get_random_data_access;
    std::uint32_t reseed_counter = 0; // 32 bits, because 32 pools
    std::chrono::steady_clock::time_point last_reseed;

public:
    static constexpr
    const std::size_t output_block_length = Generator::output_block_length;

    /**
     * \return number of blocks required to store given bytes
     */
    static constexpr
    std::size_t bytes_to_blocks(std::size_t bytes) noexcept
    {
        return ((bytes-1)/output_block_length) + 1; /* ceil( size / (float)output_block_size ) without using floats */
    }


    explicit
    Accumulator(Config&& config = Config{});

    /**
     * \throw FortunaException if blocks_count is too big (greater than 2^20/CryptoPP::AES::BLOCKSIZE).
     * \throw FortunaException if generator is not seeded.
     */
    void get_random_data(byte* output, std::size_t blocks_count);

private:
    void reseed_generator_if_needed();
    bool is_min_pool_size_satisfied() const;
    bool is_time_to_reseed(const std::chrono::steady_clock::time_point& now) const;
    void reseed_generator();

public:
    void add_random_event(std::uint8_t pool_number, std::uint8_t source_number, const byte* data, std::uint8_t length);
};


} // namespace fortuna

#endif // FORTUNA_ACCUMULATOR_HPP
