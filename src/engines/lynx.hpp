
#include "../base.h"
#include "../external/chess.hpp"
#include <cassert>
#include <array>
#include <string>

using u64 = uint64_t;

constexpr static std::array<int, 64> MiddleGamePawnTable =
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        98, 134, 61, 95, 68, 126, 34, -11,
        -6, 7, 26, 31, 65, 56, 25, -20,
        -14, 13, 6, 21, 23, 12, 17, -23,
        -27, -2, -5, 12, 17, 6, 10, -25,
        -26, -4, -4, -10, 3, 3, 33, -12,
        -35, -1, -20, -23, -15, 24, 38, -22,
        0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> EndGamePawnTable =
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        178, 173, 158, 134, 147, 132, 165, 187,
        94, 100, 85, 67, 56, 53, 82, 84,
        32, 24, 13, 5, -2, 4, 17, 17,
        13, 9, -3, -7, -7, -8, 3, -1,
        4, 7, -6, 1, 0, -5, -1, -8,
        13, 8, 8, 10, 13, 0, 2, -7,
        0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> MiddleGameKnightTable =
    {
        -167, -89, -34, -49, 61, -97, -15, -107,
        -73, -41, 72, 36, 23, 62, 7, -17,
        -47, 60, 37, 65, 84, 129, 73, 44,
        -9, 17, 19, 53, 37, 69, 18, 22,
        -13, 4, 16, 13, 28, 19, 21, -8,
        -23, -9, 12, 10, 19, 17, 25, -16,
        -29, -53, -12, -3, -1, 18, -14, -19,
        -105, -21, -58, -33, -17, -28, -19, -23};

constexpr static std::array<int, 64> EndGameKnightTable =
    {
        -58, -38, -13, -28, -31, -27, -63, -99,
        -25, -8, -25, -2, -9, -25, -24, -52,
        -24, -20, 10, 9, -1, -9, -19, -41,
        -17, 3, 22, 22, 22, 11, 8, -18,
        -18, -6, 16, 25, 16, 17, 4, -18,
        -23, -3, -1, 15, 10, -3, -20, -22,
        -42, -20, -10, -5, -2, -20, -23, -44,
        -29, -51, -23, -15, -22, -18, -50, -64};

constexpr static std::array<int, 64> MiddleGameBishopTable =
    {
        -29, 4, -82, -37, -25, -42, 7, -8,
        -26, 16, -18, -13, 30, 59, 18, -47,
        -16, 37, 43, 40, 35, 50, 37, -2,
        -4, 5, 19, 50, 37, 37, 7, -2,
        -6, 13, 13, 26, 34, 12, 10, 4,
        0, 15, 15, 15, 14, 27, 18, 10,
        4, 15, 16, 0, 7, 21, 33, 1,
        -33, -3, -14, -21, -13, -12, -39, -21};

constexpr static std::array<int, 64> EndGameBishopTable =
    {
        -14, -21, -11, -8, -7, -9, -17, -24,
        -8, -4, 7, -12, -3, -13, -4, -14,
        2, -8, 0, -1, -2, 6, 0, 4,
        -3, 9, 12, 9, 14, 10, 3, 2,
        -6, 3, 13, 19, 7, 10, -3, -9,
        -12, -3, 8, 10, 13, 3, -7, -15,
        -14, -18, -7, -1, 4, -9, -15, -27,
        -23, -9, -23, -5, -9, -16, -5, -17};

constexpr static std::array<int, 64> MiddleGameRookTable =
    {
        32, 42, 32, 51, 63, 9, 31, 43,
        27, 32, 58, 62, 80, 67, 26, 44,
        -5, 19, 26, 36, 17, 45, 61, 16,
        -24, -11, 7, 26, 24, 35, -8, -20,
        -36, -26, -12, -1, 9, -7, 6, -23,
        -45, -25, -16, -17, 3, 0, -5, -33,
        -44, -16, -20, -9, -1, 11, -6, -71,
        -19, -13, 1, 17, 16, 7, -37, -26};

