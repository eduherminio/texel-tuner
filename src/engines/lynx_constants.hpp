#include <array>
#include <cmath>

using u64 = uint64_t;

constexpr int32_t Pack(const int16_t mg, const int16_t eg) {
    return ((int32_t) eg << 16) + (int32_t) mg;
}

constexpr int16_t UnpackMG(const int32_t packed) {
    return (int16_t) packed;
}

constexpr int16_t UnpackEG(const int32_t packed) {
    return (int16_t) ((packed + 0x8000) >> 16);
}

constexpr int PieceOffset(bool isWhite)
{
    return 6 - (6 * isWhite);
}

constexpr std::array<int, 12> PieceValue = {
        +101, +137, +173, +210, +1115, // 0,
        +129, +179, +209, +382, +1425, // 0
};

constexpr std::array<int, 10> PackedPieceValue = {
    Pack(PieceValue[0], PieceValue[5]),
    Pack(PieceValue[1], PieceValue[6]),
    Pack(PieceValue[2], PieceValue[7]),
    Pack(PieceValue[3], PieceValue[8]),
    Pack(PieceValue[4], PieceValue[9])
};

std::array<int, 6> phaseValues = {0, 1, 1, 2, 4, 0};

constexpr static int EvalNormalizationCoefficient = 142;

constexpr static std::array<int, 64> MiddleGamePawnTable =
    {
        0,      0,      0,      0,      0,      0,      0,      0,
        -24,    -23,    -15,    -5,     -1,     27,     28,     -12,
        -25,    -25,    -5,     11,     18,     26,     22,     9,
        -23,    -16,    4,      18,     25,     30,     1,      -2,
        -23,    -12,    1,      20,     27,     28,     -0,     -3,
        -22,    -19,    -3,     4,      14,     23,     15,     4,
        -24,    -21,    -20,    -9,     -4,     21,     17,     -20,
        0,      0,      0,      0,      0,      0,      0,      0,
    };

constexpr static std::array<int, 64> EndGamePawnTable =
    {
        0,      0,      0,      0,      0,      0,      0,      0,
        14,     9,      4,      -10,    7,      1,      -7,     -7,
        12,     9,      -1,     -12,    -6,     -7,     -7,     -8,
        26,     16,     -1,     -19,    -15,    -14,    3,      0,
        24,     15,     -2,     -15,    -14,    -11,    1,      -2,
        13,     6,      -4,     -10,    -3,     -5,     -6,     -8,
        17,     9,      8,      -9,     15,     4,      -4,     -4,
        0,      0,      0,      0,      0,      0,      0,      0,
    };

constexpr static std::array<int, 64> MiddleGameKnightTable =
    {
        -139,   -17,    -50,    -28,    -8,     -19,    -4,     -88,
        -42,    -26,    -5,     15,     17,     21,     -15,    -13,
        -30,    -1,     17,     51,     56,     37,     31,     -4,
        -12,    22,     38,     53,     52,     54,     42,     16,
        -8,     21,     40,     40,     50,     52,     42,     15,
        -29,    2,      16,     44,     54,     31,     25,     -5,
        -44,    -20,    -2,     14,     16,     16,     -14,    -14,
        -156,   -20,    -48,    -20,    -6,     -12,    -12,    -78,
    };

constexpr static std::array<int, 64> EndGameKnightTable =
    {
        -55,    -38,    -6,     -3,     -3,     -19,    -34,    -76,
        -9,     5,      8,      4,      5,      -0,     -4,     -11,
        -9,     8,      25,     27,     23,     7,      2,      -8,
        11,     12,     38,     40,     43,     37,     17,     -2,
        8,      17,     37,     42,     44,     32,     21,     5,
        -11,    11,     16,     31,     23,     9,      -1,     -5,
        -15,    8,      0,      7,      1,      -5,     -6,     -15,
        -60,    -36,    -1,     -5,     -4,     -18,    -33,    -75,
    };

constexpr static std::array<int, 64> MiddleGameBishopTable =
    {
        -10,    16,     -5,     -18,    -10,    -16,    -20,    4,
        7,      1,      5,      -18,    1,      -2,     26,     -14,
        -6,     4,      -6,     5,      -7,     11,     4,      25,
        -8,     -8,     -3,     24,     22,     -13,    1,      -2,
        -15,    -3,     -11,    20,     9,      -9,     -7,     4,
        4,      3,      5,      -2,     6,      4,      6,      21,
        9,      13,     8,      -6,     -4,     1,      18,     -4,
        14,     20,     7,      -32,    -13,    -22,    1,      -11,
    };

constexpr static std::array<int, 64> EndGameBishopTable =
    {
        -1,     14,     -6,     3,      -0,     8,      -0,     -17,
        -0,     -8,     -7,     1,      -2,     -12,    -3,     -10,
        11,     10,     6,      0,      9,      3,      3,      9,
        11,     3,      6,      6,      4,      7,      1,      5,
        6,      6,      4,      10,     2,      6,      2,      5,
        9,      0,      0,      -2,     4,      -1,     1,      5,
        -9,     -10,    -17,    -1,     -4,     -6,     -3,     -4,
        1,      -11,    -5,     7,      8,      8,      -2,     -5,
    };

