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
    +97, +356, +373, +507, +1123,  // 0
    +123, +427, +363, +747, +1424, // 0
};

constexpr static std::array<int, 64> MiddleGamePawnTable = {
    0, 0, 0, 0, 0, 0, 0, 0,
    -21, -22, -14, -12, -6, 20, 23, -12,
    -24, -26, -6, 9, 15, 23, 20, 8,
    -20, -14, 5, 18, 27, 30, 4, -1,
    -20, -10, 3, 20, 29, 29, 3, -2,
    -22, -21, -5, 2, 10, 20, 13, 2,
    -21, -19, -19, -15, -9, 14, 12, -19,
    0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> EndGamePawnTable = {
    0, 0, 0, 0, 0, 0, 0, 0,
    16, 11, 5, -7, 8, 3, -5, -3,
    10, 6, -3, -15, -10, -10, -10, -9,
    27, 17, -1, -20, -16, -15, 4, 1,
    24, 16, -3, -16, -15, -12, 2, -1,
    12, 3, -6, -13, -7, -8, -9, -10,
    18, 11, 9, -5, 17, 7, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> MiddleGameKnightTable = {
    -142, -19, -51, -28, -10, -13, -7, -79,
    -42, -27, -5, 16, 18, 28, -8, 0,
    -35, -4, 12, 50, 52, 33, 26, -7,
    -13, 21, 37, 51, 51, 52, 43, 14,
    -10, 21, 39, 39, 50, 51, 43, 13,
    -33, -3, 12, 43, 49, 26, 20, -8,
    -45, -20, -2, 15, 17, 23, -6, 0,
    -157, -22, -49, -20, -8, -4, -15, -67};

constexpr static std::array<int, 64> EndGameKnightTable = {
    -56, -38, -5, -3, -3, -25, -31, -80,
    -9, 6, 10, 4, 4, -3, -7, -16,
    -8, 7, 26, 27, 22, 7, -1, -9,
    10, 11, 37, 38, 43, 35, 15, -3,
    7, 16, 36, 42, 43, 32, 19, 4,
    -10, 11, 16, 31, 22, 8, -3, -5,
    -15, 8, 1, 7, -0, -8, -8, -20,
    -60, -35, 0, -6, -5, -25, -30, -79};

constexpr static std::array<int, 64> MiddleGameBishopTable = {
    -13, 13, -9, -20, -15, -10, -21, 14,
    4, 1, 4, -17, 1, 3, 36, -6,
    -9, -0, -9, 4, -12, 6, -2, 26,
    -9, -8, -2, 23, 21, -13, 2, -3,
    -17, -3, -11, 20, 9, -10, -5, 3,
    2, -2, 2, -4, -0, -1, 0, 21,
    7, 12, 7, -6, -4, 5, 28, 6,
    11, 16, 3, -35, -17, -16, 2, -3};

constexpr static std::array<int, 64> EndGameBishopTable = {
    1, 17, -3, 5, 2, 3, 1, -19,
    3, -7, -6, -0, -2, -14, -7, -10,
    13, 8, 4, -2, 6, 1, 0, 10,
    13, 0, 3, 0, -1, 2, -1, 6,
    7, 3, 1, 4, -3, 3, -1, 7,
    10, -2, -2, -4, 1, -4, -2, 7,
    -6, -9, -16, -2, -4, -8, -7, -5,
    3, -8, -2, 9, 9, 4, -1, -6};

constexpr static std::array<int, 64> MiddleGameRookTable = {
    -4, -10, -6, 0, 9, 8, 11, 2,
    -29, -18, -11, -8, 4, 12, 29, -2,
    -33, -22, -23, -12, 3, 9, 48, 24,
    -29, -26, -23, -11, -6, 6, 37, 16,
    -24, -21, -17, -7, -8, 7, 28, 12,
    -26, -18, -19, -2, 0, 19, 48, 22,
    -26, -27, -7, -2, 5, 10, 35, 5,
    -2, -4, -1, 10, 17, 12, 19, 14};

constexpr static std::array<int, 64> EndGameRookTable = {
    7, 5, 9, 1, -7, -1, -3, -6,
    17, 19, 19, 9, -1, -5, -9, 0,
    14, 10, 12, 4, -8, -12, -23, -18,
    15, 11, 12, 4, -3, -5, -17, -16,
    14, 10, 11, 2, -2, -10, -15, -12,
    14, 13, 4, -3, -10, -15, -24, -11,
    20, 22, 15, 4, -3, -5, -10, -0,
    2, -1, 4, -5, -14, -9, -9, -14};

constexpr static std::array<int, 64> MiddleGameQueenTable = {
    -13, -12, -13, 5, -2, -30, 6, 2,
    -4, -10, 7, -0, 3, 11, 28, 50,
    -12, -8, -10, -10, -14, 5, 30, 53,
    -12, -20, -17, -8, -8, -3, 13, 26,
    -12, -16, -16, -16, -6, -3, 12, 23,
    -9, -6, -16, -11, -9, 1, 17, 33,
    -16, -19, 3, 12, 8, 7, 12, 38,
    -11, -12, -2, 7, 1, -35, -15, 25};

constexpr static std::array<int, 64> EndGameQueenTable = {
    -22, -16, -3, -11, -15, -15, -32, 8,
    -14, -9, -25, -2, -4, -21, -49, -5,
    -13, -4, 7, 2, 22, 19, -8, 5,
    -10, 7, 6, 11, 23, 33, 40, 28,
    -2, 3, 12, 22, 19, 29, 20, 38,
    -13, -10, 14, 11, 13, 18, 16, 18,
    -9, -3, -21, -21, -13, -17, -34, 3,
    -13, -13, -10, -6, -9, 11, 15, -2};

constexpr static std::array<int, 64> MiddleGameKingTable = {
    6, 39, 36, -64, 19, -55, 35, 24,
    -21, -15, -20, -54, -67, -44, -5, 5,
    -83, -65, -99, -97, -106, -115, -79, -99,
    -99, -80, -104, -151, -145, -122, -121, -155,
    -66, -54, -92, -129, -143, -108, -125, -151,
    -81, -44, -91, -91, -82, -92, -70, -90,
    59, -9, -21, -42, -46, -29, 11, 14,
    16, 60, 46, -46, 34, -42, 51, 38};

constexpr static std::array<int, 64> EndGameKingTable = {
    -77, -49, -28, -1, -41, -9, -43, -90,
    -23, 15, 24, 37, 43, 30, 8, -29,
    -2, 40, 69, 81, 84, 73, 40, 10,
    0, 52, 90, 122, 120, 92, 63, 23,
    -10, 43, 88, 119, 123, 91, 65, 22,
    -1, 37, 70, 81, 78, 68, 39, 5,
    -47, 11, 26, 35, 37, 26, 3, -33,
    -88, -57, -34, -9, -38, -14, -48, -96};

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
