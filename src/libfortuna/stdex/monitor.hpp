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
    mutable T obj;
    mutable std::mutex mutex;

    T synced_obj_copy() const
    {
        std::lock_guard<std::mutex> lock{mutex};
        return obj;
    }

public:
    monitor() = default;

    monitor(const T& _obj)
        : obj(_obj)
    {}

    monitor(const monitor<T>& other_monitor)
        : obj(other_monitor.synced_obj_copy())
    {}

    monitor& operator=(const monitor& other_monitor)
    {
        {
            std::lock_guard<std::mutex> lock{mutex};
            obj = other_monitor.synced_obj_copy();
        }
        return *this;
    }


    template <typename F>
    auto operator()(F f) const -> decltype(f(obj))
    {
        std::lock_guard<std::mutex> lock{mutex};
        return f(obj);
    }
};


} // namespace stdex

#endif // STDEX_MONITOR_HPP
