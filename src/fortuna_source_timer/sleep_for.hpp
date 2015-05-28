/*
Copyright 2008, 2009, 2010, 2011, 2012 Free Software Foundation, Inc.
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

/*
Workaround for sometimes missing _GLIBCXX_USE_NANOSLEEP
http://stackoverflow.com/questions/12523122/what-is-glibcxx-use-nanosleep-all-about

Its just copied sleep_for from GCC's standard library with some tweaks.

The reason for this is that on some major linux distributions gcc is compiled
without --enable-libstdcxx-time which enables std::chrono::sleep_for. Using
this workaround allows us to use nanosleep directly.
*/

#ifndef SLEEP_FOR
#define SLEEP_FOR

#include <chrono>
#include <ctime>

namespace workaround {

template<typename Rep, typename Period>
inline
void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration)
{
    using namespace std::chrono;
    const auto s = duration_cast<seconds>(sleep_duration);
    const auto ns = duration_cast<nanoseconds>(sleep_duration - s);
    
    const timespec req{
        static_cast<std::time_t>(s.count()),
        static_cast<long>(ns.count())
    };
    
    ::nanosleep(&req, 0);
}

} // namespace workaround

#endif // SLEEP_FOR
