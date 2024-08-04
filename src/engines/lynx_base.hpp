#pragma once

constexpr static int PSQTBucketCount = 29;

constexpr static std::array<int, 64> PSQTBucketLayout = {
    21, 22, 23, 24, 25, 26, 27, 28,
    21, 22, 23, 24, 25, 26, 27, 28,
    21, 22, 23, 24, 25, 26, 27, 28,
    21, 22, 23, 24, 25, 26, 27, 28,
    14, 15, 16, 17, 18, 19, 20, 7,
    14, 15, 16, 17, 18, 19, 20, 7,
    0, 8 ,9, 10, 11, 12, 13, 7,
    0, 1, 2, 3, 4, 5, 6, 7, //
};