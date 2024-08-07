#pragma once
#include <array>

constexpr static int PSQTBucketCount = 23;

constexpr static std::array<int, 64> PSQTBucketLayout = {
    15, 16, 17, 18, 19, 20, 21, 22,
    15, 16, 17, 18, 19, 20, 21, 22,
    15, 16, 17, 18, 19, 20, 21, 22,
    15, 16, 17, 18, 19, 20, 21, 22,
    15, 8, 9, 10, 11, 12, 13, 14,
    15, 8, 9, 10, 11, 12, 13, 14,
    0, 8, 9, 10, 11, 12, 13, 14,
    0, 1, 2, 3, 4, 5, 6, 7, //
};
