#pragma once
#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <ctime>
#include "lynx_tunable_eval_terms.hpp"

using u64 = uint64_t;

static int print_counter;

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

constexpr static int PSQTBucketCount = 16;

constexpr static std::array<int, 64> PSQTBucketLayout = {
    8, 9, 10, 11, 12, 13, 14, 15,
    8, 9, 10, 11, 12, 13, 14, 15,
    8, 9, 10, 11, 12, 13, 14, 15,
    8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7, //
};

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
    std::stringstream ss;

    std::array<std::array<tune_t, 12>, PSQTBucketCount> psqtPieceValues;

    // Extract and print pieces values
    for (int phase = 0; phase <= 1; ++phase)
    {
        if (phase == 0)
            ss << "\tinternal static readonly short[][] MiddleGamePieceValues =\n\t[";

        else
            ss << "\n\tinternal static readonly short[][] EndGamePieceValues =\n\t[";

        for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
        {
            ss << "\n\t\t[\n\t\t\t";

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
                ss << "+" << std::round(average + existingPieceValues[bucket][pieceIndex]) << ", ";
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

                // ss << std::endl
                //           << existingPieceValues[0][pieceIndex] << "==" << existingPieceValues[1][pieceIndex] << std::endl;

                psqtPieceValues[bucket][pieceIndex] = average;
                ss << "+" << std::round(average + existingPieceValues[bucket][pieceIndex]) << ", ";
            }

            // Kings
            auto kingIndex = 5 + phase * 6;
            psqtPieceValues[bucket][kingIndex] = 0;
            ss << psqtPieceValues[bucket][kingIndex] + existingPieceValues[bucket][kingIndex] << ",\n\t\t\t";

            for (int piece = 0; piece < 5; ++piece)
            {
                auto pieceIndex = piece + phase * 6;
                ss << "-" << std::round(psqtPieceValues[bucket][pieceIndex] + existingPieceValues[bucket][pieceIndex]) << ", ";
            }
            ss << std::round(psqtPieceValues[bucket][kingIndex] + existingPieceValues[bucket][kingIndex]) << "\n\t\t],";
        }

        ss << "\n\t];\n";
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
                    ss << "\n\tinternal static readonly short[][] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n\t[\n";
                }

                ss << "\t\t[\n\t\t\t";

                ss << "   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\n\t\t\t";

                for (int square = 0; square < 48; ++square)
                {
                    ss << std::setw(4) << std::round(parameters[48 * bucket + square][phase] - psqtPieceValues[bucket][phase * 6]) << ",";
                    if (square % 8 == 7)
                        ss << "\n\t\t";
                    if (square != 47)
                        ss << "\t";
                }
                ss << "\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0," << std::endl;
                ss << "\t\t],\n";

                if (bucket == PSQTBucketCount - 1)
                {
                    ss << "\t];\n";
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
                    ss << "\n\tinternal static readonly short[][] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n\t[\n";
                }

                ss << "\t\t[\n\t\t\t";

                for (int square = 0; square < 64; ++square)
                {
                    ss << std::setw(4) << std::round(parameters[(48 * PSQTBucketCount) +               // 64 - 16, since we're only tuning 48 pawn values
                                                                (64 * PSQTBucketCount) * (piece - 1) + // piece - 1 since we already took pawns into account
                                                                (64 * bucket) + square][phase] -
                                                     psqtPieceValues[bucket][piece + phase * 6])
                       << ",";
                    if (square % 8 == 7)
                        ss << "\n\t\t";
                    if (square != 63)
                        ss << "\t";
                }
                ss << "],\n";

                if (bucket == PSQTBucketCount - 1)
                {
                    ss << "\t];\n";
                }
            }
        }
    }
    ss << std::endl;

    // No console output
    // std::cout << ss.str();

    time_t rawtime;
    struct tm *timeInfo;

    time(&rawtime);
    timeInfo = localtime(&rawtime);

    std::ofstream file("TunableEvalParameters.cs", std::ofstream::out | std::ofstream::trunc);

    if (file.is_open())
    {
        file << "// " << timeInfo->tm_year + 1900 << "-" << timeInfo->tm_mon + 1 << "-" << timeInfo->tm_mday
             << " " << timeInfo->tm_hour << ":" << timeInfo->tm_min << ":" << timeInfo->tm_sec
             << "  " << print_counter << std::endl
             << std::endl
             << "namespace Lynx;\n"
             << std::endl
             << "#pragma warning disable IDE0055, IDE1006 // Discard formatting and naming styles\n"
             << std::endl
             << "public static partial class EvaluationConstants\n"
             << "{\n";

        file << ss.rdbuf();

        file << "}\n"
             << std::endl
             << "#pragma warning restore IDE0055, IDE1006 // Discard formatting and naming styles\n";
    }
    file.close();
}

