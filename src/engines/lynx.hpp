#include "../base.h"
#include "./lynx_constants.hpp"
#include "../external/chess.hpp"
#include <cassert>
#include <array>
#include <bit>
#include <string>
#include <cmath>

using u64 = uint64_t;

const int base = 64 * 6 - 16; // Removing pawns from 1 and 8 rank

const int DoubledPawnPenalty_MG = -5;
const int DoubledPawnPenalty_EG = -14;
const int DoubledPawnPenaltyIndex = base + 0;

const int IsolatedPawnPenalty_MG = -17;
const int IsolatedPawnPenalty_EG = -14;
const int IsolatedPawnPenaltyIndex = base + 1;

const int OpenFileRookBonus_MG = 60;
const int OpenFileRookBonus_EG = 28;
const int OpenFileRookBonusIndex = base + 2;

const int SemiOpenFileRookBonus_MG = 25;
const int SemiOpenFileRookBonus_EG = 23;
const int SemiOpenFileRookBonusIndex = base + 3;

const int BishopMobilityBonus_MG = 10;
const int BishopMobilityBonus_EG = 10;
const int BishopMobilityBonusIndex = base + 4;

const int RookMobilityBonus_MG = 2;
const int RookMobilityBonus_EG = 2;
const int RookMobilityBonusIndex = base + 5;

const int QueenMobilityBonus_MG = 3;
const int QueenMobilityBonus_EG = 9;
const int QueenMobilityBonusIndex = base + 6;

const int SemiOpenFileKingPenalty_MG = -38;
const int SemiOpenFileKingPenalty_EG = 25;
const int SemiOpenFileKingPenaltyIndex = base + 7;

const int OpenFileKingPenalty_MG = -107;
const int OpenFileKingPenalty_EG = 4;
const int OpenFileKingPenaltyIndex = base + 8;

const int KingShieldBonus_MG = 19;
const int KingShieldBonus_EG = -6;
const int KingShieldBonusIndex = base + 9;

const int BishopPairBonus_MG = 30;
const int BishopPairBonus_EG = 88;
const int BishopPairMaxBonusIndex = base + 10;

constexpr static std::array<int, 8> PassedPawnBonus_MG = {
    0, -2, -16, -16, 20, 61, 92, 0};

constexpr static std::array<int, 8> PassedPawnBonus_EG = {
    0, 7, 13, 44, 83, 175, 257, 0};

const int PassedPawnBonusStartIndex = base + 11;

static constexpr int numParameters = base +
                                     1 + // DoubledPawnPenalty
                                     1 + // IsolatedPawnPenalty
                                     1 + // OpenFileRookBonus
                                     1 + // SemiOpenFileRookBonus
                                     1 + // BishopMobilityBonus
                                     1 + // RookMobilityBonus
                                     1 + // QueenMobilityBonus
                                     1 + // SemiOpenFileKingPenalty
                                     1 + // OpenFileKingPenalty
                                     1 + // BishopPairMaxBonus
                                     1 + // KingShieldBonus
                                     6   // PassedPawnBonus - removing 1 and 8 rank values
    ;
class Lynx
{

public:
    constexpr static bool includes_additional_score = false;
    // constexpr static bool includes_additional_score = true;
    constexpr static bool supports_external_chess_eval = true;

