#include <gtest/gtest.h>
#include "Hello.hpp"
using namespace testing;

TEST(HelloTest, HelloMainTest)
{
    intnamespace::Hello hello;
    EXPECT_EQ(hello.getValue(), 123);
    EXPECT_FALSE(hello.getValue() == 124);
}


