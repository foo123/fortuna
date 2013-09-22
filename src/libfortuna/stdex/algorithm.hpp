/*
Copyright (C) 2013 Adam Mizerski <adam@mizerski.pl>
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
*/

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