constexpr static std::array<int, 64> EndGameRookTable =
    {
        13, 10, 18, 15, 12, 12, 8, 5,
        11, 13, 13, 11, -3, 3, 8, 3,
        7, 7, 7, 5, 4, -3, -5, -3,
        4, 3, 13, 1, 2, 1, -1, 2,
        3, 5, 8, 4, -5, -6, -8, -11,
        -4, 0, -5, -1, -7, -12, -8, -16,
        -6, -6, 0, 2, -9, -9, -11, -3,
        -9, 2, 3, -1, -5, -13, 4, -20};

constexpr static std::array<int, 64> MiddleGameQueenTable =
    {
        -28, 0, 29, 12, 59, 44, 43, 45,
        -24, -39, -5, 1, -16, 57, 28, 54,
        -13, -17, 7, 8, 29, 56, 47, 57,
        -27, -27, -16, -16, -1, 17, -2, 1,
        -9, -26, -9, -10, -2, -4, 3, -3,
        -14, 2, -11, -2, -5, 2, 14, 5,
        -35, -8, 11, 2, 8, 15, -3, 1,
        -1, -18, -9, 10, -15, -25, -31, -50};

constexpr static std::array<int, 64> EndGameQueenTable =
    {
        -9, 22, 22, 27, 27, 19, 10, 20,
        -17, 20, 32, 41, 58, 25, 30, 0,
        -20, 6, 9, 49, 47, 35, 19, 9,
        3, 22, 24, 45, 57, 40, 57, 36,
        -18, 28, 19, 47, 31, 34, 39, 23,
        -16, -27, 15, 6, 9, 17, 10, 5,
        -22, -23, -30, -16, -16, -23, -36, -32,
        -33, -28, -22, -43, -5, -32, -20, -41};

constexpr static std::array<int, 64> MiddleGameKingTable =
    {
        -65, 23, 16, -15, -56, -34, 2, 13,
        29, -1, -20, -7, -8, -4, -38, -29,
        -9, 24, 2, -16, -20, 6, 22, -22,
        -17, -20, -12, -27, -30, -25, -14, -36,
        -49, -1, -27, -39, -46, -44, -33, -51,
        -14, -14, -22, -46, -44, -30, -15, -27,
        1, 7, -8, -64, -43, -16, 9, 8,
        -15, 36, 12, -54, 8, -28, 24, 14};

constexpr static std::array<int, 64> EndGameKingTable =
    {
        -74, -35, -18, -18, -11, 15, 4, -17,
        -12, 17, 14, 17, 17, 38, 23, 11,
        10, 17, 23, 15, 20, 45, 44, 13,
        -8, 22, 24, 27, 26, 33, 26, 3,
        -18, -4, 21, 24, 27, 23, 9, -11,
        -19, -3, 11, 21, 23, 16, 7, -9,
        -27, -11, 4, 13, 14, 4, -5, -17,
        -53, -34, -21, -11, -28, -14, -24, -43};

constexpr static std::array<int, 64> MiddleGamePawnTableBlack =
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        35, 1, 20, 23, 15, -24, -38, 22,
        26, 4, 4, 10, -3, -3, -33, 12,
        27, 2, 5, -12, -17, -6, -10, 25,
        14, -13, -6, -21, -23, -12, -17, 23,
        6, -7, -26, -31, -65, -56, -25, 20,
        -98, -134, -61, -95, -68, -126, -34, 11,
        0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> MiddleGameKnightTableBlack{
    105, 21, 58, 33, 17, 28, 19, 23,
    29, 53, 12, 3, 1, -18, 14, 19,
    23, 9, -12, -10, -19, -17, -25, 16,
    13, -4, -16, -13, -28, -19, -21, 8,
    9, -17, -19, -53, -37, -69, -18, -22,
    47, -60, -37, -65, -84, -129, -73, -44,
    73, 41, -72, -36, -23, -62, -7, 17,
    167, 89, 34, 49, -61, 97, 15, 107};