    static parameters_t get_initial_parameters()
    {
        parameters_t result{};

        assert(MiddleGamePositionalWhiteTables.size() == 6);
        assert(EndGamePositionalWhiteTables.size() == 6);

        for (int p = 0; p < 6; ++p)
        {
            for (int sq = 0; sq < 64; ++sq)
            {
                assert(MiddleGamePositionalTables(p, sq) == MiddleGamePositionalWhiteTables[p][sq]);
                assert(MiddleGamePositionalTables(p + 6, sq) == -MiddleGamePositionalWhiteTables[p][sq ^ 56]);
                assert(MiddleGamePositionalTables(p, sq) == -MiddleGamePositionalTables(p + 6, sq ^ 56));
            }
        }

        // Pawns
        {
            const int piece = 0;

            for (int square = 8; square < 56; ++square)
                result.push_back({(double)MiddleGamePositionalTables(piece, square) + PieceValue[piece], (double)EndGamePositionalTables(piece, square) + PieceValue[piece + 5]});
        }
        // N, B, R, Q
        for (int piece = 1; piece < 5; ++piece)
        {
            for (int square = 0; square < 64; ++square)
                result.push_back({(double)MiddleGamePositionalTables(piece, square) + PieceValue[piece], (double)EndGamePositionalTables(piece, square) + PieceValue[piece + 5]});
        }

        // K
        {
            const int piece = 5;

            for (int square = 0; square < 64; ++square)
                result.push_back({(double)MiddleGamePositionalTables(piece, square), (double)EndGamePositionalTables(piece, square)});
        }

        result.push_back({(double)DoubledPawnPenalty_MG, (double)DoubledPawnPenalty_EG});
        result.push_back({(double)IsolatedPawnPenalty_MG, (double)IsolatedPawnPenalty_EG});
        result.push_back({(double)OpenFileRookBonus_MG, (double)OpenFileRookBonus_EG});
        result.push_back({(double)SemiOpenFileRookBonus_MG, (double)SemiOpenFileRookBonus_EG});
        result.push_back({(double)BishopMobilityBonus_MG, (double)BishopMobilityBonus_EG});
        result.push_back({(double)RookMobilityBonus_MG, (double)RookMobilityBonus_EG});
        result.push_back({(double)QueenMobilityBonus_MG, (double)QueenMobilityBonus_EG});
        result.push_back({(double)SemiOpenFileKingPenalty_MG, (double)SemiOpenFileKingPenalty_EG});
        result.push_back({(double)OpenFileKingPenalty_MG, (double)OpenFileKingPenalty_EG});
        result.push_back({(double)KingShieldBonus_MG, (double)KingShieldBonus_EG});
        result.push_back({(double)BishopPairBonus_MG, (double)BishopPairBonus_EG});

        for (int rank = 1; rank < 7; ++rank)
        {
            result.push_back({(double)PassedPawnBonus_MG[rank], (double)PassedPawnBonus_EG[rank]});
        }

        std::cout << result.size() << " == " << numParameters << std::endl;
        assert(result.size() == numParameters);

        return result;
    }

    static EvalResult get_fen_eval_result(const std::string &fen)
    {
        chess::Board board;
        board.setFen(fen);
        return get_external_eval_result(board);
    }

    static EvalResult get_external_eval_result(const chess::Board &board);

    static int NormalizeScore(int score);

    static int round(double value)
    {
        return std::round(value);
    }

    static void print_parameters(const parameters_t &parameters)
    {
        int pieceValues[12];

        for (int phase = 0; phase <= 1; ++phase)
        {
            if (phase == 0)
                std::cout << "public static readonly int[] MiddleGamePieceValues =\n[\n\t";

            else
                std::cout << "\npublic static readonly int[] EndGamePieceValues =\n[\n\t";

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
                std::cout << "\npublic static readonly int[] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n[\n\t";

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
                std::cout << "\npublic static readonly int[] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n[\n\t";
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

        std::cout << "\"DoubledPawnPenalty\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[DoubledPawnPenaltyIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[DoubledPawnPenaltyIndex][1]) << "\n}," << std::endl;

        std::cout << "\"IsolatedPawnPenalty\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[IsolatedPawnPenaltyIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[IsolatedPawnPenaltyIndex][1]) << "\n}," << std::endl;

