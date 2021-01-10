
#pragma once

#include "imgui.h"

namespace img {

struct ImGuiLogger
{
    virtual ~ImGuiLogger() = default;
    ImGuiLogger();
    virtual void initialize();
    virtual void clear();
    void addLog(const char* fmt, ...) IM_FMTARGS(2);
    virtual void draw(const char* title, bool* p_open = NULL);

    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool AutoScroll;           // Keep scrolling if already at the bottom.
};
} // namespace img