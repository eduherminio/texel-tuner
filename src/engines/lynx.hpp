#include "../base.h"
#include "./lynx_constants.hpp"
#include "../external/chess.hpp"
#include <cassert>
#include <array>
#include <bit>
#include <string>
#include <cmath>

using u64 = uint64_t;

const int DoubledPawnPenalty_MG = 10;
const int DoubledPawnPenalty_EG = 10;
const int DoubledPawnPenaltyIndex = 64 * 6 + 4 + 1;

const int IsolatedPawnPenalty_MG = 10;
const int IsolatedPawnPenalty_EG = 10;
const int IsolatedPawnPenaltyIndex = 64 * 6 + 4 + 2;

const int OpenFileRookBonus_MG = 15;
const int OpenFileRookBonus_EG = 15;
const int OpenFileRookBonusIndex = 64 * 6 + 4 + 3;

const int SemiOpenFileRookBonus_MG = 10;
const int SemiOpenFileRookBonus_EG = 10;
const int SemiOpenFileRookBonusIndex = 64 * 6 + 4 + 4;

const int BishopMobilityBonus_MG = 1;
const int BishopMobilityBonus_EG = 1;
const int BishopMobilityBonusIndex = 64 * 6 + 4 + 5;

const int QueenMobilityBonus_MG = 1;
const int QueenMobilityBonus_EG = 1;
const int QueenMobilityBonusIndex = 64 * 6 + 4 + 6;

const int SemiOpenFileKingPenalty_MG = 10;
const int SemiOpenFileKingPenalty_EG = 10;
const int SemiOpenFileKingPenaltyIndex = 64 * 6 + 4 + 7;

const int OpenFileKingPenalty_MG = 15;
const int OpenFileKingPenalty_EG = 15;
const int OpenFileKingPenaltyIndex = 64 * 6 + 4 + 8;

const int KingShieldBonus_MG = 5;
const int KingShieldBonus_EG = 5;
const int KingShieldBonusIndex = 64 * 6 + 4 + 9;

const int BishopPairBonus_MG = 0;
const int BishopPairBonus_EG = 100;
const int BishopPairMaxBonusIndex = 64 * 6 + 4 + 10;

constexpr static std::array<int, 8> PassedPawnBonus_MG = {
    0, 10, 30, 50, 75, 100, 150, 200};

constexpr static std::array<int, 8> PassedPawnBonus_EG = {
    0, 10, 30, 50, 75, 100, 150, 200};

const int PassedPawnBonusStartIndex = 64 * 6 + 4 + 11;

