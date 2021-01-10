#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "OpenGLModule.hpp"

class OpenGLModuleMock : public img::OpenGLModule
{
public:
    virtual ~OpenGLModuleMock() = default;
    MOCK_METHOD(void, initalize, (), (override));
    MOCK_METHOD(bool, windowShouldClose, (), (override));
    MOCK_METHOD(void, startNewFrame, (), (override));
    MOCK_METHOD(void, render, (), (override));
    MOCK_METHOD(void, shutDown, (), (override));
    MOCK_METHOD(void, createDockspace, (bool&), (override));
};