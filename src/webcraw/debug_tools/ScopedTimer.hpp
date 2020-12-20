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
    entt::registry& get();

private:
    static entt::registry registry;
};

class ScopedTimer : public Timer
{
public:
    ScopedTimer(const char* name, Tag tag);
    ~ScopedTimer() override;
    uint32_t timeElapsed() override;

private:
    Tag tag;
};

static void registryExampleUsage()
{
    double getPageDuraton{};
    double parseDuration{};

    utils::Registry reg;
    reg.get().view<utils::Duration, utils::Tag>().each(
        [&getPageDuraton, &parseDuration](auto entity, auto& duration, auto& tag) {
            spdlog::warn("{} duraton = {}", tag, duration);
            if(tag == "getPage")
                getPageDuraton += duration;
            if(tag == "extractLinks")
                parseDuration += duration;
        });

    spdlog::error("Downloat time = {}", getPageDuraton);
    spdlog::error("Parse time = {}", parseDuration);
}
} // namespace utils