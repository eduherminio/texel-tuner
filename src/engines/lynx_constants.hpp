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

constexpr static int PSQTBucketCount = 8;

constexpr static std::array<std::array<int, 12>, PSQTBucketCount> PieceValue = {
    {
        {
            +97, +332, +350, +451, +999,    // 0,
            +128, +405, +348, +708, +1326,  // 0
        },
        {
            +103, +367, +382, +517, +1150,  // 0
            +121, +436, +375, +773, +1463,  // 0
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGamePawnTable = {
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            17, 50, 39, 14, -21, -19, -47, -66,
            18, 21, 29, 12, 9, -1, -38, -54,
            8, 16, 26, 28, 22, 7, -27, -49,
            21, 26, 28, 34, 20, 2, -26, -52,
            37, 37, 29, 6, -1, -10, -45, -74,
            27, 66, 28, 10, -12, -19, -55, -71,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            -31, -32, -23, -14, -5, 32, 46, 7,
            -34, -35, -14, 3, 12, 25, 33, 19,
            -30, -21, -2, 12, 23, 30, 12, 7,
            -30, -18, -4, 13, 25, 29, 12, 8,
            -34, -30, -13, -4, 7, 23, 27, 15,
            -31, -31, -27, -17, -8, 26, 38, 1,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGamePawnTable = {
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            3, -11, -11, -15, 1, 1, 10, 28,
            3, 1, -9, -21, -15, -13, -1, 11,
            25, 16, -4, -22, -12, -10, 10, 16,
            23, 12, -6, -18, -12, -7, 7, 15,
            5, -10, -12, -15, -10, -10, 1, 10,
            4, -14, -3, 3, 5, 3, 16, 30,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            19, 14, 5, -8, 12, 4, -9, -12,
            12, 7, -1, -10, -5, -6, -12, -13,
            27, 16, -1, -18, -15, -14, 3, -2,
            24, 16, -2, -15, -14, -12, 2, -5,
            12, 6, -4, -10, -3, -5, -11, -14,
            22, 15, 9, -10, 19, 7, -7, -10,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameKnightTable = {
    {
        {
            -106, -47, -40, -12, -43, -35, -28, -93,
            -46, -56, 10, 20, 14, 44, -5, -28,
            -37, 15, 44, 67, 59, 22, 13, -10,
            12, 37, 54, 81, 75, 56, 55, 9,
            1, 33, 59, 52, 66, 49, 46, 26,
            -20, 1, 38, 62, 58, 15, 9, -26,
            -24, -14, 19, 11, 12, 12, -19, -32,
            -156, -60, -51, -32, -28, -44, -32, -127 //
        },
        {
            -148, -18, -52, -30, -9, -10, -2, -76,
            -42, -26, -7, 14, 18, 28, -5, 1,
            -34, -5, 9, 47, 51, 33, 27, -6,
            -15, 19, 35, 47, 48, 51, 41, 14,
            -11, 19, 36, 36, 47, 50, 41, 12,
            -34, -4, 8, 40, 47, 26, 20, -6,
            -47, -20, -4, 14, 17, 25, -1, 2,
            -143, -21, -46, -19, -6, -1, -10, -64 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameKnightTable = {
    {
        {
            -66, 5, 2, -16, 5, -30, -10, -64,
            -12, 20, 5, 1, -1, -6, -3, -23,
            7, -0, 7, 19, 21, 12, -3, -12,
            1, 7, 30, 24, 31, 36, 3, -10,
            8, 15, 29, 32, 37, 25, 3, -15,
            -2, 18, 0, 19, 19, 11, -6, 3,
            -15, 7, -4, 12, -2, -9, 2, -10,
            -54, -13, 9, -1, -4, -8, -11, -76 //
        },
        {
            -51, -43, -8, -0, -3, -23, -39, -82,
            -8, 4, 10, 5, 5, -3, -8, -13,
            -13, 8, 26, 27, 21, 6, 1, -8,
            11, 12, 38, 40, 44, 33, 18, -1,
            6, 15, 37, 43, 43, 34, 24, 8,
            -14, 8, 16, 33, 22, 7, -2, -7,
            -15, 8, 2, 7, 1, -6, -10, -20,
            -66, -34, -3, -6, -5, -26, -36, -71 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameBishopTable = {
    {
        {
            -20, -23, -9, -15, -19, -7, -17, -12,
            -14, 45, 20, -1, 1, 7, -2, -11,
            -3, 7, 22, 27, 4, -3, -1, 6,
            -0, 4, 18, 39, 37, -15, -2, -7,
            -18, 17, 6, 34, 16, -6, 2, -2,
            -9, 18, 25, 12, 3, -5, -15, 3,
            -20, 59, 36, 15, -10, -5, -19, -24,
            -7, -9, 1, -37, -18, -8, -14, -74 //
        },
        {
            -13, 14, -9, -19, -12, -8, -16, 14,
            5, -1, 3, -19, 1, 4, 38, -6,
            -10, -0, -12, 2, -14, 7, -2, 27,
            -11, -10, -5, 22, 19, -13, 2, -1,
            -18, -5, -14, 18, 9, -10, -7, 5,
            1, -5, -0, -7, -0, 0, 2, 21,
            8, 9, 5, -8, -4, 9, 31, 9,
            10, 17, 3, -34, -16, -17, 8, 4 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameBishopTable = {
    {
        {
            14, 12, 4, -0, 7, 5, 3, 7,
            19, -16, -9, -9, -6, -13, -1, -9,
            19, 6, -12, -14, -10, 2, -5, 8,
            17, -6, -9, -8, -14, 4, 1, 14,
            13, -2, -11, -9, -7, -6, -4, 10,
            31, -16, -7, -17, -5, -1, -3, -8,
            4, -19, -21, -16, -5, -8, 4, 14,
            13, 13, 11, 16, 5, -2, 6, 28 //
        },
        {
            -2, 19, -4, 6, -1, 3, -1, -26,
            -2, -7, -6, 1, -1, -14, -5, -10,
            12, 8, 7, -1, 10, 1, 3, 13,
            12, 1, 5, 2, 2, 2, -1, 2,
            6, 2, 4, 7, -2, 6, 1, 6,
            7, 1, -1, -2, 2, -5, 0, 14,
            -8, -9, -16, 1, -3, -7, -6, -8,
            2, -14, -3, 5, 11, 6, -6, -16 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameRookTable = {
    {
        {
            -57, -31, -20, 9, 4, 9, 22, 1,
            -7, 6, 15, -6, 0, 10, 3, -18,
            -3, -8, -33, 6, -12, -21, 18, -7,
            -16, 17, -1, -0, -2, 6, 15, 3,
            -0, 12, 9, 6, -10, -8, -9, -0,
            5, 8, 40, 3, -9, 18, 18, -5,
            11, 16, 2, 5, -2, -1, 8, -15,
            -36, -28, -3, 20, 11, 7, 23, 2 //
        },
        {
            -5, -12, -7, -0, 10, 8, 2, 2,
            -33, -21, -14, -8, 4, 15, 38, 4,
            -37, -25, -24, -14, 4, 12, 51, 31,
            -33, -31, -25, -12, -7, 7, 37, 15,
            -27, -25, -20, -9, -8, 9, 35, 15,
            -31, -22, -25, -3, 1, 21, 52, 30,
            -30, -30, -8, -3, 6, 13, 44, 19,
            -4, -5, -3, 9, 18, 14, 11, 21 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameRookTable = {
    {
        {
            26, 22, 26, -1, -6, -17, -21, 3,
            5, 18, 15, 9, -5, -9, -3, 0,
            6, 12, 18, -8, -2, -4, -18, -10,
            9, 2, 7, 4, -8, -11, -17, -13,
            13, 7, 2, -2, 4, -9, -10, -6,
            4, 8, -17, -6, -9, -7, -19, -7,
            9, 13, 13, 0, 1, -5, -8, 6,
            14, 23, 12, -5, -4, -15, -23, -4 //
        },
        {
            8, 3, 7, -0, -7, 2, 8, -15,
            20, 19, 19, 8, -1, -4, -10, 1,
            15, 9, 12, 6, -9, -12, -23, -19,
            17, 12, 13, 2, -2, -3, -15, -14,
            14, 10, 13, 2, -4, -9, -14, -14,
            15, 13, 6, -4, -11, -18, -24, -11,
            21, 22, 15, 5, -4, -6, -10, -5,
            4, -2, 4, -7, -17, -7, 0, -24 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameQueenTable = {
    {
        {
            -34, -60, -48, -14, -4, -1, 5, 23,
            -11, -20, 19, 15, 6, 11, 29, 46,
            -42, -0, -8, 5, -5, 1, 31, 70,
            -4, -32, -7, 13, 7, 4, 20, 39,
            -6, -7, -9, 13, 9, 10, 16, 25,
            -26, -10, -10, -2, 10, -2, 29, 46,
            -49, -48, 18, 25, 19, -3, 22, 49,
            -54, -53, -30, -21, 1, -12, -14, 12 //
        },
        {
            -14, -11, -11, 7, 1, -31, 0, -8,
            -5, -9, 8, -1, 5, 14, 28, 45,
            -9, -7, -8, -10, -13, 8, 31, 47,
            -12, -17, -16, -8, -7, -2, 12, 23,
            -13, -15, -15, -17, -6, -2, 12, 21,
            -8, -5, -14, -10, -9, 4, 17, 29,
            -18, -17, 3, 11, 9, 13, 12, 28,
            -15, -11, -1, 8, 4, -35, -18, 25 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameQueenTable = {
    {
        {
            -40, -3, 34, -2, -13, -48, -72, -56,
            -10, -7, -27, 2, -4, -23, -36, -16,
            36, 6, 33, 3, 27, 10, -41, -33,
            -2, 35, 17, 15, 20, 14, 4, -8,
            1, 20, 32, 21, 17, -5, -9, 14,
            15, 18, 32, 16, 16, 35, -8, 15,
            14, 18, -18, -3, 4, 25, -37, -40,
            -37, -16, 18, 24, 6, -1, 15, -15 //
        },
        {
            -15, -16, -7, -14, -18, -8, -10, 37,
            -15, -10, -27, -5, -6, -20, -46, 7,
            -22, -9, -0, -1, 18, 17, -2, 16,
            -16, -2, 0, 5, 20, 33, 46, 35,
            -7, -6, 4, 16, 16, 31, 23, 43,
            -20, -16, 7, 6, 10, 13, 19, 21,
            -9, -5, -23, -25, -20, -26, -30, 26,
            -2, -12, -13, -9, -16, 11, 19, 7 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameKingTable = {
    {
        {
            330, 351, 334, 276, 0, 0, 0, 0,
            309, 321, 313, 292, 0, 0, 0, 0,
            252, 273, 238, 236, 0, 0, 0, 0,
            233, 251, 231, 180, 0, 0, 0, 0,
            255, 277, 239, 199, 0, 0, 0, 0,
            240, 274, 230, 227, 0, 0, 0, 0,
            347, 313, 286, 280, 0, 0, 0, 0,
            317, 345, 322, 266, 0, 0, 0, 0 //
        },
        {
            0, 0, 0, 0, -108, -171, -78, -90,
            0, 0, 0, 0, -176, -153, -107, -101,
            0, 0, 0, 0, -215, -215, -174, -199,
            0, 0, 0, 0, -260, -227, -220, -256,
            0, 0, 0, 0, -256, -215, -221, -251,
            0, 0, 0, 0, -193, -194, -167, -190,
            0, 0, 0, 0, -163, -142, -91, -93,
            0, 0, 0, 0, -101, -163, -64, -76 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameKingTable = {
    {
        {
            -98, -68, -47, -30, 0, 0, 0, 0,
            -46, -11, -1, 8, 0, 0, 0, 0,
            -26, 15, 44, 55, 0, 0, 0, 0,
            -23, 29, 65, 97, 0, 0, 0, 0,
            -31, 20, 63, 94, 0, 0, 0, 0,
            -22, 15, 46, 58, 0, 0, 0, 0,
            -62, -13, 6, 10, 0, 0, 0, 0,
            -102, -74, -49, -30, 0, 0, 0, 0 //
        },
        {
            0, 0, 0, 0, -26, 1, -32, -79,
            0, 0, 0, 0, 52, 39, 15, -22,
            0, 0, 0, 0, 92, 80, 44, 15,
            0, 0, 0, 0, 128, 98, 67, 27,
            0, 0, 0, 0, 130, 97, 69, 26,
            0, 0, 0, 0, 88, 75, 43, 11,
            0, 0, 0, 0, 48, 36, 10, -25,
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
