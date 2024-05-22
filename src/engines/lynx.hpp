#include "../base.h"
#include "./lynx_constants.hpp"
#include "../external/chess.hpp"
#include "../external/builtin.hpp"
#include <cassert>
#include <array>
#include <bit>
#include <string>
#include <cmath>

using u64 = uint64_t;

const int base = 64 * 6 - 16; // Removing pawns from 1 and 8 rank

// const int DoubledPawnPenalty_MG = -6;
// const int DoubledPawnPenalty_EG = -12;
// const int DoubledPawnPenalty_Packed = Pack(DoubledPawnPenalty_MG, DoubledPawnPenalty_EG);
// const int DoubledPawnPenaltyIndex = base + 0;

const int IsolatedPawnPenalty_MG = -21;
const int IsolatedPawnPenalty_EG = -18;
const int IsolatedPawnPenalty_Packed = Pack(IsolatedPawnPenalty_MG, IsolatedPawnPenalty_EG);
const int IsolatedPawnPenaltyIndex = base + 0;

const int OpenFileRookBonus_MG = 46;
const int OpenFileRookBonus_EG = 9;
const int OpenFileRookBonus_Packed = Pack(OpenFileRookBonus_MG, OpenFileRookBonus_EG);
const int OpenFileRookBonusIndex = base + 1;

const int SemiOpenFileRookBonus_MG = 15;
const int SemiOpenFileRookBonus_EG = 14;
const int SemiOpenFileRookBonus_Packed = Pack(SemiOpenFileRookBonus_MG, SemiOpenFileRookBonus_EG);
const int SemiOpenFileRookBonusIndex = base + 2;

const int RookMobilityBonus_MG = 5;
const int RookMobilityBonus_EG = 5;
const int RookMobilityBonus_Packed = Pack(RookMobilityBonus_MG, RookMobilityBonus_EG);
const int RookMobilityBonusIndex = base + 3;

const int QueenMobilityBonus_MG = 4;
const int QueenMobilityBonus_EG = 7;
const int QueenMobilityBonus_Packed = Pack(QueenMobilityBonus_MG, QueenMobilityBonus_EG);
const int QueenMobilityBonusIndex = base + 4;

const int SemiOpenFileKingPenalty_MG = -39;
const int SemiOpenFileKingPenalty_EG = 21;
const int SemiOpenFileKingPenalty_Packed = Pack(SemiOpenFileKingPenalty_MG, SemiOpenFileKingPenalty_EG);
const int SemiOpenFileKingPenaltyIndex = base + 5;

const int OpenFileKingPenalty_MG = -105;
const int OpenFileKingPenalty_EG = 7;
const int OpenFileKingPenalty_Packed = Pack(OpenFileKingPenalty_MG, OpenFileKingPenalty_EG);
const int OpenFileKingPenaltyIndex = base + 6;

const int KingShieldBonus_MG = 16;
const int KingShieldBonus_EG = -6;
const int KingShieldBonus_Packed = Pack(KingShieldBonus_MG, KingShieldBonus_EG);
const int KingShieldBonusIndex = base + 7;

const int BishopPairBonus_MG = 31;
const int BishopPairBonus_EG = 80;
const int BishopPairBonus_Packed = Pack(BishopPairBonus_MG, BishopPairBonus_EG);
const int BishopPairMaxBonusIndex = base + 8;

constexpr static std::array<int, 8> PassedPawnBonus_MG = {
    0, 2, -11, -11, 19, 58, 95, 0};

constexpr static std::array<int, 8> PassedPawnBonus_EG = {
    0, 12, 19, 47, 80, 156, 223, 0};

