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

constexpr std::array<int, 12> PieceValue = {
    +101, +357, +374, +507, +1121, // 0
    +128, +427, +361, +747, +1423, // 0
};

constexpr static std::array<int, 64> MiddleGamePawnTable = {
    0, 0, 0, 0, 0, 0, 0, 0,
    -24, -22, -15, -11, -3, 18, 23, -16,
    -25, -25, -5, 12, 19, 26, 23, 9,
    -23, -16, 4, 19, 27, 30, 2, -3,
    -23, -11, 2, 20, 28, 28, 1, -4,
    -22, -18, -3, 5, 14, 23, 15, 3,
    -25, -20, -20, -14, -6, 12, 12, -23,
    0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> EndGamePawnTable = {
    0, 0, 0, 0, 0, 0, 0, 0,
    15, 8, 3, -9, 5, 2, -7, -4,
    13, 9, -1, -14, -8, -8, -8, -7,
    27, 16, -1, -19, -16, -14, 3, 1,
    24, 15, -1, -15, -14, -11, 2, -1,
    14, 6, -4, -12, -5, -6, -7, -8,
    17, 8, 7, -7, 13, 5, -4, -1,
    0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> MiddleGameKnightTable = {
    -142, -22, -53, -30, -12, -14, -10, -79,
    -43, -29, -8, 14, 16, 26, -9, -2,
    -31, -1, 14, 49, 55, 37, 32, -5,
    -13, 21, 37, 52, 51, 53, 40, 16,
    -9, 20, 38, 38, 48, 51, 40, 15,
    -29, 1, 15, 42, 52, 31, 26, -5,
    -45, -21, -5, 13, 14, 21, -7, -2,
    -156, -25, -50, -22, -10, -6, -17, -67};

constexpr static std::array<int, 64> EndGameKnightTable = {
    -55, -35, -4, -1, -2, -24, -29, -80,
    -8, 7, 11, 5, 5, -1, -6, -14,
    -9, 6, 23, 26, 20, 3, -3, -9,
    11, 11, 36, 37, 41, 32, 15, -4,
    7, 16, 35, 41, 42, 30, 19, 3,
    -12, 9, 13, 31, 20, 4, -6, -6,
    -13, 9, 2, 8, 1, -6, -7, -19,
    -60, -33, 1, -4, -3, -23, -28, -79};

constexpr static std::array<int, 64> MiddleGameBishopTable = {
    -13, 13, -11, -22, -16, -11, -20, 14,
    4, -1, 2, -19, -1, 1, 33, -6,
    -9, 5, -7, 4, -7, 10, 5, 23,
    -11, -9, -2, 26, 23, -12, -1, -5,
    -18, -4, -11, 21, 9, -9, -8, 1,
    1, 4, 4, -4, 4, 4, 8, 19,
    7, 10, 6, -7, -6, 4, 25, 5,
    11, 17, 2, -36, -19, -17, 2, -3};

constexpr static std::array<int, 64> EndGameBishopTable = {
    3, 17, -1, 7, 3, 4, 3, -18,
    4, -5, -5, 0, -2, -13, -5, -10,
    14, 6, 2, -4, 2, -2, -3, 12,
    14, -0, 1, -3, -4, -0, -2, 7,
    8, 2, -0, 1, -5, 0, -1, 8,
    12, -4, -4, -6, -2, -7, -6, 9,
    -5, -7, -15, -1, -3, -7, -5, -4,
    5, -8, -0, 10, 11, 6, -0, -5};

constexpr static std::array<int, 64> MiddleGameRookTable = {
    -6, -12, -8, -1, 8, 7, 9, 1,
    -31, -19, -13, -10, 2, 10, 26, -3,
    -33, -20, -22, -11, 6, 12, 51, 25,
    -28, -23, -19, -8, -4, 8, 36, 16,
    -23, -18, -14, -4, -7, 8, 28, 11,
    -25, -16, -17, -1, 5, 22, 51, 25,
    -28, -28, -8, -3, 4, 8, 32, 2,
    -4, -5, -3, 8, 15, 11, 17, 12};

constexpr static std::array<int, 64> EndGameRookTable = {
    8, 6, 10, 2, -7, 0, -1, -5,
    18, 20, 20, 10, -0, -4, -7, 2,
    14, 9, 11, 3, -10, -13, -25, -18,
    15, 10, 10, 2, -4, -6, -18, -16,
    14, 9, 10, 1, -3, -11, -15, -12,
    13, 12, 3, -4, -13, -17, -26, -12,
    21, 23, 16, 5, -2, -4, -8, 1,
    4, 0, 5, -4, -14, -7, -8, -13};

constexpr static std::array<int, 64> MiddleGameQueenTable = {
    -13, -13, -14, 4, -2, -29, 6, 1,
    -4, -11, 5, -2, 2, 8, 25, 48,
    -10, -5, -8, -9, -10, 10, 34, 54,
    -14, -19, -17, -7, -8, -2, 11, 23,
    -14, -15, -17, -16, -8, -3, 10, 21,
    -8, -2, -14, -10, -4, 7, 22, 35,
    -18, -21, 1, 10, 6, 5, 10, 36,
    -11, -12, -4, 6, -0, -35, -14, 24};

constexpr static std::array<int, 64> EndGameQueenTable = {
    -22, -16, -2, -9, -15, -14, -31, 10,
    -12, -7, -23, -1, -2, -19, -47, -3,
    -15, -7, 4, -0, 17, 14, -12, 3,
    -7, 8, 6, 10, 23, 32, 41, 31,
    0, 3, 12, 21, 19, 28, 21, 41,
    -15, -14, 12, 9, 9, 13, 12, 15,
    -7, -1, -18, -18, -12, -14, -32, 5,
    -12, -12, -9, -4, -8, 12, 15, -2};

constexpr static std::array<int, 64> MiddleGameKingTable = {
    6, 39, 35, -65, 18, -55, 35, 25,
    -23, -15, -19, -54, -67, -45, -6, 3,
    -81, -60, -95, -95, -103, -111, -75, -97,
    -97, -77, -101, -148, -142, -119, -118, -154,
    -66, -51, -90, -126, -139, -105, -122, -150,
    -79, -39, -88, -90, -79, -89, -67, -88,
    59, -9, -21, -42, -46, -31, 10, 12,
    18, 59, 44, -47, 32, -43, 50, 38};

constexpr static std::array<int, 64> EndGameKingTable = {
    -77, -48, -28, -1, -41, -9, -43, -90,
    -22, 15, 24, 37, 44, 31, 9, -28,
    -3, 40, 69, 81, 83, 73, 39, 10,
    0, 51, 90, 122, 119, 91, 62, 23,
    -10, 43, 87, 118, 122, 90, 64, 22,
    -2, 37, 69, 81, 78, 68, 39, 5,
    -47, 11, 27, 35, 37, 27, 4, -31,
    -89, -57, -34, -9, -38, -14, -47, -95};

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

constexpr std::array<int, 10> PackedPieceValue = {
    Pack(PieceValue[0], PieceValue[5]),
    Pack(PieceValue[1], PieceValue[6]),
    Pack(PieceValue[2], PieceValue[7]),
    Pack(PieceValue[3], PieceValue[8]),
    Pack(PieceValue[4], PieceValue[9])};

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

static void print_psqts_csharp(const parameters_t &parameters, std::array<tune_t, 12> &existingPieceValues)
{
    std::array<tune_t, 12> psqtPieceValues;

    // Exctract and print pieces values
    for (int phase = 0; phase <= 1; ++phase)
    {
        if (phase == 0)
            std::cout << "internal static readonly short[] MiddleGamePieceValues =\n[\n\t";

        else
            std::cout << "\ninternal static readonly short[] EndGamePieceValues =\n[\n\t";

        // Pawns
        {
            tune_t pawnSum = 0;

            for (int square = 0; square < 48; ++square)
            {
                pawnSum += parameters[square][phase];
            }

            auto average = (pawnSum / 48.0);

            auto pieceIndex = phase * 6;
            // pieceValues[pieceIndex] = PieceValue[phase * 5];
            psqtPieceValues[pieceIndex] = average;
            std::cout << "+" << std::round(psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex]) << ", ";
        }

        for (int piece = 1; piece < 5; ++piece)
        {
            tune_t sum = 0;

            for (int square = 0; square < 64; ++square)
            {
                sum += parameters[piece * 64 - 16 + square][phase]; // Substract 16 since we're only tuning 48 pawn values
            }

            auto average = (sum / 64.0);

            auto pieceIndex = piece + phase * 6;
            // pieceValues[pieceIndex] = PieceValue[piece + phase * 5];
            psqtPieceValues[pieceIndex] = average;
            std::cout << "+" << std::round(psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex]) << ", ";
        }

        // Kings
        auto kingIndex = 5 + phase * 6;
        psqtPieceValues[kingIndex] = 0;
        std::cout << psqtPieceValues[kingIndex] + existingPieceValues[kingIndex] << ",\n\t";

        for (int piece = 0; piece < 5; ++piece)
        {
            auto pieceIndex = piece + phase * 6;
            std::cout << "-" << std::round(psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex]) << ", ";
        }

        std::cout << std::round(psqtPieceValues[kingIndex] + existingPieceValues[kingIndex]) << "\n];\n";
    }

    // Print PSQTs

    std::string names[] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};

    // Pawns
    {
        const int piece = 0;
        for (int phase = 0; phase <= 1; ++phase)
        {
            std::cout << "\ninternal static readonly short[] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n[\n\t";

            std::cout << "   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\n\t";

            for (int square = 0; square < 48; ++square)
            {
                std::cout << std::setw(4) << std::round(parameters[square][phase] - psqtPieceValues[phase * 6]) << ",";
                if (square % 8 == 7)
                    std::cout << "\n";
                if (square != 47)
                    std::cout << "\t";
            }
            std::cout << "\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0," << std::endl;

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
                std::cout << std::setw(4) << std::round(parameters[piece * 64 - 16 + square][phase] - psqtPieceValues[piece + phase * 6]) << ","; // We substract the 16 non-tuned pawn valeus
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

static void print_psqts_cpp(const parameters_t &parameters, std::array<tune_t, 12> &existingPieceValues)
{
    std::array<tune_t, 12> psqtPieceValues;

    std::cout << "constexpr std::array<int, 12> PieceValue = {";

    // Exctract and print pieces values
    for (int phase = 0; phase <= 1; ++phase)
    {
        std::cout << "\n\t";

        // Pawns
        {
            tune_t pawnSum = 0;

            for (int square = 0; square < 48; ++square)
            {
                pawnSum += parameters[square][phase];
            }

            auto average = (pawnSum / 48.0);

            auto pieceIndex = phase * 6;
            // pieceValues[pieceIndex] = PieceValue[phase * 5];
            psqtPieceValues[pieceIndex] = average;
            std::cout << "+" << std::round(psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex]) << ", ";
        }

        for (int piece = 1; piece < 5; ++piece)
        {
            tune_t sum = 0;

            for (int square = 0; square < 64; ++square)
            {
                sum += parameters[piece * 64 - 16 + square][phase]; // Substract 16 since we're only tuning 48 pawn values
            }

            auto average = (sum / 64.0);

            auto pieceIndex = piece + phase * 6;
            // pieceValues[pieceIndex] = PieceValue[piece + phase * 5];
            psqtPieceValues[pieceIndex] = average;
            std::cout << "+" << std::round(psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex]) << ", ";
        }

        // Kings
        auto kingIndex = 5 + phase * 6;
        psqtPieceValues[kingIndex] = 0;
        std::cout << "// " << std::round(psqtPieceValues[kingIndex] + existingPieceValues[kingIndex]);
    }

    std::cout << "\n};\n";

    // Print PSQTs

    std::string names[] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};

    // Pawns
    {
        const int piece = 0;
        for (int phase = 0; phase <= 1; ++phase)
        {
            std::cout << "\nconstexpr static std::array<int, 64> " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table = {\n\t";

            std::cout << "   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\n\t";

            for (int square = 0; square < 48; ++square)
            {
                std::cout << std::setw(4) << std::round(parameters[square][phase] - psqtPieceValues[phase * 6]) << ",";
                if (square % 8 == 7)
                    std::cout << "\n";
                if (square != 47)
                    std::cout << "\t";
            }

            std::cout << "\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0};" << std::endl;
        }
    }

    for (int piece = 1; piece < 6; ++piece)
    {
        for (int phase = 0; phase <= 1; ++phase)
        {
            std::cout << "\nconstexpr static std::array<int, 64> " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table = {\n\t";
            for (int square = 0; square < 64; ++square)
            {
                std::cout << std::setw(4) << std::round(parameters[piece * 64 - 16 + square][phase] - psqtPieceValues[piece + phase * 6]); // We substract the 16 non-tuned pawn valeus
                if (square != 63)
                {
                    std::cout << ",";

                    if (square % 8 == 7)
                        std::cout << "\n";

                    std::cout << "\t";
                }
            }
            std::cout << "};\n";
        }
    }
    std::cout << std::endl;
}