constexpr static std::array<int, 64> MiddleGameBishopTableBlack{
    33, 3, 14, 21, 13, 12, 39, 21,
    -4, -15, -16, 0, -7, -21, -33, -1,
    0, -15, -15, -15, -14, -27, -18, -10,
    6, -13, -13, -26, -34, -12, -10, -4,
    4, -5, -19, -50, -37, -37, -7, 2,
    16, -37, -43, -40, -35, -50, -37, 2,
    26, -16, 18, 13, -30, -59, -18, 47,
    29, -4, 82, 37, 25, 42, -7, 8};

constexpr static std::array<int, 64> MiddleGameRookTableBlack{
    19, 13, -1, -17, -16, -7, 37, 26,
    44, 16, 20, 9, 1, -11, 6, 71,
    45, 25, 16, 17, -3, 0, 5, 33,
    36, 26, 12, 1, -9, 7, -6, 23,
    24, 11, -7, -26, -24, -35, 8, 20,
    5, -19, -26, -36, -17, -45, -61, -16,
    -27, -32, -58, -62, -80, -67, -26, -44,
    -32, -42, -32, -51, -63, -9, -31, -43};

constexpr static std::array<int, 64> MiddleGameQueenTableBlack{
    1, 18, 9, -10, 15, 25, 31, 50,
    35, 8, -11, -2, -8, -15, 3, -1,
    14, -2, 11, 2, 5, -2, -14, -5,
    9, 26, 9, 10, 2, 4, -3, 3,
    27, 27, 16, 16, 1, -17, 2, -1,
    13, 17, -7, -8, -29, -56, -47, -57,
    24, 39, 5, -1, 16, -57, -28, -54,
    28, 0, -29, -12, -59, -44, -43, -45};

constexpr static std::array<int, 64> MiddleGameKingTableBlack{
    15, -36, -12, 54, -8, 28, -24, -14,
    -1, -7, 8, 64, 43, 16, -9, -8,
    14, 14, 22, 46, 44, 30, 15, 27,
    49, 1, 27, 39, 46, 44, 33, 51,
    17, 20, 12, 27, 30, 25, 14, 36,
    9, -24, -2, 16, 20, -6, -22, 22,
    -29, 1, 20, 7, 8, 4, 38, 29,
    65, -23, -16, 15, 56, 34, -2, -13};

constexpr static std::array<int, 64> EndGamePawnTableBlack =
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        -13, -8, -8, -10, -13, 0, -2, 7,
        -4, -7, 6, -1, 0, 5, 1, 8,
        -13, -9, 3, 7, 7, 8, -3, 1,
        -32, -24, -13, -5, 2, -4, -17, -17,
        -94, -100, -85, -67, -56, -53, -82, -84,
        -178, -173, -158, -134, -147, -132, -165, -187,
        0, 0, 0, 0, 0, 0, 0, 0};

constexpr static std::array<int, 64> EndGameKnightTableBlack =
    {
        29, 51, 23, 15, 22, 18, 50, 64,
        42, 20, 10, 5, 2, 20, 23, 44,
        23, 3, 1, -15, -10, 3, 20, 22,
        18, 6, -16, -25, -16, -17, -4, 18,
        17, -3, -22, -22, -22, -11, -8, 18,
        24, 20, -10, -9, 1, 9, 19, 41,
        25, 8, 25, 2, 9, 25, 24, 52,
        58, 38, 13, 28, 31, 27, 63, 99};

constexpr static std::array<int, 64> EndGameBishopTableBlack =
    {
        23, 9, 23, 5, 9, 16, 5, 17,
        14, 18, 7, 1, -4, 9, 15, 27,
        12, 3, -8, -10, -13, -3, 7, 15,
        6, -3, -13, -19, -7, -10, 3, 9,
        3, -9, -12, -9, -14, -10, -3, -2,
        -2, 8, 0, 1, 2, -6, 0, -4,
        8, 4, -7, 12, 3, 13, 4, 14,
        14, 21, 11, 8, 7, 9, 17, 24};

