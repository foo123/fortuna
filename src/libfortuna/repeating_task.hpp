/*
Copyright 2015 Adam Mizerski <adam@mizerski.pl>

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

#ifndef FORTUNA_REPEATING_TASK_HPP
#define FORTUNA_REPEATING_TASK_HPP

#include <chrono>
#include <functional>
#include <mutex>
#include <thread>

namespace fortuna {

class RepeatingTask
{
private:
    std::chrono::minutes interval;
    std::function<void()> callback;
    std::timed_mutex sleeper;
    std::thread thread;

public:
    ~RepeatingTask() noexcept;

    void start(const std::chrono::minutes& _interval, std::function<void()> _callback);

    void stop();
};

} // namespace fortuna

#endif // FORTUNA_REPEATING_TASK_HPP
