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

#include <array>

#include "generator.hpp"
#include "monitor.hpp"
#include "pool.hpp"

typedef unsigned char byte;


namespace fortuna {


class Accumulator
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

    std::array<monitor<Pool>, 32> monitored_pools;
    monitor<Generator> monitored_generator;

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


    Accumulator();

    explicit
    Accumulator(const Config& _config);

    explicit
    Accumulator(Config&& _config);

    Accumulator(const Accumulator&) = delete;
    Accumulator& operator=(const Accumulator&) = delete;

    ~Accumulator();


    /**
     * \throw FortunaException if Pool::is_event_data_length_invalig(length)
     * \throw std::out_of_range if pool_number > 31
     */
    void add_random_event(std::uint8_t pool_number, std::uint8_t source_number, const byte* data, std::uint8_t length);

    /**
     * \throw FortunaException if blocks_count is too big (greater than 2^20/CryptoPP::AES::BLOCKSIZE).
     * \throw FortunaException if generator is not seeded.
     */
    void get_random_data(byte* output, std::size_t blocks_count);

private:
    void reseed_if_needed(Generator* generator);
    bool is_min_pool_size_satisfied() const;
    void reseed(Generator* generator);
};


} // namespace fortuna

#endif // FORTUNA_ACCUMULATOR_HPP
