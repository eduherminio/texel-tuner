
#include "../base.h"
#include "./lynx_constants.hpp"
#include "../external/chess.hpp"
#include <cassert>
#include <array>
#include <bit>
#include <string>
#include <cmath>

using u64 = uint64_t;

const int DoubledPawnPenalty = 10;
const int DoubledPawnPenaltyIndex = 64 * 6 + 4 + 1;

const int IsolatedPawnPenalty = 10;
const int IsolatedPawnPenaltyIndex = 64 * 6 + 4 + 2;

const int OpenFileRookBonus = 15;
const int OpenFileRookBonusIndex = 64 * 6 + 4 + 3;

const int SemiOpenFileRookBonus = 10;
const int SemiOpenFileRookBonusIndex = 64 * 6 + 4 + 4;

const int BishopMobilityBonus = 1;
const int BishopMobilityBonusIndex = 64 * 6 + 4 + 5;

const int QueenMobilityBonus = 1;
const int QueenMobilityBonusIndex = 64 * 6 + 4 + 6;

const int SemiOpenFileKingPenalty = 10;
const int SemiOpenFileKingPenaltyIndex = 64 * 6 + 4 + 7;

const int OpenFileKingPenalty = 15;
const int OpenFileKingPenaltyIndex = 64 * 6 + 4 + 8;

// const int KingShieldBonus = 5;

const int BishopPairMaxBonus = 100;
const int BishopPairMaxBonusIndex = 64 * 6 + 4 + 9;

constexpr static std::array<int, 8> PassedPawnBonus = {
    0, 10, 30, 50, 75, 100, 150, 200};

const int PassedPawnBonusStartIndex = 64 * 6 + 4 + 10;

// std::array<int, 12> pestoPieceValue = {
//     82, 337, 365, 477, 1025, // 0,
//     94, 281, 297, 512,  936, // 0
// };
std::array<int, 12> pestoPieceValue = {
    100, 300, 300, 500, 900, // 0,
    100, 300, 300, 500, 900, // 0,
};

