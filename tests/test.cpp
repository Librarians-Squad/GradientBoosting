#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include "utility.hpp"

TEST(UtilityTest, Reorder) {
    std::vector<size_t> a(10000000);
    std::iota(a.begin(), a.end(), 0);
    auto b(a);
    std::shuffle(b.begin(), b.end(), std::mt19937(42));
    reorder(a.begin(), a.end(), b.begin());
    EXPECT_TRUE(std::equal(a.begin(), a.end(), b.begin()));
}