constexpr static std::array<int, 64> MiddleGameRookTable =
    {
        -4,     -9,     -4,     3,      16,     5,      8,      -2,
        -28,    -17,    -11,    -9,     4,      6,      20,     -5,
        -31,    -20,    -21,    -11,    6,      11,     50,     25,
        -27,    -23,    -19,    -9,     -5,     6,      35,     16,
        -22,    -18,    -14,    -5,     -8,     5,      26,     12,
        -24,    -16,    -17,    -1,     5,      21,     49,     25,
        -26,    -26,    -7,     -2,     5,      4,      26,     1,
        -2,     -3,     1,      13,     24,     10,     16,     10,
    };

constexpr static std::array<int, 64> EndGameRookTable =
    {
        6,      3,      7,      -2,     -10,    5,      -1,     -3,
        15,     18,     18,     8,      -1,     -2,     -5,     1,
        12,     10,     12,     5,      -6,     -9,     -21,    -17,
        14,     11,     13,     6,      -0,     -0,     -13,    -14,
        13,     10,     13,     4,      1,      -5,     -10,    -10,
        12,     12,     4,      -3,     -9,     -13,    -21,    -12,
        18,     21,     14,     4,      -2,     -2,     -6,     1,
        1,      -2,     2,      -7,     -17,    -3,     -8,     -12,
    };

constexpr static std::array<int, 64> MiddleGameQueenTable =
    {
        -10,    -10,    -10,    5,      1,      -31,    8,      1,
        -3,     -11,    6,      -2,     3,      5,      22,     49,
        -10,    -6,     -8,     -9,     -11,    8,      33,     55,
        -12,    -19,    -18,    -8,     -9,     -4,     11,     24,
        -12,    -15,    -18,    -17,    -8,     -4,     10,     22,
        -7,     -3,     -14,    -11,    -4,     5,      20,     36,
        -16,    -20,    2,      10,     7,      2,      6,      36,
        -8,     -10,    0,      7,      4,      -37,    -13,    25,
    };

constexpr static std::array<int, 64> EndGameQueenTable =
    {
        -26,    -19,    -6,     -8,     -15,    -10,    -33,    8,
        -16,    -9,     -24,    -1,     -3,     -15,    -46,    -7,
        -16,    -5,     5,      1,      21,     19,     -8,     3,
        -10,    8,      9,      13,     27,     37,     44,     30,
        -3,     4,      15,     25,     22,     32,     25,     40,
        -16,    -11,    13,     11,     13,     18,     18,     15,
        -11,    -3,     -20,    -18,    -12,    -11,    -31,    3,
        -17,    -16,    -13,    -1,     -8,     17,     13,     -3,
    };

constexpr static std::array<int, 64> MiddleGameKingTable =
    {
        27,     52,     27,     -77,    5,      -62,    41,     51,
        -13,    -17,    -36,    -73,    -85,    -60,    -12,    18,
        -84,    -70,    -108,   -111,   -118,   -126,   -85,    -97,
        -108,   -98,    -117,   -153,   -150,   -140,   -138,   -162,
        -77,    -71,    -106,   -133,   -149,   -124,   -142,   -159,
        -83,    -47,    -100,   -107,   -96,    -106,   -77,    -89,
        72,     -11,    -40,    -65,    -70,    -49,    3,      25,
        40,     77,     38,     -62,    16,     -52,    54,     65,
    };

constexpr static std::array<int, 64> EndGameKingTable =
    {
        -71,    -45,    -19,    7,      -31,    -1,     -37,    -89,
        -12,    18,     27,     39,     45,     33,     13,     -23,
        10,     43,     59,     69,     72,     64,     44,     22,
        15,     54,     76,     90,     89,     81,     68,     39,
        6,      45,     73,     87,     92,     79,     71,     39,
        12,     39,     58,     69,     67,     59,     43,     18,
        -38,    13,     29,     37,     40,     30,     8,      -25,
        -83,    -57,    -26,    -0,     -27,    -5,     -42,    -95,
    };

constexpr static std::array<std::array<int, 64>, 6> MiddleGamePositionalWhiteTables =
    {
        MiddleGamePawnTable,
        MiddleGameKnightTable,
        MiddleGameBishopTable,
        MiddleGameRookTable,
        MiddleGameQueenTable,
        MiddleGameKingTable};

constexpr static std::array<std::array<int, 64>, 6> EndGamePositionalWhiteTables{
    EndGamePawnTable,
    EndGameKnightTable,
    EndGameBishopTable,
    EndGameRookTable,
    EndGameQueenTable,
    EndGameKingTable};

