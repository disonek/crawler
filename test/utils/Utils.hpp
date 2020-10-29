#pragma once

#include <gtest/gtest.h>
#include <utility>

namespace dp_tests {
template <typename T>
void checkProductDesc(T&& productsDesc, T&& toCompare)
{
    if(productsDesc.size() != toCompare.size())
    {
        bool sizesNotEaual{false};
        EXPECT_TRUE(sizesNotEaual);
    }

    auto res = std::mismatch(productsDesc.cbegin(), productsDesc.cend(), std::move(toCompare).cbegin());
    if(res.first == productsDesc.cend())
    {
    }
    else
    {
        bool rangesNotEqual = false;
        EXPECT_TRUE(rangesNotEqual);

        std::cout << *res.first << std::endl;
        std::cout << *res.second << std::endl;
    }
}
}
