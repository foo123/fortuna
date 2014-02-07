/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna_source_timer.

fortuna_source_timer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fortuna_source_timer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fortuna_source_timer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <chrono>

#include <cryptopp/misc.h>

#include "application.hpp"
#include "main.hpp"
#include "sleep_for.hpp"


namespace fortuna_source_timer {

namespace {

typedef unsigned char byte;

std::chrono::nanoseconds measure_sleep()
{
    using namespace std::chrono;
    const auto start = steady_clock::now();
    workaround::sleep_for(microseconds{100});
    return duration_cast<nanoseconds>(steady_clock::now() - start);
}

bool generate_random_bit()
{
    std::chrono::nanoseconds t1, t2;
    do {
        t1 = measure_sleep();
        t2 = measure_sleep();
    } while (do_run && t1 == t2);
    return t1 > t2;
}

// Both bitset and vector<bool> does not provide continuous storage.
template <std::size_t Nbytes>
class ContinuousBitsetSecure
{
public:
    static_assert(Nbytes > 0, "Nbytes > 0");

    class BitReference
    {
    private:
        byte* it;
        byte mask;

    public:
        BitReference() = default;
        BitReference(byte* _it, byte _mask)
            : it(_it)
            , mask(_mask)
        {}

        BitReference& operator=(bool b)
        {
            if (b)
                *it |= mask;
            else
                *it &= ~mask;
            return *this;
        }
    };

    class Iterator
    {
    private:
        byte* it;
        byte byte_pos;

    public:
        Iterator(byte *_it, byte _byte_pos)
            : it(_it)
            , byte_pos(_byte_pos)
        {}

        bool operator==(const Iterator& other) const
        { return it == other.it && byte_pos == other.byte_pos; }

        bool operator!=(const Iterator& other) const
        { return !operator==(other); }

        Iterator& operator++()
        {
            if (byte_pos++ == 7) {
                byte_pos = 0;
                ++it;
            }
            return *this;
        }

        BitReference operator*() const
        {
            return {it, static_cast<byte>(1 << byte_pos)};
        }
    };

private:
    byte _data[Nbytes];

public:
    ~ContinuousBitsetSecure() noexcept {
        CryptoPP::SecureWipeArray(_data, Nbytes);
    }

    Iterator begin()
    { return {_data, 0}; }

    Iterator end()
    { return {_data+Nbytes, 0}; }

    byte* data()
    { return _data; }

    const byte* data() const
    { return _data; }

    std::size_t size_in_bytes() const
    { return Nbytes; }
};

} // namespace


void Application::run()
{
    ContinuousBitsetSecure<32> buffer;
    while (do_run) {
        for (auto bit_ref : buffer) {
            bit_ref = generate_random_bit();
            if (!do_run) break;
        }
        if (!do_run) break;
        fortuna.add_random_event(buffer.data(), buffer.size_in_bytes());
    }
}


} // namespace fortuna_source_timer
