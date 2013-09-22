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

#ifndef FORTUNA_COUNTER_HPP
#define FORTUNA_COUNTER_HPP

#include <cryptopp/secblock.h>

#include "stdex/algorithm.hpp"


namespace fortuna {


template <typename T, unsigned int S>
class Counter
{
private:
    CryptoPP::FixedSizeSecBlock<T, S> data;
    bool _is_zero {true};

public:
    typedef typename decltype(data)::const_iterator const_iterator;

    Counter()
    {
        stdex::fill(data, 0);
    }

    const_iterator begin() const
    { return data.begin(); }

    const_iterator end() const
    { return data.end(); }

    operator const T*() const
    { return data; }


    bool is_zero() const noexcept
    { return _is_zero; }

    Counter& operator++()
    {
        CryptoPP::IncrementCounterByOne(data, S);
        _is_zero = false;
        return *this;
    }
};


} // namespace fortuna

#endif // FORTUNA_GENERATOR_HPP
