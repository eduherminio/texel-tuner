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
    +100, +356, +373, +506, +1121, // 0
    +129, +429, +368, +746, +1425, // 0
};

constexpr static std::array<int, 64> MiddleGamePawnTable = {
    0, 0, 0, 0, 0, 0, 0, 0,
    -24, -23, -16, -12, -5, 18, 21, -16,
    -24, -25, -5, 12, 19, 26, 23, 9,
    -23, -15, 4, 19, 27, 31, 3, -2,
    -22, -11, 2, 21, 29, 28, 1, -3,
    -22, -18, -3, 6, 14, 23, 16, 4,
    -24, -21, -20, -15, -8, 12, 11, -23,
    0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> EndGamePawnTable = {
    0, 0, 0, 0, 0, 0, 0, 0,
    15, 9, 4, -7, 8, 3, -5, -3,
    12, 8, -2, -14, -9, -9, -9, -8,
    26, 16, -1, -20, -17, -15, 2, 0,
    24, 15, -2, -16, -15, -12, 0, -2,
    13, 6, -4, -12, -5, -7, -7, -8,
    17, 9, 8, -6, 16, 6, -2, -0,
    0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> MiddleGameKnightTable = {
    -141, -21, -52, -29, -11, -14, -8, -79,
    -42, -28, -6, 15, 17, 27, -8, -1,
    -32, -2, 13, 49, 54, 35, 30, -5,
    -13, 20, 36, 50, 50, 52, 40, 15,
    -9, 20, 38, 38, 48, 50, 40, 14,
    -30, -0, 13, 42, 51, 29, 24, -6,
    -44, -20, -4, 14, 16, 22, -7, -1,
    -156, -23, -49, -21, -9, -5, -16, -66};

constexpr static std::array<int, 64> EndGameKnightTable = {
    -55, -38, -5, -4, -4, -26, -31, -80,
    -9, 5, 7, 1, 1, -4, -8, -16,
    -8, 8, 25, 26, 22, 7, 1, -8,
    12, 13, 38, 40, 43, 37, 17, -2,
    9, 18, 37, 43, 44, 33, 21, 5,
    -10, 11, 16, 30, 22, 9, -2, -4,
    -14, 8, -1, 4, -2, -9, -8, -20,
    -61, -36, -1, -6, -5, -25, -31, -80};

constexpr static std::array<int, 64> MiddleGameBishopTable = {
    -13, 14, -10, -21, -15, -10, -19, 15,
    5, -0, 3, -18, -0, 2, 34, -5,
    -9, 3, -8, 3, -10, 9, 3, 24,
    -10, -9, -4, 24, 20, -14, -1, -4,
    -17, -5, -12, 19, 7, -11, -8, 2,
    2, 2, 3, -4, 2, 2, 5, 20,
    8, 11, 7, -6, -5, 5, 26, 7,
    11, 17, 3, -36, -18, -16, 2, -3};

constexpr static std::array<int, 64> EndGameBishopTable = {
    0, 15, -6, 3, 0, 1, -1, -20,
    1, -8, -7, -3, -4, -15, -8, -12,
    12, 10, 5, -1, 7, 2, 2, 9,
    12, 3, 6, 4, 3, 6, 1, 5,
    7, 6, 4, 8, 1, 5, 2, 6,
    10, 0, -0, -4, 3, -3, 0, 6,
    -8, -10, -18, -3, -6, -9, -8, -7,
    2, -10, -4, 7, 8, 3, -3, -8};

constexpr static std::array<int, 64> MiddleGameRookTable = {
    -5, -11, -7, -1, 8, 7, 10, 1,
    -33, -24, -14, -9, 3, 11, 25, -5,
    -32, -20, -21, -10, 6, 12, 51, 25,
    -28, -23, -19, -7, -4, 7, 36, 16,
    -22, -18, -14, -4, -7, 7, 27, 12,
    -25, -16, -16, -1, 4, 22, 50, 25,
    -31, -34, -8, -1, 5, 9, 31, 1,
    -3, -4, -2, 9, 16, 12, 18, 13};

constexpr static std::array<int, 64> EndGameRookTable = {
    9, 7, 11, 3, -5, 2, -0, -4,
    5, 7, 8, -1, -9, -11, -13, -5,
    16, 12, 14, 7, -5, -9, -20, -16,
    18, 14, 15, 8, 1, -0, -12, -13,
    17, 14, 15, 6, 3, -5, -9, -9,
    16, 15, 6, -1, -8, -12, -21, -10,
    7, 10, 3, -5, -10, -11, -15, -5,
    5, 2, 7, -2, -12, -6, -7, -12};

constexpr static std::array<int, 64> MiddleGameQueenTable = {
    -13, -13, -13, 4, -2, -29, 6, 1,
    -4, -11, 6, -1, 2, 9, 26, 49,
    -11, -6, -9, -9, -12, 8, 33, 54,
    -13, -20, -17, -7, -8, -3, 11, 24,
    -13, -16, -17, -16, -7, -3, 10, 21,
    -8, -4, -14, -11, -6, 5, 20, 35,
    -17, -21, 2, 11, 7, 6, 10, 37,
    -10, -12, -3, 6, 0, -35, -13, 24};

constexpr static std::array<int, 64> EndGameQueenTable = {
    -23, -17, -4, -12, -16, -15, -32, 8,
    -14, -9, -26, -4, -5, -20, -48, -5,
    -15, -5, 6, 1, 21, 18, -8, 3,
    -9, 10, 8, 12, 26, 35, 43, 31,
    -1, 5, 14, 24, 21, 30, 23, 40,
    -15, -11, 14, 10, 13, 17, 17, 16,
    -9, -2, -21, -21, -14, -16, -33, 3,
    -14, -14, -11, -6, -9, 11, 13, -4};

constexpr static std::array<int, 64> MiddleGameKingTable = {
    6, 39, 34, -65, 18, -55, 35, 24,
    -23, -15, -20, -54, -67, -45, -6, 2,
    -80, -58, -94, -94, -102, -110, -75, -98,
    -97, -78, -103, -150, -145, -122, -121, -154,
    -65, -53, -91, -128, -143, -108, -124, -149,
    -79, -37, -86, -90, -79, -89, -67, -89,
    57, -10, -22, -42, -46, -31, 10, 11,
    17, 59, 44, -47, 32, -43, 50, 38};

constexpr static std::array<int, 64> EndGameKingTable = {
    -76, -48, -28, -1, -41, -9, -44, -89,
    -21, 16, 24, 37, 42, 30, 9, -28,
    -2, 41, 70, 81, 84, 74, 42, 11,
    1, 53, 92, 124, 122, 95, 65, 24,
    -9, 45, 89, 120, 124, 93, 67, 23,
    -0, 39, 70, 82, 79, 70, 42, 7,
    -45, 12, 27, 35, 37, 26, 4, -31,
    -88, -57, -34, -8, -38, -14, -48, -94};

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

constexpr static std::array<u64, 64> SeventhRankMasks = {65280UL, 71776119061217280UL};

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