        std::cout << "\"OpenFileRookBonus\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[OpenFileRookBonusIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[OpenFileRookBonusIndex][1]) << "\n}," << std::endl;

        std::cout << "\"SemiOpenFileRookBonus\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[SemiOpenFileRookBonusIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[SemiOpenFileRookBonusIndex][1]) << "\n}," << std::endl;

        std::cout << "\"BishopMobilityBonus\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[BishopMobilityBonusIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[BishopMobilityBonusIndex][1]) << "\n}," << std::endl;

        std::cout << "\"RookMobilityBonus\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[RookMobilityBonusIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[RookMobilityBonusIndex][1]) << "\n}," << std::endl;

        std::cout << "\"QueenMobilityBonus\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[QueenMobilityBonusIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[QueenMobilityBonusIndex][1]) << "\n}," << std::endl;

        std::cout << "\"SemiOpenFileKingPenalty\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[SemiOpenFileKingPenaltyIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[SemiOpenFileKingPenaltyIndex][1]) << "\n}," << std::endl;

        std::cout << "\"OpenFileKingPenalty\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[OpenFileKingPenaltyIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[OpenFileKingPenaltyIndex][1]) << "\n}," << std::endl;

        std::cout << "\"KingShieldBonus\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[KingShieldBonusIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[KingShieldBonusIndex][1]) << "\n}," << std::endl;

        std::cout << "\"BishopPairBonus\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[BishopPairMaxBonusIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[BishopPairMaxBonusIndex][1]) << "\n}," << std::endl;

        std::cout << "\"PassedPawnBonus\": {" << std::endl;
        std::cout << "\t\"Rank" << 0 << "\": {" << std::endl;
        std::cout << "\t\t\"MG\": " << 0 << ",\n";
        std::cout << "\t\t\"EG\": " << 0 << "\n\t}," << std::endl;
        for (int rank = 0; rank < 6; ++rank)
        {
            std::cout << "\t\"Rank" << rank + 1 << "\": {" << std::endl;
            std::cout << "\t\t\"MG\": " << round(parameters[PassedPawnBonusStartIndex + rank][0]) << ",\n";
            std::cout << "\t\t\"EG\": " << round(parameters[PassedPawnBonusStartIndex + rank][1]) << "\n\t}," << std::endl;
        }
        std::cout << "\t\"Rank" << 7 << "\": {" << std::endl;
        std::cout << "\t\t\"MG\": " << 0 << ",\n";
        std::cout << "\t\t\"EG\": " << 0 << "\n\t}," << std::endl;
        std::cout << "}";

        std::cout << '\n'
                  << std::endl;
    }
};

static inline parameters_t initialParameters = Lynx::get_initial_parameters();

void IncrementCoefficients(coefficients_t &coefficients, int index, const chess::Color &color, int increment = 1)
{
    coefficients[index] += color == chess::Color::WHITE
                               ? increment
                               : -increment;
}

chess::U64 GetPieceSwappingEndianness(const chess::Board &board, const chess::PieceType &piece, const chess::Color &color)
{
    return __builtin_bswap64(board.pieces(piece, color));
}

std::pair<int, int> PawnAdditionalEvaluation(int squareIndex, int pieceIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    int middleGameBonus = 0, endGameBonus = 0;
    auto doublePawnsCount = chess::builtin::popcount(GetPieceSwappingEndianness(board, chess::PieceType::PAWN, color) & (FileMasks[squareIndex]));
    if (doublePawnsCount > 1)
    {
        middleGameBonus += doublePawnsCount * DoubledPawnPenalty_MG;
        endGameBonus += doublePawnsCount * DoubledPawnPenalty_EG;
        IncrementCoefficients(coefficients, DoubledPawnPenaltyIndex, color);
    }

    if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, color) & IsolatedPawnMasks[squareIndex]) == 0) // isIsolatedPawn
    {
        middleGameBonus += IsolatedPawnPenalty_MG;
        endGameBonus += IsolatedPawnPenalty_EG;
        IncrementCoefficients(coefficients, IsolatedPawnPenaltyIndex, color);
    }

    if (color == chess::Color::WHITE)
    {

        if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK) & WhitePassedPawnMasks[squareIndex]) == 0) // isPassedPawn
        {
            // std::cout << "Piece: " << GetPiece(board, chess::PieceType::PAWN, chess::Color::BLACK) << std::endl;
            // std::cout << "Mask: " << WhitePassedPawnMasks[squareIndex] << std::endl;
            auto rank = Rank[squareIndex];
            if (pieceIndex == 6)
            {
                rank = 7 - rank;
            }
            middleGameBonus += PassedPawnBonus_MG[rank];
            endGameBonus += PassedPawnBonus_EG[rank];
            IncrementCoefficients(coefficients, PassedPawnBonusStartIndex + rank - 1, color); // There's no coefficient for rank 0
            // std::cout << "White pawn on " << squareIndex << " is passed, bonus " << PassedPawnBonus[rank] << std::endl;
        }
    }
    else
    {

        if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) & BlackPassedPawnMasks[squareIndex]) == 0) // isPassedPawn
        {
            auto rank = Rank[squareIndex];
            rank = 7 - rank;

            middleGameBonus += PassedPawnBonus_MG[rank];
            endGameBonus += PassedPawnBonus_EG[rank];
            IncrementCoefficients(coefficients, PassedPawnBonusStartIndex + rank - 1, color); // There's no coefficient for rank 0
            // std::cout << "Black pawn on " << squareIndex << " is passed, bonus " << -PassedPawnBonus[rank] << std::endl;
        }
    }

    return std::make_pair(middleGameBonus, endGameBonus);
}

