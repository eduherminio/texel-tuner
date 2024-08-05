#pragma once
#include <array>

constexpr static int PSQTBucketCount = 24;

constexpr static std::array<int, 64> PSQTBucketLayout = {
    20, 20, 21, 21, 22, 22, 23, 23,
    20, 20, 21, 21, 22, 22, 23, 23,
    16, 16, 17, 17, 18, 18, 19, 19,
    16, 16, 17, 17, 18, 18, 19, 19,
    8, 9, 10, 11, 12, 13, 14, 15,
    8, 9, 10, 11, 12, 13, 14, 15,
    8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7, //
};
