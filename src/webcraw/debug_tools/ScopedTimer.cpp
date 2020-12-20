#include "ScopedTimer.hpp"

namespace utils {

entt::registry Registry::registry{};

entt::registry& Registry::get()
{
    return registry;
}

ScopedTimer::ScopedTimer(const char* name, Tag tag)
    : Timer{name}
    , tag{tag}
{
}

ScopedTimer::~ScopedTimer()
{
    double final = timeElapsed() * 0.001;
    spdlog::info("{} time elapsed = {}", name, final);
    Registry reg{};
    auto entity = reg.get().create();
    reg.get().emplace<Duration>(entity, final);
    reg.get().emplace<Tag>(entity, tag);
}

uint32_t ScopedTimer::timeElapsed()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch() -
                       std::chrono::time_point_cast<std::chrono::milliseconds>(startTimepoint).time_since_epoch();

    return static_cast<uint32_t>(elapsedTime.count());
}

} // namespace utils
