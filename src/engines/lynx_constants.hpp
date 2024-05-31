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

constexpr static int EvalNormalizationCoefficient = 142;

constexpr std::array<int, 12> PieceValue = {
    +101, +137, +174, +210, +1114, // 0,
    +128, +179, +209, +383, +1429 // 0
};

constexpr static std::array<int, 64> MiddleGamePawnTable = {
    0, 0, 0, 0, 0, 0, 0, 0,
    -24, -23, -15, -6, -1, 26, 27, -10,
    -25, -25, -5, 11, 18, 26, 22, 9,
    -23, -16, 4, 18, 26, 30, 1, -2,
    -23, -12, 2, 20, 28, 27, 0, -3,
    -22, -19, -3, 4, 13, 24, 15, 4,
    -24, -21, -19, -10, -4, 19, 16, -17,
    0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> EndGamePawnTable = {
    0, 0, 0, 0, 0, 0, 0, 0,
    16, 10, 5, -9, 7, 2, -6, -4,
    13, 10, -0, -12, -7, -7, -7, -7,
    27, 17, 0, -18, -15, -13, 4, 1,
    25, 16, -1, -15, -13, -11, 2, -1,
    14, 7, -3, -11, -4, -6, -6, -7,
    18, 10, 9, -8, 15, 5, -3, -1,
    0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> MiddleGameKnightTable = {
    -139, -17, -51, -29, -9, -21, -5, -85,
    -41, -26, -5, 15, 17, 20, -16, -10,
    -30, -0, 16, 51, 56, 37, 31, -3,
    -11, 23, 39, 53, 52, 54, 42, 17,
    -8, 22, 40, 40, 50, 52, 42, 16,
    -28, 2, 16, 44, 53, 31, 25, -4,
    -44, -19, -2, 14, 16, 14, -14, -11,
    -155, -20, -49, -21, -8, -14, -13, -74};

constexpr static std::array<int, 64> EndGameKnightTable = {
    -56, -40, -6, -3, -4, -20, -32, -77,
    -9, 5, 8, 3, 4, 1, -3, -10,
    -9, 8, 25, 26, 23, 7, 2, -8,
    11, 12, 37, 40, 43, 37, 17, -2,
    8, 17, 37, 42, 44, 33, 22, 5,
    -11, 11, 15, 30, 23, 8, -1, -4,
    -15, 8, -0, 6, 0, -4, -4, -14,
    -60, -37, -0, -5, -5, -18, -31, -76};

constexpr static std::array<int, 64> MiddleGameBishopTable = {
    -10, 16, -6, -19, -13, -18, -20, 6,
    7, 2, 5, -19, 0, -3, 25, -10,
    -6, 4, -7, 5, -8, 10, 3, 25,
    -8, -7, -2, 25, 21, -13, 1, -2,
    -15, -3, -11, 20, 8, -9, -7, 5,
    4, 3, 5, -3, 4, 3, 6, 21,
    10, 12, 8, -7, -5, -1, 17, -1,
    13, 19, 6, -34, -16, -24, 2, -9};

constexpr static std::array<int, 64> EndGameBishopTable = {
    -1, 14, -7, 3, -0, 8, -1, -17,
    0, -8, -7, -0, -3, -12, -2, -10,
    11, 10, 5, -1, 8, 2, 3, 9,
    11, 3, 6, 5, 4, 7, 1, 4,
    6, 6, 4, 8, 2, 6, 2, 5,
    9, -0, -0, -3, 3, -2, 0, 5,
    -9, -11, -18, -2, -5, -6, -2, -3,
    1, -11, -5, 7, 7, 8, -3, -5};

constexpr static std::array<int, 64> MiddleGameRookTable = {
    -3, -9, -5, 1, 11, -0, 6, -3,
    -28, -17, -11, -8, 4, 4, 20, -4,
    -31, -19, -20, -9, 6, 12, 50, 26,
    -26, -22, -18, -7, -4, 8, 35, 17,
    -21, -17, -13, -3, -7, 7, 27, 12,
    -24, -15, -16, 0, 5, 22, 49, 26,
    -26, -26, -7, -1, 5, 2, 26, 1,
    -2, -3, -0, 10, 19, 4, 13, 9};

constexpr static std::array<int, 64> EndGameRookTable = {
    6, 3, 7, -0, -10, 4, 0, -3,
    17, 18, 18, 8, -1, -1, -5, 2,
    13, 10, 11, 4, -7, -10, -21, -18,
    15, 11, 12, 5, -2, -2, -14, -15,
    14, 10, 12, 3, 0, -7, -11, -10,
    13, 12, 3, -4, -10, -14, -22, -12,
    19, 21, 14, 4, -3, -1, -6, 1,
    1, -2, 3, -6, -17, -3, -7, -12};

constexpr static std::array<int, 64> MiddleGameQueenTable = {
    -10, -11, -12, 3, -1, -37, 4, 1,
    -3, -10, 6, -2, 2, 3, 21, 49,
    -9, -5, -8, -9, -11, 9, 33, 56,
    -12, -18, -17, -7, -8, -3, 12, 25,
    -11, -15, -17, -16, -7, -3, 11, 23,
    -6, -2, -13, -10, -5, 6, 20, 36,
    -15, -20, 3, 10, 7, 0, 6, 36,
    -8, -10, -1, 6, 2, -41, -18, 25};

constexpr static std::array<int, 64> EndGameQueenTable = {
    -24, -17, -4, -8, -16, -9, -30, 9,
    -14, -8, -24, -1, -3, -15, -45, -6,
    -15, -5, 6, 1, 20, 18, -8, 2,
    -9, 9, 8, 12, 25, 35, 43, 30,
    -2, 5, 14, 24, 21, 31, 24, 40,
    -15, -12, 13, 10, 12, 17, 17, 16,
    -9, -3, -21, -18, -13, -11, -31, 3,
    -15, -14, -11, -3, -9, 16, 17, -3};

constexpr static std::array<int, 64> MiddleGameKingTable = {
    14, 48, 27, -76, 6, -60, 37, 29,
    -18, -11, -26, -63, -73, -49, -3, 13,
    -78, -56, -95, -97, -105, -109, -70, -92,
    -98, -80, -106, -153, -146, -123, -120, -154,
    -67, -54, -94, -130, -144, -108, -123, -149,
    -76, -34, -88, -93, -83, -88, -62, -83,
    67, -4, -28, -53, -56, -36, 13, 22,
    27, 74, 40, -58, 20, -49, 53, 44};

constexpr static std::array<int, 64> EndGameKingTable = {
    -83, -54, -26, 1, -38, -9, -47, -97,
    -25, 15, 27, 40, 46, 33, 9, -35,
    -4, 42, 73, 85, 89, 77, 43, 8,
    0, 56, 96, 130, 128, 99, 68, 23,
    -9, 47, 94, 126, 130, 97, 70, 22,
    -2, 40, 74, 86, 84, 73, 42, 3,
    -51, 11, 30, 39, 41, 30, 4, -38,
    -95, -65, -33, -7, -34, -13, -51, -103};

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

static void print_psqts_csharp(const parameters_t &parameters, std::array<int, 12> &existingPieceValues)
{
    std::array<int, 12> psqtPieceValues;

    // Exctract and print pieces values
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
            psqtPieceValues[pieceIndex] = static_cast<int>(std::round(average));
            std::cout << "+" << psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex] << ", ";
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
            psqtPieceValues[pieceIndex] = static_cast<int>(std::round(average));
            std::cout << "+" << psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex] << ", ";
        }

        // Kings
        auto kingIndex = 5 + phase * 6;
        psqtPieceValues[kingIndex] = 0;
        std::cout << psqtPieceValues[kingIndex] + existingPieceValues[kingIndex] << ",\n\t";

        for (int piece = 0; piece < 5; ++piece)
        {
            auto pieceIndex = piece + phase * 6;
            std::cout << "-" << psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex] << ", ";
        }

        std::cout << psqtPieceValues[kingIndex] + existingPieceValues[kingIndex] << "\n];\n";
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
                std::cout << std::setw(4) << round(parameters[square][phase] - psqtPieceValues[phase * 6]) << ",";
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
                std::cout << std::setw(4) << round(parameters[piece * 64 - 16 + square][phase] - psqtPieceValues[piece + phase * 6]) << ","; // We substract the 16 non-tuned pawn valeus
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

static void print_psqts_cpp(const parameters_t &parameters, std::array<int, 12> &existingPieceValues)
{
    std::array<int, 12> psqtPieceValues;

    std::cout << "constexpr std::array<int, 12> PieceValue = {";

    // Exctract and print pieces values
    for (int phase = 0; phase <= 1; ++phase)
    {
        std::cout << "\n\t";

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
            psqtPieceValues[pieceIndex] = static_cast<int>(std::round(average));
            std::cout << "+" << psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex] << ", ";
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
            psqtPieceValues[pieceIndex] = static_cast<int>(std::round(average));
            std::cout << "+" << psqtPieceValues[pieceIndex] + existingPieceValues[pieceIndex] << ", ";
        }

        // Kings
        auto kingIndex = 5 + phase * 6;
        psqtPieceValues[kingIndex] = 0;
        std::cout << "// " << psqtPieceValues[kingIndex] + existingPieceValues[kingIndex];
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
                std::cout << std::setw(4) << round(parameters[square][phase] - psqtPieceValues[phase * 6]) << ",";
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
                std::cout << std::setw(4) << round(parameters[piece * 64 - 16 + square][phase] - psqtPieceValues[piece + phase * 6]); // We substract the 16 non-tuned pawn valeus
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
