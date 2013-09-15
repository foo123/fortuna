
//          Copyright Adam Mizerski 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STDEX_ALGORITHM_HPP
#define STDEX_ALGORITHM_HPP

#include <algorithm>
#include <iterator>

namespace stdex {

using std::begin;
using std::end;


template <typename Iterable, class T>
void fill(Iterable& iterable, const T& value)
{
    std::fill(begin(iterable), end(iterable), value);
}


} // namespace stdex

#endif // STDEX_ALGORITHM_HPP
