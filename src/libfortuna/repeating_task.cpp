#include "repeating_task.hpp"

#include <utility>

namespace fortuna {

void RepeatingTask::start(const std::chrono::minutes& _interval, std::function<void()> _callback)
{
    if (thread.joinable()) {
        stop();
    }
    
    interval = _interval;
    callback = std::move(_callback);
    sleeper.lock();
    thread = std::thread([this] {
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