std::pair<int, int> RookAdditonalEvaluation(int squareIndex, int pieceIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = chess::builtin::popcount(chess::attacks::rook(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ())));
    IncrementCoefficients(coefficients, RookMobilityBonusIndex, color, mobilityCount);

    int middleGameBonus = RookMobilityBonus_MG * mobilityCount;
    int endGameBonus = RookMobilityBonus_EG * mobilityCount;

    if (((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) | GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
    {
        // std::cout << "OpenFileRookBonus" << std::endl;
        IncrementCoefficients(coefficients, OpenFileRookBonusIndex, color);
        middleGameBonus += OpenFileRookBonus_MG;
        endGameBonus += OpenFileRookBonus_EG;
    }
    else
    {
        if (color == chess::Color::WHITE)
        {
            if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
            {
                // std::cout << "Piece: " << GetPiece(board, chess::PieceType::ROOK, chess::Color::BLACK) << std::endl;
                // std::cout << "Mask: " << FileMasks[squareIndex] << std::endl;
                // std::cout << "SemiOpenFileRookBonus white" << std::endl;
                IncrementCoefficients(coefficients, SemiOpenFileRookBonusIndex, color);
                middleGameBonus += SemiOpenFileRookBonus_MG;
                endGameBonus += SemiOpenFileRookBonus_EG;
            }
        }
        else
        {
            if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
            {
                // std::cout << "SemiOpenFileRookBonus black" << std::endl;
                IncrementCoefficients(coefficients, SemiOpenFileRookBonusIndex, color);
                middleGameBonus += SemiOpenFileRookBonus_MG;
                endGameBonus += SemiOpenFileRookBonus_EG;
            }
        }
    }

    return std::make_pair(middleGameBonus, endGameBonus);
}

std::pair<int, int> BishopAdditionalEvaluation(int squareIndex, int pieceIndex, const int pieceCount[], const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    int middleGameBonus = 0, endGameBonus = 0;

    auto mobilityCount = chess::builtin::popcount(chess::attacks::bishop(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ())));
    IncrementCoefficients(coefficients, BishopMobilityBonusIndex, color, mobilityCount);

    middleGameBonus += BishopMobilityBonus_MG * mobilityCount;
    endGameBonus += BishopMobilityBonus_EG * mobilityCount;

    if (pieceCount[pieceIndex] == 2)
    {
        middleGameBonus += BishopPairBonus_MG;
        endGameBonus += BishopPairBonus_EG;
        IncrementCoefficients(coefficients, BishopPairMaxBonusIndex, color);
    }

    return std::make_pair(middleGameBonus, endGameBonus);
}

std::pair<int, int> QueenAdditionalEvaluation(int squareIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = chess::builtin::popcount(chess::attacks::queen(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ())));
    IncrementCoefficients(coefficients, QueenMobilityBonusIndex, color, mobilityCount);

    return std::make_pair(QueenMobilityBonus_MG * mobilityCount, QueenMobilityBonus_EG * mobilityCount);
}

