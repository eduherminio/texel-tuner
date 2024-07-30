#include <array>
#include <cmath>
#include <iomanip>

using u64 = uint64_t;

constexpr int32_t Pack(const int16_t mg, const int16_t eg)
{
    return ((int32_t)eg << 16) + (int32_t)mg;
}

constexpr int16_t UnpackMG(const int32_t packed)
{
    return (int16_t)packed;
}

constexpr int16_t UnpackEG(const int32_t packed)
{
    return (int16_t)((packed + 0x8000) >> 16);
}

constexpr int PieceOffset(bool isWhite)
{
    return 6 - (6 * isWhite);
}

std::array<int, 6> phaseValues = {0, 1, 1, 2, 4, 0};

constexpr static int EvalNormalizationCoefficient = 90;

constexpr static int PSQTBucketCount = 2;

constexpr static std::array<std::array<int, 12>, PSQTBucketCount> PieceValue = {
    {
        {
            +99, +334, +352, +441, +979,   // 0
            +128, +406, +350, +717, +1318, // 0
        },
        {
            +102, +368, +382, +518, +1153, // 0
            +121, +435, +375, +772, +1466, // 0
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGamePawnTable = {
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            15, 46, 36, 16, -22, -20, -50, -63,
            16, 20, 30, 18, 7, -3, -41, -51,
            8, 16, 28, 34, 21, 6, -29, -46,
            21, 26, 30, 40, 19, 2, -28, -49,
            36, 37, 30, 12, -2, -11, -49, -71,
            24, 62, 25, 12, -13, -19, -58, -68,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            -31, -31, -23, -14, -5, 32, 46, 7,
            -35, -34, -14, 3, 12, 24, 33, 19,
            -30, -21, -2, 11, 23, 30, 11, 6,
            -30, -18, -4, 13, 25, 29, 12, 8,
            -34, -30, -12, -4, 7, 23, 27, 15,
            -32, -30, -27, -17, -8, 26, 38, 1,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGamePawnTable = {
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            1, -9, -11, -16, -0, 1, 12, 25,
            2, 3, -8, -22, -14, -12, 3, 9,
            24, 17, -4, -22, -12, -10, 12, 14,
            21, 14, -6, -19, -12, -7, 9, 13,
            5, -7, -12, -17, -9, -9, 4, 8,
            2, -12, -3, 1, 4, 3, 18, 26,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            20, 13, 6, -7, 11, 3, -10, -12,
            13, 6, -1, -10, -6, -6, -14, -13,
            28, 16, -1, -18, -15, -14, 2, -2,
            25, 16, -2, -14, -14, -12, 1, -5,
            13, 5, -4, -9, -4, -6, -13, -14,
            23, 14, 10, -9, 19, 7, -8, -10,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameKnightTable = {
    {
        {
            -105, -47, -40, -13, -42, -34, -28, -92,
            -46, -55, 10, 22, 16, 44, -6, -29,
            -38, 15, 44, 67, 59, 21, 12, -11,
            11, 37, 54, 81, 75, 55, 54, 7,
            0, 34, 59, 52, 66, 48, 45, 25,
            -21, -0, 37, 63, 59, 16, 7, -27,
            -23, -13, 20, 13, 14, 12, -19, -33,
            -155, -60, -52, -33, -28, -44, -32, -126 //
        },
        {
            -148, -18, -52, -30, -9, -10, -2, -76,
            -42, -26, -7, 14, 18, 28, -5, 1,
            -34, -5, 9, 47, 51, 33, 27, -6,
            -15, 19, 35, 47, 48, 51, 41, 15,
            -11, 19, 36, 36, 47, 50, 41, 12,
            -34, -4, 8, 40, 47, 26, 20, -6,
            -47, -20, -4, 14, 17, 25, -1, 2,
            -143, -21, -46, -19, -6, -1, -10, -64 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameKnightTable = {
    {
        {
            -67, 5, 1, -16, 5, -31, -10, -65,
            -12, 19, 4, 0, -2, -7, -3, -23,
            7, 0, 8, 19, 21, 13, -2, -11,
            1, 7, 31, 25, 31, 37, 4, -9,
            8, 15, 29, 33, 37, 26, 5, -15,
            -2, 19, 1, 19, 19, 11, -4, 3,
            -16, 7, -5, 11, -4, -9, 2, -10,
            -55, -13, 9, -1, -4, -8, -11, -78 //
        },
        {
            -51, -43, -8, -0, -3, -22, -39, -82,
            -8, 4, 10, 6, 6, -3, -8, -13,
            -13, 8, 26, 27, 21, 6, 0, -8,
            10, 11, 37, 40, 44, 33, 18, -1,
            5, 15, 37, 43, 43, 34, 24, 7,
            -14, 8, 16, 33, 22, 7, -2, -7,
            -16, 8, 2, 7, 1, -6, -10, -20,
            -66, -34, -3, -6, -4, -26, -36, -70 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameBishopTable = {
    {
        {
            -19, -24, -7, -15, -19, -6, -17, -11,
            -16, 46, 21, -0, 2, 7, -2, -12,
            -3, 7, 22, 28, 5, -4, -2, 4,
            -0, 4, 18, 39, 36, -16, -3, -8,
            -19, 17, 5, 34, 16, -6, 1, -3,
            -10, 17, 25, 14, 4, -5, -16, 1,
            -21, 61, 37, 17, -9, -4, -19, -25,
            -6, -11, 2, -37, -18, -8, -15, -74 //
        },
        {
            -13, 14, -10, -19, -12, -8, -16, 14,
            6, -1, 3, -20, 1, 4, 38, -6,
            -10, -0, -12, 1, -14, 7, -2, 27,
            -11, -10, -5, 22, 19, -13, 3, -1,
            -18, -5, -14, 18, 9, -10, -7, 5,
            1, -4, -0, -7, -0, 0, 2, 21,
            8, 9, 5, -8, -5, 9, 31, 9,
            10, 17, 3, -34, -16, -17, 8, 4 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameBishopTable = {
    {
        {
            13, 12, 3, -1, 7, 4, 3, 6,
            19, -17, -10, -10, -7, -13, -1, -9,
            19, 7, -11, -15, -10, 3, -3, 8,
            17, -5, -8, -7, -13, 5, 2, 15,
            13, -1, -10, -9, -6, -5, -3, 10,
            31, -14, -7, -17, -5, -0, -1, -8,
            4, -20, -22, -17, -6, -9, 4, 13,
            12, 13, 10, 16, 4, -2, 6, 27 //
        },
        {
            -2, 20, -4, 6, -1, 3, -1, -26,
            -2, -7, -6, 2, -1, -14, -5, -10,
            12, 8, 7, -1, 9, 0, 2, 13,
            12, 1, 4, 2, 2, 2, -2, 2,
            6, 2, 4, 7, -2, 5, 0, 6,
            7, 1, -1, -2, 2, -5, -0, 14,
            -8, -9, -16, 1, -3, -7, -6, -8,
            2, -14, -2, 5, 11, 7, -6, -16 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameRookTable = {
    {
        {
            -49, -29, -20, 11, 2, 10, 23, 7,
            -5, 7, 15, -6, -2, 8, 1, -18,
            -2, -8, -33, 2, -16, -23, 16, -6,
            -14, 20, -2, -3, -8, 3, 13, 3,
            1, 13, 9, 4, -14, -8, -11, -1,
            5, 8, 41, 2, -11, 19, 18, -4,
            12, 16, 1, 5, -3, -2, 7, -15,
            -28, -28, -3, 21, 9, 8, 25, 8 //
        },
        {
            -5, -12, -6, -0, 10, 7, 1, 1,
            -33, -21, -13, -7, 4, 14, 38, 4,
            -37, -24, -24, -14, 4, 12, 51, 30,
            -33, -31, -25, -11, -6, 7, 37, 14,
            -27, -25, -19, -8, -7, 9, 34, 15,
            -30, -22, -24, -2, 1, 21, 52, 29,
            -30, -30, -8, -3, 6, 13, 44, 19,
            -4, -6, -2, 10, 18, 13, 11, 20 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameRookTable = {
    {
        {
            22, 19, 26, -3, -6, -18, -22, -2,
            4, 17, 16, 10, -4, -9, -2, -0,
            5, 12, 19, -5, 1, -4, -18, -11,
            8, 1, 9, 7, -5, -10, -17, -14,
            12, 8, 4, 1, 6, -8, -9, -6,
            4, 8, -16, -4, -7, -7, -19, -7,
            8, 13, 15, 1, 2, -5, -8, 6,
            10, 21, 12, -7, -4, -17, -25, -8 //
        },
        {
            9, 4, 7, -0, -7, 3, 8, -14,
            20, 19, 18, 7, -1, -4, -10, 2,
            15, 9, 11, 5, -10, -12, -23, -19,
            17, 11, 12, 2, -3, -4, -14, -14,
            14, 9, 12, 1, -5, -10, -14, -14,
            15, 13, 6, -4, -11, -18, -24, -10,
            21, 22, 14, 4, -4, -6, -9, -5,
            5, -2, 4, -7, -17, -7, 1, -23 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameQueenTable = {
    {
        {
            -23, -51, -42, -10, 2, 2, 8, 29,
            -6, -19, 21, 17, 7, 11, 28, 48,
            -39, -2, -11, -0, -10, -3, 29, 72,
            -3, -36, -15, 5, -1, -3, 15, 39,
            -5, -11, -15, 6, 1, 3, 12, 25,
            -24, -11, -14, -7, 6, -7, 26, 47,
            -44, -47, 19, 25, 19, -4, 21, 50,
            -44, -46, -23, -18, 6, -9, -10, 17 //
        },
        {
            -15, -11, -11, 6, 1, -32, -0, -8,
            -5, -9, 8, -1, 5, 14, 28, 44,
            -9, -7, -8, -10, -13, 8, 31, 47,
            -12, -16, -15, -8, -6, -1, 12, 23,
            -13, -14, -15, -16, -6, -1, 12, 21,
            -8, -5, -14, -10, -9, 4, 17, 29,
            -18, -17, 3, 11, 9, 13, 12, 28,
            -16, -11, -2, 8, 4, -35, -18, 25 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameQueenTable = {
    {
        {
            -43, -7, 32, -4, -16, -49, -73, -55,
            -10, -7, -29, -1, -5, -24, -34, -14,
            38, 8, 33, 4, 27, 10, -40, -31,
            2, 37, 18, 14, 19, 15, 6, -5,
            3, 22, 33, 20, 15, -5, -8, 15,
            18, 21, 31, 15, 15, 34, -7, 17,
            15, 18, -20, -6, 3, 24, -37, -37,
            -38, -17, 15, 23, 4, -1, 15, -15 //
        },
        {
            -15, -16, -7, -14, -18, -8, -11, 36,
            -15, -10, -26, -4, -6, -20, -46, 6,
            -23, -9, 0, -0, 18, 17, -2, 15,
            -16, -2, 1, 7, 21, 33, 45, 34,
            -8, -5, 4, 17, 16, 31, 23, 43,
            -21, -16, 7, 7, 10, 13, 18, 20,
            -10, -5, -22, -24, -19, -25, -30, 26,
            -2, -12, -12, -8, -16, 11, 18, 7 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameKingTable = {
    {
        {
            298, 318, 304, 248, 0, 0, 0, 0,
            275, 284, 279, 259, 0, 0, 0, 0,
            220, 238, 202, 202, 0, 0, 0, 0,
            205, 221, 199, 149, 0, 0, 0, 0,
            227, 247, 209, 167, 0, 0, 0, 0,
            208, 239, 196, 193, 0, 0, 0, 0,
            315, 277, 253, 248, 0, 0, 0, 0,
            287, 312, 292, 238, 0, 0, 0, 0 //
        },
        {
            0, 0, 0, 0, -107, -170, -77, -89,
            0, 0, 0, 0, -175, -151, -106, -99,
            0, 0, 0, 0, -214, -214, -173, -198,
            0, 0, 0, 0, -263, -228, -219, -256,
            0, 0, 0, 0, -258, -216, -221, -251,
            0, 0, 0, 0, -193, -193, -165, -189,
            0, 0, 0, 0, -162, -141, -90, -92,
            0, 0, 0, 0, -101, -162, -63, -75 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameKingTable = {
    {
        {
            -100, -70, -49, -33, 0, 0, 0, 0,
            -48, -13, -3, 6, 0, 0, 0, 0,
            -28, 14, 42, 53, 0, 0, 0, 0,
            -26, 27, 63, 95, 0, 0, 0, 0,
            -33, 18, 62, 92, 0, 0, 0, 0,
            -24, 14, 45, 56, 0, 0, 0, 0,
            -63, -14, 4, 8, 0, 0, 0, 0,
            -104, -75, -51, -33, 0, 0, 0, 0 //
        },
        {
            0, 0, 0, 0, -25, 2, -32, -79,
            0, 0, 0, 0, 53, 39, 15, -22,
            0, 0, 0, 0, 93, 80, 44, 15,
            0, 0, 0, 0, 129, 99, 68, 28,
            0, 0, 0, 0, 131, 98, 70, 27,
            0, 0, 0, 0, 88, 75, 44, 11,
            0, 0, 0, 0, 48, 37, 10, -25,
            0, 0, 0, 0, -21, -2, -35, -83 //
        },
    }};

constexpr static std::array<std::array<std::array<int, 64>, PSQTBucketCount>, 6> MiddleGamePositionalWhiteTables = {
    MiddleGamePawnTable,
    MiddleGameKnightTable,
    MiddleGameBishopTable,
    MiddleGameRookTable,
    MiddleGameQueenTable,
    MiddleGameKingTable};

constexpr static std::array<std::array<std::array<int, 64>, PSQTBucketCount>, 6> EndGamePositionalWhiteTables{
    EndGamePawnTable,
    EndGameKnightTable,
    EndGameBishopTable,
    EndGameRookTable,
    EndGameQueenTable,
    EndGameKingTable};

constexpr int PackedPieceValue(int bucket, int piece)
{
    return Pack(PieceValue[bucket][piece], PieceValue[bucket][piece + 5]);
}

constexpr int MiddleGamePositionalTables(int bucket, int piece, int square)
{
    int coefficient = 1;
    if (piece >= 6)
    {
        piece -= 6;
        square ^= 56;
        coefficient = -1;
    }

    return MiddleGamePositionalWhiteTables[piece][bucket][square] * coefficient;
}

constexpr int EndGamePositionalTables(int bucket, int piece, int square)
{
    int coefficient = 1;
    if (piece >= 6)
    {
        piece -= 6;
        square ^= 56;
        coefficient = -1;
    }

    return EndGamePositionalWhiteTables[piece][bucket][square] * coefficient;
}

constexpr int PackedPositionalTables(int bucket, int piece, int square)
{
    int coefficient = 1;
    if (piece >= 6)
    {
        piece -= 6;
        square ^= 56;
        coefficient = -1;
    }

    return Pack(
        MiddleGamePositionalWhiteTables[piece][bucket][square] * coefficient,
        EndGamePositionalWhiteTables[piece][bucket][square] * coefficient);
}

constexpr static std::array<int, 64> File = {
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7};

constexpr static std::array<u64, 64> FileMasks = {
    72340172838076673UL, 144680345676153346UL, 289360691352306692UL, 578721382704613384UL, 1157442765409226768UL, 2314885530818453536UL, 4629771061636907072UL, 9259542123273814144UL,
    72340172838076673UL, 144680345676153346UL, 289360691352306692UL, 578721382704613384UL, 1157442765409226768UL, 2314885530818453536UL, 4629771061636907072UL, 9259542123273814144UL,
    72340172838076673UL, 144680345676153346UL, 289360691352306692UL, 578721382704613384UL, 1157442765409226768UL, 2314885530818453536UL, 4629771061636907072UL, 9259542123273814144UL,
    72340172838076673UL, 144680345676153346UL, 289360691352306692UL, 578721382704613384UL, 1157442765409226768UL, 2314885530818453536UL, 4629771061636907072UL, 9259542123273814144UL,
    72340172838076673UL, 144680345676153346UL, 289360691352306692UL, 578721382704613384UL, 1157442765409226768UL, 2314885530818453536UL, 4629771061636907072UL, 9259542123273814144UL,
    72340172838076673UL, 144680345676153346UL, 289360691352306692UL, 578721382704613384UL, 1157442765409226768UL, 2314885530818453536UL, 4629771061636907072UL, 9259542123273814144UL,
    72340172838076673UL, 144680345676153346UL, 289360691352306692UL, 578721382704613384UL, 1157442765409226768UL, 2314885530818453536UL, 4629771061636907072UL, 9259542123273814144UL,
    72340172838076673UL, 144680345676153346UL, 289360691352306692UL, 578721382704613384UL, 1157442765409226768UL, 2314885530818453536UL, 4629771061636907072UL, 9259542123273814144UL};

constexpr static std::array<u64, 64> IsolatedPawnMasks = {
    144680345676153346UL,
    361700864190383365UL,
    723401728380766730UL,
    1446803456761533460UL,
    2893606913523066920UL,
    5787213827046133840UL,
    11574427654092267680UL,
    4629771061636907072UL,
    144680345676153346UL,
    361700864190383365UL,
    723401728380766730UL,
    1446803456761533460UL,
    2893606913523066920UL,
    5787213827046133840UL,
    11574427654092267680UL,
    4629771061636907072UL,
    144680345676153346UL,
    361700864190383365UL,
    723401728380766730UL,
    1446803456761533460UL,
    2893606913523066920UL,
    5787213827046133840UL,
    11574427654092267680UL,
    4629771061636907072UL,
    144680345676153346UL,
    361700864190383365UL,
    723401728380766730UL,
    1446803456761533460UL,
    2893606913523066920UL,
    5787213827046133840UL,
    11574427654092267680UL,
    4629771061636907072UL,
    144680345676153346UL,
    361700864190383365UL,
    723401728380766730UL,
    1446803456761533460UL,
    2893606913523066920UL,
    5787213827046133840UL,
    11574427654092267680UL,
    4629771061636907072UL,
    144680345676153346UL,
    361700864190383365UL,
    723401728380766730UL,
    1446803456761533460UL,
    2893606913523066920UL,
    5787213827046133840UL,
    11574427654092267680UL,
    4629771061636907072UL,
    144680345676153346UL,
    361700864190383365UL,
    723401728380766730UL,
    1446803456761533460UL,
    2893606913523066920UL,
    5787213827046133840UL,
    11574427654092267680UL,
    4629771061636907072UL,
    144680345676153346UL,
    361700864190383365UL,
    723401728380766730UL,
    1446803456761533460UL,
    2893606913523066920UL,
    5787213827046133840UL,
    11574427654092267680UL,
    4629771061636907072UL,
};

constexpr static std::array<u64, 64> WhitePassedPawnMasks = {
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL,
    3UL, 7UL, 14UL, 28UL, 56UL, 112UL, 224UL, 192UL,
    771UL, 1799UL, 3598UL, 7196UL, 14392UL, 28784UL, 57568UL, 49344UL,
    197379UL, 460551UL, 921102UL, 1842204UL, 3684408UL, 7368816UL, 14737632UL, 12632256UL,
    50529027UL, 117901063UL, 235802126UL, 471604252UL, 943208504UL, 1886417008UL, 3772834016UL, 3233857728UL,
    12935430915UL, 30182672135UL, 60365344270UL, 120730688540UL, 241461377080UL, 482922754160UL, 965845508320UL, 827867578560UL,
    3311470314243UL, 7726764066567UL, 15453528133134UL, 30907056266268UL, 61814112532536UL, 123628225065072UL, 247256450130144UL, 211934100111552UL,
    847736400446211UL, 1978051601041159UL, 3956103202082318UL, 7912206404164636UL, 15824412808329272UL, 31648825616658544UL, 63297651233317088UL, 54255129628557504UL};

constexpr static std::array<u64, 64> BlackPassedPawnMasks = {
    217020518514230016UL, 506381209866536704UL, 1012762419733073408UL, 2025524839466146816UL, 4051049678932293632UL, 8102099357864587264UL, 16204198715729174528UL, 13889313184910721024UL,
    217020518514229248UL, 506381209866534912UL, 1012762419733069824UL, 2025524839466139648UL, 4051049678932279296UL, 8102099357864558592UL, 16204198715729117184UL, 13889313184910671872UL,
    217020518514032640UL, 506381209866076160UL, 1012762419732152320UL, 2025524839464304640UL, 4051049678928609280UL, 8102099357857218560UL, 16204198715714437120UL, 13889313184898088960UL,
    217020518463700992UL, 506381209748635648UL, 1012762419497271296UL, 2025524838994542592UL, 4051049677989085184UL, 8102099355978170368UL, 16204198711956340736UL, 13889313181676863488UL,
    217020505578799104UL, 506381179683864576UL, 1012762359367729152UL, 2025524718735458304UL, 4051049437470916608UL, 8102098874941833216UL, 16204197749883666432UL, 13889312357043142656UL,
    217017207043915776UL, 506373483102470144UL, 1012746966204940288UL, 2025493932409880576UL, 4050987864819761152UL, 8101975729639522304UL, 16203951459279044608UL, 13889101250810609664UL,
    216172782113783808UL, 504403158265495552UL, 1008806316530991104UL, 2017612633061982208UL, 4035225266123964416UL, 8070450532247928832UL, 16140901064495857664UL, 13835058055282163712UL,
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL};

constexpr static std::array<int, 64> Rank = {
    7UL, 7UL, 7UL, 7UL, 7UL, 7UL, 7UL, 7UL,
    6UL, 6UL, 6UL, 6UL, 6UL, 6UL, 6UL, 6UL,
    5UL, 5UL, 5UL, 5UL, 5UL, 5UL, 5UL, 5UL,
    4UL, 4UL, 4UL, 4UL, 4UL, 4UL, 4UL, 4UL,
    3UL, 3UL, 3UL, 3UL, 3UL, 3UL, 3UL, 3UL,
    2UL, 2UL, 2UL, 2UL, 2UL, 2UL, 2UL, 2UL,
    1UL, 1UL, 1UL, 1UL, 1UL, 1UL, 1UL, 1UL,
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL};

const u64 NotAFile = 0xFEFEFEFEFEFEFEFE;

const u64 NotHFile = 0x7F7F7F7F7F7F7F7F;

const int PositiveCheckmateDetectionLimit = 27000;

const int NegativeCheckmateDetectionLimit = -27000;

const int MinEval = NegativeCheckmateDetectionLimit + 1;

const int MaxEval = PositiveCheckmateDetectionLimit - 1;

[[nodiscard]] static u64 ShiftUp(const u64 board)
{
    return board >> 8;
}

[[nodiscard]] static u64 ShiftDown(const u64 board)
{
    return board << 8;
}

[[nodiscard]] static u64 ShiftLeft(const u64 board)
{
    return (board >> 1) & NotHFile;
}

[[nodiscard]] static u64 ShiftRight(const u64 board)
{
    return (board << 1) & NotAFile;
}

[[nodiscard]] static u64 ShiftUpRight(const u64 board)
{
    return ShiftRight(ShiftUp(board));
}

[[nodiscard]] static u64 ShiftUpLeft(const u64 board)
{
    return ShiftLeft(ShiftUp(board));
}

[[nodiscard]] static u64 ShiftDownRight(const u64 board)
{
    return ShiftRight(ShiftDown(board));
}

[[nodiscard]] static u64 ShiftDownLeft(const u64 board)
{
    return ShiftLeft(ShiftDown(board));
}

static void print_psqts_csharp(const parameters_t &parameters, std::array<std::array<tune_t, 12>, PSQTBucketCount> &existingPieceValues)
{
    std::array<std::array<tune_t, 12>, PSQTBucketCount> psqtPieceValues;

    // Extract and print pieces values
    for (int phase = 0; phase <= 1; ++phase)
    {
        if (phase == 0)
            std::cout << "\tinternal static readonly short[][] MiddleGamePieceValues =\n\t[";

        else
            std::cout << "\n\tinternal static readonly short[][] EndGamePieceValues =\n\t[";

        for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
        {
            std::cout << "\n\t\t[\n\t\t\t";

            // Pawns
            {
                tune_t pawnSum = 0;

                for (int square = 0; square < 48; ++square)
                {
                    pawnSum += parameters[48 * bucket + square][phase];
                }

                auto average = (pawnSum / 48.0);
                auto pieceIndex = phase * 6;

                psqtPieceValues[bucket][pieceIndex] = average;
                std::cout << "+" << std::round(average + existingPieceValues[bucket][pieceIndex]) << ", ";
            }

            for (int piece = 1; piece < 5; ++piece)
            {
                tune_t sum = 0;

                for (int square = 0; square < 64; ++square)
                {
                    sum += parameters
                        [(48 * PSQTBucketCount) +               // 64 - 16, since we're only tuning 48 pawn values
                         (64 * PSQTBucketCount) * (piece - 1) + // piece - 1 since we already took pawns into account
                         (64 * bucket) +
                         square]
                        [phase];
                }

                auto average = (sum / 64.0);
                auto pieceIndex = piece + phase * 6;

                // std::cout << std::endl
                //           << existingPieceValues[0][pieceIndex] << "==" << existingPieceValues[1][pieceIndex] << std::endl;

                psqtPieceValues[bucket][pieceIndex] = average;
                std::cout << "+" << std::round(average + existingPieceValues[bucket][pieceIndex]) << ", ";
            }

            // Kings
            auto kingIndex = 5 + phase * 6;
            psqtPieceValues[bucket][kingIndex] = 0;
            std::cout << psqtPieceValues[bucket][kingIndex] + existingPieceValues[bucket][kingIndex] << ",\n\t\t\t";

            for (int piece = 0; piece < 5; ++piece)
            {
                auto pieceIndex = piece + phase * 6;
                std::cout << "-" << std::round(psqtPieceValues[bucket][pieceIndex] + existingPieceValues[bucket][pieceIndex]) << ", ";
            }
            std::cout << std::round(psqtPieceValues[bucket][kingIndex] + existingPieceValues[bucket][kingIndex]) << "\n\t\t],";
        }

        std::cout << "\n\t];\n";
    }

    // Print PSQTs

    std::string names[] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};

    // Pawns
    {
        const int piece = 0;
        for (int phase = 0; phase <= 1; ++phase)
        {
            for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
            {
                if (bucket == 0)
                {
                    std::cout << "\n\tinternal static readonly short[][] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n\t[\n";
                }

                std::cout << "\t\t[\n\t\t\t";

                std::cout << "   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\n\t\t\t";

                for (int square = 0; square < 48; ++square)
                {
                    std::cout << std::setw(4) << std::round(parameters[48 * bucket + square][phase] - psqtPieceValues[bucket][phase * 6]) << ",";
                    if (square % 8 == 7)
                        std::cout << "\n\t\t";
                    if (square != 47)
                        std::cout << "\t";
                }
                std::cout << "\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0," << std::endl;
                std::cout << "\t\t],\n";

                if (bucket == PSQTBucketCount - 1)
                {
                    std::cout << "\t];\n";
                }
            }
        }
    }

    for (int piece = 1; piece < 6; ++piece)
    {
        for (int phase = 0; phase <= 1; ++phase)
        {
            for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
            {
                if (bucket == 0)
                {
                    std::cout << "\n\tinternal static readonly short[][] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n\t[\n";
                }

                std::cout << "\t\t[\n\t\t\t";

                for (int square = 0; square < 64; ++square)
                {
                    std::cout << std::setw(4) << std::round(parameters[(48 * PSQTBucketCount) +               // 64 - 16, since we're only tuning 48 pawn values
                                                                       (64 * PSQTBucketCount) * (piece - 1) + // piece - 1 since we already took pawns into account
                                                                       (64 * bucket) + square][phase] -
                                                            psqtPieceValues[bucket][piece + phase * 6])
                              << ",";
                    if (square % 8 == 7)
                        std::cout << "\n\t\t";
                    if (square != 63)
                        std::cout << "\t";
                }
                std::cout << "],\n";

                if (bucket == PSQTBucketCount - 1)
                {
                    std::cout << "\t];\n";
                }
            }
        }
    }
    std::cout << std::endl;
}

static void print_psqts_cpp(const parameters_t &parameters, std::array<std::array<tune_t, 12>, PSQTBucketCount> &existingPieceValues)
{
    std::array<std::array<tune_t, 12>, PSQTBucketCount> psqtPieceValues;

    std::cout << "constexpr static std::array<std::array<int, 12>, PSQTBucketCount> PieceValue = {\n\t{\n\t";

    // Exctract and print pieces values
    for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
    {
        std::cout << "\t{\n";

        for (int phase = 0; phase <= 1; ++phase)
        {
            std::cout << "\t\t\t";

            // Pawns
            {
                tune_t pawnSum = 0;

                for (int square = 0; square < 48; ++square)
                {
                    pawnSum += parameters[48 * bucket + square][phase];
                }

                auto average = (pawnSum / 48.0);
                auto pieceIndex = phase * 6;

                psqtPieceValues[bucket][pieceIndex] = average;
                std::cout << "+" << std::round(average + existingPieceValues[bucket][pieceIndex]) << ", ";
            }

            for (int piece = 1; piece < 5; ++piece)
            {
                tune_t sum = 0;

                for (int square = 0; square < 64; ++square)
                {
                    sum += parameters
                        [(48 * PSQTBucketCount) +               // 64 - 16, since we're only tuning 48 pawn values
                         (64 * PSQTBucketCount) * (piece - 1) + // piece - 1 since we already took pawns into account
                         (64 * bucket) +
                         square]
                        [phase];
                }

                auto average = (sum / 64.0);
                auto pieceIndex = piece + phase * 6;

                psqtPieceValues[bucket][pieceIndex] = average;
                std::cout << "+" << std::round(average + existingPieceValues[bucket][pieceIndex]) << ", ";
            }

            // Kings
            auto kingIndex = 5 + phase * 6;
            psqtPieceValues[bucket][kingIndex] = 0;
            std::cout << "// " << std::round(existingPieceValues[bucket][kingIndex]) << "\n";
        }
        std::cout << "\t\t},\n\t";
    }

    std::cout << "}};\n";

    // Print PSQTs

    std::string names[] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};

    // Pawns
    {
        const int piece = 0;
        for (int phase = 0; phase <= 1; ++phase)
        {
            for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
            {
                if (bucket == 0)
                {
                    std::cout << "\nconstexpr static std::array<std::array<int, 64>, PSQTBucketCount> " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table = {\n\t{\n\t";
                }

                std::cout << "\t{\n\t\t\t";

                std::cout << "   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\n\t\t\t";

                for (int square = 0; square < 48; ++square)
                {
                    std::cout << std::setw(4) << std::round(parameters[48 * bucket + square][phase] - psqtPieceValues[bucket][phase * 6]) << ",";
                    if (square % 8 == 7)
                        std::cout << "\n\t\t";
                    if (square != 47)
                        std::cout << "\t";
                }

                std::cout << "\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0\t//\n"
                          << std::endl;
                std::cout << "\t\t},\n\t";

                if (bucket == PSQTBucketCount - 1)
                {
                    std::cout << "}};\n";
                }
            }
        }
    }

    for (int piece = 1; piece < 6; ++piece)
    {
        for (int phase = 0; phase <= 1; ++phase)
        {
            for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
            {
                if (bucket == 0)
                {
                    std::cout << "\nconstexpr static std::array<std::array<int, 64>, PSQTBucketCount> " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table = {\n\t{\n\t";
                }

                std::cout << "\t{\n\t\t\t";

                for (int square = 0; square < 64; ++square)
                {
                    std::cout << std::setw(4) << std::round(parameters[(48 * PSQTBucketCount) +               // 64 - 16, since we're only tuning 48 pawn values
                                                                       (64 * PSQTBucketCount) * (piece - 1) + // piece - 1 since we already took pawns into account
                                                                       (64 * bucket) + square][phase] -
                                                            psqtPieceValues[bucket][piece + phase * 6]); // We substract the 16 non-tuned pawn valeus
                    if (square != 63)
                    {
                        std::cout << ",";

                        if (square % 8 == 7)
                            std::cout << "\n\t\t";

                        std::cout << "\t";
                    }
                }

                std::cout << "\t//\n\t\t},\n\t";

                if (bucket == PSQTBucketCount - 1)
                {
                    std::cout << "}};\n";
                }
            }
        }
    }
    std::cout << std::endl;
}
