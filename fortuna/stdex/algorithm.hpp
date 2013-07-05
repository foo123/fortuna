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
