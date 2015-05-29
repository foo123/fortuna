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

#include <chrono>
#include <cstdint>

#include <cryptopp/aes.h>
#include <cryptopp/secblock.h>


namespace fortuna {


class Generator
{
public:
    struct Config
    {
        std::chrono::milliseconds reseed_interval{100};

        constexpr
        Config()
        {}
    };

private:
    class Key
    {
    private:
        static constexpr
        const std::size_t data_size = 32; // 256 bit

        CryptoPP::FixedSizeSecBlock<byte, data_size> data;

    public:
        Key();

        static constexpr
        std::size_t size()
        { return data_size; }

        operator byte*()
        { return data; }

        operator const byte*() const
        { return data; }


        void reseed(const byte* seed, std::size_t seed_length);
    };

    class Counter
    {
    private:
        CryptoPP::FixedSizeSecBlock<byte, 16> data; // 128 bit
        bool _is_zero {true};

    public:
        Counter();

        auto begin() const
        { return data.begin(); }

        auto end() const
        { return data.end(); }

        operator const byte*() const
        { return data; }


        bool is_zero() const noexcept
        { return _is_zero; }

        void increment();
    };

    const Config config;

    Key key;
    Counter counter;

    std::uint32_t reseed_count = 0; // 32 bits, because 32 pools
    std::chrono::steady_clock::time_point last_reseed;

public:
    static constexpr
    const std::size_t output_block_length = CryptoPP::AES::BLOCKSIZE;

    Generator(Config _config = Config{});
    ~Generator();

    bool is_seeded() const
    { return !counter.is_zero(); }

    std::uint32_t get_reseed_count() const noexcept
    { return reseed_count; }

    bool is_time_to_reseed() const;

    void reseed(const byte* seed, std::size_t seed_length);

    /**
     * \pre !is_request_too_big(blocks_count)
     * \pre is_seeded()
     */
    void get_pseudo_random_data(byte* output, std::size_t blocks_count);

    static constexpr
    bool is_request_too_big(std::size_t blocks_count) noexcept
    {
        return blocks_count > /*2^20*/ 1048576ul / CryptoPP::AES::BLOCKSIZE;
    }

private:
    void generate_blocks(byte* output, std::size_t blocks_count);
};


} // namespace fortuna

#endif // FORTUNA_GENERATOR_HPP