constexpr static std::array<int, 8> PassedPawnBonus_Packed = {
    Pack(PassedPawnBonus_MG[0], PassedPawnBonus_EG[0]),
    Pack(PassedPawnBonus_MG[1], PassedPawnBonus_EG[1]),
    Pack(PassedPawnBonus_MG[2], PassedPawnBonus_EG[2]),
    Pack(PassedPawnBonus_MG[3], PassedPawnBonus_EG[3]),
    Pack(PassedPawnBonus_MG[4], PassedPawnBonus_EG[4]),
    Pack(PassedPawnBonus_MG[5], PassedPawnBonus_EG[5]),
    Pack(PassedPawnBonus_MG[6], PassedPawnBonus_EG[6]),
    Pack(PassedPawnBonus_MG[7], PassedPawnBonus_EG[7])};

const int PassedPawnBonusStartIndex = base + 9;

constexpr static std::array<int, 15> BishopMobilityBonus_MG = {
    0, 196, 208, 219, 233, 241, 256,
    266, 275, 276, 282, 284, 286, 315,
    0};

constexpr static std::array<int, 15> BishopMobilityBonus_EG = {
    0, 160, 159, 198, 214, 229, 249,
    259, 271, 277, 282, 279, 278, 272,
    0};

constexpr static std::array<int, 15> BishopMobilityBonus_Packed = {
    Pack(BishopMobilityBonus_MG[0], BishopMobilityBonus_EG[0]),
    Pack(BishopMobilityBonus_MG[1], BishopMobilityBonus_EG[1]),
    Pack(BishopMobilityBonus_MG[2], BishopMobilityBonus_EG[2]),
    Pack(BishopMobilityBonus_MG[3], BishopMobilityBonus_EG[3]),
    Pack(BishopMobilityBonus_MG[4], BishopMobilityBonus_EG[4]),
    Pack(BishopMobilityBonus_MG[5], BishopMobilityBonus_EG[5]),
    Pack(BishopMobilityBonus_MG[6], BishopMobilityBonus_EG[6]),
    Pack(BishopMobilityBonus_MG[7], BishopMobilityBonus_EG[7]),
    Pack(BishopMobilityBonus_MG[8], BishopMobilityBonus_EG[8]),
    Pack(BishopMobilityBonus_MG[9], BishopMobilityBonus_EG[9]),
    Pack(BishopMobilityBonus_MG[10], BishopMobilityBonus_EG[10]),
    Pack(BishopMobilityBonus_MG[11], BishopMobilityBonus_EG[11]),
    Pack(BishopMobilityBonus_MG[12], BishopMobilityBonus_EG[12]),
    Pack(BishopMobilityBonus_MG[13], BishopMobilityBonus_EG[13]),
    Pack(BishopMobilityBonus_MG[14], BishopMobilityBonus_EG[14])};

const int BishopMobilityBonusStartIndex = base + 5 + 10;

