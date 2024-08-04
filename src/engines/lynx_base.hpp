#pragma once

constexpr static int PSQTBucketCount = 24;

constexpr static std::array<int, 64> PSQTBucketLayout = {
    16, 17, 18, 19, 20, 21, 22, 23,
    16, 17, 18, 19, 20, 21, 22, 23,
    16, 17, 18, 19, 20, 21, 22, 23,
    16, 17, 18, 19, 20, 21, 22, 23,
    8, 9, 10, 11, 12, 13, 14, 15,
    8, 9, 10, 11, 12, 13, 14, 15,
    8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7, //
};
