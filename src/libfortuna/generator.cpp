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

#include "generator.hpp"

#include <algorithm>
#include <iterator>

#include <cryptopp/misc.h>
#include <cryptopp/sha3.h>


namespace fortuna {


template <class Iterable, class T>
static inline
void my_fill(Iterable& iterable, const T& value)
{
    using std::begin;
    using std::end;
    std::fill(begin(iterable), end(iterable), value);
}


Generator::Key::Key()
{
    my_fill(data, 0);
}

void Generator::Key::reseed(const byte* seed, std::size_t seed_length)
{
    CryptoPP::SHA3 hash{/*digestSize=*/data_size};
    hash.Update(data, data_size);
    hash.Update(seed, seed_length);
    hash.Final(data);
}



Generator::Counter::Counter()
{
    my_fill(data, 0);
}

void Generator::Counter::increment()
{
    CryptoPP::IncrementCounterByOne(data, data.size());
    _is_zero = false;
}



/**
 * Using time_point::min() causes overflow.
 * 102 ms is enough distant past that forces generator reseed.
 */
static inline
std::chrono::steady_clock::time_point distant_past()
{
    using namespace std::chrono;
    return steady_clock::now() - milliseconds(102);
}

Generator::Generator()
    : last_reseed{distant_past()}
{}


bool Generator::is_time_to_reseed() const
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now() - last_reseed).count() > 100; // TODO: config?
}


void Generator::reseed(const byte* seed, std::size_t seed_length)
{
    key.reseed(seed, seed_length);
    counter.increment();
    ++reseed_counter;
    last_reseed = std::chrono::steady_clock::now();
}


void Generator::get_pseudo_random_data(byte* output, std::size_t blocks_count)
{
    generate_blocks(output, blocks_count);
    static_assert(key.size() % CryptoPP::AES::BLOCKSIZE == 0, "key.size must be multiple of CryptoPP::AES::BLOCKSIZE");
    generate_blocks(key, key.size()/CryptoPP::AES::BLOCKSIZE);
}

void Generator::generate_blocks(byte* output, std::size_t blocks_count)
{
    CryptoPP::AES::Encryption aes{key, key.size()}; // makes a copy of the key, so it's not a problem when the key is also an output
    for (unsigned long i = 0; i < blocks_count; ++i) {
        aes.ProcessBlock(counter, output + i*CryptoPP::AES::BLOCKSIZE);
        counter.increment();
    }
}


} // namespace fortuna
