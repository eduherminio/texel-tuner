#pragma once
#include <array>

constexpr static int PSQTBucketCount = 19;

constexpr static std::array<int, 64> PSQTBucketLayout = {
    16, 16, 17, 17, 17, 17, 18, 18,
    16, 16, 16, 17, 17, 18, 18, 18,
    16, 16, 16, 17, 17, 18, 18, 18,
    16, 16, 16, 17, 17, 18, 18, 18,
    8, 9, 10, 11, 12, 13, 14, 15,
    8, 9, 10, 11, 12, 13, 14, 15,
    8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7, //
};
