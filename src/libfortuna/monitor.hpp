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
/*
Based on Herb Shutter's monitor implementation presented on
https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Herb-Sutter-Concurrency-and-Parallelism at 40:30,
which is, I hope, public domain.
*/

#ifndef FORTUNA_MONITOR_HPP
#define FORTUNA_MONITOR_HPP

#include <utility>

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>


namespace fortuna {


template <class T>
class monitor
{
private:
    mutable T obj;
    mutable boost::shared_mutex shared_mutex;

public:
    monitor() = default;

    explicit
    monitor(const T& _obj)
        : obj{_obj}
    {}

    explicit
    monitor(T&& _obj)
        : obj{std::move(_obj)}
    {}

    monitor(const monitor&) = delete;
    monitor& operator=(const monitor&) = delete;

    template <typename F>
    auto exec_ro(F f) const -> decltype(f(obj))
    {
        boost::shared_lock<boost::shared_mutex> shared_lock{shared_mutex};
        return f(const_cast<const T&>(obj));
    }

    template <typename F>
    auto exec_rw(F f) const -> decltype(f(&obj))
    {
        boost::unique_lock<boost::shared_mutex> unique_lock{shared_mutex};
        return f(&obj);
    }
};


} // namespace fortuna

#endif // FORTUNA_MONITOR_HPP
