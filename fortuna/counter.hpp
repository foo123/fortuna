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
    : protected CryptoPP::FixedSizeSecBlock<T, S>
{
private:
    typedef typename CryptoPP::FixedSizeSecBlock<T, S> base_type;

    bool _is_zero {true};

public:
    typedef typename base_type::const_iterator const_iterator;

    Counter()
        : base_type()
    {
        stdex::fill(static_cast<base_type&>(*this), 0);
    }

    const_iterator begin() const
    { return base_type::begin(); }

    const_iterator end() const
    { return base_type::end(); }

    operator const T*() const
    { return base_type::operator const T*(); }


    bool is_zero() const noexcept
    { return _is_zero; }

    void increment()
    {
        CryptoPP::IncrementCounterByOne(*this, S);
        _is_zero = false;
    }
};


} // namespace fortuna

#endif // FORTUNA_GENERATOR_HPP
