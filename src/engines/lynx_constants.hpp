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
            +97, +332, +350, +451, +998,   // 0
            +127, +404, +348, +707, +1327, // 0
        },
        {
            +103, +367, +382, +517, +1150, // 0
            +121, +436, +376, +773, +1464, // 0
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGamePawnTable = {
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            16, 49, 38, 14, -22, -19, -47, -66,
            18, 21, 29, 12, 9, -1, -38, -54,
            9, 16, 26, 28, 21, 7, -27, -48,
            21, 25, 29, 34, 20, 2, -26, -52,
            37, 37, 29, 6, 0, -9, -45, -73,
            26, 66, 27, 9, -13, -19, -55, -71,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            -31, -31, -23, -13, -5, 32, 46, 7,
            -34, -35, -14, 3, 12, 25, 33, 19,
            -30, -21, -2, 12, 24, 30, 12, 7,
            -30, -18, -4, 13, 26, 29, 12, 8,
            -34, -30, -13, -4, 7, 23, 27, 15,
            -31, -30, -27, -17, -8, 26, 38, 1,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGamePawnTable = {
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            3, -11, -10, -15, 1, 1, 10, 27,
            3, 1, -8, -21, -15, -13, -1, 11,
            26, 17, -3, -22, -12, -11, 9, 15,
            23, 13, -6, -18, -11, -7, 6, 15,
            6, -10, -12, -15, -10, -10, 0, 9,
            4, -14, -3, 3, 5, 3, 16, 29,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            19, 14, 5, -8, 11, 4, -9, -12,
            12, 6, -2, -10, -6, -6, -12, -13,
            27, 16, -1, -18, -15, -13, 4, -2,
            24, 16, -2, -15, -14, -12, 2, -5,
            12, 6, -4, -10, -3, -5, -11, -14,
            22, 14, 9, -10, 19, 7, -7, -10,
            0, 0, 0, 0, 0, 0, 0, 0 //

        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameKnightTable = {
    {
        {
            -105, -48, -40, -12, -44, -37, -30, -93,
            -47, -57, 10, 20, 14, 44, -5, -28,
            -38, 15, 43, 68, 58, 22, 14, -11,
            12, 37, 54, 81, 75, 56, 55, 8,
            1, 33, 60, 51, 66, 49, 47, 26,
            -20, 1, 37, 63, 59, 16, 9, -26,
            -24, -13, 19, 11, 12, 12, -19, -32,
            -155, -61, -50, -32, -28, -45, -33, -125 //
        },
        {
            -148, -18, -52, -30, -9, -10, -2, -76,
            -42, -26, -7, 14, 18, 28, -5, 1,
            -34, -5, 9, 47, 50, 33, 27, -6,
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
            -67, 6, 1, -16, 5, -29, -9, -64,
            -12, 20, 5, 1, -1, -5, -2, -21,
            7, -0, 7, 19, 21, 12, -4, -12,
            1, 7, 30, 23, 30, 35, 3, -10,
            8, 15, 29, 32, 37, 24, 3, -15,
            -2, 18, 0, 19, 19, 11, -6, 3,
            -15, 7, -4, 12, -2, -8, 3, -9,
            -55, -12, 9, -1, -4, -7, -10, -76 //
        },
        {
            -51, -43, -8, -0, -3, -23, -38, -82,
            -8, 4, 10, 5, 5, -3, -8, -13,
            -13, 8, 26, 27, 21, 6, 1, -8,
            11, 12, 37, 40, 44, 33, 18, -1,
            6, 15, 37, 43, 43, 34, 24, 8,
            -14, 8, 16, 33, 22, 7, -1, -7,
            -15, 8, 2, 7, 1, -6, -10, -20,
            -66, -35, -3, -6, -5, -26, -36, -71 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameBishopTable = {
    {
        {
            -20, -22, -10, -14, -20, -7, -18, -13,
            -14, 45, 20, -2, 1, 7, -2, -14,
            -3, 9, 21, 27, 4, -4, -1, 6,
            -0, 5, 18, 39, 37, -16, -2, -6,
            -17, 17, 5, 35, 16, -6, 2, -2,
            -9, 18, 25, 12, 3, -6, -14, 3,
            -21, 60, 36, 15, -11, -5, -19, -24,
            -7, -8, 1, -37, -18, -9, -14, -76 //
        },
        {
            -13, 14, -9, -19, -12, -8, -16, 14,
            5, -2, 3, -19, 1, 4, 38, -6,
            -10, -0, -12, 2, -14, 7, -2, 27,
            -11, -10, -5, 22, 19, -13, 2, -1,
            -18, -5, -14, 18, 9, -10, -7, 5,
            1, -4, -0, -6, -0, 0, 2, 21,
            8, 9, 5, -8, -4, 9, 31, 9,
            10, 17, 3, -34, -16, -17, 7, 4 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameBishopTable = {
    {
        {
            14, 12, 4, -0, 8, 5, 4, 7,
            18, -17, -10, -9, -6, -12, -1, -7,
            19, 6, -11, -15, -10, 2, -5, 7,
            17, -5, -9, -8, -14, 4, 1, 14,
            13, -2, -10, -9, -7, -6, -5, 10,
            30, -15, -8, -17, -5, -1, -3, -8,
            4, -20, -21, -17, -5, -8, 5, 14,
            13, 12, 11, 16, 4, -1, 6, 29 //
        },
        {
            -2, 20, -4, 6, -1, 3, -2, -26,
            -2, -7, -6, 1, -1, -14, -5, -10,
            12, 8, 7, -1, 10, 1, 3, 13,
            12, 1, 5, 2, 3, 2, -1, 2,
            5, 2, 4, 7, -2, 6, 1, 6,
            7, 1, -1, -2, 2, -5, 0, 14,
            -8, -9, -16, 1, -3, -7, -6, -8,
            2, -14, -3, 5, 11, 6, -6, -17 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameRookTable = {
    {
        {
            -58, -31, -19, 8, 4, 8, 22, 1,
            -9, 7, 16, -6, 1, 10, 3, -17,
            -4, -8, -33, 6, -13, -22, 18, -6,
            -16, 16, -0, -1, -3, 5, 16, 3,
            -1, 12, 10, 6, -10, -8, -8, 0,
            5, 8, 40, 2, -9, 17, 18, -5,
            11, 16, 2, 5, -2, -1, 7, -15,
            -35, -27, -2, 20, 11, 7, 23, 2 //
        },
        {
            -5, -12, -7, -0, 10, 8, 2, 1,
            -33, -21, -14, -8, 4, 15, 39, 5,
            -37, -25, -24, -14, 4, 12, 51, 31,
            -33, -31, -25, -12, -7, 7, 37, 15,
            -27, -25, -20, -9, -8, 9, 34, 15,
            -31, -22, -25, -3, 1, 21, 52, 30,
            -30, -30, -9, -3, 6, 13, 44, 19,
            -4, -5, -3, 9, 18, 14, 11, 20 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameRookTable = {
    {
        {
            26, 21, 25, -1, -6, -17, -20, 2,
            5, 16, 14, 8, -6, -9, -1, 1,
            6, 12, 18, -8, -2, -4, -18, -10,
            10, 2, 7, 4, -8, -11, -17, -14,
            13, 8, 2, -2, 3, -8, -11, -7,
            4, 8, -17, -6, -9, -6, -19, -7,
            8, 12, 13, 0, 1, -3, -6, 7,
            14, 22, 11, -5, -5, -15, -22, -4 //
        },
        {
            8, 3, 7, -0, -7, 2, 7, -14,
            21, 19, 19, 8, -0, -4, -11, 1,
            15, 9, 12, 6, -9, -12, -23, -19,
            17, 12, 13, 2, -2, -3, -14, -14,
            14, 9, 13, 2, -4, -9, -14, -14,
            15, 13, 6, -4, -11, -18, -24, -11,
            21, 22, 15, 5, -4, -6, -10, -5,
            4, -2, 4, -7, -17, -8, -0, -23 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameQueenTable = {
    {
        {
            -34, -60, -49, -16, -5, -3, 5, 23,
            -11, -20, 19, 15, 5, 10, 28, 46,
            -41, -0, -8, 5, -5, 0, 33, 71,
            -4, -31, -6, 13, 8, 5, 21, 40,
            -5, -7, -8, 13, 9, 10, 17, 26,
            -25, -10, -9, -3, 11, -3, 29, 46,
            -49, -47, 19, 24, 19, -3, 21, 49,
            -55, -53, -30, -21, 0, -13, -16, 11 //
        },
        {
            -14, -10, -10, 7, 1, -31, -0, -7,
            -5, -9, 8, -1, 5, 14, 28, 45,
            -9, -7, -8, -10, -13, 8, 31, 47,
            -12, -17, -16, -8, -7, -2, 12, 23,
            -13, -15, -15, -17, -6, -2, 12, 21,
            -8, -5, -14, -10, -9, 4, 17, 29,
            -18, -17, 3, 11, 9, 13, 12, 29,
            -15, -11, -1, 8, 4, -35, -17, 26 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameQueenTable = {
    {
        {
            -39, -2, 35, -1, -12, -46, -69, -55,
            -11, -8, -28, 1, -4, -21, -34, -15,
            36, 6, 32, 2, 28, 10, -43, -35,
            -2, 35, 16, 15, 19, 12, 2, -11,
            -0, 19, 31, 22, 16, -5, -11, 11,
            14, 19, 30, 16, 16, 36, -9, 14,
            12, 16, -20, -5, 5, 27, -35, -40,
            -35, -16, 18, 23, 7, 2, 20, -13 //
        },
        {
            -15, -17, -7, -14, -19, -8, -11, 36,
            -15, -10, -27, -5, -6, -20, -46, 6,
            -22, -9, -0, -1, 18, 18, -1, 17,
            -16, -2, 0, 6, 21, 34, 46, 35,
            -7, -5, 4, 16, 16, 31, 23, 44,
            -20, -16, 7, 7, 10, 13, 19, 21,
            -9, -5, -22, -25, -20, -26, -30, 26,
            -2, -12, -13, -9, -16, 11, 18, 6 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameKingTable = {
    {
        {
            135, 139, 130, 102, 0, 0, 0, 0,
            122, 131, 124, 112, 0, 0, 0, 0,
            93, 102, 79, 81, 0, 0, 0, 0,
            72, 86, 80, 44, 0, 0, 0, 0,
            87, 98, 81, 57, 0, 0, 0, 0,
            81, 105, 79, 77, 0, 0, 0, 0,
            155, 132, 111, 107, 0, 0, 0, 0,
            135, 145, 126, 95, 0, 0, 0, 0 //
        },
        {
            0, 0, 0, 0, -34, -60, -13, -13,
            0, 0, 0, 0, -75, -63, -42, -28,
            0, 0, 0, 0, -120, -120, -90, -88,
            0, 0, 0, 0, -167, -131, -123, -133,
            0, 0, 0, 0, -165, -126, -123, -125,
            0, 0, 0, 0, -116, -109, -85, -89,
            0, 0, 0, 0, -71, -65, -37, -27,
            0, 0, 0, 0, -29, -57, -12, -8 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameKingTable = {
    {
        {
            -59, -39, -24, -16, 0, 0, 0, 0,
            -28, -7, -1, 5, 0, 0, 0, 0,
            -15, 9, 26, 31, 0, 0, 0, 0,
            -13, 20, 41, 59, 0, 0, 0, 0,
            -17, 15, 41, 57, 0, 0, 0, 0,
            -12, 10, 29, 35, 0, 0, 0, 0,
            -37, -8, 4, 7, 0, 0, 0, 0,
            -61, -44, -27, -16, 0, 0, 0, 0 //
        },
        {
            0, 0, 0, 0, -13, 0, -17, -42,
            0, 0, 0, 0, 28, 23, 9, -13,
            0, 0, 0, 0, 54, 48, 26, 7,
            0, 0, 0, 0, 77, 58, 42, 15,
            0, 0, 0, 0, 76, 58, 43, 14,
            0, 0, 0, 0, 55, 46, 25, 5,
            0, 0, 0, 0, 26, 23, 6, -16,
            0, 0, 0, 0, -10, -2, -20, -47 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameEnemyKingTable = {
    {
        {
            -17, -36, -27, 3, 140, 166, 115, 134,
            5, -23, -31, 0, 140, 152, 137, 127,
            -16, 12, 36, 11, 152, 157, 147, 134,
            42, 37, 21, 56, 170, 128, 116, 110,
            14, 43, 39, 41, 167, 126, 121, 87,
            -3, -11, 16, 12, 154, 130, 139, 162,
            -41, -16, -15, 1, 142, 138, 121, 117,
            -22, -23, -9, 16, 144, 160, 101, 118 //
        },
        {
            -149, -163, -155, -123, -1, 38, -7, 4,
            -156, -138, -133, -130, 33, 16, -10, 1,
            -91, -122, -113, -98, 21, 19, 8, 43,
            -113, -114, -96, -93, 12, 30, 36, 66,
            -106, -131, -110, -94, 12, 22, 33, 72,
            -89, -104, -91, -87, -3, 13, 6, 25,
            -131, -126, -118, -118, 19, 2, -20, -7,
            -122, -148, -147, -120, -3, 33, -20, -5 //
        },
    }};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameEnemyKingTable = {
    {
        {
            27, 13, 15, 11, 5, 6, 32, 42,
            7, 3, 0, -5, -23, -18, -8, 10,
            18, -7, -20, -22, -40, -33, -24, -5,
            14, -4, -20, -41, -56, -40, -23, -9,
            23, -3, -23, -40, -56, -39, -28, -9,
            21, -0, -14, -19, -36, -27, -25, -18,
            24, 3, -4, -3, -22, -13, -6, 9,
            34, 20, 8, 10, 4, 11, 30, 39 //
        },
        {
            49, 39, 26, 15, 14, -3, 12, 35,
            30, 6, 1, -3, -24, -16, -5, 10,
            4, -6, -19, -26, -38, -30, -15, -7,
            3, -16, -29, -38, -49, -40, -25, -11,
            2, -11, -25, -37, -53, -38, -24, -10,
            -1, -11, -22, -28, -31, -28, -16, -1,
            24, 7, -2, -6, -21, -14, -4, 10,
            43, 35, 27, 14, 13, -3, 13, 36 //
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

constexpr int PackedEnemyKingTables(chess::Color color, int bucket, int square)
{
    int coefficient = 1;
    if (color == chess::Color::BLACK)
    {
        square ^= 56;
        coefficient = -1;
    }

    return Pack(
        MiddleGameEnemyKingTable[bucket][square] * coefficient,
        EndGameEnemyKingTable[bucket][square] * coefficient);
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

    for (int phase = 0; phase <= 1; ++phase)
    {
        for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
        {
            if (bucket == 0)
            {
                std::cout << "\n\tinternal static readonly short[][] " << (phase == 0 ? "MiddleGame" : "EndGame") << "EnemyKing" << "Table =\n\t[\n";
            }

            std::cout << "\t\t[\n\t\t\t";

            for (int square = 0; square < 64; ++square)
            {
                std::cout << std::setw(4) << std::round(parameters[(48 * PSQTBucketCount) +     // 64 - 16, since we're only tuning 48 pawn values
                                                                   (64 * PSQTBucketCount * 5) + // piece - 1 since we already took pawns into account
                                                                   (64 * bucket) + square][phase])
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
                                                            psqtPieceValues[bucket][piece + phase * 6]);
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

    for (int phase = 0; phase <= 1; ++phase)
    {
        for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
        {
            if (bucket == 0)
            {
                std::cout << "\nconstexpr static std::array<std::array<int, 64>, PSQTBucketCount> " << (phase == 0 ? "MiddleGame" : "EndGame") << "EnemyKing" << "Table = {\n\t{\n\t";
            }

            std::cout << "\t{\n\t\t\t";

            for (int square = 0; square < 64; ++square)
            {
                std::cout << std::setw(4) << std::round(parameters[(48 * PSQTBucketCount) +     // 64 - 16, since we're only tuning 48 pawn values
                                                                   (64 * PSQTBucketCount * 5) + // piece - 1 since we already took pawns into account
                                                                   (64 * bucket) + square][phase]);
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
    std::cout << std::endl;
}
