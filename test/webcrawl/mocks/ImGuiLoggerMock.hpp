#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ImGuiLogger.hpp"

class ImGuiLoggerMock : public img::ImGuiLogger
{
public:
    virtual ~ImGuiLoggerMock() = default;
    MOCK_METHOD(void, initialize, (), (override));
    MOCK_METHOD(void, clear, (), (override));
    MOCK_METHOD(void, draw, (const char*, bool*), (override));
    MOCK_METHOD(void, addSimpleLog, (std::string), (override));
};