constexpr static std::array<int, 64> EndGameRookTableBlack =
    {
        9, -2, -3, 1, 5, 13, -4, 20,
        6, 6, 0, -2, 9, 9, 11, 3,
        4, 0, 5, 1, 7, 12, 8, 16,
        -3, -5, -8, -4, 5, 6, 8, 11,
        -4, -3, -13, -1, -2, -1, 1, -2,
        -7, -7, -7, -5, -4, 3, 5, 3,
        -11, -13, -13, -11, 3, -3, -8, -3,
        -13, -10, -18, -15, -12, -12, -8, -5};

constexpr static std::array<int, 64> EndGameQueenTableBlack =
    {
        33, 28, 22, 43, 5, 32, 20, 41,
        22, 23, 30, 16, 16, 23, 36, 32,
        16, 27, -15, -6, -9, -17, -10, -5,
        18, -28, -19, -47, -31, -34, -39, -23,
        -3, -22, -24, -45, -57, -40, -57, -36,
        20, -6, -9, -49, -47, -35, -19, -9,
        17, -20, -32, -41, -58, -25, -30, 0,
        9, -22, -22, -27, -27, -19, -10, -20};

constexpr static std::array<int, 64> EndGameKingTableBlack =
    {
        53, 34, 21, 11, 28, 14, 24, 43,
        27, 11, -4, -13, -14, -4, 5, 17,
        19, 3, -11, -21, -23, -16, -7, 9,
        18, 4, -21, -24, -27, -23, -9, 11,
        8, -22, -24, -27, -26, -33, -26, -3,
        -10, -17, -23, -15, -20, -45, -44, -13,
        12, -17, -14, -17, -17, -38, -23, -11,
        74, 35, 18, 18, 11, -15, -4, 17};

constexpr static std::array<std::array<int, 64>, 12> MiddleGamePositionalTables =
    {
        MiddleGamePawnTable,
        MiddleGameKnightTable,
        MiddleGameBishopTable,
        MiddleGameRookTable,
        MiddleGameQueenTable,
        MiddleGameKingTable,

        MiddleGamePawnTableBlack,
        MiddleGameKnightTableBlack,
        MiddleGameBishopTableBlack,
        MiddleGameRookTableBlack,
        MiddleGameQueenTableBlack,
        MiddleGameKingTableBlack};

constexpr static std::array<std::array<int, 64>, 12> EndGamePositionalTables{
    EndGamePawnTable,
    EndGameKnightTable,
    EndGameBishopTable,
    EndGameRookTable,
    EndGameQueenTable,
    EndGameKingTable,

    EndGamePawnTableBlack,
    EndGameKnightTableBlack,
    EndGameBishopTableBlack,
    EndGameRookTableBlack,
    EndGameQueenTableBlack,
    EndGameKingTableBlack};

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
    847736400446211UL, 1978051601041159UL, 3956103202082318UL, 7912206404164636UL, 15824412808329272UL, 31648825616658544UL, 63297651233317088UL, 54255129628557504};
constexpr static std::array<u64, 64> BlackPassedPawnMasks = {
    217020518514230016UL, 506381209866536704UL, 1012762419733073408UL, 2025524839466146816UL, 4051049678932293632UL, 8102099357864587264UL, 16204198715729174528UL, 13889313184910721024UL,
    217020518514229248UL, 506381209866534912UL, 1012762419733069824UL, 2025524839466139648UL, 4051049678932279296UL, 8102099357864558592UL, 16204198715729117184UL, 13889313184910671872UL,
    217020518514032640UL, 506381209866076160UL, 1012762419732152320UL, 2025524839464304640UL, 4051049678928609280UL, 8102099357857218560UL, 16204198715714437120UL, 13889313184898088960UL,
    217020518463700992UL, 506381209748635648UL, 1012762419497271296UL, 2025524838994542592UL, 4051049677989085184UL, 8102099355978170368UL, 16204198711956340736UL, 13889313181676863488UL,
    217020505578799104UL, 506381179683864576UL, 1012762359367729152UL, 2025524718735458304UL, 4051049437470916608UL, 8102098874941833216UL, 16204197749883666432UL, 13889312357043142656UL,
    217017207043915776UL, 506373483102470144UL, 1012746966204940288UL, 2025493932409880576UL, 4050987864819761152UL, 8101975729639522304UL, 16203951459279044608UL, 13889101250810609664UL,
    216172782113783808UL, 504403158265495552UL, 1008806316530991104UL, 2017612633061982208UL, 4035225266123964416UL, 8070450532247928832UL, 16140901064495857664UL, 13835058055282163712UL,
    0, 0, 0, 0, 0, 0, 0, 0};
