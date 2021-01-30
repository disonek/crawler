
#pragma once

#include <string>

#include "imgui.h"

namespace img {

class ImGuiLogger
{
public:
    virtual ~ImGuiLogger() = default;
    ImGuiLogger();
    virtual void initialize();
    virtual void clear();
    virtual void addSimpleLog(std::string logMessage);
    virtual bool draw(const char* title, bool* p_open = NULL);

private:
    void addLog(const char* fmt, ...) IM_FMTARGS(2);
    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool AutoScroll;           // Keep scrolling if already at the bottom.
};
} // namespace img