std::array<int, 6> phaseValues = {0, 1, 1, 2, 4, 0};

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
                                         //  1 +    // KingShieldBonus
                                     1 + // BishopPairMaxBonus
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

        result.push_back({(double)DoubledPawnPenalty, (double)DoubledPawnPenalty});
        result.push_back({(double)IsolatedPawnPenalty, (double)IsolatedPawnPenalty});
        result.push_back({(double)OpenFileRookBonus, (double)OpenFileRookBonus});
        result.push_back({(double)SemiOpenFileRookBonus, (double)SemiOpenFileRookBonus});
        result.push_back({(double)BishopMobilityBonus, (double)BishopMobilityBonus});
        result.push_back({(double)QueenMobilityBonus, (double)QueenMobilityBonus});
        result.push_back({(double)SemiOpenFileKingPenalty, (double)SemiOpenFileKingPenalty});
        result.push_back({(double)OpenFileKingPenalty, (double)OpenFileKingPenalty});
        result.push_back({(double)BishopPairMaxBonus, (double)BishopPairMaxBonus});

        for (int rank = 0; rank < 8; ++rank)
        {
            result.push_back({(double)PassedPawnBonus[rank], (double)PassedPawnBonus[rank]});
        }

        std::cout << result.size() << " == " << numParameters << std::endl;
        assert(result.size() == numParameters);
        // assert(result.back()[0] == 5.0);
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
        std::cout << std::round(value);
    }

    static void print_parameters(const parameters_t &parameters)
    {
        std::string names[] = {"pawn", "knight", "bishop", "rook", "queen", "king"};
        std::cout << "{";
        for (int piece = 0; piece < 6; ++piece)
        {
            std::cout << "// " << names[piece] << std::endl;
            for (int phase = 0; phase <= 1; ++phase)
            {
                std::cout << "\n { // " << names[piece] << " " << (phase == 0 ? "mg" : "eg") << "\n";
                for (int square = 0; square < 64; ++square)
                {
                    std::cout << round(parameters[piece * 64 + square][phase]) << ", ";
                    if (square % 8 == 7)
                        std::cout << "\n";
                }
                std::cout << " },\n";
            }
        }
        std::cout << "};\n{ ";

        std::cout << "// Piece values\n{ ";
        // Piece values mg
        std::cout << "// Piece values mg" << std::endl;
        for (int piece = 0; piece < 5; ++piece)
        {
            std::cout << round(parameters[64 * 6 + piece][0]) << ", ";
        }
        std::cout << "00, \n"; // King

        // Piece values eg
        std::cout << "// Piece values eg" << std::endl;
        for (int piece = 0; piece < 5; ++piece)
        {
            std::cout << round(parameters[64 * 6 + piece][1]) << ", ";
        }
        std::cout << "00 };\n"; // King

        auto base = 64 * 6 + 4;

        std::cout << "// DoubledPawnPenalty" << std::endl;
        std::cout << "[" << round(parameters[DoubledPawnPenaltyIndex][0]) << ", " << round(parameters[DoubledPawnPenaltyIndex][1]) << "], " << std::endl;

        std::cout << "// IsolatedPawnPenalty" << std::endl;
        std::cout << "[" << round(parameters[IsolatedPawnPenaltyIndex][0]) << ", " << round(parameters[IsolatedPawnPenaltyIndex][1]) << "], " << std::endl;

        std::cout << "// OpenFileRookBonus" << std::endl;
        std::cout << "[" << round(parameters[OpenFileRookBonusIndex][0]) << ", " << round(parameters[OpenFileRookBonusIndex][1]) << "], " << std::endl;

        std::cout << "// SemiOpenFileRookBonus" << std::endl;
        std::cout << "[" << round(parameters[SemiOpenFileRookBonusIndex][0]) << ", " << round(parameters[SemiOpenFileRookBonusIndex][1]) << "], " << std::endl;

        std::cout << "// BishopMobilityBonus" << std::endl;
        std::cout << "[" << round(parameters[BishopMobilityBonusIndex][0]) << ", " << round(parameters[BishopMobilityBonusIndex][1]) << "], " << std::endl;

        std::cout << "// QueenMobilityBonus" << std::endl;
        std::cout << "[" << round(parameters[QueenMobilityBonusIndex][0]) << ", " << round(parameters[QueenMobilityBonusIndex][1]) << "], " << std::endl;

        std::cout << "// SemiOpenFileKingPenalty" << std::endl;
        std::cout << "[" << round(parameters[SemiOpenFileKingPenaltyIndex][0]) << ", " << round(parameters[SemiOpenFileKingPenaltyIndex][1]) << "], " << std::endl;

        std::cout << "// OpenFileKingPenalty" << std::endl;
        std::cout << "[" << round(parameters[OpenFileKingPenaltyIndex][0]) << ", " << round(parameters[OpenFileKingPenaltyIndex][1]) << "], " << std::endl;

        // std::cout << "// KingShieldBonus" << std::endl;
        // std::cout << "[" << round(parameters[KingShieldBonusIndex][0]) << ", " << round(parameters[KingShieldBonusIndex][1] << "]), " << std::endl;

        std::cout << "// BishopPairMaxBonus" << std::endl;
        std::cout << "[" << round(parameters[BishopPairMaxBonusIndex][0]) << ", " << round(parameters[BishopPairMaxBonusIndex][1]) << "], " << std::endl;

        std::cout << "// Passed pawns" << std::endl;
        std::cout << "{ ";
        for (int rank = 0; rank < 8; ++rank)
        {
            std::cout << round(parameters[PassedPawnBonusStartIndex + rank][0]) << ", ";
        }
        std::cout << "};\n{ ";
        for (int rank = 0; rank < 8; ++rank)
        {
            std::cout << round(parameters[PassedPawnBonusStartIndex + rank][1]) << ", ";
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

Chess::U64 GetPiece(const Chess::Board &board, const Chess::PieceType &piece, const Chess::Color &color)
{
    return __builtin_bswap64(board.pieces(piece, color));
}

int PawnAdditionalEvaluation(int squareIndex, int pieceIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    auto bonus = 0;
    auto doublePawnsCount = Chess::popcount(GetPiece(board, Chess::PieceType::PAWN, color) & (FileMasks[squareIndex]));
    if (doublePawnsCount > 1)
    {
        bonus -= doublePawnsCount * DoubledPawnPenalty;
        IncrementCoefficients(coefficients, DoubledPawnPenaltyIndex, color, DoubledPawnPenalty * doublePawnsCount);
    }

    if ((GetPiece(board, Chess::PieceType::PAWN, color) & IsolatedPawnMasks[squareIndex]) == 0) // isIsolatedPawn
    {
        bonus -= IsolatedPawnPenalty;
        IncrementCoefficients(coefficients, IsolatedPawnPenaltyIndex, color, IsolatedPawnPenalty);
    }

    if (color == Chess::Color::WHITE)
    {

        if ((GetPiece(board, Chess::PieceType::PAWN, Chess::Color::BLACK) & WhitePassedPawnMasks[squareIndex]) == 0) // isPassedPawn
        {
            // std::cout << "Piece: " << GetPiece(board, Chess::PieceType::PAWN, Chess::Color::BLACK) << std::endl;
            // std::cout << "Mask: " << WhitePassedPawnMasks[squareIndex] << std::endl;
            auto rank = Rank[squareIndex];
            if (pieceIndex == 6)
            {
                rank = 7 - rank;
            }
            bonus += PassedPawnBonus[rank];
            IncrementCoefficients(coefficients, PassedPawnBonusStartIndex + rank, color);
            // std::cout << "White pawn on " << squareIndex << " is passed, bonus " << PassedPawnBonus[rank] << std::endl;
        }
    }
    else
    {

        if ((GetPiece(board, Chess::PieceType::PAWN, Chess::Color::WHITE) & BlackPassedPawnMasks[squareIndex]) == 0) // isPassedPawn
        {
            auto rank = Rank[squareIndex];
            rank = 7 - rank;

            bonus += PassedPawnBonus[rank];
            IncrementCoefficients(coefficients, PassedPawnBonusStartIndex + rank, color);
            // std::cout << "Black pawn on " << squareIndex << " is passed, bonus " << -PassedPawnBonus[rank] << std::endl;
        }
    }

    return bonus;
}

int RookAdditonalEvaluation(int squareIndex, int pieceIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    const int pawnToRookOffset = 3;

    if (((GetPiece(board, Chess::PieceType::PAWN, Chess::Color::WHITE) | GetPiece(board, Chess::PieceType::PAWN, Chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
    {
        // std::cout << "OpenFileRookBonus" << std::endl;
        IncrementCoefficients(coefficients, OpenFileRookBonusIndex, color);

        return OpenFileRookBonus;
    }

    if (color == Chess::Color::WHITE)
    {
        if ((GetPiece(board, Chess::PieceType::PAWN, Chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Piece: " << GetPiece(board, Chess::PieceType::ROOK, Chess::Color::BLACK) << std::endl;
            // std::cout << "Mask: " << FileMasks[squareIndex] << std::endl;
            // std::cout << "SemiOpenFileRookBonus white" << std::endl;
            IncrementCoefficients(coefficients, SemiOpenFileRookBonusIndex, color);

            return SemiOpenFileRookBonus;
        }
    }
    else
    {
        if ((GetPiece(board, Chess::PieceType::PAWN, Chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "SemiOpenFileRookBonus black" << std::endl;
            IncrementCoefficients(coefficients, SemiOpenFileRookBonusIndex, color);

            return SemiOpenFileRookBonus;
        }
    }

    return 0;
}

int BishopAdditionalEvaluation(int squareIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    return 0;
    auto mobilityCount = Chess::popcount(Chess::Attacks::BISHOP(static_cast<Chess::Square>(squareIndex), __builtin_bswap64(board.occ())));
    IncrementCoefficients(coefficients, BishopMobilityBonusIndex, color, mobilityCount);

    return BishopMobilityBonus * mobilityCount;
}

int QueenAdditionalEvaluation(int squareIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    return 0;
    auto mobilityCount = Chess::popcount(Chess::Attacks::QUEEN(static_cast<Chess::Square>(squareIndex), __builtin_bswap64(board.occ())));
    IncrementCoefficients(coefficients, QueenMobilityBonusIndex, color, mobilityCount);

    return QueenMobilityBonus * mobilityCount;
}

int KingAdditionalEvaluation(int squareIndex, Chess::Color kingSide, const Chess::Board &board, const int pieceCount[], coefficients_t &coefficients)
{
    auto bonus = 0;
    auto kingSideOffset = kingSide == Chess::Color::WHITE ? 0 : 6;

    if (pieceCount[9 - kingSideOffset] + pieceCount[10 - kingSideOffset] != 0) // areThereOppositeSideRooksOrQueens
    {
        if (((GetPiece(board, Chess::PieceType::PAWN, Chess::Color::WHITE) | GetPiece(board, Chess::PieceType::PAWN, Chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
        {
            // std::cout << "Open: " << (kingSide == Chess::Color::WHITE ? "White" : "Black") << std::endl;
            bonus -= OpenFileKingPenalty;
            IncrementCoefficients(coefficients, OpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == Chess::Color::WHITE && (GetPiece(board, Chess::PieceType::PAWN, Chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: " << (kingSide == Chess::Color::WHITE ? "White" : "Black") << std::endl;
            bonus -= SemiOpenFileKingPenalty;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == Chess::Color::BLACK && (GetPiece(board, Chess::PieceType::PAWN, Chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: "  << (kingSide == Chess::Color::WHITE ? "White" : "Black") << std::endl;
            bonus -= SemiOpenFileKingPenalty;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
    }

    return bonus;

    // return bonus + KingShieldBonus *
    //                Chess::popcount(Chess::Attacks::KING(static_cast<Chess::Square>(squareIndex) & __builtin_bswap64(OccupancyBitBoards[(int)kingSide])));
}

int AdditionalPieceEvaluation(int pieceSquareIndex, int pieceIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
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
        return BishopAdditionalEvaluation(pieceSquareIndex, board, color, coefficients);

    case 4:
    case 10:
        return QueenAdditionalEvaluation(pieceSquareIndex, board, color, coefficients);

    default:
        return 0;
    }

    return 0;
}

EvalResult Lynx::get_external_eval_result(const Chess::Board &board)
{
    std::vector<std::int16_t> coefficients(numParameters, 0);

    int pieceCount[12] = {}; // Init to zeros

    int middleGameScore = 0;
    int endGameScore = 0;
    int gamePhase = 0;
    int eval = 0;

    for (int pieceIndex = 0; pieceIndex < 5; ++pieceIndex)
    {
        // Bitboard copy that we 'empty'
        auto bitboard = GetPiece(board, (Chess::PieceType)pieceIndex, Chess::Color::WHITE);
        // std::cout << pieceIndex << "bb: " << bitboard << std::endl;
        while (bitboard != 0)
        {
            auto pieceSquareIndex = Chess::lsb(bitboard);
            Chess::poplsb(bitboard);
            // auto rotatedPieceSquareIndex = pieceSquareIndex ^ 56;
            // std::cout << "\t"
            //   << "sq: " << rotatedPieceSquareIndex << "value: " << MiddleGamePositionalTables[pieceIndex][rotatedPieceSquareIndex] << std::endl;

            middleGameScore += MiddleGamePositionalTables[pieceIndex][pieceSquareIndex] + pestoPieceValue[pieceIndex];
            endGameScore += EndGamePositionalTables[pieceIndex][pieceSquareIndex] + pestoPieceValue[pieceIndex + 5];
            gamePhase += phaseValues[pieceIndex];

            ++pieceCount[pieceIndex];

            eval += AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, board, Chess::Color::WHITE, coefficients);

            IncrementCoefficients(coefficients, 64 * 6 + pieceIndex, Chess::Color::WHITE);
            IncrementCoefficients(coefficients, 64 * pieceIndex + pieceSquareIndex, Chess::Color::WHITE);
        }
    }

    for (int pieceIndex = 6; pieceIndex < 11; ++pieceIndex)
    {
        auto tunerPieceIndex = (pieceIndex - 6); // [0, 5]
        // Bitboard copy that we 'empty'
        auto bitboard = GetPiece(board, (Chess::PieceType)tunerPieceIndex, Chess::Color::BLACK);

        while (bitboard != 0)
        {
            auto pieceSquareIndex = Chess::lsb(bitboard);
            Chess::poplsb(bitboard);
            // auto rotatedPieceSquareIndex = pieceSquareIndex ^ 56;

            middleGameScore += MiddleGamePositionalTables[pieceIndex][pieceSquareIndex] - pestoPieceValue[tunerPieceIndex];
            endGameScore += EndGamePositionalTables[pieceIndex][pieceSquareIndex] - pestoPieceValue[tunerPieceIndex + 5];
            gamePhase += phaseValues[tunerPieceIndex];

            ++pieceCount[pieceIndex];

            eval -= AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, board, Chess::Color::BLACK, coefficients);

            IncrementCoefficients(coefficients, 64 * 6 + tunerPieceIndex, Chess::Color::BLACK);
            IncrementCoefficients(coefficients, 64 * tunerPieceIndex + pieceSquareIndex, Chess::Color::BLACK);
        }
    }

    auto whiteKing = Chess::lsb(GetPiece(board, Chess::PieceType::KING, Chess::Color::WHITE));
    middleGameScore += MiddleGamePositionalTables[5][whiteKing];
    endGameScore += EndGamePositionalTables[5][whiteKing];
    eval += KingAdditionalEvaluation(whiteKing, Chess::Color::WHITE, board, pieceCount, coefficients);
    IncrementCoefficients(coefficients, 64 * 5 + whiteKing, Chess::Color::WHITE);

    auto blackKing = Chess::lsb(GetPiece(board, Chess::PieceType::KING, Chess::Color::BLACK));
    middleGameScore += MiddleGamePositionalTables[11][blackKing];
    endGameScore += EndGamePositionalTables[11][blackKing];
    eval -= KingAdditionalEvaluation(blackKing, Chess::Color::BLACK, board, pieceCount, coefficients);
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
            eval = 0;
        }
    }
    else
    {
        bool blackCannotWin = pieceCount[6] == 0 && pieceCount[10] == 0 && pieceCount[9] == 0 &&
                              (pieceCount[8] + pieceCount[7] == 1              // B or N
                               || (pieceCount[8] == 0 && pieceCount[7] == 2)); // N+N

        if (blackCannotWin)
        {
            eval = 0;
        }
    }

    const int maxPhase = 24;

    if (gamePhase > maxPhase) // Early promotions
    {
        gamePhase = maxPhase;
    }

    int endGamePhase = maxPhase - gamePhase;

    eval += ((middleGameScore * gamePhase) + (endGameScore * endGamePhase)) / 24;

    if (pieceCount[2] >= 2)
    {
        eval += BishopPairMaxBonus * endGamePhase / 24;
        IncrementCoefficients(coefficients, BishopPairMaxBonusIndex, Chess::Color::WHITE);
    }
    if (pieceCount[8] >= 2)
    {
        eval -= BishopPairMaxBonus * endGamePhase / 24;
        IncrementCoefficients(coefficients, BishopPairMaxBonusIndex, Chess::Color::BLACK);
    }

    return EvalResult{
        std::move(coefficients),
        (double)eval};
    // (double)(board.sideToMove() == Chess::Color::WHITE
    //              ? eval
    //              : -eval)};
}
