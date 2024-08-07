#pragma once
#include <array>

constexpr static int PSQTBucketCount = 22;

constexpr static std::array<int, 64> PSQTBucketLayout = {
    14, 15, 16, 17, 18, 19, 20, 21,
    14, 15, 16, 17, 18, 19, 20, 21,
    14, 15, 16, 17, 18, 19, 20, 21,
    14, 15, 16, 17, 18, 19, 20, 21,
    14, 8, 9, 10, 11, 12, 13, 21,
    14, 8, 9, 10, 11, 12, 13, 21,
    0, 8, 9, 10, 11, 12, 13, 7,
    0, 1, 2, 3, 4, 5, 6, 7, //
};
