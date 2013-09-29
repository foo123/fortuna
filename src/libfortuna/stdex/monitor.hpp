/*
Copyright (C) 2013 Adam Mizerski <adam@mizerski.pl>
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
*/

#ifndef STDEX_MONITOR_HPP
#define STDEX_MONITOR_HPP

#include <mutex>


namespace stdex {


template <class T>
class monitor
{
private:
    mutable T t;
    mutable std::mutex mutex;

public:
    monitor() = default;

    monitor(const T& _t)
        : t(_t)
    {}

    template <typename F>
    auto operator()(F f) const -> decltype(f(t))
    {
        std::lock_guard<std::mutex> lock{mutex};
        return f(t);
    }
};


} // namespace stdex

#endif // STDEX_MONITOR_HPP
