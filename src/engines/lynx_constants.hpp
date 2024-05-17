#include <array>
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
        +105, +399, +365, +493, +1129, // 0,
        +128, +440, +392, +769, +1408, // 0
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
        -26,    -25,    -16,    -9,     -3,     29,     29,     -11,
        -27,    -27,    -7,     10,     17,     26,     23,     11,
        -25,    -17,    1,      18,     26,     29,     1,      -1,
        -25,    -13,    0,      20,     27,     27,     1,      -2,
        -23,    -21,    -5,     3,      13,     23,     16,     5,
        -26,    -23,    -20,    -13,    -6,     23,     19,     -19,
        0,      0,      0,      0,      0,      0,      0,      0,
    };

constexpr static std::array<int, 64> EndGamePawnTable =
    {
        0,      0,      0,      0,      0,      0,      0,      0,
        15,     10,     5,      -12,    7,      1,      -7,     -7,
        13,     9,      -1,     -12,    -6,     -7,     -7,     -8,
        27,     17,     0,      -19,    -15,    -13,    4,      0,
        25,     16,     -1,     -15,    -14,    -10,    2,      -2,
        14,     7,      -3,     -10,    -3,     -5,     -6,     -8,
        17,     11,     9,      -11,    16,     4,      -4,     -4,
        0,      0,      0,      0,      0,      0,      0,      0,
    };

constexpr static std::array<int, 64> MiddleGameKnightTable =
    {
        -147,   -24,    -51,    -30,    -14,    -21,    -11,    -98,
        -45,    -28,    -5,     15,     16,     25,     -12,    -17,
        -29,    0,      18,     56,     60,     39,     33,     -4,
        -11,    25,     42,     59,     58,     58,     44,     17,
        -8,     23,     44,     46,     56,     56,     44,     16,
        -27,    2,      18,     48,     57,     32,     26,     -5,
        -46,    -18,    0,      14,     15,     19,     -12,    -18,
        -164,   -26,    -50,    -20,    -9,     -11,    -19,    -88,
    };

constexpr static std::array<int, 64> EndGameKnightTable =
    {
        -62,    -59,    -12,    -12,    -10,    -27,    -53,    -82,
        -17,    0,      13,     8,      8,      5,      -13,    -22,
        -13,    14,     35,     34,     32,     16,     8,      -13,
        5,      18,     47,     48,     52,     46,     23,     -5,
        4,      23,     45,     51,     52,     41,     27,     -1,
        -15,    17,     26,     39,     31,     18,     6,      -9,
        -25,    2,      5,      11,     4,      0,      -12,    -26,
        -66,    -58,    -8,     -16,    -12,    -26,    -51,    -82,
    };

constexpr static std::array<int, 64> MiddleGameBishopTable =
    {
        -17,    15,     -2,     -14,    -10,    -16,    -21,    1,
        6,      2,      7,      -17,    2,      3,      28,     -7,
        -6,     5,      -4,     2,      -8,     14,     5,      28,
        -6,     -6,     -6,     24,     20,     -17,    2,      -1,
        -13,    -1,     -13,    20,     6,      -14,    -6,     5,
        5,      5,      7,      -5,     6,      8,      7,      24,
        9,      13,     11,     -5,     -2,     5,      20,     -1,
        8,      20,     12,     -29,    -11,    -20,    2,      -14,
    };

constexpr static std::array<int, 64> EndGameBishopTable =
    {
        -8,     15,     -12,    4,      -1,     6,      -1,     -25,
        0,      -7,     -3,     5,      2,      -10,    -3,     -15,
        14,     13,     6,      1,      10,     3,      6,      9,
        13,     7,      6,      -4,     -7,     6,      5,      8,
        8,      10,     5,      -1,     -10,    6,      7,      10,
        10,     3,      -1,     -1,     5,      -2,     3,      7,
        -10,    -8,     -14,    3,      1,      -3,     -2,     -7,
        -6,     -13,    -8,     8,      7,      7,      -3,     -11,
    };

constexpr static std::array<int, 64> MiddleGameRookTable =
    {
        -5,     -10,    -5,     2,      14,     3,      6,      -4,
        -26,    -17,    -13,    -11,    1,      5,      19,     -2,
        -28,    -19,    -23,    -12,    5,      11,     50,     30,
        -25,    -20,    -18,    -6,     -4,     10,     38,     21,
        -18,    -16,    -12,    -4,     -6,     8,      29,     14,
        -22,    -15,    -19,    -4,     2,      19,     48,     27,
        -24,    -26,    -10,    -6,     3,      3,      26,     3,
        -4,     -4,     -1,     11,     21,     7,      14,     7,
    };