constexpr static std::array<int, 64> Rank =
    {
        7, 7, 7, 7, 7, 7, 7, 7,
        6, 6, 6, 6, 6, 6, 6, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        4, 4, 4, 4, 4, 4, 4, 4,
        3, 3, 3, 3, 3, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0};

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
                    std::cout << parameters[piece * 64 + square][phase] << ", ";
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
            std::cout << parameters[64 * 6 + piece][0] << ", ";
        }
        std::cout << "0.0, \n"; // King

        // Piece values eg
        std::cout << "// Piece values eg" << std::endl;
        for (int piece = 0; piece < 5; ++piece)
        {
            std::cout << parameters[64 * 6 + piece][1] << ", ";
        }
        std::cout << "0.0 };\n"; // King

        auto base = 64 * 6 + 4;

        std::cout << "// DoubledPawnPenalty" << std::endl;
        std::cout << "[" << parameters[DoubledPawnPenaltyIndex][0] << ", " << parameters[DoubledPawnPenaltyIndex][1] << "], " << std::endl;

        std::cout << "// IsolatedPawnPenalty" << std::endl;
        std::cout << "[" << parameters[IsolatedPawnPenaltyIndex][0] << ", " << parameters[IsolatedPawnPenaltyIndex][1] << "], " << std::endl;

        std::cout << "// OpenFileRookBonus" << std::endl;
        std::cout << "[" << parameters[OpenFileRookBonusIndex][0] << ", " << parameters[OpenFileRookBonusIndex][1] << "], " << std::endl;

        std::cout << "// SemiOpenFileRookBonus" << std::endl;
        std::cout << "[" << parameters[SemiOpenFileRookBonusIndex][0] << ", " << parameters[SemiOpenFileRookBonusIndex][1] << "], " << std::endl;

        std::cout << "// BishopMobilityBonus" << std::endl;
        std::cout << "[" << parameters[BishopMobilityBonusIndex][0] << ", " << parameters[BishopMobilityBonusIndex][1] << "], " << std::endl;

        std::cout << "// QueenMobilityBonus" << std::endl;
        std::cout << "[" << parameters[QueenMobilityBonusIndex][0] << ", " << parameters[QueenMobilityBonusIndex][1] << "], " << std::endl;

        std::cout << "// SemiOpenFileKingPenalty" << std::endl;
        std::cout << "[" << parameters[SemiOpenFileKingPenaltyIndex][0] << ", " << parameters[SemiOpenFileKingPenaltyIndex][1] << "], " << std::endl;

        std::cout << "// OpenFileKingPenalty" << std::endl;
        std::cout << "[" << parameters[OpenFileKingPenaltyIndex][0] << ", " << parameters[OpenFileKingPenaltyIndex][1] << "], " << std::endl;

        // std::cout << "// KingShieldBonus" << std::endl;
        // std::cout << "[" << parameters[KingShieldBonusIndex][0] << ", " << parameters[KingShieldBonusIndex][1] << "], " << std::endl;

        std::cout << "// BishopPairMaxBonus" << std::endl;
        std::cout << "[" << parameters[BishopPairMaxBonusIndex][0] << ", " << parameters[BishopPairMaxBonusIndex][1] << "], " << std::endl;

        std::cout << "// Passed pawns" << std::endl;
        std::cout << "{ ";
        for (int rank = 0; rank < 8; ++rank)
        {
            std::cout << parameters[PassedPawnBonusStartIndex + rank][0] << ", ";
        }
        std::cout << "};\n{ ";
        for (int rank = 0; rank < 8; ++rank)
        {
            std::cout << parameters[PassedPawnBonusStartIndex + rank][1] << ", ";
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

int PawnAdditionalEvaluation(int squareIndex, int pieceIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    auto bonus = 0;
    auto doublePawnsCount = Chess::popcount(board.pieces(Chess::PieceType::PAWN, color) & (FileMasks[squareIndex]));
    if (doublePawnsCount > 1)
    {
        bonus -= doublePawnsCount * DoubledPawnPenalty;
        IncrementCoefficients(coefficients, DoubledPawnPenaltyIndex, color, DoubledPawnPenalty * doublePawnsCount);
    }

    if ((board.pieces(Chess::PieceType::PAWN, color) & IsolatedPawnMasks[squareIndex]) == 0) // isIsolatedPawn
    {
        bonus -= IsolatedPawnPenalty;
        IncrementCoefficients(coefficients, IsolatedPawnPenaltyIndex, color, IsolatedPawnPenalty);
    }

    if (color == Chess::Color::WHITE)
    {
        if ((board.pieces(Chess::PieceType::PAWN, Chess::Color::BLACK) & WhitePassedPawnMasks[squareIndex]) == 0) // isPassedPawn
        {
            auto rank = Rank[squareIndex];
            if (pieceIndex == 6)
            {
                rank = 7 - rank;
            }
            bonus += PassedPawnBonus[rank];
            IncrementCoefficients(coefficients, PassedPawnBonusStartIndex + rank, color);
        }
    }
    else
    {
        if ((board.pieces(Chess::PieceType::PAWN, Chess::Color::WHITE) & BlackPassedPawnMasks[squareIndex]) == 0) // isPassedPawn
        {
            auto rank = Rank[squareIndex];
            rank = 7 - rank;

            bonus -= PassedPawnBonus[rank];
            IncrementCoefficients(coefficients, PassedPawnBonusStartIndex + rank, color);
        }
    }

    return bonus;
}

int RookAdditonalEvaluation(int squareIndex, int pieceIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    const int pawnToRookOffset = 3;

    if (((board.pieces(Chess::PieceType::PAWN, Chess::Color::WHITE) | board.pieces(Chess::PieceType::PAWN, Chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
    {
        IncrementCoefficients(coefficients, OpenFileRookBonusIndex, color);

        return OpenFileRookBonus;
    }

    if (color == Chess::Color::WHITE)
    {
        if ((board.pieces(Chess::PieceType::ROOK, Chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            IncrementCoefficients(coefficients, SemiOpenFileRookBonusIndex, color);

            return SemiOpenFileRookBonus;
        }
    }
    else
    {
        if ((board.pieces(Chess::PieceType::ROOK, Chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            IncrementCoefficients(coefficients, SemiOpenFileRookBonusIndex, color);

            return SemiOpenFileRookBonus;
        }
    }

    return 0;
}

int BishopAdditionalEvaluation(int squareIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = Chess::popcount(Chess::Attacks::BISHOP(static_cast<Chess::Square>(squareIndex), board.occ()));
    IncrementCoefficients(coefficients, BishopMobilityBonusIndex, color, mobilityCount);

    return BishopMobilityBonus * mobilityCount;
}

int QueenAdditionalEvaluation(int squareIndex, const Chess::Board &board, const Chess::Color &color, coefficients_t &coefficients)
{
    auto mobilityCount = Chess::popcount(Chess::Attacks::QUEEN(static_cast<Chess::Square>(squareIndex), board.occ()));
    IncrementCoefficients(coefficients, QueenMobilityBonusIndex, color, mobilityCount);

    return QueenMobilityBonus * mobilityCount;
}

int KingAdditionalEvaluation(int squareIndex, Chess::Color kingSide, const Chess::Board &board, const int pieceCount[], coefficients_t &coefficients)
{
    auto bonus = 0;
    auto kingSideOffset = kingSide == Chess::Color::WHITE ? 0 : 6;

    if (pieceCount[9 - kingSideOffset] + pieceCount[10 - kingSideOffset] != 0) // areThereOppositeSideRooksOrQueens
    {
        if (((board.pieces(Chess::PieceType::PAWN, Chess::Color::WHITE) | board.pieces(Chess::PieceType::PAWN, Chess::Color::BLACK)) & FileMasks[squareIndex]) == 0) // isOpenFile
        {
            bonus -= OpenFileKingPenalty;
            IncrementCoefficients(coefficients, OpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == Chess::Color::WHITE && (board.pieces(Chess::PieceType::KING, Chess::Color::WHITE) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            bonus -= SemiOpenFileKingPenalty;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
        else if (kingSide == Chess::Color::BLACK && (board.pieces(Chess::PieceType::KING, Chess::Color::BLACK) & FileMasks[squareIndex]) == 0) // isSemiOpenFile
        {
            bonus += SemiOpenFileKingPenalty;
            IncrementCoefficients(coefficients, SemiOpenFileKingPenaltyIndex, kingSide);
        }
    }

    return bonus;

    // return bonus + KingShieldBonus *
    //                Chess::popcount(Chess::Attacks::KING(static_cast<Chess::Square>(squareIndex) & OccupancyBitBoards[(int)kingSide]));
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
        auto bitboard = board.pieces((Chess::PieceType)pieceIndex, Chess::Color::WHITE);

        while (bitboard != 0)
        {
            auto pieceSquareIndex = Chess::lsb(bitboard);
            Chess::poplsb(bitboard);
            auto rotatedPieceSquareIndex = pieceSquareIndex ^ 56;

            middleGameScore += MiddleGamePositionalTables[pieceIndex][rotatedPieceSquareIndex];
            endGameScore += EndGamePositionalTables[pieceIndex][rotatedPieceSquareIndex];
            gamePhase += phaseValues[pieceIndex];

            ++pieceCount[pieceIndex];

            eval += AdditionalPieceEvaluation(rotatedPieceSquareIndex, pieceIndex, board, Chess::Color::WHITE, coefficients);

            coefficients[pieceIndex * 64 + pieceSquareIndex] += 1;
        }
    }

    for (int pieceIndex = 6; pieceIndex < 11; ++pieceIndex)
    {
        // Bitboard copy that we 'empty'
        auto bitboard = board.pieces((Chess::PieceType)pieceIndex, Chess::Color::BLACK);

        while (bitboard != 0)
        {
            auto pieceSquareIndex = Chess::lsb(bitboard);
            Chess::poplsb(bitboard);
            auto rotatedPieceSquareIndex = pieceSquareIndex ^ 56;

            middleGameScore += MiddleGamePositionalTables[pieceIndex][rotatedPieceSquareIndex];
            endGameScore += EndGamePositionalTables[pieceIndex][rotatedPieceSquareIndex];
            gamePhase += phaseValues[pieceIndex];

            ++pieceCount[pieceIndex];

            eval -= AdditionalPieceEvaluation(rotatedPieceSquareIndex, pieceIndex, board, Chess::Color::BLACK, coefficients);

            coefficients[pieceIndex * 64 + pieceSquareIndex] -= 1;
        }
    }

    auto whiteKing = Chess::lsb(board.pieces(Chess::PieceType::KING, Chess::Color::WHITE)) ^ 56;
    middleGameScore += MiddleGamePositionalTables[5][whiteKing];
    endGameScore += EndGamePositionalTables[5][whiteKing];
    eval += KingAdditionalEvaluation(whiteKing, Chess::Color::WHITE, board, pieceCount, coefficients);

    auto blackKing = Chess::lsb(board.pieces(Chess::PieceType::KING, Chess::Color::BLACK)) ^ 56;
    middleGameScore += MiddleGamePositionalTables[11][blackKing];
    endGameScore += EndGamePositionalTables[11][blackKing];
    eval -= KingAdditionalEvaluation(blackKing, Chess::Color::BLACK, board, pieceCount, coefficients);

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
        ++coefficients[BishopPairMaxBonusIndex];
    }
    if (pieceCount[8] >= 2)
    {
        eval -= BishopPairMaxBonus * endGamePhase / 24;
        --coefficients[BishopPairMaxBonusIndex];
    }

    return EvalResult{
        std::move(coefficients),
        (double)(board.sideToMove() == Chess::Color::WHITE
                     ? eval
                     : -eval)};
}