static constexpr int numParameters = base +
                                    //  1 + // DoubledPawnPenalty
                                     1 + // IsolatedPawnPenalty
                                     1 + // OpenFileRookBonus
                                     1 + // SemiOpenFileRookBonus
                                     1 + // RookMobilityBonus
                                     1 + // QueenMobilityBonus
                                     1 + // SemiOpenFileKingPenalty
                                     1 + // OpenFileKingPenalty
                                     1 + // BishopPairMaxBonus
                                     1 + // KingShieldBonus
                                     6 + // PassedPawnBonus - removing 1 and 8 rank values
                                     14  // BishopMobilityBonus - removing count 14
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

        // result.push_back({(double)DoubledPawnPenalty_MG, (double)DoubledPawnPenalty_EG});
        result.push_back({(double)IsolatedPawnPenalty_MG, (double)IsolatedPawnPenalty_EG});
        result.push_back({(double)OpenFileRookBonus_MG, (double)OpenFileRookBonus_EG});
        result.push_back({(double)SemiOpenFileRookBonus_MG, (double)SemiOpenFileRookBonus_EG});
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

        for (int count = 0; count < 14; ++count)
        {
            result.push_back({(double)BishopMobilityBonus_MG[count], (double)BishopMobilityBonus_EG[count]});
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

        // std::cout << "\"DoubledPawnPenalty\": {" << std::endl;
        // std::cout << "\t\"MG\": " << round(parameters[DoubledPawnPenaltyIndex][0]) << ",\n";
        // std::cout << "\t\"EG\": " << round(parameters[DoubledPawnPenaltyIndex][1]) << "\n}," << std::endl;

        std::cout << "\"IsolatedPawnPenalty\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[IsolatedPawnPenaltyIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[IsolatedPawnPenaltyIndex][1]) << "\n}," << std::endl;

        std::cout << "\"OpenFileRookBonus\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[OpenFileRookBonusIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[OpenFileRookBonusIndex][1]) << "\n}," << std::endl;

        std::cout << "\"SemiOpenFileRookBonus\": {" << std::endl;
        std::cout << "\t\"MG\": " << round(parameters[SemiOpenFileRookBonusIndex][0]) << ",\n";
        std::cout << "\t\"EG\": " << round(parameters[SemiOpenFileRookBonusIndex][1]) << "\n}," << std::endl;

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
        std::cout << "\t\t\"EG\": " << 0 << "\n\t}" << std::endl;
        std::cout << "},";

        std::cout << std::endl;

        std::cout << "\"BishopMobilityBonus\": {" << std::endl;
        for (int count = 0; count < 14; ++count)
        {
            std::cout << "\t\"Count" << count << "\": {" << std::endl;
            std::cout << "\t\t\"MG\": " << round(parameters[BishopMobilityBonusStartIndex + count][0]) << ",\n";
            std::cout << "\t\t\"EG\": " << round(parameters[BishopMobilityBonusStartIndex + count][1]) << "\n\t}," << std::endl;
        }
        std::cout << "\t\"Count" << 14 << "\": {" << std::endl;
        std::cout << "\t\t\"MG\": " << 0 << ",\n";
        std::cout << "\t\t\"EG\": " << 0 << "\n\t}" << std::endl;
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
    return __builtin_bswap64(board.pieces(piece, color).getBits());
}

void ResetLS1B(std::uint64_t &board)
{
    board &= (board - 1);
}

int PawnAdditionalEvaluation(int squareIndex, int pieceIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    int packedBonus = 0;
    // auto doublePawnsCount = chess::builtin::popcount(GetPieceSwappingEndianness(board, chess::PieceType::PAWN, color) & (FileMasks[squareIndex]));
    // if (doublePawnsCount > 1)
    // {
    //     packedBonus += doublePawnsCount * DoubledPawnPenalty_Packed;
    //     IncrementCoefficients(coefficients, DoubledPawnPenaltyIndex, color);
    // }

    if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, color) & IsolatedPawnMasks[squareIndex]) == 0) // isIsolatedPawn
    {
        packedBonus += IsolatedPawnPenalty_Packed;
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
            packedBonus += PassedPawnBonus_Packed[rank];
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

            packedBonus += PassedPawnBonus_Packed[rank];
            IncrementCoefficients(coefficients, PassedPawnBonusStartIndex + rank - 1, color); // There's no coefficient for rank 0
            // std::cout << "Black pawn on " << squareIndex << " is passed, bonus " << -PassedPawnBonus[rank] << std::endl;
        }
    }

    return packedBonus;
}

int RookAdditonalEvaluation(int squareIndex, int pieceIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = chess::attacks::rook(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ().getBits())).count();
    IncrementCoefficients(coefficients, RookMobilityBonusIndex, color, mobilityCount);

    int packedBonus = RookMobilityBonus_Packed * mobilityCount;

    if (((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) | GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
    {
        // std::cout << "OpenFileRookBonus" << std::endl;
        IncrementCoefficients(coefficients, OpenFileRookBonusIndex, color);
        packedBonus += OpenFileRookBonus_Packed;
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
                packedBonus += SemiOpenFileRookBonus_Packed;
            }
        }
        else
        {
            if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
            {
                // std::cout << "SemiOpenFileRookBonus black" << std::endl;
                IncrementCoefficients(coefficients, SemiOpenFileRookBonusIndex, color);
                packedBonus += SemiOpenFileRookBonus_Packed;
            }
        }
    }

    return packedBonus;
}

