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
        +101, +392, +170, +208, +1114, // 0,
        +129, +445, +207, +381, +1427, // 0
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
        -24,    -23,    -15,    -7,     -2,     27,     28,     -12,
        -25,    -26,    -6,     11,     18,     25,     22,     9,
        -23,    -16,    3,      18,     25,     30,     0,      -2,
        -23,    -12,    1,      19,     27,     27,     -1,     -4,
        -22,    -19,    -4,     4,      14,     23,     15,     4,
        -25,    -21,    -19,    -11,    -4,     22,     17,     -20,
        0,      0,      0,      0,      0,      0,      0,      0,
    };

constexpr static std::array<int, 64> EndGamePawnTable =
    {
        0,      0,      0,      0,      0,      0,      0,      0,
        14,     9,      4,      -9,     7,      1,      -7,     -7,
        12,     9,      -1,     -12,    -6,     -7,     -7,     -8,
        26,     16,     -1,     -19,    -15,    -14,    3,      0,
        24,     15,     -2,     -15,    -14,    -11,    1,      -2,
        13,     6,      -3,     -10,    -3,     -5,     -6,     -8,
        17,     9,      8,      -8,     15,     4,      -4,     -4,
        0,      0,      0,      0,      0,      0,      0,      0,
    };

constexpr static std::array<int, 64> MiddleGameKnightTable =
    {
        -146,   -23,    -52,    -33,    -13,    -22,    -10,    -98,
        -45,    -27,    -3,     17,     18,     23,     -15,    -18,
        -28,    2,      20,     57,     60,     40,     34,     -4,
        -10,    25,     45,     60,     60,     60,     45,     18,
        -6,     24,     46,     47,     58,     59,     45,     17,
        -26,    4,      20,     49,     58,     33,     27,     -6,
        -47,    -19,    0,      16,     16,     18,     -13,    -19,
        -164,   -25,    -49,    -24,    -11,    -13,    -18,    -89,
    };

constexpr static std::array<int, 64> EndGameKnightTable =
    {
        -66,    -49,    -11,    -10,    -9,     -26,    -43,    -87,
        -19,    1,      13,     8,      8,      5,      -10,    -20,
        -14,    14,     35,     35,     32,     17,     8,      -14,
        6,      19,     47,     48,     51,     45,     23,     -8,
        3,      24,     46,     51,     52,     41,     27,     -2,
        -16,    17,     25,     40,     32,     18,     5,      -10,
        -25,    3,      6,      11,     4,      0,      -11,    -24,
        -71,    -46,    -6,     -12,    -10,    -25,    -42,    -87,
    };

constexpr static std::array<int, 64> MiddleGameBishopTable =
    {
        -10,    16,     -5,     -18,    -11,    -16,    -20,    4,
        7,      2,      5,      -19,    0,      -1,     26,     -13,
        -7,     4,      -7,     5,      -7,     10,     4,      25,
        -8,     -7,     -3,     24,     22,     -13,    1,      -2,
        -15,    -3,     -11,    20,     9,      -9,     -7,     4,
        4,      3,      4,      -2,     6,      3,      6,      21,
        9,      13,     9,      -7,     -4,     1,      19,     -3,
        14,     20,     8,      -32,    -13,    -21,    1,      -11,
    };

constexpr static std::array<int, 64> EndGameBishopTable =
    {
        -2,     13,     -7,     3,      -1,     7,      -1,     -18,
        -1,     -8,     -7,     0,      -2,     -13,    -4,     -11,
        11,     9,      6,      -0,     9,      3,      2,      8,
        11,     2,      6,      5,      4,      6,      1,      4,
        6,      5,      4,      9,      1,      6,      2,      5,
        8,      -0,     -0,     -3,     4,      -2,     0,      5,
        -10,    -11,    -18,    -2,     -4,     -7,     -3,     -5,
        0,      -12,    -6,     7,      7,      7,      -3,     -6,
    };

