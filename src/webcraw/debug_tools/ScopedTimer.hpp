#pragma once

#include <spdlog/spdlog.h>

#include <entt/entt.hpp>

#include "Timer.hpp"

namespace utils {

using Duration = double;
using Tag = std::string;

class Registry
{
public:
    entt::registry& get()
    {
        return registry;
    }

private:
    static entt::registry registry;
};

class ScopedTimer : public Timer
{
public:
    ScopedTimer(const char* name, Tag tag)
        : Timer{name}
        , tag{tag}
    {
    }

    ~ScopedTimer() override
    {
        double final = timeElapsed() * 0.001;
        // spdlog::info("{} time elapsed = {}", name, final);
        Registry reg{};
        auto entity = reg.get().create();
        reg.get().emplace<Duration>(entity, final);
        reg.get().emplace<Tag>(entity, tag);
    }

    uint32_t timeElapsed() override
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch() -
                           std::chrono::time_point_cast<std::chrono::milliseconds>(startTimepoint).time_since_epoch();

        return static_cast<uint32_t>(elapsedTime.count());
    }

private:
    Tag tag;
};
} // namespace utils