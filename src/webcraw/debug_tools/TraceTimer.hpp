#pragma once

#include "Timer.hpp"
#include "Trace.hpp"

namespace utils {

class TraceTimer : public Timer
{
public:
    TraceTimer(const char* name)
        : Timer{name}
        , stopped{false}
    {
    }

    ~TraceTimer() override
    {
        if(!stopped)
            timeElapsed();
    }

    uint32_t timeElapsed() override
    {
        auto currentTime = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(currentTime).time_since_epoch().count();

        uint32_t threadID = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
        Trace::get().writeProfile({name, start, end, threadID});

        stopped = true;
        auto elapsedTime = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch() -
                           std::chrono::time_point_cast<std::chrono::milliseconds>(startTimepoint).time_since_epoch();

        return static_cast<uint32_t>(elapsedTime.count());
    }

private:
    bool stopped;
};
} // namespace utils