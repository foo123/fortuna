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

#include "repeating_task.hpp"

#include <utility>

namespace fortuna {

RepeatingTask::~RepeatingTask() noexcept
{
    if (thread.joinable()) {
        stop();
    }
}

void RepeatingTask::start(const std::chrono::minutes& interval, std::function<void()> callback)
{
    if (thread.joinable()) {
        stop();
    }
    
    sleeper.lock();
    thread = std::thread([this, interval, callback] {
        while (!sleeper.try_lock_for(interval)) {
            callback();
        }
    });
}

void RepeatingTask::stop()
{
    sleeper.unlock();
    thread.join();
}

} // namespace fortuna
