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

#include <cryptopp/misc.h>
#include <cryptopp/sha3.h>

#include "stdex/algorithm.hpp"


namespace fortuna {


Generator::Key::Key()
{
    stdex::fill(data, 0);
}

void Generator::Key::reseed(const byte* seed, std::size_t seed_length)
{
    CryptoPP::SHA3 hash(/*digestSize=*/data.size());
    hash.Update(data, data.size());
    hash.Update(seed, seed_length);
    hash.Final(data);
}



Generator::Counter::Counter()
{
    stdex::fill(data, 0);
}

auto Generator::Counter::operator++() -> Counter&
{
    CryptoPP::IncrementCounterByOne(data, data.size());
    _is_zero = false;
    return *this;
}



bool Generator::is_seeded() const
{
    std::lock_guard<std::mutex> lock(key_and_counter_access);
    return !counter.is_zero();
}

void Generator::reseed(const byte* seed, std::size_t seed_length)
{
    std::lock_guard<std::mutex> lock(key_and_counter_access);
    key.reseed(seed, seed_length);
    ++counter;
}


void Generator::get_pseudo_random_data(byte* output, std::size_t blocks_count)
{
    if (is_request_too_big(blocks_count))
        throw FortunaException::request_length_too_big();
    
    std::lock_guard<std::mutex> lock(key_and_counter_access);
    
    if (counter.is_zero())
        throw FortunaException::generator_is_not_seeded();
    
    generate_blocks(output, blocks_count);
    generate_blocks(key, key.size()/CryptoPP::AES::BLOCKSIZE /* we're lucky that's exactly 2 */ );
}

void Generator::generate_blocks(byte* output, std::size_t blocks_count)
{
    // assert(!counter.is_zero()) is done at the beginning of Generator::get_pseudo_random_data
    CryptoPP::AES::Encryption aes(key, key.size()); // makes a copy of the key, so it's not a problem when the key is also an output
    for (unsigned long i = 0; i < blocks_count; ++i) {
        aes.ProcessBlock(counter, output + i*CryptoPP::AES::BLOCKSIZE);
        ++counter;
    }
}


} // namespace fortuna