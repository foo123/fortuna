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


Generator::Generator()
{
    stdex::fill(key, 0);
    stdex::fill(counter, 0);
}

void Generator::reseed(const byte* seed, std::size_t length)
{
    std::lock_guard<std::mutex> lock(key_and_counter_access);
    compute_new_key(seed, length);
    increment_counter();
}

void Generator::compute_new_key(const byte* seed, std::size_t length)
{
    CryptoPP::SHA3 hash(/*digestSize=*/key_length);
    hash.Update(key, key_length);
    hash.Update(seed, length);
    hash.Final(key);
}

void Generator::increment_counter()
{
    CryptoPP::IncrementCounterByOne(counter, counter_length);
    counter_is_zero = false;
}

void Generator::get_pseudo_random_data(byte* output, std::size_t blocks_count)
{
    if (is_request_too_big(blocks_count))
        throw FortunaException::request_length_too_big();
    if (counter_is_zero)
        throw FortunaException::generator_is_not_seeded();
    
    std::lock_guard<std::mutex> lock(key_and_counter_access);
    generate_blocks(output, blocks_count);
    generate_blocks(key, key_length/CryptoPP::AES::BLOCKSIZE); // we're lucky that's exactly 2
}

constexpr
bool Generator::is_request_too_big(std::size_t blocks_count) /*noexcept*/
{
    return blocks_count > /*2^20*/ 1048576ul / CryptoPP::AES::BLOCKSIZE;
}

void Generator::generate_blocks(byte* output, std::size_t blocks_count)
{
    // assert(counter != 0) is done at the beginning of Generator::get_pseudo_random_data
    CryptoPP::AES::Encryption aes(key, key_length); // makes copy of key, so it's not a problem when key is also an output
    for (unsigned long i = 0; i < blocks_count; ++i) {
        aes.ProcessBlock(counter, output + i*CryptoPP::AES::BLOCKSIZE);
        increment_counter();
    }
}


} // namespace fortuna