static constexpr int numParameters = 64 * 6 +
                                     5 + // Piece values
                                     1 + // DoubledPawnPenalty
                                     1 + // IsolatedPawnPenalty
                                     1 + // OpenFileRookBonus
                                     1 + // SemiOpenFileRookBonus
                                     1 + // BishopMobilityBonus
                                     1 + // QueenMobilityBonus
                                     1 + // SemiOpenFileKingPenalty
                                     1 + // OpenFileKingPenalty
                                     1 + // BishopPairMaxBonus
                                     1 + // KingShieldBonus
                                     8   // PassedPawnBonus
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

        assert(MiddleGamePositionalTables.size() == 12);
        assert(EndGamePositionalTables.size() == 12);

        for (int piece = 0; piece < 6; ++piece)
        {
            for (int square = 0; square < 64; ++square)
            {
                result.push_back({(double)MiddleGamePositionalTables[piece][square], (double)EndGamePositionalTables[piece + 6][square]});
            }
        }
        for (int piece = 0; piece < 5; ++piece)
        {
            result.push_back({(double)pestoPieceValue[piece], (double)pestoPieceValue[piece + 5]});
        }

        result.push_back({(double)DoubledPawnPenalty_MG, (double)DoubledPawnPenalty_EG});
        result.push_back({(double)IsolatedPawnPenalty_MG, (double)IsolatedPawnPenalty_EG});
        result.push_back({(double)OpenFileRookBonus_MG, (double)OpenFileRookBonus_EG});
        result.push_back({(double)SemiOpenFileRookBonus_MG, (double)SemiOpenFileRookBonus_EG});
        result.push_back({(double)BishopMobilityBonus_MG, (double)BishopMobilityBonus_EG});
        result.push_back({(double)QueenMobilityBonus_MG, (double)QueenMobilityBonus_EG});
        result.push_back({(double)SemiOpenFileKingPenalty_MG, (double)SemiOpenFileKingPenalty_EG});
        result.push_back({(double)OpenFileKingPenalty_MG, (double)OpenFileKingPenalty_EG});
        result.push_back({(double)KingShieldBonus_MG, (double)KingShieldBonus_EG});
        result.push_back({(double)BishopPairBonus_MG, (double)BishopPairBonus_EG});

        for (int rank = 0; rank < 8; ++rank)
        {
            result.push_back({(double)PassedPawnBonus_MG[rank], (double)PassedPawnBonus_EG[rank]});
        }

        std::cout << result.size() << " == " << numParameters << std::endl;
        assert(result.size() == numParameters);

        return result;
    }

    static EvalResult get_fen_eval_result(const std::string &fen)
    {
        Chess::Board board;
        board.loadFen(fen);
        return get_external_eval_result(board);
    }

    static EvalResult get_external_eval_result(const Chess::Board &board);

    static int round(double value)
    {
        return std::round(value);
    }

    static void print_parameters(const parameters_t &parameters)
    {
        std::string names[] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};
        for (int piece = 0; piece < 6; ++piece)
        {
            for (int phase = 0; phase <= 1; ++phase)
            {
                std::cout << "\npublic static readonly int[] " << (phase == 0 ? "MiddleGame" : "EndGame") << names[piece] << "Table =\n[\n\t";
                for (int square = 0; square < 64; ++square)
                {
                    std::cout << round(parameters[piece * 64 + square][phase]) << ", ";
                    if (square % 8 == 7)
                        std::cout << "\n";
                    if (square != 63)
                        std::cout << "\t";
                }
                std::cout << "];\n";
            }
        }
        std::cout << std::endl;

        std::cout << "public static readonly int[] MiddleGamePieceValues =\n[\n\t";
        for (int piece = 0; piece < 5; ++piece)
        {
            std::cout << "+" << round(parameters[64 * 6 + piece][0]) << ", ";
        }
        std::cout << "0,\n\t"; // King
        for (int piece = 0; piece < 5; ++piece)
        {
            std::cout << "-" << round(parameters[64 * 6 + piece][0]) << ", ";
        }
        std::cout << "0\n];\n" << std::endl; // King

        std::cout << "public static readonly int[] EndGamePieceValues =\n[\n\t";
        for (int piece = 0; piece < 5; ++piece)
        {
            std::cout << "+" << round(parameters[64 * 6 + piece][1]) << ", ";
        }
        std::cout << "0,\n\t"; // King
        for (int piece = 0; piece < 5; ++piece)
        {
            std::cout << "-" << round(parameters[64 * 6 + piece][1]) << ", ";
        }
        std::cout << "0\n];\n" << std::endl; // King

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
        for (int rank = 0; rank < 8; ++rank)
        {
            std::cout << "\t\"Rank" << rank << "\": {" << std::endl;
            std::cout << "\t\t\"MG\": " << round(parameters[PassedPawnBonusStartIndex + rank][0]) << ",\n";
            std::cout << "\t\t\"EG\": " << round(parameters[PassedPawnBonusStartIndex + rank][1]) << "\n\t}," << std::endl;
        }
        std::cout << "}";

        std::cout << '\n'
                  << std::endl;
    }
};

static inline parameters_t initialParameters = Lynx::get_initial_parameters();

void IncrementCoefficients(coefficients_t &coefficients, int index, const Chess::Color &color, int increment = 1)
{
    coefficients[index] += color == Chess::Color::WHITE
                               ? increment
                               : -increment;
}

Chess::U64 GetPieceSwappingEndianness(const Chess::Board &board, const Chess::PieceType &piece, const Chess::Color &color)
{
    return __builtin_bswap64(board.pieces(piece, color));
}

std::pair<int, int> PawnAdditionalEvaluation(int squareIndex, int pieceIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    int middleGameBonus = 0, endGameBonus = 0;
    auto doublePawnsCount = Chess::popcount(GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, color) & (FileMasks[squareIndex]));
    if (doublePawnsCount > 1)
    {
        middleGameBonus += doublePawnsCount * DoubledPawnPenalty_MG;
        endGameBonus += doublePawnsCount * DoubledPawnPenalty_EG;
        IncrementCoefficients(coefficients, DoubledPawnPenaltyIndex, color);
    }

    if ((GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, color) & IsolatedPawnMasks[squareIndex]) == 0) // isIsolatedPawn
    {
        middleGameBonus += IsolatedPawnPenalty_MG;
        endGameBonus += IsolatedPawnPenalty_EG;
        IncrementCoefficients(coefficients, IsolatedPawnPenaltyIndex, color);
    }

    if (color == Chess::Color::WHITE)
    {

        if ((GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::BLACK) & WhitePassedPawnMasks[squareIndex]) == 0) // isPassedPawn
        {
            // std::cout << "Piece: " << GetPiece(board, Chess::PieceType::PAWN, Chess::Color::BLACK) << std::endl;
            // std::cout << "Mask: " << WhitePassedPawnMasks[squareIndex] << std::endl;
            auto rank = Rank[squareIndex];
            if (pieceIndex == 6)
            {
                rank = 7 - rank;
            }
            middleGameBonus += PassedPawnBonus_MG[rank];
            endGameBonus += PassedPawnBonus_EG[rank];
            IncrementCoefficients(coefficients, PassedPawnBonusStartIndex + rank, color);
            // std::cout << "White pawn on " << squareIndex << " is passed, bonus " << PassedPawnBonus[rank] << std::endl;
        }
    }
    else
    {

        if ((GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::WHITE) & BlackPassedPawnMasks[squareIndex]) == 0) // isPassedPawn
        {
            auto rank = Rank[squareIndex];
            rank = 7 - rank;

            middleGameBonus += PassedPawnBonus_MG[rank];
            endGameBonus += PassedPawnBonus_EG[rank];
            IncrementCoefficients(coefficients, PassedPawnBonusStartIndex + rank, color);
            // std::cout << "Black pawn on " << squareIndex << " is passed, bonus " << -PassedPawnBonus[rank] << std::endl;
        }
    }

    return std::make_pair(middleGameBonus, endGameBonus);
}

