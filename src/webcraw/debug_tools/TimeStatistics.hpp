#pragma once

#include <mutex>
#include <stack>

#include "ScopedTimer.hpp"

namespace utils {

class TimeStatistics
{
public:
    void addTimePeriod(uit32_t timePriod)
    {
        std::lock_guard<std::mutex> lock(statMutex);
        timePeriods.push(timePriod);
    }

private:
    std::stack<uint32_t> timePeriods;
    std::mutex statMutex;
};
} // namespace utils