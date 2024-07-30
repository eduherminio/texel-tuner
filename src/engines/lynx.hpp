#include "../base.h"
#include "../config.h"
#include "./lynx_constants.hpp"
#include "./lynx_tunable.hpp"
#include "../external/chess.hpp"
#include "../external/builtin.hpp"
#include <algorithm>
#include <cassert>
#include <array>
#include <bit>
#include <string>
#include <cmath>

using u64 = uint64_t;

// TunableSingle DoubledPawnPenalty_MG(6, -12);
TunableSingle IsolatedPawnPenalty(-20, -14);
TunableSingle OpenFileRookBonus(45, 6);
TunableSingle SemiOpenFileRookBonus(15, 8);
TunableSingle QueenMobilityBonus(3, 8);
TunableSingle SemiOpenFileKingPenalty(-21, 17);
TunableSingle OpenFileKingPenalty(-86, 15);
TunableSingle KingShieldBonus(11, -9);
TunableSingle BishopPairBonus(31, 81);

TunableSingle PieceProtectedByPawnBonus(6, 11);
TunableSingle PieceAttackedByPawnPenalty(-45, -18);

TunableArray PassedPawnBonus(
    chess::PieceType::PAWN,
    std::vector<int>{0, 7, -6, -8, 21, 64, 100, 0},
    std::vector<int>{0, 11, 19, 46, 81, 161, 227, 0},
    1,
    1);

TunableArray VirtualKingMobilityBonus(
    chess::PieceType::QUEEN,
    std::vector<int>{0, 0, 0, 36, 47, 20, 18, 18, 14, 11, 9, 4, 3, -3, -12, -22, -30, -42, -47, -53, -42, -35, -35, -26, -34, -9, -58, -22},
    std::vector<int>{0, 0, 0, 0, -6, 23, 14, 3, 6, 5, 9, 13, 9, 13, 15, 17, 14, 12, 9, 2, -6, -15, -24, -35, -44, -64, -71, -90},
    0,
    0);

TunableArray KnightMobilityBonus(
    chess::PieceType::KNIGHT,
    std::vector<int>{0, 24, 31, 36, 41, 39, 39, 42, 53},
    std::vector<int>{0, -3, 6, 6, 12, 20, 24, 25, 18},
    0,
    0);

TunableArray BishopMobilityBonus(
    chess::PieceType::BISHOP,
    std::vector<int>{-214, 0, 14, 21, 35, 42, 57, 66, 75, 75, 81, 85, 86, 117, 0},
    std::vector<int>{-182, 0, -2, 37, 54, 68, 89, 98, 111, 118, 123, 122, 122, 116, 0},
    0,
    1);

TunableArray RookMobilityBonus(
    chess::PieceType::ROOK,
    std::vector<int>{0, 9, 14, 19, 17, 24, 27, 32, 33, 36, 41, 44, 44, 59, 54},
    std::vector<int>{0, 28, 29, 36, 46, 50, 57, 61, 73, 80, 82, 83, 88, 87, 85},
    0,
    0);

