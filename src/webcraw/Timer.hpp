
#include <spdlog/spdlog.h>

#include <chrono>
#include <string>

class Timer
{
public:
    Timer(std::string name)
        : name{name}
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        double final = timeElapsed() * 0.001;
        spdlog::info("{} time elapsed = {}", name, final);
    }

    uint32_t timeElapsed()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch() -
                           std::chrono::time_point_cast<std::chrono::milliseconds>(startTime).time_since_epoch();

        return static_cast<uint32_t>(elapsedTime.count());
    }

private:
    const std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};