constexpr static std::array<int, 64> MiddleGameRookTable =
    {
         -3,     -9,     -4,     3,      16,     5,      8,      -2,
        -28,    -17,    -11,    -9,     4,      6,      20,     -5,
        -31,    -20,    -21,    -10,    6,      11,     50,     26,
        -26,    -23,    -19,    -8,     -5,     6,      36,     17,
        -21,    -18,    -14,    -5,     -7,     6,      27,     12,
        -23,    -16,    -17,    -1,     5,      21,     49,     26,
        -26,    -26,    -7,     -2,     5,      4,      27,     1,
        -1,     -3,     1,      13,     24,     10,     16,     10,
    };

constexpr static std::array<int, 64> EndGameRookTable =
    {
        6,      3,      7,      -2,     -10,    5,      -1,     -3,
        15,     18,     18,     9,      -1,     -2,     -5,     1,
        12,     10,     12,     5,      -6,     -9,     -21,    -18,
        14,     11,     13,     6,      -0,     -0,     -13,    -14,
        13,     10,     13,     4,      1,      -5,     -10,    -10,
        12,     12,     4,      -3,     -9,     -13,    -21,    -12,
        18,     21,     14,     4,      -3,     -2,     -6,     1,
        1,      -2,     2,      -7,     -17,    -3,     -9,     -12,
    };

constexpr static std::array<int, 64> MiddleGameQueenTable =
    {
        -10,    -11,    -10,    5,      1,      -32,    7,      2,
        -3,     -11,    6,      -2,     2,      5,      22,     49,
        -10,    -6,     -9,     -10,    -11,    7,      33,     55,
        -12,    -19,    -19,    -9,     -9,     -5,     11,     25,
        -12,    -15,    -18,    -18,    -9,     -4,     9,      22,
        -7,     -4,     -15,    -11,    -5,     4,      20,     36,
        -16,    -20,    2,      9,      7,      2,      6,      36,
        -8,     -11,    0,      7,      4,      -37,    -14,    25,
    };

constexpr static std::array<int, 64> EndGameQueenTable =
    {
        -26,    -18,    -6,     -8,     -15,    -10,    -33,    7,
        -17,    -9,     -25,    -1,     -2,     -16,    -46,    -7,
        -16,    -5,     6,      2,      22,     20,     -8,     2,
        -10,    8,      9,      14,     27,     37,     44,     29,
        -3,     4,      15,     25,     23,     33,     24,     39,
        -16,    -11,    14,     11,     13,     19,     18,     14,
        -11,    -3,     -20,    -18,    -12,    -11,    -31,    2,
        -18,    -15,    -13,    -2,     -7,     16,     13,     -3,
    };

constexpr static std::array<int, 64> MiddleGameKingTable =
    {
        28,     52,     27,     -77,    6,      -61,    41,     51,
        -13,    -17,    -36,    -73,    -85,    -59,    -11,    18,
        -84,    -70,    -109,   -110,   -118,   -126,   -85,    -97,
        -107,   -97,    -117,   -153,   -150,   -139,   -138,   -162,
        -77,    -70,    -105,   -133,   -149,   -124,   -142,   -159,
        -83,    -47,    -100,   -107,   -96,    -106,   -76,    -88,
        72,     -10,    -39,    -65,    -70,    -48,    3,      26,
        41,     77,     39,     -62,    17,     -52,    55,     65,
    };

constexpr static std::array<int, 64> EndGameKingTable =
    {
        -72,    -45,    -20,    7,      -31,    -1,     -38,    -90,
        -12,    18,     27,     39,     45,     33,     12,     -23,
        10,     43,     59,     69,     72,     64,     44,     22,
        15,     54,     75,     90,     89,     81,     68,     39,
        5,      45,     73,     86,     92,     79,     71,     39,
        12,     39,     58,     68,     67,     59,     43,     18,
        -38,    13,     29,     37,     40,     29,     8,      -25,
        -83,    -57,    -27,    -0,     -27,    -5,     -42,    -95,
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
