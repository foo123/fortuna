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

#include "accumulator.hpp"

#include <functional>
#include <utility>

#include "fortuna_exception.hpp"


namespace fortuna {


Accumulator::Accumulator(AllConfig all_config)
    : config{std::move(all_config.accumulator)}
    , seed_file_manager{std::move(all_config.seed_file_manager), *this}
{}

Accumulator::~Accumulator()
{}


void Accumulator::add_random_event(std::uint8_t pool_number, std::uint8_t source_number, const byte* data, std::uint8_t length)
{
    if (pool_number >= monitored_pools.size())
        throw FortunaException::invalid_pool_number();
    if (Pool::is_event_data_length_invalid(length))
        throw FortunaException::invaild_event_length();

    using namespace std::placeholders;
    monitored_pools[pool_number].exec_rw(std::bind(&Pool::add_random_event, _1, source_number, data, length));
}


void Accumulator::get_random_data(byte* output, std::size_t blocks_count)
{
    if (Generator::is_request_too_big(blocks_count))
        throw FortunaException::request_length_too_big();
    monitored_generator.exec_rw([=](auto& generator){
        // workaround gcc bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=61636
        this->reseed_if_needed(generator);
        if (!generator.is_seeded())
            throw FortunaException::generator_is_not_seeded();
        generator.get_pseudo_random_data(output, blocks_count);
    });
}

void Accumulator::reseed_if_needed(Generator& generator)
{
    if (is_min_pool_size_satisfied() && generator.is_time_to_reseed())
        reseed(generator);
}

bool Accumulator::is_min_pool_size_satisfied() const
{
    return monitored_pools[0].exec_ro(std::mem_fn(&Pool::get_total_length_of_appended_data)) >= config.min_pool_size;
}

static inline constexpr
std::uint32_t greatest_power_of_2_that_divides(std::uint32_t n)
{
    return n & -n; // Bits magic. Powered by "Two's complement"
}

/**
 * \todo use something faster like http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup or some one instruction assembly
 */
static inline
std::uint8_t ilog2(std::uint32_t n)
{
    std::uint8_t ans = 0;
    while (n >>= 1)
        ++ans;
    return ans;
}

void Accumulator::reseed(Generator& generator)
{
    const unsigned long pools_to_use = ilog2(greatest_power_of_2_that_divides(generator.get_reseed_count())) + 1;

    CryptoPP::SecByteBlock buffer{pools_to_use * Pool::hash_length};

    #pragma omp parallel for
    for (byte i = 0; i < pools_to_use; ++i) {
        byte* dest = buffer.BytePtr() + i*Pool::hash_length;
        using namespace std::placeholders;
        monitored_pools[i].exec_rw(std::bind(&Pool::get_hash_and_clear, _1, dest));
    }

    generator.reseed(buffer, buffer.SizeInBytes());
}


} // namespace fortuna