static void print_psqts_cpp(const parameters_t &parameters, std::array<std::array<tune_t, 12>, PSQTBucketCount> &existingPieceValues)
{
    ++print_counter;

    std::stringstream ss;

    std::array<std::array<tune_t, 12>, PSQTBucketCount> psqtPieceValues;

    ss << "constexpr static std::array<std::array<int, 12>, PSQTBucketCount> PieceValue = {\n\t{\n\t";

    // Exctract and print pieces values
    for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
    {
        ss << "\t{\n";

        for (int phase = 0; phase <= 1; ++phase)
        {
            ss << "\t\t\t";

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
                ss << "+" << std::round(average + existingPieceValues[bucket][pieceIndex]) << ", ";
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
                ss << "+" << std::round(average + existingPieceValues[bucket][pieceIndex]) << ", ";
            }

            // Kings
            auto kingIndex = 5 + phase * 6;
            psqtPieceValues[bucket][kingIndex] = 0;
            ss << "// " << std::round(existingPieceValues[bucket][kingIndex]) << "\n";
        }
        ss << "\t\t},\n\t";
    }

    ss << "}};\n";

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
                    ss << "\nconstexpr static std::array<std::array<int, 64>, PSQTBucketCount> " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table = {\n\t{\n\t";
                }

                ss << "\t{\n\t\t\t";

                ss << "   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\n\t\t\t";

                for (int square = 0; square < 48; ++square)
                {
                    ss << std::setw(4) << std::round(parameters[48 * bucket + square][phase] - psqtPieceValues[bucket][phase * 6]) << ",";
                    if (square % 8 == 7)
                        ss << "\n\t\t";
                    if (square != 47)
                        ss << "\t";
                }

                ss << "\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0,\t   0\t//\n"
                   << std::endl;
                ss << "\t\t},\n\t";

                if (bucket == PSQTBucketCount - 1)
                {
                    ss << "}};\n";
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
                    ss << "\nconstexpr static std::array<std::array<int, 64>, PSQTBucketCount> " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table = {\n\t{\n\t";
                }

                ss << "\t{\n\t\t\t";

                for (int square = 0; square < 64; ++square)
                {
                    ss << std::setw(4) << std::round(parameters[(48 * PSQTBucketCount) +               // 64 - 16, since we're only tuning 48 pawn values
                                                                (64 * PSQTBucketCount) * (piece - 1) + // piece - 1 since we already took pawns into account
                                                                (64 * bucket) + square][phase] -
                                                     psqtPieceValues[bucket][piece + phase * 6]); // We substract the 16 non-tuned pawn valeus
                    if (square != 63)
                    {
                        ss << ",";

                        if (square % 8 == 7)
                            ss << "\n\t\t";

                        ss << "\t";
                    }
                }

                ss << "\t//\n\t\t},\n\t";

                if (bucket == PSQTBucketCount - 1)
                {
                    ss << "}};\n";
                }
            }
        }
    }
    ss << std::endl;

    // No console output
    // std::cout << ss.str();

    time_t rawtime;
    struct tm *timeInfo;

    time(&rawtime);
    timeInfo = localtime(&rawtime);

    std::ofstream file("tunable_eval_terms.cpp", std::ofstream::out | std::ofstream::trunc);

    if (file.is_open())
    {
        file << "// " << timeInfo->tm_year + 1900 << "-" << timeInfo->tm_mon + 1 << "-" << timeInfo->tm_mday
             << " " << timeInfo->tm_hour << ":" << timeInfo->tm_min << ":" << timeInfo->tm_sec
             << "  " << print_counter << std::endl
             << std::endl
             << "#include <array>\n"
             << std::endl
             << "constexpr static int PSQTBucketCount = " << PSQTBucketCount << ";\n"
             << std::endl;

        file << ss.rdbuf();
    }
    file.close();
}
