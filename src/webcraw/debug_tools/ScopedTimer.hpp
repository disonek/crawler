#pragma once

#include <spdlog/spdlog.h>

#include "Timer.hpp"

namespace utils {

class ScopedTimer : public Timer
{
public:
    ScopedTimer(const char* name)
        : Timer{name}
    {
    }

    ~ScopedTimer() override
    {
        double final = timeElapsed() * 0.001;
        spdlog::info("{} time elapsed = {}", name, final);
    }

    uint32_t timeElapsed() override
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch() -
                           std::chrono::time_point_cast<std::chrono::milliseconds>(startTimepoint).time_since_epoch();

        return static_cast<uint32_t>(elapsedTime.count());
    }
};
} // namespace utils