const int base = (64 * 6 - 16) * PSQTBucketCount; // PSQT but removing pawns from 1 and 8 rank
static int numParameters = base +
                           // DoubledPawnPenalty.size
                           IsolatedPawnPenalty.size +
                           OpenFileRookBonus.size +
                           SemiOpenFileRookBonus.size +
                           QueenMobilityBonus.size +
                           SemiOpenFileKingPenalty.size +
                           OpenFileKingPenalty.size +
                           BishopPairBonus.size +
                           PieceProtectedByPawnBonus.size +
                           PieceAttackedByPawnPenalty.size +
                           KingShieldBonus.size +
                           PassedPawnBonus.tunableSize +          // 6, removing 1 and 8 rank values
                           VirtualKingMobilityBonus.tunableSize + // 28
                           KnightMobilityBonus.tunableSize +      // 9
                           BishopMobilityBonus.tunableSize +      // 14, removing end
                           RookMobilityBonus.tunableSize          // 15
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

        for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
        {
            for (int p = 0; p < 6; ++p)
            {
                for (int sq = 0; sq < 64; ++sq)
                {
                    assert(MiddleGamePositionalTables(bucket, p, sq) == MiddleGamePositionalWhiteTables[p][bucket][sq]);
                    assert(MiddleGamePositionalTables(bucket, p + 6, sq) == -MiddleGamePositionalWhiteTables[p][bucket][sq ^ 56]);
                    assert(MiddleGamePositionalTables(bucket, p, sq) == -MiddleGamePositionalTables(bucket, p + 6, sq ^ 56));
                }
            }
        }

        // Pawns
        {
            const int piece = 0;
            for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
            {
                for (int square = 8; square < 56; ++square)
                    result.push_back({(double)MiddleGamePositionalTables(bucket, piece, square) + PieceValue[bucket][piece], (double)EndGamePositionalTables(bucket, piece, square) + PieceValue[bucket][piece + 5]});
            }
        }

        // N, B, R, Q
        for (int piece = 1; piece < 5; ++piece)
        {
            for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
            {
                for (int square = 0; square < 64; ++square)
                    result.push_back({(double)MiddleGamePositionalTables(bucket, piece, square) + PieceValue[bucket][piece], (double)EndGamePositionalTables(bucket, piece, square) + PieceValue[bucket][piece + 5]});
            }
        }

        // K
        {
            const int piece = 5;

            for (int bucket = 0; bucket < PSQTBucketCount; ++bucket)
            {
                for (int square = 0; square < 64; ++square)
                    result.push_back({(double)MiddleGamePositionalTables(bucket, piece, square), (double)EndGamePositionalTables(bucket, piece, square)});
            }
        }

        // DoubledPawnPenalty.add(result);
        IsolatedPawnPenalty.add(result);
        OpenFileRookBonus.add(result);
        SemiOpenFileRookBonus.add(result);
        QueenMobilityBonus.add(result);
        SemiOpenFileKingPenalty.add(result);
        OpenFileKingPenalty.add(result);
        KingShieldBonus.add(result);
        BishopPairBonus.add(result);
        PieceProtectedByPawnBonus.add(result);
        PieceAttackedByPawnPenalty.add(result);

        PassedPawnBonus.add(result);
        VirtualKingMobilityBonus.add(result);
        KnightMobilityBonus.add(result);
        BishopMobilityBonus.add(result);
        RookMobilityBonus.add(result);

        assert(PassedPawnBonus.tunableSize == 6);
        assert(VirtualKingMobilityBonus.tunableSize == 28);
        assert(KnightMobilityBonus.tunableSize == 9);
        assert(BishopMobilityBonus.tunableSize == 14);
        assert(RookMobilityBonus.tunableSize == 15);

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

    static std::array<std::array<tune_t, 12>, PSQTBucketCount> extract_mobility_offset(const parameters_t &parameters, bool isInitial)
    {
        std::array<std::array<tune_t, 12>, PSQTBucketCount> mobilityPieceValues;

        auto knightMobility = KnightMobilityBonus.extract_offset(parameters);
        auto bishopMobility = BishopMobilityBonus.extract_offset(parameters);
        auto rookMobility = RookMobilityBonus.extract_offset(parameters);

        for (int b = 0; b < PSQTBucketCount; ++b)
        {
            mobilityPieceValues[b].fill(0);

            if (isInitial)
            {
                continue;
            }

            mobilityPieceValues[b][KnightMobilityBonus.pieceIndex] = knightMobility[0];
            mobilityPieceValues[b][KnightMobilityBonus.pieceIndex + 6] = knightMobility[1];

            mobilityPieceValues[b][BishopMobilityBonus.pieceIndex] = bishopMobility[0];
            mobilityPieceValues[b][BishopMobilityBonus.pieceIndex + 6] = bishopMobility[1];

            mobilityPieceValues[b][RookMobilityBonus.pieceIndex] = rookMobility[0];
            mobilityPieceValues[b][RookMobilityBonus.pieceIndex + 6] = rookMobility[1];
        }

        return mobilityPieceValues;
    }

    static void print_parameters(const parameters_t &parameters, bool isInitial = false)
    {
        auto mobilityPieceValues = extract_mobility_offset(parameters, isInitial);

        std::cout << "------------------------------------------------------------------------" << std::endl;
        print_psqts_cpp(parameters, mobilityPieceValues);
        std::cout << "------------------------------------------------------------------------" << std::endl;
        print_cpp_parameters(parameters, mobilityPieceValues);
        std::cout << "------------------------------------------------------------------------" << std::endl;

        print_psqts_csharp(parameters, mobilityPieceValues);
        std::cout << "------------------------------------------------------------------------" << std::endl;
        print_json_parameters(parameters, mobilityPieceValues);
        std::cout << "------------------------------------------------------------------------" << std::endl;
        print_csharp_parameters(parameters, mobilityPieceValues);
        std::cout << "------------------------------------------------------------------------" << std::endl;
    }

    static void print_step_parameters(const parameters_t &parameters)
    {
        auto mobilityPieceValues = extract_mobility_offset(parameters, false);

        print_psqts_cpp(parameters, mobilityPieceValues);
        print_cpp_parameters(parameters, mobilityPieceValues);
    }

    static void print_json_parameters(const parameters_t &parameters, const std::array<std::array<tune_t, 12>, PSQTBucketCount> &mobilityPieceValues)
    {
        std::stringstream ss;
        std::string name;

        // name = NAME(DoubledPawnPenalty);
        // DoubledPawnPenalty.to_json(parameters, ss, name);
        // ss << ",\n";

        name = NAME(IsolatedPawnPenalty);
        IsolatedPawnPenalty.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(OpenFileRookBonus);
        OpenFileRookBonus.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(SemiOpenFileRookBonus);
        SemiOpenFileRookBonus.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(QueenMobilityBonus);
        QueenMobilityBonus.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(SemiOpenFileKingPenalty);
        SemiOpenFileKingPenalty.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(OpenFileKingPenalty);
        OpenFileKingPenalty.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(KingShieldBonus);
        KingShieldBonus.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(BishopPairBonus);
        BishopPairBonus.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(PieceProtectedByPawnBonus);
        PieceProtectedByPawnBonus.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(PieceAttackedByPawnPenalty);
        PieceAttackedByPawnPenalty.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(PassedPawnBonus);
        PassedPawnBonus.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(VirtualKingMobilityBonus);
        VirtualKingMobilityBonus.to_json(parameters, ss, name);
        ss << ",\n";

        name = NAME(KnightMobilityBonus);
        KnightMobilityBonus.to_json(parameters, ss, name, mobilityPieceValues);
        ss << ",\n";

        name = NAME(BishopMobilityBonus);
        BishopMobilityBonus.to_json(parameters, ss, name, mobilityPieceValues);
        ss << ",\n";

        name = NAME(RookMobilityBonus);
        RookMobilityBonus.to_json(parameters, ss, name, mobilityPieceValues);
        ss << ",\n";

        std::cout << ss.str() << std::endl
                  << std::endl;
    }

    static void print_csharp_parameters(const parameters_t &parameters, const std::array<std::array<tune_t, 12>, PSQTBucketCount> &mobilityPieceValues)
    {
        std::stringstream ss;
        std::string name;

        // name = NAME(DoubledPawnPenalty);
        // DoubledPawnPenalty.to_json(parameters, ss, name);

        name = NAME(IsolatedPawnPenalty);
        IsolatedPawnPenalty.to_csharp(parameters, ss, name);

        name = NAME(OpenFileRookBonus);
        OpenFileRookBonus.to_csharp(parameters, ss, name);

        name = NAME(SemiOpenFileRookBonus);
        SemiOpenFileRookBonus.to_csharp(parameters, ss, name);

        name = NAME(QueenMobilityBonus);
        QueenMobilityBonus.to_csharp(parameters, ss, name);

        name = NAME(SemiOpenFileKingPenalty);
        SemiOpenFileKingPenalty.to_csharp(parameters, ss, name);

        name = NAME(OpenFileKingPenalty);
        OpenFileKingPenalty.to_csharp(parameters, ss, name);

        name = NAME(KingShieldBonus);
        KingShieldBonus.to_csharp(parameters, ss, name);

        name = NAME(BishopPairBonus);
        BishopPairBonus.to_csharp(parameters, ss, name);

        name = NAME(PieceProtectedByPawnBonus);
        PieceProtectedByPawnBonus.to_csharp(parameters, ss, name);

        name = NAME(PieceAttackedByPawnPenalty);
        PieceAttackedByPawnPenalty.to_csharp(parameters, ss, name);

        name = NAME(PassedPawnBonus);
        PassedPawnBonus.to_csharp(parameters, ss, name);

        name = NAME(VirtualKingMobilityBonus);
        VirtualKingMobilityBonus.to_csharp(parameters, ss, name);

        name = NAME(KnightMobilityBonus);
        KnightMobilityBonus.to_csharp(parameters, ss, name, mobilityPieceValues);

        name = NAME(BishopMobilityBonus);
        BishopMobilityBonus.to_csharp(parameters, ss, name, mobilityPieceValues);

        name = NAME(RookMobilityBonus);
        RookMobilityBonus.to_csharp(parameters, ss, name, mobilityPieceValues);

        std::cout << ss.str() << std::endl;
    }

    static void print_cpp_parameters(const parameters_t &parameters, const std::array<std::array<tune_t, 12>, PSQTBucketCount> &mobilityPieceValues)
    {
        std::stringstream ss;
        std::string name;

        // name = NAME(DoubledPawnPenalty);
        // DoubledPawnPenalty.to_json(parameters, ss, name);

        name = NAME(IsolatedPawnPenalty);
        IsolatedPawnPenalty.to_cpp(parameters, ss, name);

        name = NAME(OpenFileRookBonus);
        OpenFileRookBonus.to_cpp(parameters, ss, name);

        name = NAME(SemiOpenFileRookBonus);
        SemiOpenFileRookBonus.to_cpp(parameters, ss, name);

        name = NAME(QueenMobilityBonus);
        QueenMobilityBonus.to_cpp(parameters, ss, name);

        name = NAME(SemiOpenFileKingPenalty);
        SemiOpenFileKingPenalty.to_cpp(parameters, ss, name);

        name = NAME(OpenFileKingPenalty);
        OpenFileKingPenalty.to_cpp(parameters, ss, name);

        name = NAME(KingShieldBonus);
        KingShieldBonus.to_cpp(parameters, ss, name);

        name = NAME(BishopPairBonus);
        BishopPairBonus.to_cpp(parameters, ss, name);
        ss << "\n";

        name = NAME(PieceProtectedByPawnBonus);
        PieceProtectedByPawnBonus.to_cpp(parameters, ss, name);

        name = NAME(PieceAttackedByPawnPenalty);
        PieceAttackedByPawnPenalty.to_cpp(parameters, ss, name);
        ss << "\n";

        name = NAME(PassedPawnBonus);
        PassedPawnBonus.to_cpp(parameters, ss, name);

        name = NAME(VirtualKingMobilityBonus);
        VirtualKingMobilityBonus.to_cpp(parameters, ss, name);

        name = NAME(KnightMobilityBonus);
        KnightMobilityBonus.to_cpp(parameters, ss, name, mobilityPieceValues);

        name = NAME(BishopMobilityBonus);
        BishopMobilityBonus.to_cpp(parameters, ss, name, mobilityPieceValues);

        name = NAME(RookMobilityBonus);
        RookMobilityBonus.to_cpp(parameters, ss, name, mobilityPieceValues);

        std::cout << ss.str() << std::endl;
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
    //     IncrementCoefficients(coefficients, DoubledPawnPenalty_Index, color);
    // }

    if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, color) & IsolatedPawnMasks[squareIndex]) == 0) // isIsolatedPawn
    {
        packedBonus += IsolatedPawnPenalty.packed;
        IncrementCoefficients(coefficients, IsolatedPawnPenalty.index, color);
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
            packedBonus += PassedPawnBonus.packed[rank];
            IncrementCoefficients(coefficients, PassedPawnBonus.index + rank - 1, color); // There's no coefficient for rank 0
            // std::cout << "White pawn on " << squareIndex << " is passed, bonus " << PassedPawnBonus[rank] << std::endl;
        }
    }
    else
    {

        if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) & BlackPassedPawnMasks[squareIndex]) == 0) // isPassedPawn
        {
            auto rank = Rank[squareIndex];
            rank = 7 - rank;

            packedBonus += PassedPawnBonus.packed[rank];
            IncrementCoefficients(coefficients, PassedPawnBonus.index + rank - 1, color); // There's no coefficient for rank 0
            // std::cout << "Black pawn on " << squareIndex << " is passed, bonus " << -PassedPawnBonus[rank] << std::endl;
        }
    }

    return packedBonus;
}

