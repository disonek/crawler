#pragma once

#include <chrono>

namespace utils {

class Timer
{
public:
    Timer(const char* name)
        : name{name}
        , startTimepoint{std::chrono::high_resolution_clock::now()}
    {
    }

    virtual ~Timer() = default;
    virtual uint32_t timeElapsed() = 0;

protected:
    const char* name;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
};
} // namespace utils