std::pair<int, int> RookAdditonalEvaluation(int squareIndex, int pieceIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    const int pawnToRookOffset = 3;

    if (((GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::WHITE) | GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
    {
        // std::cout << "OpenFileRookBonus" << std::endl;
        IncrementCoefficients(coefficients, OpenFileRookBonusIndex, color);

        return std::make_pair(OpenFileRookBonus_MG, OpenFileRookBonus_EG);
    }

    if (color == Chess::Color::WHITE)
    {
        if ((GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Piece: " << GetPiece(board, Chess::PieceType::ROOK, Chess::Color::BLACK) << std::endl;
            // std::cout << "Mask: " << FileMasks[squareIndex] << std::endl;
            // std::cout << "SemiOpenFileRookBonus white" << std::endl;
            IncrementCoefficients(coefficients, SemiOpenFileRookBonusIndex, color);

            return std::make_pair(SemiOpenFileRookBonus_MG, SemiOpenFileRookBonus_EG);
        }
    }
    else
    {
        if ((GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "SemiOpenFileRookBonus black" << std::endl;
            IncrementCoefficients(coefficients, SemiOpenFileRookBonusIndex, color);

            return std::make_pair(SemiOpenFileRookBonus_MG, SemiOpenFileRookBonus_EG);
        }
    }

    return std::make_pair(0, 0);
}

std::pair<int, int> BishopAdditionalEvaluation(int squareIndex, int pieceIndex, const int pieceCount[], const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    int middleGameBonus = 0, endGameBonus = 0;

    auto mobilityCount = Chess::popcount(Chess::Attacks::BISHOP(static_cast<Chess::Square>(squareIndex), __builtin_bswap64(board.occ())));
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

std::pair<int, int> QueenAdditionalEvaluation(int squareIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = Chess::popcount(Chess::Attacks::QUEEN(static_cast<Chess::Square>(squareIndex), __builtin_bswap64(board.occ())));
    IncrementCoefficients(coefficients, QueenMobilityBonusIndex, color, mobilityCount);

    return std::make_pair(QueenMobilityBonus_MG * mobilityCount, QueenMobilityBonus_EG * mobilityCount);
}

std::pair<int, int> KingAdditionalEvaluation(int squareIndex, Chess::Color kingSide, const Chess::Board &board, const int pieceCount[], coefficients_t &coefficients)
{
    int middleGameBonus = 0, endGameBonus = 0;
    auto kingSideOffset = kingSide == Chess::Color::WHITE ? 0 : 6;

    if (pieceCount[9 - kingSideOffset] + pieceCount[10 - kingSideOffset] != 0) // areThereOppositeSideRooksOrQueens
    {
        if (((GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::WHITE) | GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
        {
            // std::cout << "Open: " << (kingSide == Chess::Color::WHITE ? "White" : "Black") << std::endl;
            middleGameBonus += OpenFileKingPenalty_MG;
            endGameBonus += OpenFileKingPenalty_EG;
            IncrementCoefficients(coefficients, OpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == Chess::Color::WHITE && (GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: " << (kingSide == Chess::Color::WHITE ? "White" : "Black") << std::endl;
            middleGameBonus += SemiOpenFileKingPenalty_MG;
            endGameBonus += SemiOpenFileKingPenalty_EG;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == Chess::Color::BLACK && (GetPieceSwappingEndianness(board, Chess::PieceType::PAWN, Chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: "  << (kingSide == Chess::Color::WHITE ? "White" : "Black") << std::endl;
            middleGameBonus += SemiOpenFileKingPenalty_MG;
            endGameBonus += SemiOpenFileKingPenalty_EG;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
    }

    auto ownPiecesAroundCount = Chess::popcount(Chess::Attacks::KING(static_cast<Chess::Square>(squareIndex)) & __builtin_bswap64(board.us(kingSide)));
    IncrementCoefficients(coefficients, KingShieldBonusIndex, kingSide, ownPiecesAroundCount);

    return std::make_pair(middleGameBonus + KingShieldBonus_MG * ownPiecesAroundCount,
                          endGameBonus + KingShieldBonus_EG * ownPiecesAroundCount);
}

std::pair<int, int> AdditionalPieceEvaluation(int pieceSquareIndex, int pieceIndex, const int pieceCount[], const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
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

EvalResult Lynx::get_external_eval_result(const Chess::Board &board)
{
    std::vector<std::int16_t> coefficients(numParameters, 0);

    int pieceCount[12] = {}; // Init to zeros

    int middleGameScore = 0;
    int endGameScore = 0;
    int gamePhase = 0;

    for (int pieceIndex = 0; pieceIndex < 5; ++pieceIndex)
    {
        // Bitboard copy that we 'empty'
        auto bitboard = GetPieceSwappingEndianness(board, (Chess::PieceType)pieceIndex, Chess::Color::WHITE);
        // std::cout << pieceIndex << "bb: " << bitboard << std::endl;
        while (bitboard != 0)
        {
            auto pieceSquareIndex = Chess::lsb(bitboard);
            Chess::poplsb(bitboard);

            middleGameScore += MiddleGamePositionalTables[pieceIndex][pieceSquareIndex] + pestoPieceValue[pieceIndex];
            endGameScore += EndGamePositionalTables[pieceIndex][pieceSquareIndex] + pestoPieceValue[pieceIndex + 5];
            gamePhase += phaseValues[pieceIndex];

            ++pieceCount[pieceIndex];

            auto pair = AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, pieceCount, board, Chess::Color::WHITE, coefficients);
            middleGameScore += pair.first;
            endGameScore += pair.second;

            IncrementCoefficients(coefficients, 64 * 6 + pieceIndex, Chess::Color::WHITE);
            IncrementCoefficients(coefficients, 64 * pieceIndex + pieceSquareIndex, Chess::Color::WHITE);
        }
    }

    for (int pieceIndex = 6; pieceIndex < 11; ++pieceIndex)
    {
        auto tunerPieceIndex = (pieceIndex - 6); // [0, 5]
        // Bitboard copy that we 'empty'
        auto bitboard = GetPieceSwappingEndianness(board, (Chess::PieceType)tunerPieceIndex, Chess::Color::BLACK);

        while (bitboard != 0)
        {
            auto pieceSquareIndex = Chess::lsb(bitboard);
            Chess::poplsb(bitboard);

            middleGameScore += MiddleGamePositionalTables[pieceIndex][pieceSquareIndex] - pestoPieceValue[tunerPieceIndex];
            endGameScore += EndGamePositionalTables[pieceIndex][pieceSquareIndex] - pestoPieceValue[tunerPieceIndex + 5];
            gamePhase += phaseValues[tunerPieceIndex];

            ++pieceCount[pieceIndex];

            auto pair = AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, pieceCount, board, Chess::Color::BLACK, coefficients);
            middleGameScore -= pair.first;
            endGameScore -= pair.second;

            IncrementCoefficients(coefficients, 64 * 6 + tunerPieceIndex, Chess::Color::BLACK);
            IncrementCoefficients(coefficients, 64 * tunerPieceIndex + pieceSquareIndex, Chess::Color::BLACK);
        }
    }

    auto whiteKing = Chess::lsb(GetPieceSwappingEndianness(board, Chess::PieceType::KING, Chess::Color::WHITE));
    auto kingPair = KingAdditionalEvaluation(whiteKing, Chess::Color::WHITE, board, pieceCount, coefficients);
    middleGameScore += MiddleGamePositionalTables[5][whiteKing] + kingPair.first;
    endGameScore += EndGamePositionalTables[5][whiteKing] + kingPair.second;
    IncrementCoefficients(coefficients, 64 * 5 + whiteKing, Chess::Color::WHITE);

    auto blackKing = Chess::lsb(GetPieceSwappingEndianness(board, Chess::PieceType::KING, Chess::Color::BLACK));
    kingPair = KingAdditionalEvaluation(blackKing, Chess::Color::BLACK, board, pieceCount, coefficients);
    middleGameScore += MiddleGamePositionalTables[11][blackKing] - kingPair.first;
    endGameScore += EndGamePositionalTables[11][blackKing] - kingPair.second;
    IncrementCoefficients(coefficients, 64 * 5 + blackKing, Chess::Color::BLACK);

    // Debugging eval
    // return EvalResult{
    //     std::move(coefficients),
    //     (double)(board.sideToMove() == Chess::Color::WHITE
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

    // Always white's perspective
    return EvalResult{
        std::move(coefficients),
        (double)eval};
}