int RookAdditonalEvaluation(int squareIndex, int pieceIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = chess::builtin::popcount(
        chess::attacks::rook(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ().getBits())).getBits() &
        (~__builtin_bswap64(board.us(color).getBits())));

    IncrementCoefficients(coefficients, RookMobilityBonus.index + mobilityCount, color);

    int packedBonus = RookMobilityBonus.packed[mobilityCount];

    if (((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) | GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
    {
        // std::cout << "OpenFileRookBonus" << std::endl;
        IncrementCoefficients(coefficients, OpenFileRookBonus.index, color);
        packedBonus += OpenFileRookBonus.packed;
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
                IncrementCoefficients(coefficients, SemiOpenFileRookBonus.index, color);
                packedBonus += SemiOpenFileRookBonus.packed;
            }
        }
        else
        {
            if ((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
            {
                // std::cout << "SemiOpenFileRookBonus black" << std::endl;
                IncrementCoefficients(coefficients, SemiOpenFileRookBonus.index, color);
                packedBonus += SemiOpenFileRookBonus.packed;
            }
        }
    }

    return packedBonus;
}

int KnightAdditionalEvaluation(int squareIndex, int pieceIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = chess::builtin::popcount(
        chess::attacks::knight(static_cast<chess::Square>(squareIndex)).getBits() &
        (~__builtin_bswap64(board.us(color).getBits())));

    IncrementCoefficients(coefficients, KnightMobilityBonus.index + mobilityCount, color);

    return KnightMobilityBonus.packed[mobilityCount];
}

int BishopAdditionalEvaluation(int squareIndex, int pieceIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = chess::attacks::bishop(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ().getBits())).count();
    IncrementCoefficients(coefficients, BishopMobilityBonus.index + mobilityCount, color);

    return BishopMobilityBonus.packed[mobilityCount];
}