std::pair<int, int> KingAdditionalEvaluation(int squareIndex, chess::Color kingSide, const chess::Board &board, const int pieceCount[], coefficients_t &coefficients)
{
    int middleGameBonus = 0, endGameBonus = 0;
    auto kingSideOffset = kingSide == chess::Color::WHITE ? 0 : 6;

    if (pieceCount[9 - kingSideOffset] + pieceCount[10 - kingSideOffset] != 0) // areThereOppositeSideRooksOrQueens
    {
        if (((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) | GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
        {
            // std::cout << "Open: " << (kingSide == chess::Color::WHITE ? "White" : "Black") << std::endl;
            middleGameBonus += OpenFileKingPenalty_MG;
            endGameBonus += OpenFileKingPenalty_EG;
            IncrementCoefficients(coefficients, OpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == chess::Color::WHITE && (GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: " << (kingSide == chess::Color::WHITE ? "White" : "Black") << std::endl;
            middleGameBonus += SemiOpenFileKingPenalty_MG;
            endGameBonus += SemiOpenFileKingPenalty_EG;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == chess::Color::BLACK && (GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: "  << (kingSide == chess::Color::WHITE ? "White" : "Black") << std::endl;
            middleGameBonus += SemiOpenFileKingPenalty_MG;
            endGameBonus += SemiOpenFileKingPenalty_EG;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
    }

    auto ownPiecesAroundCount = chess::builtin::popcount(chess::attacks::king(static_cast<chess::Square>(squareIndex)) & __builtin_bswap64(board.us(kingSide)));
    IncrementCoefficients(coefficients, KingShieldBonusIndex, kingSide, ownPiecesAroundCount);

    return std::make_pair(middleGameBonus + KingShieldBonus_MG * ownPiecesAroundCount,
                          endGameBonus + KingShieldBonus_EG * ownPiecesAroundCount);
}

std::pair<int, int> AdditionalPieceEvaluation(int pieceSquareIndex, int pieceIndex, const int pieceCount[], const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    switch (pieceIndex)
    {
    case 0:
    case 6:
        return PawnAdditionalEvaluation(pieceSquareIndex, pieceIndex, board, color, coefficients);
    case 3:
    case 9:
        return RookAdditonalEvaluation(pieceSquareIndex, pieceIndex, board, color, coefficients);
    case 2:
    case 8:
        return BishopAdditionalEvaluation(pieceSquareIndex, pieceIndex, pieceCount, board, color, coefficients);

    case 4:
    case 10:
        return QueenAdditionalEvaluation(pieceSquareIndex, board, color, coefficients);

    default:
        return std::make_pair(0, 0);
    }
}

int Lynx::NormalizeScore(int score)
{
    return (score == 0 || score > 27000 || score < -27000)

               ? score
               : score * 100 / EvalNormalizationCoefficient;
}

EvalResult Lynx::get_external_eval_result(const chess::Board &board)
{
    std::vector<std::int16_t> coefficients(numParameters, 0);

    int pieceCount[12] = {}; // Init to zeros

    int middleGameScore = 0;
    int endGameScore = 0;
    int gamePhase = 0;

    for (int pieceIndex = 0; pieceIndex < 5; ++pieceIndex)
    {
        // Bitboard copy that we 'empty'
        auto bitboard = GetPieceSwappingEndianness(board, (chess::PieceType)pieceIndex, chess::Color::WHITE);
        // std::cout << pieceIndex << "bb: " << bitboard << std::endl;
        while (bitboard != 0)
        {
            auto pieceSquareIndex = chess::builtin::lsb(bitboard);
            chess::builtin::poplsb(bitboard);

            middleGameScore += MiddleGamePositionalTables(pieceIndex, pieceSquareIndex) + PieceValue[pieceIndex];
            endGameScore += EndGamePositionalTables(pieceIndex, pieceSquareIndex) + PieceValue[pieceIndex + 5];
            gamePhase += phaseValues[pieceIndex];

            ++pieceCount[pieceIndex];

            auto pair = AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, pieceCount, board, chess::Color::WHITE, coefficients);
            middleGameScore += pair.first;
            endGameScore += pair.second;

            if (pieceIndex == 0)
                IncrementCoefficients(coefficients, pieceSquareIndex - 8, chess::Color::WHITE);
            else
                IncrementCoefficients(coefficients, 64 * pieceIndex + pieceSquareIndex - 16, chess::Color::WHITE);
        }
    }

    for (int pieceIndex = 6; pieceIndex < 11; ++pieceIndex)
    {
        auto tunerPieceIndex = (pieceIndex - 6); // [0, 5]
        // Bitboard copy that we 'empty'
        auto bitboard = GetPieceSwappingEndianness(board, (chess::PieceType)tunerPieceIndex, chess::Color::BLACK);

        while (bitboard != 0)
        {
            auto pieceSquareIndex = chess::builtin::lsb(bitboard);
            chess::builtin::poplsb(bitboard);

            middleGameScore += MiddleGamePositionalTables(pieceIndex, pieceSquareIndex) - PieceValue[tunerPieceIndex];
            endGameScore += EndGamePositionalTables(pieceIndex, pieceSquareIndex) - PieceValue[tunerPieceIndex + 5];
            gamePhase += phaseValues[tunerPieceIndex];

            ++pieceCount[pieceIndex];

            auto pair = AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, pieceCount, board, chess::Color::BLACK, coefficients);
            middleGameScore -= pair.first;
            endGameScore -= pair.second;

            if (pieceIndex == 6)
                IncrementCoefficients(coefficients, pieceSquareIndex - 8, chess::Color::BLACK);
            else
                IncrementCoefficients(coefficients, 64 * tunerPieceIndex + pieceSquareIndex - 16, chess::Color::BLACK);
        }
    }

    auto whiteKing = chess::builtin::lsb(GetPieceSwappingEndianness(board, chess::PieceType::KING, chess::Color::WHITE));
    auto kingPair = KingAdditionalEvaluation(whiteKing, chess::Color::WHITE, board, pieceCount, coefficients);
    middleGameScore += MiddleGamePositionalTables(5, whiteKing) + kingPair.first;
    endGameScore += EndGamePositionalTables(5, whiteKing) + kingPair.second;
    IncrementCoefficients(coefficients, 64 * 5 + whiteKing - 16, chess::Color::WHITE);

    auto blackKing = chess::builtin::lsb(GetPieceSwappingEndianness(board, chess::PieceType::KING, chess::Color::BLACK));
    kingPair = KingAdditionalEvaluation(blackKing, chess::Color::BLACK, board, pieceCount, coefficients);
    middleGameScore += MiddleGamePositionalTables(11, blackKing) - kingPair.first;
    endGameScore += EndGamePositionalTables(11, blackKing) - kingPair.second;
    IncrementCoefficients(coefficients, 64 * 5 + blackKing - 16, chess::Color::BLACK);

    // Debugging eval
    // return EvalResult{
    //     std::move(coefficients),
    //     (double)(board.sideToMove() == chess::Color::WHITE
    //                  ? eval
    //                  : -eval)};

    // Check if drawn position due to lack of material
    if (endGameScore >= 0)
    {
        bool whiteCannotWin = pieceCount[0] == 0 && pieceCount[4] == 0 && pieceCount[3] == 0 &&
                              (pieceCount[2] + pieceCount[1] == 1              // B or N
                               || (pieceCount[2] == 0 && pieceCount[1] == 2)); // N+N

        if (whiteCannotWin)
        {

            return EvalResult{
                std::move(coefficients),
                (double)0};
        }
    }
    else
    {
        bool blackCannotWin = pieceCount[6] == 0 && pieceCount[10] == 0 && pieceCount[9] == 0 &&
                              (pieceCount[8] + pieceCount[7] == 1              // B or N
                               || (pieceCount[8] == 0 && pieceCount[7] == 2)); // N+N

        if (blackCannotWin)
        {

            return EvalResult{
                std::move(coefficients),
                (double)0};
        }
    }

    const int maxPhase = 24;

    if (gamePhase > maxPhase) // Early promotions
    {
        gamePhase = maxPhase;
    }

    int endGamePhase = maxPhase - gamePhase;

    int eval = ((middleGameScore * gamePhase) + (endGameScore * endGamePhase)) / maxPhase;
    // eval = ((middleGameScore * gamePhase) + (endGameScore * endGamePhase));

    // Always white's perspective
    return EvalResult{
        std::move(coefficients),
        (double)eval};
}