int BishopAdditionalEvaluation(int squareIndex, int pieceIndex, const int pieceCount[], const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    int packedBonus = 0;

    auto mobilityCount = chess::attacks::bishop(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ().getBits())).count();
    IncrementCoefficients(coefficients, BishopMobilityBonusStartIndex + mobilityCount, color);

    packedBonus += BishopMobilityBonus_Packed[mobilityCount];

    if (pieceCount[pieceIndex] == 2)
    {
        packedBonus += BishopPairBonus_Packed;
        IncrementCoefficients(coefficients, BishopPairMaxBonusIndex, color);
    }

    return packedBonus;
}

int QueenAdditionalEvaluation(int squareIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = chess::attacks::queen(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ().getBits())).count();
    IncrementCoefficients(coefficients, QueenMobilityBonusIndex, color, mobilityCount);

    return QueenMobilityBonus_Packed * mobilityCount;
}

int KingAdditionalEvaluation(int squareIndex, chess::Color kingSide, const chess::Board &board, const int pieceCount[], coefficients_t &coefficients)
{
    int packedBonus = 0;
    auto kingSideOffset = kingSide == chess::Color::WHITE ? 0 : 6;

    if (pieceCount[9 - kingSideOffset] + pieceCount[10 - kingSideOffset] != 0) // areThereOppositeSideRooksOrQueens
    {
        if (((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) | GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
        {
            // std::cout << "Open: " << (kingSide == chess::Color::WHITE ? "White" : "Black") << std::endl;
            packedBonus += OpenFileKingPenalty_Packed;
            IncrementCoefficients(coefficients, OpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == chess::Color::WHITE && (GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: " << (kingSide == chess::Color::WHITE ? "White" : "Black") << std::endl;
            packedBonus += SemiOpenFileKingPenalty_Packed;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == chess::Color::BLACK && (GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: "  << (kingSide == chess::Color::WHITE ? "White" : "Black") << std::endl;
            packedBonus += SemiOpenFileKingPenalty_Packed;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
    }

    auto ownPiecesAroundCount = chess::builtin::popcount(chess::attacks::king(static_cast<chess::Square>(squareIndex)).getBits() & __builtin_bswap64(board.us(kingSide).getBits()));
    IncrementCoefficients(coefficients, KingShieldBonusIndex, kingSide, ownPiecesAroundCount);

    return packedBonus + KingShieldBonus_Packed * ownPiecesAroundCount;
}

int AdditionalPieceEvaluation(int pieceSquareIndex, int pieceIndex, const int pieceCount[], const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
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
        return 0;
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

    int packedScore = 0;
    int gamePhase = 0;

    for (int pieceIndex = 0; pieceIndex < 5; ++pieceIndex)
    {
        // Bitboard copy that we 'empty'
        auto bitboard = GetPieceSwappingEndianness(board, chess::PieceType(static_cast<chess::PieceType::underlying>(pieceIndex)), chess::Color::WHITE);
        // std::cout << pieceIndex << "bb: " << bitboard << std::endl;
        while (bitboard != 0)
        {
            auto pieceSquareIndex = chess::builtin::lsb(bitboard).index();
            ResetLS1B(bitboard);

            packedScore += PackedPositionalTables(pieceIndex, pieceSquareIndex) + PackedPieceValue[pieceIndex];
            gamePhase += phaseValues[pieceIndex];

            ++pieceCount[pieceIndex];

            packedScore += AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, pieceCount, board, chess::Color::WHITE, coefficients);

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
        auto bitboard = GetPieceSwappingEndianness(board, chess::PieceType(static_cast<chess::PieceType::underlying>(tunerPieceIndex)), chess::Color::BLACK);

        while (bitboard != 0)
        {
            auto pieceSquareIndex = chess::builtin::lsb(bitboard).index();
            ResetLS1B(bitboard);

            packedScore += PackedPositionalTables(pieceIndex, pieceSquareIndex) - PackedPieceValue[tunerPieceIndex];
            gamePhase += phaseValues[tunerPieceIndex];

            ++pieceCount[pieceIndex];

            packedScore -= AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, pieceCount, board, chess::Color::BLACK, coefficients);

            if (pieceIndex == 6)
                IncrementCoefficients(coefficients, pieceSquareIndex - 8, chess::Color::BLACK);
            else
                IncrementCoefficients(coefficients, 64 * tunerPieceIndex + pieceSquareIndex - 16, chess::Color::BLACK);
        }
    }

    auto whiteKing = chess::builtin::lsb(GetPieceSwappingEndianness(board, chess::PieceType::KING, chess::Color::WHITE)).index();
    auto blackKing = chess::builtin::lsb(GetPieceSwappingEndianness(board, chess::PieceType::KING, chess::Color::BLACK)).index();
    packedScore += PackedPositionalTables(5, whiteKing) +
                   PackedPositionalTables(11, blackKing) +
                   KingAdditionalEvaluation(whiteKing, chess::Color::WHITE, board, pieceCount, coefficients) -
                   KingAdditionalEvaluation(blackKing, chess::Color::BLACK, board, pieceCount, coefficients);

    IncrementCoefficients(coefficients, 64 * 5 + whiteKing - 16, chess::Color::WHITE);
    IncrementCoefficients(coefficients, 64 * 5 + blackKing - 16, chess::Color::BLACK);

    // Debugging eval
    // return EvalResult{
    //     std::move(coefficients),
    //     (double)(board.sideToMove() == chess::Color::WHITE
    //                  ? eval
    //                  : -eval)};

    // Check if drawn position due to lack of material

    if (gamePhase <= 3 && pieceCount[0] == 0 && pieceCount[6] == 0)
    {
        switch (gamePhase)
        {
        // case 5:
        //     {
        //         // RB vs R, RN vs R - escale it down due to the chances of it being a draw
        //         if (pieceCount[(int)Piece.R] == 1 && pieceCount[(int)Piece.r] == 1)
        //         {
        //             packedScore >>= 1; // /2
        //         }

        //        break;
        //    }
        case 3:
        {
            auto winningSideOffset = PieceOffset(packedScore >= 0);

            if (pieceCount[1 + winningSideOffset] == 2) // NN vs N, NN vs B
            {
                return EvalResult{
                    std::move(coefficients),
                    (double)0};
            }

            // Without rooks, only BB vs N is a win and BN vs N can have some chances
            // Not taking that into account here though, we would need this to rule them out: `pieceCount[(int)Piece.b - winningSideOffset] == 1 || pieceCount[(int)Piece.B + winningSideOffset] <= 1`
            // if (pieceCount[(int)Piece.R + winningSideOffset] == 0)  // BN vs B, NN vs B, BB vs B, BN vs N, NN vs N
            //{
            //    packedScore >>= 1; // /2
            //}

            break;
        }
        case 2:
        {
            if (pieceCount[1] + pieceCount[7] == 2     // NN vs -, N vs N
                || pieceCount[1] + pieceCount[2] == 1) // B vs N, B vs B
            {
                return EvalResult{
                    std::move(coefficients),
                    (double)0};
            }

            break;
        }
        case 1:
        case 0:
        {
            return EvalResult{
                std::move(coefficients),
                (double)0};
        }
        }
    }

    const int maxPhase = 24;

    if (gamePhase > maxPhase) // Early promotions
    {
        gamePhase = maxPhase;
    }

    int endGamePhase = maxPhase - gamePhase;

    auto middleGameScore = UnpackMG(packedScore);
    auto endGameScore = UnpackEG(packedScore);
    int eval = ((middleGameScore * gamePhase) + (endGameScore * endGamePhase)) / maxPhase;
    // eval = ((middleGameScore * gamePhase) + (endGameScore * endGamePhase));

    // Always white's perspective
    return EvalResult{
        std::move(coefficients),
        (double)eval};
}