constexpr static std::array<int, 64> EndGameRookTable =
    {
        5,      3,      7,      -1,     -10,    5,      1,      -3,
        16,     19,     18,     8,      -2,     -2,     -3,     1,
        12,     10,     11,     5,      -8,     -10,    -21,    -18,
        14,     10,     12,     4,      0,      -1,     -14,    -13,
        14,     10,     12,     3,      1,      -6,     -12,    -10,
        13,     12,     4,      -3,     -10,    -14,    -20,    -12,
        19,     21,     14,     4,      -4,     -3,     -6,     2,
        0,      -2,     3,      -8,     -17,    -3,     -7,     -11,
    };

constexpr static std::array<int, 64> MiddleGameQueenTable =
    {
        -11,    -9,     -5,     10,     4,      -30,    8,      -5,
        -2,     -11,    8,      -1,     4,      7,      24,     62,
        -7,     -6,     -9,     -10,    -12,    7,      33,     58,
        -13,    -19,    -19,    -10,    -11,    -4,     11,     24,
        -11,    -16,    -20,    -20,    -11,    -5,     9,      22,
        -8,     -3,     -14,    -11,    -5,     4,      20,     38,
        -15,    -21,    4,      11,     9,      5,      8,      46,
        -9,     -9,     7,      12,     8,      -35,    -14,    25,
    };

constexpr static std::array<int, 64> EndGameQueenTable =
    {
        -24,    -22,    -11,    -11,    -16,    -6,     -33,    14,
        -15,    -10,    -28,    -2,     -3,     -15,    -41,    -7,
        -17,    -6,     5,      0,      22,     21,     -1,     3,
        -10,    7,      7,      12,     27,     37,     44,     36,
        -4,     3,      15,     24,     24,     33,     28,     44,
        -11,    -14,    11,     6,      13,     20,     20,     20,
        -10,    -2,     -22,    -20,    -15,    -15,    -26,    4,
        -15,    -16,    -20,    -5,     -11,    16,     14,     0,
    };

constexpr static std::array<int, 64> MiddleGameKingTable =
    {
        27,     51,     26,     -74,    9,      -61,    38,     48,
        -6,     -16,    -32,    -72,    -85,    -59,    -11,    18,
        -74,    -63,    -104,   -106,   -115,   -128,   -85,    -93,
        -95,    -91,    -110,   -144,   -142,   -137,   -133,   -168,
        -70,    -65,    -99,    -122,   -139,   -122,   -139,   -158,
        -79,    -35,    -91,    -101,   -92,    -104,   -75,    -87,
        76,     -8,     -35,    -63,    -67,    -46,    4,      27,
        41,     77,     39,     -56,    21,     -50,    52,     64,
    };

constexpr static std::array<int, 64> EndGameKingTable =
    {
        -70,    -45,    -19,    7,      -31,    -2,     -36,    -88,
        -15,    18,     27,     39,     46,     34,     13,     -22,
        8,      40,     58,     68,     71,     64,     43,     21,
        11,     51,     73,     87,     86,     79,     65,     39,
        3,      42,     71,     83,     89,     77,     69,     38,
        10,     35,     56,     67,     65,     58,     41,     18,
        -38,    13,     28,     37,     40,     29,     8,      -25,
        -82,    -57,    -26,    -1,     -27,    -5,     -41,    -95,
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

const u64 NotAFile = 0xFEFEFEFEFEFEFEFE;

const u64 NotHFile = 0x7F7F7F7F7F7F7F7F;

[[nodiscard]] static chess::U64 ShiftUp(const chess::U64 board)
{
    return board >> 8;
}

[[nodiscard]] static chess::U64 ShiftDown(const chess::U64 board)
{
    return board << 8;
}

[[nodiscard]] static chess::U64 ShiftLeft(const chess::U64 board)
{
    return (board >> 1) & NotHFile;
}

[[nodiscard]] static chess::U64 ShiftRight(const chess::U64 board)
{
    return (board << 1) & NotAFile;
}

[[nodiscard]] static chess::U64 ShiftUpRight(const chess::U64 board)
{
    return ShiftRight(ShiftUp(board));
}

[[nodiscard]] static chess::U64 ShiftUpLeft(const chess::U64 board)
{
    return ShiftLeft(ShiftUp(board));
}

[[nodiscard]] static chess::U64 ShiftDownRight(const chess::U64 board)
{
    return ShiftRight(ShiftDown(board));
}

[[nodiscard]] static chess::U64 ShiftDownLeft(const chess::U64 board)
{
    return ShiftLeft(ShiftDown(board));
}