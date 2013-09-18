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

#include "accumulator.hpp"

#include <fstream>

namespace fortuna {


namespace {


inline constexpr
std::uint32_t greatest_power_of_2_that_divides(std::uint32_t n) noexcept
{
    return n & -n; // Bits magic. Powered by "Two's complement"
}

/**
 * \todo use something faster like http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup or some one instruction assembly
 */
inline
std::uint8_t ilog2(std::uint32_t n) noexcept
{
    std::uint8_t ans = 0;
    while (n >>= 1)
        ++ans;
    return ans;
}

/**
 * Using time_point::min() causes overflow.
 * 102 ms is enough distant past that generator must be reseeded.
 */
inline
std::chrono::steady_clock::time_point distant_past()
{
    return std::chrono::steady_clock::now() - std::chrono::milliseconds(102);
}


} // namespace



Accumulator::Accumulator(Config&& config)
    : config(std::move(config))
    , last_reseed(distant_past())
{}

void Accumulator::get_random_data(byte* output, std::size_t blocks_count)
{
    std::lock_guard<std::mutex> lock(get_random_data_access);
    reseed_generator_if_needed();
    generator.get_pseudo_random_data(output, blocks_count);
}

void Accumulator::reseed_generator_if_needed()
{
    const auto now = std::chrono::steady_clock::now();
    
    if (is_min_pool_size_satisfied() && is_time_to_reseed(now)) {
        last_reseed = now;
        ++reseed_counter;
        reseed_generator();
    }
}

bool Accumulator::is_min_pool_size_satisfied() const
{
    return pools[0].get_total_length_of_appended_data() >= config.min_pool_size;
}

bool Accumulator::is_time_to_reseed(const std::chrono::steady_clock::time_point& now) const
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(now - last_reseed).count() > 100;
}

void Accumulator::reseed_generator()
{
    const std::uint8_t pools_to_use = ilog2(greatest_power_of_2_that_divides(reseed_counter)) + 1;
    CryptoPP::SecByteBlock buffer(pools_to_use * Pool::hash_length);
    for (byte i = 0; i < pools_to_use; ++i)
        pools[i].get_hash_and_clear(buffer.BytePtr() + i*Pool::hash_length);
    generator.reseed(buffer, buffer.SizeInBytes());
}


} // namespace fortuna