int QueenAdditionalEvaluation(int squareIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = chess::attacks::queen(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ().getBits())).count();
    IncrementCoefficients(coefficients, QueenMobilityBonus.index, color, mobilityCount);

    return QueenMobilityBonus.packed * mobilityCount;
}

int KingAdditionalEvaluation(int squareIndex, chess::Color kingSide, const chess::Board &board, const int pieceCount[], coefficients_t &coefficients)
{
    auto mobilityCount = chess::attacks::queen(static_cast<chess::Square>(squareIndex), __builtin_bswap64(board.occ().getBits())).count();
    IncrementCoefficients(coefficients, VirtualKingMobilityBonus.index + mobilityCount, kingSide);

    int packedBonus = VirtualKingMobilityBonus.packed[mobilityCount];

    auto kingSideOffset = kingSide == chess::Color::WHITE ? 0 : 6;

    if (pieceCount[9 - kingSideOffset] + pieceCount[10 - kingSideOffset] != 0) // areThereOppositeSideRooksOrQueens
    {
        if (((GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) | GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
        {
            // std::cout << "Open: " << (kingSide == chess::Color::WHITE ? "White" : "Black") << std::endl;
            packedBonus += OpenFileKingPenalty.packed;
            IncrementCoefficients(coefficients, OpenFileKingPenalty.index, kingSide);
        }
        else if (kingSide == chess::Color::WHITE && (GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: " << (kingSide == chess::Color::WHITE ? "White" : "Black") << std::endl;
            packedBonus += SemiOpenFileKingPenalty.packed;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenalty.index, kingSide);
        }
        else if (kingSide == chess::Color::BLACK && (GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            // std::cout << "Semiopen: "  << (kingSide == chess::Color::WHITE ? "White" : "Black") << std::endl;
            packedBonus += SemiOpenFileKingPenalty.packed;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenalty.index, kingSide);
        }
    }

    auto ownPawnsAroundCount = chess::builtin::popcount(
        chess::attacks::king(static_cast<chess::Square>(squareIndex)).getBits() &
        GetPieceSwappingEndianness(board, chess::PieceType::PAWN, kingSide));

    IncrementCoefficients(coefficients, KingShieldBonus.index, kingSide, ownPawnsAroundCount);

    return packedBonus + KingShieldBonus.packed * ownPawnsAroundCount;
}

int AdditionalPieceEvaluation(int pieceSquareIndex, int pieceIndex, const chess::Board &board, const chess::Color &color, coefficients_t &coefficients)
{
    switch (pieceIndex)
    {
    case 0:
    case 6:
        return PawnAdditionalEvaluation(pieceSquareIndex, pieceIndex, board, color, coefficients);

    case 1:
    case 7:
        return KnightAdditionalEvaluation(pieceSquareIndex, pieceIndex, board, color, coefficients);

    case 3:
    case 9:
        return RookAdditonalEvaluation(pieceSquareIndex, pieceIndex, board, color, coefficients);

    case 2:
    case 8:
        return BishopAdditionalEvaluation(pieceSquareIndex, pieceIndex, board, color, coefficients);

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

    auto whitePawns = GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE);
    auto whitePawnAttacks = ShiftUpLeft(whitePawns) | ShiftUpRight(whitePawns);

    auto blackPawns = GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK);
    auto blackPawnAttacks = ShiftDownLeft(blackPawns) | ShiftDownRight(blackPawns);

    auto whiteKing = chess::builtin::lsb(GetPieceSwappingEndianness(board, chess::PieceType::KING, chess::Color::WHITE)).index();
    auto blackKing = chess::builtin::lsb(GetPieceSwappingEndianness(board, chess::PieceType::KING, chess::Color::BLACK)).index();

    auto whiteBucket = File[whiteKing] / 4;
    auto blackBucket = File[blackKing] / 4;

    for (int pieceIndex = 0; pieceIndex < 5; ++pieceIndex)
    {
        // Bitboard copy that we 'empty'
        auto bitboard = GetPieceSwappingEndianness(board, chess::PieceType(static_cast<chess::PieceType::underlying>(pieceIndex)), chess::Color::WHITE);
        // std::cout << pieceIndex << "bb: " << bitboard << std::endl;
        while (bitboard != 0)
        {
            auto pieceSquareIndex = chess::builtin::lsb(bitboard).index();
            ResetLS1B(bitboard);

            packedScore += PackedPositionalTables(whiteBucket, pieceIndex, pieceSquareIndex) + PackedPieceValue(whiteBucket, pieceIndex);
            gamePhase += phaseValues[pieceIndex];

            ++pieceCount[pieceIndex];

            packedScore += AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, board, chess::Color::WHITE, coefficients);

            if (pieceIndex == 0)
                IncrementCoefficients(coefficients, (48 * whiteBucket) + pieceSquareIndex - 8, chess::Color::WHITE);
            else
                IncrementCoefficients(
                    coefficients,
                    (48 * PSQTBucketCount) + (64 * PSQTBucketCount * (pieceIndex - 1)) + (64 * whiteBucket) + pieceSquareIndex,
                    chess::Color::WHITE);
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

            packedScore += PackedPositionalTables(blackBucket, pieceIndex, pieceSquareIndex) - PackedPieceValue(blackBucket, tunerPieceIndex);
            gamePhase += phaseValues[tunerPieceIndex];

            ++pieceCount[pieceIndex];

            packedScore -= AdditionalPieceEvaluation(pieceSquareIndex, pieceIndex, board, chess::Color::BLACK, coefficients);

            if (pieceIndex == 6)
                IncrementCoefficients(coefficients, (48 * blackBucket) + pieceSquareIndex - 8, chess::Color::BLACK);
            else
                IncrementCoefficients(
                    coefficients,
                    (48 * PSQTBucketCount) + (64 * PSQTBucketCount * (tunerPieceIndex - 1)) + (64 * blackBucket) + pieceSquareIndex,
                    chess::Color::BLACK);
        }
    }

    auto protectedPiecesByWhitePawns = chess::builtin::popcount(whitePawnAttacks & __builtin_bswap64(board.us(chess::Color::WHITE).getBits()) /*&(~GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE))*/);
    auto protectedPiecesByBlackPawns = chess::builtin::popcount(blackPawnAttacks & __builtin_bswap64(board.us(chess::Color::BLACK).getBits()) /*&(~GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK))*/);

    IncrementCoefficients(coefficients, PieceProtectedByPawnBonus.index, chess::Color::WHITE, protectedPiecesByWhitePawns);
    IncrementCoefficients(coefficients, PieceProtectedByPawnBonus.index, chess::Color::BLACK, protectedPiecesByBlackPawns);

    packedScore += PieceProtectedByPawnBonus.packed * (protectedPiecesByWhitePawns - protectedPiecesByBlackPawns);

    auto attackedPiecesByWhitePawns = chess::builtin::popcount(blackPawnAttacks & __builtin_bswap64(board.us(chess::Color::WHITE).getBits()) /*&(~GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::WHITE))*/);
    auto attackedPiecesByBlackPawns = chess::builtin::popcount(whitePawnAttacks & __builtin_bswap64(board.us(chess::Color::BLACK).getBits()) /*&(~GetPieceSwappingEndianness(board, chess::PieceType::PAWN, chess::Color::BLACK))*/);

    IncrementCoefficients(coefficients, PieceAttackedByPawnPenalty.index, chess::Color::WHITE, attackedPiecesByWhitePawns);
    IncrementCoefficients(coefficients, PieceAttackedByPawnPenalty.index, chess::Color::BLACK, attackedPiecesByBlackPawns);

    packedScore += PieceAttackedByPawnPenalty.packed * (attackedPiecesByWhitePawns - attackedPiecesByBlackPawns);

    if (board.pieces(chess::PieceType::BISHOP, chess::Color::WHITE).count() >= 2)
    {
        packedScore += BishopPairBonus.packed;
        IncrementCoefficients(coefficients, BishopPairBonus.index, chess::Color::WHITE);
    }

    if (board.pieces(chess::PieceType::BISHOP, chess::Color::BLACK).count() >= 2)
    {
        packedScore -= BishopPairBonus.packed;
        IncrementCoefficients(coefficients, BishopPairBonus.index, chess::Color::BLACK);
    }

    packedScore += PackedPositionalTables(whiteBucket, 5, whiteKing) +
                   PackedPositionalTables(blackBucket, 11, blackKing) +
                   KingAdditionalEvaluation(whiteKing, chess::Color::WHITE, board, pieceCount, coefficients) -
                   KingAdditionalEvaluation(blackKing, chess::Color::BLACK, board, pieceCount, coefficients);

    IncrementCoefficients(
        coefficients,
        (48 * PSQTBucketCount) + (64 * PSQTBucketCount * 4) + (64 * whiteBucket) + whiteKing,
        chess::Color::WHITE);

    IncrementCoefficients(
        coefficients,
        (48 * PSQTBucketCount) + (64 * PSQTBucketCount * 4) + (64 * blackBucket) + blackKing,
        chess::Color::BLACK);

    // Debugging eval
    // return EvalResult{
    //     std::move(coefficients),
    //     (double)(board.sideToMove() == chess::Color::WHITE
    //                  ? eval
    //                  : -eval)};

    // Check if drawn position due to lack of material

    int totalPawnsCount = INT_MIN;

    if (gamePhase <= 3)
    {
        totalPawnsCount = board.pieces(chess::PieceType::PAWN, chess::Color::WHITE).count() +
                          board.pieces(chess::PieceType::PAWN, chess::Color::BLACK).count();

        if (totalPawnsCount == 0)
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

    if (gamePhase <= 3)
    {
        auto pawnScalingPenalty = 16 - totalPawnsCount;

        if (eval > 1)
        {
            pawnScalingPenalty = std::min(eval - 1, pawnScalingPenalty);
            eval -= pawnScalingPenalty;
        }
        else if (eval < -1)
        {
            pawnScalingPenalty = std::min(-eval - 1, pawnScalingPenalty);
            eval += pawnScalingPenalty;
        }
    }

    eval = std::clamp(eval, MinEval, MaxEval);

    // Always white's perspective
    return EvalResult{
        std::move(coefficients),
        (double)eval};
}