int MiddleGamePositionalTables(int piece, int square)
{
    int coefficient = 1;
    if (piece >= 6)
    {
        piece -= 6;
        square ^= 56;
        coefficient = -1;
    }

    return MiddleGamePositionalWhiteTables[piece][square] * coefficient;
}

int EndGamePositionalTables(int piece, int square)
{
    int coefficient = 1;
    if (piece >= 6)
    {
        piece -= 6;
        square ^= 56;
        coefficient = -1;
    }

    return EndGamePositionalWhiteTables[piece][square] * coefficient;
}

int PackedPositionalTables(int piece, int square)
{
    int coefficient = 1;
    if (piece >= 6)
    {
        piece -= 6;
        square ^= 56;
        coefficient = -1;
    }

    return Pack(
        MiddleGamePositionalWhiteTables[piece][square] * coefficient,
        EndGamePositionalWhiteTables[piece][square] * coefficient);
}

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

constexpr static std::array<int, 64> Rank =
    {
        7UL, 7UL, 7UL, 7UL, 7UL, 7UL, 7UL, 7UL,
        6UL, 6UL, 6UL, 6UL, 6UL, 6UL, 6UL, 6UL,
        5UL, 5UL, 5UL, 5UL, 5UL, 5UL, 5UL, 5UL,
        4UL, 4UL, 4UL, 4UL, 4UL, 4UL, 4UL, 4UL,
        3UL, 3UL, 3UL, 3UL, 3UL, 3UL, 3UL, 3UL,
        2UL, 2UL, 2UL, 2UL, 2UL, 2UL, 2UL, 2UL,
        1UL, 1UL, 1UL, 1UL, 1UL, 1UL, 1UL, 1UL,
        0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL};

static void print_psqts(const parameters_t &parameters)
{
    int pieceValues[12];

    for (int phase = 0; phase <= 1; ++phase)
    {
        if (phase == 0)
            std::cout << "internal static readonly short[] MiddleGamePieceValues =\n[\n\t";

        else
            std::cout << "\ninternal static readonly short[] EndGamePieceValues =\n[\n\t";

        // Pawns
        {
            int pawnSum = 0;

            for (int square = 0; square < 48; ++square)
            {
                pawnSum += parameters[square][phase];
            }

            auto average = (pawnSum / 48.0);

            auto pieceIndex = phase * 6;
            // pieceValues[pieceIndex] = PieceValue[phase * 5];
            pieceValues[pieceIndex] = static_cast<int>(std::round(average));
            std::cout << "+" << pieceValues[pieceIndex] << ", ";
        }

        for (int piece = 1; piece < 5; ++piece)
        {
            int sum = 0;

            for (int square = 0; square < 64; ++square)
            {
                sum += parameters[piece * 64 - 16 + square][phase]; // Substract 16 since we're only tuning 48 pawn values
            }

            auto average = (sum / 64.0);

            auto pieceIndex = piece + phase * 6;
            // pieceValues[pieceIndex] = PieceValue[piece + phase * 5];
            pieceValues[pieceIndex] = static_cast<int>(std::round(average));
            std::cout << "+" << pieceValues[pieceIndex] << ", ";
        }

        // Kings
        auto kingIndex = 5 + phase * 6;
        pieceValues[kingIndex] = 0;
        std::cout << pieceValues[kingIndex] << ",\n\t";

        for (int piece = 0; piece < 5; ++piece)
        {
            auto pieceIndex = piece + phase * 6;
            std::cout << "-" << pieceValues[pieceIndex] << ", ";
        }

        std::cout << pieceValues[kingIndex] << "\n];\n";
    }

    std::string names[] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};

    // Pawns
    {
        const int piece = 0;
        for (int phase = 0; phase <= 1; ++phase)
        {
            std::cout << "\ninternal static readonly short[] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n[\n\t";

            std::cout << "0,\t0,\t0,\t0,\t0,\t0,\t0,\t0,\n\t";
            for (int square = 0; square < 48; ++square)
            {
                std::cout << round(parameters[square][phase] - pieceValues[phase * 6]) << ", ";
                if (square % 8 == 7)
                    std::cout << "\n";
                if (square != 63)
                    std::cout << "\t";
            }
            std::cout << "0,\t0,\t0,\t0,\t0,\t0,\t0,\t0," << std::endl;

            std::cout << "];\n";
        }
    }

    for (int piece = 1; piece < 6; ++piece)
    {
        for (int phase = 0; phase <= 1; ++phase)
        {
            std::cout << "\ninternal static readonly short[] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n[\n\t";
            for (int square = 0; square < 64; ++square)
            {
                std::cout << round(parameters[piece * 64 - 16 + square][phase] - pieceValues[piece + phase * 6]) << ", "; // We substract the 16 non-tuned pawn valeus
                if (square % 8 == 7)
                    std::cout << "\n";
                if (square != 63)
                    std::cout << "\t";
            }
            std::cout << "];\n";
        }
    }
    std::cout << std::endl;
}
