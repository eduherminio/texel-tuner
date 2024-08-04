// 2024-8-1 20:24:18  103
#pragma once
#include <array>
#include "lynx_tunable.hpp"

constexpr static std::array<std::array<int, 12>, PSQTBucketCount> PieceValue = {
	{
		{
			+90, +384, +354, +554, +1232,  // 0
			+133, +449, +429, +802, +1485, // 0
		},
		{
			+89, +387, +376, +556, +1231,  // 0
			+130, +474, +426, +831, +1536, // 0
		},
		{
			+103, +430, +420, +632, +1393, // 0
			+118, +489, +444, +883, +1658, // 0
		},
		{
			+85, +299, +293, +371, +872,   // 0
			+130, +360, +328, +649, +1174, // 0
		},
		{
			+87, +320, +329, +411, +966,   // 0
			+129, +396, +348, +707, +1271, // 0
		},
		{
			+94, +322, +331, +406, +908,   // 0
			+126, +403, +350, +710, +1300, // 0
		},
		{
			+96, +325, +318, +419, +941,   // 0
			+122, +409, +358, +726, +1322, // 0
		},
		{
			+106, +337, +333, +419, +947,  // 0
			+116, +400, +351, +716, +1310, // 0
		},
		{
			+101, +346, +338, +467, +1076, // 0
			+116, +411, +366, +739, +1365, // 0
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGamePawnTable = {
	{
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			-61, 31, -6, 40, -1, -74, -180, -187,
			-22, 37, 86, 64, 68, 44, 11, -30,
			45, 62, 48, 15, 23, 9, -21, -42,
			50, 44, 45, 44, 14, -13, -30, -59,
			67, 72, 36, 2, -24, -28, -63, -79,
			49, 105, 34, -7, -39, -36, -72, -71,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			31, -1, -14, 2, -114, -114, -140, -207,
			8, 23, 52, 36, 26, 99, 24, -20,
			24, 29, 28, 42, 34, 27, -4, -16,
			13, 18, 30, 43, 29, 22, -7, -31,
			22, 7, 24, 14, 12, 6, -14, -38,
			16, 21, 14, -6, -11, 4, -7, -35,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			19, 3, 6, 23, 6, -33, -191, -153,
			4, 10, 37, 43, 66, 116, 79, 52,
			-22, -12, 1, 6, 33, 35, 19, 24,
			-36, -24, -10, 6, 17, 19, 9, 9,
			-39, -38, -23, -17, -3, 8, 25, 21,
			-35, -37, -37, -26, -12, 15, 35, 2,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			12, 80, 49, 18, -18, -44, -58, -72,
			20, 39, 39, 3, -3, -22, -55, -60,
			6, 31, 27, 35, 12, -8, -37, -59,
			35, 60, 40, 23, 24, 8, -15, -48,
			47, 84, 40, 17, 11, 3, -7, -35,
			-46, -8, 23, 33, 10, -40, -80, -113,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			55, 64, 43, 17, -20, -14, -51, -73,
			50, 33, 33, 15, 12, -10, -38, -67,
			27, 21, 34, 43, 29, 1, -27, -57,
			41, 41, 36, 33, 40, 18, -16, -44,
			4, 17, 31, 49, 18, 17, 9, -43,
			-41, -56, -5, 47, -82, -49, -72, -112,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			-23, 3, 30, 8, 0, 11, -28, -68,
			-20, 5, 37, 39, 44, 12, -19, -48,
			-19, -2, 41, 52, 56, 26, -23, -50,
			-6, 15, 47, 48, 58, 23, -26, -21,
			-32, -6, 16, 38, 33, 29, 6, -35,
			-49, -41, -23, 20, -30, 11, -52, -87,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			-17, -0, 2, 1, -8, 24, 18, -27,
			-11, -13, 10, 27, 19, 20, 6, -28,
			-16, -6, 21, 40, 24, 34, 3, -36,
			-5, 0, 9, 30, 36, 43, 10, -11,
			-14, -12, 20, 14, 19, 50, -2, -29,
			-45, -66, -12, 53, 6, -46, -54, -80,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			-45, -31, -3, -23, -4, 56, 22, -24,
			-54, -39, -2, 25, 28, 37, 27, -19,
			-42, -28, 3, 20, 36, 35, 24, -18,
			-38, -21, -3, -2, 57, 48, 27, 3,
			-49, -25, 6, 14, 48, 105, -9, -20,
			-66, -69, -31, 109, 31, 19, -59, -56,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			-33, -41, -32, -22, -12, 22, 46, 10,
			-39, -42, -24, -11, 1, 9, 33, 26,
			-33, -28, -10, 3, 16, 23, 7, 9,
			-15, -12, -6, 7, 33, 40, 19, 19,
			1, 16, 28, 20, 39, 93, 69, 32,
			-51, -54, -41, 3, 13, -6, -47, -76,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGamePawnTable = {
	{
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			81, 54, 109, -54, -45, -12, -5, 49,
			44, 36, 8, -72, -76, -37, 20, 49,
			25, 13, -9, -12, -22, -7, 14, 22,
			-8, -13, -25, -23, -19, -12, -5, 18,
			-26, -39, -16, -14, 3, -11, 2, 11,
			-27, -28, -1, 33, -20, 0, 22, 26,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			6, 16, 26, -18, 9, -1, 23, 51,
			32, 30, 0, -60, -36, -15, 11, 21,
			20, 13, -6, -38, -17, -16, 10, 12,
			1, -2, -18, -17, -13, -14, -1, -1,
			-6, -5, -13, -6, -6, -6, -8, -4,
			-0, -2, 1, 17, 25, -1, -3, 9,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			26, 25, 2, -49, -66, -19, 18, 47,
			46, 40, 3, -63, -63, -40, 6, 20,
			42, 29, 3, -22, -23, -27, 2, -5,
			19, 15, -3, -8, -9, -14, -10, -19,
			12, 11, 3, 5, 8, 2, -14, -24,
			22, 19, 16, 13, 32, 17, -6, -16,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			-3, -24, -11, -8, -12, 3, 10, 26,
			1, -5, -5, -12, -12, -12, -5, 8,
			34, 9, 6, -11, -15, -16, 5, 13,
			39, 13, 7, -10, -12, -6, -0, 14,
			23, 2, -10, -18, -19, -15, -5, -3,
			33, 2, 1, -37, -16, 7, 15, 32,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			-12, -19, -10, -21, 10, 1, 16, 31,
			-9, -1, -12, -14, -15, -12, 1, 10,
			21, 18, -15, -17, -10, -8, 8, 12,
			21, 20, -8, -10, -16, -9, 6, 16,
			14, 2, -9, -30, -15, -20, -13, 6,
			23, 11, -3, -13, 14, 2, 16, 41,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			19, 0, -14, -13, 0, -3, 5, 30,
			11, 0, -13, -29, -16, -12, -6, 9,
			24, 16, -2, -29, -14, -10, 9, 17,
			29, 14, -4, -24, -14, -9, 9, 9,
			21, 0, -9, -28, -14, -17, -14, 5,
			26, 8, -5, -8, -1, -1, 12, 29,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			20, 7, 5, -5, 12, -8, -4, 9,
			5, 2, -6, -11, -8, -8, -6, 1,
			24, 14, -6, -14, -10, -6, 8, 10,
			20, 16, 1, -12, -13, -6, 7, 7,
			13, -4, -12, -22, -16, -19, -9, 2,
			26, 20, 3, -54, -6, 4, 10, 20,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			27, 14, 1, -0, 5, -5, -5, 5,
			19, 10, -2, -14, -12, -9, -10, -2,
			31, 18, 3, -11, -8, -14, 5, 5,
			32, 18, 6, -5, -14, -13, 6, 3,
			18, 3, -13, -23, -26, -21, -8, 1,
			31, 21, 6, -81, -5, -0, 1, 9,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			19, 19, 10, -5, 26, 15, -5, -15,
			13, 8, 3, 5, 10, 4, -11, -17,
			24, 13, -0, -8, -8, -6, 3, -4,
			30, 15, 4, -13, -17, -7, 8, 5,
			15, -8, -13, -29, -21, -17, -7, -5,
			26, 12, 7, -57, -45, 4, 8, 13,
			0, 0, 0, 0, 0, 0, 0, 0 //

		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameKnightTable = {
	{
		{
			-126, 75, 128, 7, -102, -43, -217, -57,
			-34, -68, 71, 27, 39, 168, -13, 11,
			-14, 44, 112, 82, 102, 90, 78, 10,
			-9, 41, 47, 110, 61, 64, 30, -14,
			-9, 15, 18, 28, 52, 28, 27, -18,
			-20, -21, 26, 26, 37, -10, -27, -71,
			-42, -55, 14, -22, -3, -16, -45, -95,
			-99, -74, -74, -59, -38, -83, -52, -37 //
		},
		{
			-179, -257, -168, -93, 120, -145, 30, -132,
			-90, -12, 46, 17, 30, 116, -59, 54,
			-68, 74, 53, 91, 130, 132, 75, -44,
			25, 28, 59, 104, 52, 87, 30, 51,
			-8, 2, 42, 25, 56, 27, 15, -4,
			-23, 2, 21, 19, 52, 16, 4, -27,
			-34, -12, 12, 14, 11, 24, -12, -30,
			-130, -11, -12, -25, -31, -20, -23, -100 //
		},
		{
			-214, -212, -88, -78, 75, -68, -41, -82,
			-22, -17, 38, 66, 28, 131, 31, 32,
			-23, 27, 68, 73, 135, 175, 80, 31,
			-14, 8, 45, 70, 40, 83, 29, 42,
			-27, -8, 24, 17, 39, 24, 31, -11,
			-54, -25, -7, 14, 25, 6, 3, -24,
			-66, -40, -24, -3, -3, 2, -13, -14,
			-67, -41, -58, -33, -23, -11, -23, -57 //
		},
		{
			8, -56, -29, -33, -49, -79, -62, -113,
			-47, -54, 6, 0, 1, 25, 12, -54,
			-36, -2, 30, 23, 41, 2, -2, -11,
			9, 24, 41, 67, 59, 22, 89, -11,
			9, 55, 75, 116, 52, 65, 37, 56,
			3, 98, 55, 123, 136, 52, 103, -10,
			13, 19, 44, 37, 41, 57, 14, -14,
			-152, -79, -149, -1, -5, -263, -266, -140 //
		},
		{
			-47, -74, -31, -10, -66, -26, -36, -111,
			-37, -92, -22, 12, 2, 14, -46, -59,
			-49, 8, 40, 37, 9, 4, -10, -26,
			12, 13, 50, 36, 70, 32, 52, -17,
			-24, 37, 89, 117, 50, 89, 33, 65,
			-113, 20, 81, 128, 105, 68, 81, 14,
			-99, 26, 69, 60, 32, 57, -32, -12,
			-233, -154, -27, 67, -38, -22, 41, -205 //
		},
		{
			-169, -1, -96, -0, -20, -21, -17, -288,
			-1, 12, 12, 38, 33, 19, 37, -33,
			-28, 29, 62, 114, 52, 41, 17, -4,
			-6, 28, 69, 89, 102, 80, 73, 4,
			22, 20, 89, 122, 115, 129, 56, 60,
			14, 0, 56, 153, 143, 68, 66, -51,
			-60, 8, 7, 23, -0, 8, -99, 43,
			-300, -63, -118, -124, -35, -160, -168, -250 //
		},
		{
			-238, -10, -53, -39, -34, -29, -8, -49,
			-43, -19, -11, 15, 25, 12, -11, -14,
			-25, 2, 18, 37, 37, 27, 17, -11,
			-9, -4, 39, 43, 56, 49, 33, 6,
			-5, 26, 46, 71, 43, 98, 38, 20,
			-15, 47, 47, 83, 123, 96, 81, 26,
			-44, -42, 9, 36, -9, 88, 25, -18,
			-219, -42, -115, -115, 25, -76, -36, -96 //
		},
		{
			-67, -55, -92, -55, -11, -16, -21, -36,
			-23, -53, 4, 3, 4, 30, -25, -15,
			-71, -47, 12, 46, 66, 69, 32, -34,
			-27, -3, 32, 41, 76, 75, 67, 39,
			2, -3, 58, 92, 100, 139, 65, 41,
			-47, 15, 40, 97, 117, 135, 97, 42,
			-76, -25, -7, 24, -7, 42, -50, 59,
			-262, -47, -125, -90, -56, -96, 35, -254 //
		},
		{
			-98, -34, -55, -32, -19, -11, -22, -69,
			-55, -42, -22, 4, 1, 11, -8, -9,
			-43, -18, -2, 24, 34, 16, 17, -22,
			-27, 9, 25, 29, 40, 28, 27, -4,
			-11, 15, 43, 64, 41, 80, 32, 38,
			-6, 27, 63, 85, 135, 146, 81, 45,
			-38, -16, 27, 42, 43, 127, 12, 20,
			-194, -109, -81, -31, -12, -66, -115, -159 //
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameKnightTable = {
	{
		{
			-76, -84, -39, -41, 47, 32, 28, -97,
			-2, -13, -45, 1, -18, -67, 16, -11,
			-14, -31, -6, 21, 45, -26, -19, 6,
			-17, 7, 57, 49, 49, 80, 37, 21,
			-1, -18, 52, 60, 63, 27, 50, 4,
			-3, 27, 5, 39, 41, 16, 15, 22,
			18, 15, -34, 22, -6, -34, -3, 19,
			-30, 10, 30, 4, -17, 30, -44, -270 //
		},
		{
			-122, 54, 38, 11, -18, 1, -0, -152,
			-3, -11, 7, 6, -1, -15, 15, -50,
			23, -4, 32, 22, 11, -0, -8, -16,
			-4, 23, 40, 35, 46, 32, 16, -3,
			3, 12, 42, 47, 45, 23, 20, -19,
			-18, 9, 11, 29, 21, 7, 3, 7,
			-27, 6, -4, 1, -4, -8, -15, -38,
			-13, -50, -11, -5, -5, -19, -35, -18 //
		},
		{
			-33, 36, 18, 4, -34, -25, -25, -138,
			-11, 7, 7, 14, 3, -36, -25, -45,
			13, 13, 39, 31, 5, -22, 0, -20,
			16, 26, 46, 54, 57, 39, 32, -15,
			7, 18, 47, 48, 53, 32, 18, 1,
			-14, 6, 19, 45, 30, 13, -7, -20,
			-17, -2, 0, 11, 6, -24, -12, -7,
			-91, -34, -16, -10, -5, -31, -29, -68 //
		},
		{
			-96, -3, -13, -31, 2, -35, -13, -33,
			-9, 16, 19, 12, 11, -5, -14, -28,
			15, -12, 19, 48, 29, 27, -1, -28,
			-4, 17, 43, 40, 45, 52, -5, -40,
			13, 11, 41, 22, 48, 34, -3, -27,
			-6, -12, -2, 7, 1, 15, -38, -4,
			-31, 15, -10, 13, -21, -17, -9, -7,
			-77, 13, 25, -3, 8, 34, 29, -85 //
		},
		{
			-42, 17, 9, -12, 3, -64, -16, -55,
			-10, 38, 2, 1, 9, -1, -15, -24,
			12, 11, 8, 34, 40, 20, 1, -12,
			8, 8, 31, 41, 39, 52, 3, -8,
			25, 22, 17, 16, 59, 26, 16, -22,
			13, 27, 15, 10, 13, 16, -15, -30,
			-2, -9, -10, -7, -10, -23, 16, -22,
			-69, 2, -10, -28, -9, -28, -64, -65 //
		},
		{
			2, -30, 11, -12, -10, -20, -19, 9,
			-21, 13, 4, -4, -17, 18, -5, -27,
			-23, 3, 2, 5, 19, 6, -5, -17,
			5, 6, 22, 17, 21, 24, -3, -1,
			-6, 25, 28, 15, 14, 15, 6, -19,
			-4, 12, 13, 5, 6, -4, -26, 7,
			-4, -5, 10, 1, 8, 1, 18, -19,
			-26, -47, 17, -6, -0, 20, 20, -47 //
		},
		{
			-12, -47, -2, 15, 8, -32, -40, -59,
			-10, 4, 11, -9, 3, 13, -9, -25,
			-21, 5, 21, 27, 24, 5, 2, -12,
			16, 17, 43, 39, 41, 32, 8, 3,
			7, 18, 31, 34, 36, 24, 18, -4,
			-20, 5, 19, 16, 4, 1, -9, -12,
			-6, 7, 1, 4, 11, -3, -6, -36,
			-94, -30, 25, 40, -20, 9, -38, -94 //
		},
		{
			-58, -37, -12, 15, -1, -2, -44, -34,
			-26, -1, 4, 11, 1, 6, -12, -19,
			-11, 22, 27, 30, 18, -6, 3, 6,
			3, 17, 36, 40, 27, 21, 14, -32,
			-7, 25, 35, 27, 20, 6, 15, 4,
			-17, 10, 18, 23, 10, -10, -10, -12,
			-17, -1, 3, 16, 14, 12, -1, -46,
			-67, -45, 2, -8, 2, 26, -51, 21 //
		},
		{
			-62, -52, -13, -11, -9, -26, -45, -54,
			-24, -4, 7, 5, 16, -16, -14, -10,
			-33, 3, 28, 38, 33, 15, -2, -14,
			1, 6, 43, 50, 57, 41, 14, -2,
			1, 24, 41, 56, 61, 47, 36, 8,
			-21, 9, 28, 29, 3, -10, 1, -31,
			-20, 14, 5, 0, 4, -31, -18, -44,
			-31, -20, 17, -10, -7, -24, 4, -89 //
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameBishopTable = {
	{
		{
			-47, -342, -130, -141, -269, -135, -70, -44,
			-57, -100, 20, -7, -65, 53, 177, -2,
			-1, 28, 33, 12, 18, 65, 10, 15,
			28, 27, 31, 37, 53, 32, 10, -11,
			17, 64, 28, 70, 16, -4, 13, 41,
			5, 60, 49, 32, 22, 10, -1, -13,
			49, 118, 71, 34, 4, 24, 0, -47,
			90, 42, 43, -18, 26, 2, 20, -96 //
		},
		{
			-80, -107, -43, -121, -154, -156, -40, -74,
			-96, -5, -13, -40, 9, -9, 13, 17,
			4, 19, 2, 44, 30, 57, 40, 45,
			20, 21, 20, 32, 24, 4, 17, -6,
			10, -4, 12, 29, 10, -1, 11, 22,
			32, 24, 22, 17, 15, 10, 3, 17,
			24, 45, 40, 15, 15, 15, 30, 12,
			53, 24, 16, -5, 15, -4, -8, 10 //
		},
		{
			-16, -46, -74, -94, -60, -55, -15, 13,
			-40, 2, 11, -20, 11, 15, -13, -33,
			-3, 5, 20, 30, 25, 94, 58, 34,
			-8, -6, 7, 33, 17, 20, 8, -6,
			-15, -11, -16, 7, 8, -9, -15, 12,
			2, -7, -2, -10, -3, -5, 3, 25,
			10, 8, 8, -9, -7, 10, 34, 19,
			16, 20, 3, -27, -15, 11, 24, 19 //
		},
		{
			44, -43, 15, 15, 7, -19, -3, -8,
			20, 71, 43, -8, -0, 3, -14, -25,
			56, 22, 21, 26, 0, -17, -12, -24,
			5, -0, 24, 77, 41, -18, 3, 13,
			-59, 47, 27, 69, 50, 32, 8, 1,
			-34, 54, 5, 23, 38, 42, -38, 52,
			-24, -94, -48, -43, -28, 57, -46, 34,
			-106, 91, -22, 22, -127, -41, -117, -141 //
		},
		{
			-23, 4, 19, -19, -28, 1, -48, -27,
			57, 62, 46, -7, -1, 21, -6, 17,
			16, 10, 35, 22, -4, -10, -6, -10,
			-1, 7, 18, 47, 42, -29, -18, -14,
			-17, 11, 38, 33, 26, 47, -3, 30,
			-25, -35, 27, 37, 26, 57, 45, 18,
			-75, -56, -48, -57, -13, 63, -7, -20,
			-141, 106, -19, -134, -60, 14, -18, -25 //
		},
		{
			-43, 36, -12, -6, -11, 5, 1, -13,
			-51, 8, 5, 38, 28, 15, 21, -7,
			-45, 3, 20, 49, 49, 13, 7, -1,
			-45, -4, 46, 73, 48, 28, 21, -9,
			8, 24, 51, 86, 62, 55, 40, 1,
			-3, 19, 19, 95, 35, 33, 13, 72,
			-116, -53, -1, -4, -21, -34, -33, 64,
			-25, -91, -66, -98, -87, -84, -36, -191 //
		},
		{
			17, 14, -3, -6, 54, -3, -46, -3,
			43, 25, 25, -3, 19, 17, 37, 9,
			2, 10, 7, 21, 4, 13, 2, 14,
			20, -19, 26, 23, 41, -9, -4, 13,
			-17, 16, 11, 45, 11, 21, 8, -3,
			-23, 28, 1, 33, 51, 53, 14, 34,
			-59, -17, -25, -39, 8, 2, -5, -22,
			-84, -51, -74, -90, -120, -17, 25, -75 //
		},
		{
			-39, -47, -4, -15, 11, -8, -24, -24,
			-2, -4, 24, -9, 8, 12, 32, -15,
			-38, 21, 20, 32, 22, 49, 18, 25,
			-15, -21, 25, 44, 79, 20, 3, -4,
			-20, 16, 6, 65, 53, 69, 17, 43,
			-38, 19, 22, -1, 47, 56, 21, 29,
			-31, -18, -62, -2, -50, -24, -23, -7,
			-19, -49, -20, -23, -70, -26, -58, -98 //
		},
		{
			-2, 29, -5, -5, -6, 22, 16, 39,
			22, 0, 10, -17, 5, 11, 49, 9,
			-4, 4, -12, 1, -14, 5, 1, 35,
			-10, -12, -7, 21, 29, -9, -3, 9,
			-14, 2, 3, 44, 22, 30, 2, -5,
			3, 11, 24, 24, 21, 75, 58, 28,
			-14, -0, -18, -64, 6, 8, 6, -15,
			-40, -58, -35, -94, -76, -52, -64, -28 //
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameBishopTable = {
	{
		{
			40, 107, 54, 31, 119, 28, 31, 30,
			-20, 63, -37, -5, 3, -24, -40, 3,
			19, 10, 7, 15, -35, -4, 33, 25,
			1, -10, 11, 16, -12, 5, 2, 40,
			22, -24, -11, -35, 7, 6, -13, -23,
			-6, -48, -6, -22, -8, -24, -23, -6,
			-54, -33, -34, -28, -12, -55, 3, 23,
			-1, -21, -11, -10, -32, -15, -21, 10 //
		},
		{
			37, 33, 5, 40, 42, 20, 23, 15,
			26, 2, 4, 3, -7, 11, -4, -13,
			24, 13, 8, -11, -29, -5, 6, 5,
			0, 2, -9, -0, -6, 17, -8, 30,
			-14, 12, -8, -8, -8, 2, -3, -18,
			9, -14, -7, -6, -6, -13, -8, -22,
			-10, -21, -22, -12, -15, -19, -19, -3,
			-0, -15, -13, 6, -11, -4, 8, -15 //
		},
		{
			11, 16, 18, 45, 21, 19, 1, -20,
			15, -4, -4, 1, -7, -12, -2, -8,
			18, 8, -9, -22, -5, -14, -2, 29,
			18, 9, 2, 11, 7, -2, 2, 12,
			5, 5, 13, 9, -1, -3, -3, -10,
			7, 4, 2, 5, 6, -5, -11, -1,
			-1, -11, -25, -1, -0, -4, -6, -16,
			-9, -28, -8, -2, 12, -6, -33, -37 //
		},
		{
			-14, 5, -11, -7, 0, -6, 5, 5,
			19, -25, -19, -7, -5, -30, -5, -11,
			-3, -2, -14, -12, 2, 10, -2, 8,
			18, -1, -13, -14, -18, 1, 0, 11,
			22, -6, -12, -5, -10, -18, 9, 15,
			40, -22, -10, -22, -19, 2, 22, -10,
			-1, -4, 14, -5, 7, -31, 10, -7,
			44, -15, 22, 20, 18, 4, 5, 74 //
		},
		{
			9, 11, -16, 6, 8, 1, 12, 19,
			46, -28, -24, -4, -4, -23, 4, -13,
			21, -1, -21, -16, -7, -12, -11, 1,
			23, -10, -11, -17, -26, 0, 12, 7,
			20, -5, -24, -9, -13, -23, -3, 2,
			55, 0, -7, -26, -13, -11, -15, -10,
			34, 10, -2, 6, -4, -18, -11, 13,
			30, 11, 6, 33, 23, -11, 11, 20 //
		},
		{
			41, 10, 8, -5, -3, 5, -14, 1,
			37, -4, 7, -26, -17, -4, -9, -6,
			28, 12, -15, -23, -22, 3, -14, 14,
			32, -9, -11, -22, -17, -16, -11, 7,
			8, -1, -26, -30, -26, -22, -17, 14,
			24, -3, -9, -43, -23, -14, -12, -5,
			25, 23, -9, -14, -3, 15, 1, 5,
			10, 27, 29, 18, 22, 12, 18, 48 //
		},
		{
			-17, 47, -7, 10, -22, -6, 3, -7,
			6, -12, -4, -11, -11, -15, -16, 3,
			5, -0, -4, -13, -11, -5, -1, 10,
			8, -3, -13, -11, -7, -8, 6, 7,
			9, -2, -16, -11, -18, -11, -0, 15,
			24, -11, -5, -35, -12, -20, 3, 8,
			20, 9, 1, 16, -14, 6, 9, 16,
			29, 6, 25, 19, 11, 12, -1, 17 //
		},
		{
			17, 58, -6, 9, -10, 13, 4, -2,
			13, -3, -11, -2, -5, -26, -11, -1,
			20, -0, -6, -12, -14, -22, -3, 2,
			8, 3, -9, -10, -18, -18, 2, 9,
			17, -1, -7, -19, -22, -10, -3, 7,
			16, -3, -12, -9, -29, -23, 7, 12,
			-4, 2, 17, 6, 6, -7, 3, 12,
			-1, 15, 8, 4, 21, 9, 9, 8 //
		},
		{
			-12, -7, -11, -5, -4, -9, -13, -43,
			-2, -9, -12, -1, -1, -18, -9, -10,
			4, 5, 8, 4, 18, -0, -9, -1,
			6, -6, 11, 3, 0, -1, -1, -14,
			0, 2, -2, 9, 2, -1, 3, 20,
			5, -1, -11, -17, -2, -19, -8, 30,
			6, -3, -3, 18, -12, -12, -17, -6,
			25, 26, 15, 19, 31, 13, 21, 11 //
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameRookTable = {
	{
		{
			-29, -83, -12, -7, -38, 49, 4, -4,
			54, 38, 68, 68, -25, 78, 12, 56,
			29, -8, -27, 10, 25, -23, 128, 40,
			13, 31, 34, 10, -6, -13, 50, -2,
			-13, 23, 11, -5, -36, -70, -36, -19,
			-18, -3, 21, -20, -22, -2, -31, -32,
			-31, 17, -17, -13, -10, -27, -26, -35,
			-58, -11, -12, 2, -12, -14, 1, -21 //
		},
		{
			-21, -95, 8, -23, -33, 37, 67, 58,
			10, 6, 15, 30, 34, 47, 32, 67,
			8, -9, 9, 8, 59, 41, 79, 48,
			-37, -16, -12, -35, 14, 28, 32, 20,
			-13, -24, -27, -22, -1, -31, -18, -29,
			-27, -21, 8, -29, -26, -14, 7, -26,
			-22, -21, -16, -14, -14, -19, 3, -30,
			-11, -17, -16, -12, 5, -13, 5, 14 //
		},
		{
			8, 20, 1, 21, 30, 50, 44, 49,
			-19, -17, 14, 22, 3, 77, 106, 123,
			-33, -5, -6, -11, 39, 73, 158, 105,
			-45, -41, -39, -16, -21, 13, 25, 13,
			-56, -55, -47, -38, -35, -27, 12, -16,
			-60, -58, -53, -29, -29, -13, 32, 9,
			-55, -56, -29, -26, -15, -5, 37, 10,
			-30, -27, -26, -14, -7, -9, 8, -32 //
		},
		{
			-88, 1, -1, 11, 1, 7, 15, 6,
			-22, 31, 36, -16, -10, -8, -4, -48,
			-30, -7, -10, -16, -41, -40, -18, -51,
			-69, 37, -22, -20, -23, -19, -34, -23,
			-49, -20, -21, 30, -10, 15, -6, -9,
			17, -34, 62, 15, 65, 43, 65, 57,
			79, 119, 115, 24, 16, 61, 40, 30,
			-74, 26, -26, -50, -47, -12, -17, -27 //
		},
		{
			-71, -38, -60, 2, 6, 2, 19, -13,
			-19, -9, -36, -6, 12, -31, -11, -44,
			-12, -43, -60, -13, -60, -52, -12, -39,
			4, -33, -15, -0, -32, -24, -10, -30,
			19, 0, 7, 12, -31, 39, 12, -4,
			114, 42, 47, 38, 37, 50, 61, 32,
			39, -7, 84, 46, 78, 70, 20, 22,
			2, -39, -7, 29, -24, 4, -20, -48 //
		},
		{
			-38, -33, -25, -44, 3, 11, 20, 5,
			-40, -72, -12, 5, -6, 27, -22, -47,
			-7, -15, -40, 37, 7, -35, -5, -43,
			-50, -10, -54, 40, 2, -9, 28, -14,
			29, -32, 54, 34, 24, 34, -46, 46,
			-30, 35, -2, 94, -6, 9, 73, -26,
			19, 23, 1, 62, -2, -18, 66, 92,
			24, -67, -52, -18, -17, 21, 26, -13 //
		},
		{
			40, 2, 1, -7, -8, -38, -1, 15,
			-19, -4, -3, -20, 35, -7, 8, -25,
			-15, -2, -38, -22, 48, -16, -16, -21,
			-34, -19, -39, -15, 24, -3, -16, -19,
			-27, -8, -34, -18, 38, 36, 8, -23,
			-1, 26, -40, 13, 75, 56, 69, 24,
			10, -41, -7, 20, 7, 25, -4, 58,
			3, -26, -1, 6, -19, -10, -12, 29 //
		},
		{
			-20, -19, -20, -10, -2, -29, -2, -14,
			-78, -66, 9, -3, 3, 1, -4, -3,
			-56, -33, -25, -25, -14, 19, 7, 34,
			-79, -45, -32, 2, 40, 5, 17, -12,
			-51, -20, -13, 21, -9, 48, 10, 27,
			-10, -0, 15, 13, 55, 47, 23, 61,
			-23, -4, 37, 50, 26, 52, 47, 100,
			-23, -25, -9, 17, 41, -29, -43, 17 //
		},
		{
			-21, -20, -16, -9, -1, 2, 9, -53,
			-48, -37, -29, -20, -8, 6, 40, -9,
			-59, -56, -48, -37, -22, -19, 38, 8,
			-52, -57, -45, -35, -20, -24, 28, -6,
			-30, -29, -22, 2, -3, 16, 48, 50,
			-21, -1, -7, 1, 43, 80, 125, 67,
			-16, -19, 14, 36, 20, 72, 98, 111,
			-24, -14, -6, -11, 17, 9, 14, -2 //
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameRookTable = {
	{
		{
			14, 60, 44, 23, 31, -9, 16, 13,
			-45, 14, 13, -15, 48, -18, 8, -21,
			-33, 7, 30, -2, -14, -4, -33, -11,
			-21, -13, 4, 8, -11, 0, -23, -9,
			-1, 3, -8, 2, 20, 12, -18, -12,
			-19, 20, -12, 13, -4, -26, -20, -26,
			-11, -3, 14, 17, 3, -16, 7, -8,
			18, -4, 20, 7, -0, -12, -14, 7 //
		},
		{
			24, 54, 35, 35, 43, 9, 9, 11,
			13, 29, 30, 24, 14, -7, 5, -6,
			1, 10, 7, -6, -17, -15, -21, -21,
			18, 10, 25, 18, -8, -13, -14, -20,
			13, 6, 15, 13, 1, -5, -7, -1,
			7, 4, -14, -7, -6, -7, -31, -14,
			-17, -14, -6, -18, -15, -16, -38, -29,
			-7, -6, 5, -2, -9, -17, -25, -28 //
		},
		{
			12, 16, 26, 15, 13, 15, 30, 6,
			28, 33, 31, 23, 25, -3, -16, -34,
			20, 15, 17, 15, -7, -27, -52, -45,
			27, 24, 21, 10, -0, -10, -14, -17,
			22, 18, 17, 10, 3, -3, -21, -21,
			9, 15, 6, 4, -5, -17, -45, -30,
			3, 10, 1, 1, -13, -24, -39, -52,
			13, -0, 8, -2, -12, -6, -28, -20 //
		},
		{
			23, -1, 13, -4, -13, -27, -21, -7,
			2, 14, -1, 11, -12, -10, -9, 4,
			14, 14, 21, 6, 4, 0, -8, 4,
			23, 13, 18, 21, -8, -9, -13, -7,
			26, 24, 15, -8, 4, -13, -5, -11,
			-8, 19, -23, -7, -31, -20, -28, -26,
			-17, -16, -21, 7, 1, -19, -23, 11,
			25, -5, 16, 33, 34, -2, 6, 7 //
		},
		{
			26, 18, 26, -8, -14, -27, -32, 0,
			13, 11, 32, -9, -26, 3, -8, -2,
			19, 25, 5, -10, 9, -1, -26, -8,
			11, 26, -1, -10, 0, 3, -17, -5,
			10, 5, -3, -9, 11, -28, -8, -3,
			-18, 1, -19, -9, -18, -12, -29, -12,
			3, 21, -5, 2, -15, -13, 2, 6,
			16, 33, 10, 4, 20, -2, 11, 26 //
		},
		{
			7, 11, 19, 13, -17, -21, -36, -22,
			20, 32, 11, -8, -14, -23, -1, 5,
			16, 8, 19, -25, -10, -9, -17, -3,
			21, -2, 13, -10, -4, -8, -11, -6,
			3, 24, -14, -1, -14, -14, 2, -11,
			12, -4, -3, -33, -13, 7, -21, 9,
			12, 16, 19, -16, 13, 16, -15, -21,
			3, 45, 35, 12, 13, -3, -10, 4 //
		},
		{
			-22, -3, 3, -4, -23, -2, -12, -36,
			9, 6, 5, -6, -19, 0, -3, -2,
			11, 1, 16, -2, -28, -4, -7, 2,
			19, 7, 15, -6, -1, -2, 9, -1,
			13, 12, 28, -1, -13, -35, 0, 13,
			13, 3, 13, -18, -30, -21, -18, -4,
			15, 30, 15, -2, 3, 0, 10, -14,
			7, 21, 13, 11, 5, 2, 20, -8 //
		},
		{
			-1, 0, 3, 0, -12, 3, -6, -24,
			30, 28, 2, -7, -15, -2, 0, 2,
			19, 9, -0, 5, -14, -26, -8, -19,
			26, 11, 11, -7, -17, -10, 1, -1,
			19, 7, 12, -4, -3, -17, -6, -17,
			6, 3, -9, -5, -28, -22, -2, -10,
			23, 16, 4, -7, 1, -10, -2, -21,
			13, 10, 9, 2, -13, 26, 39, 6 //
		},
		{
			8, -5, -0, -7, -12, -9, -8, -0,
			15, 11, 6, -1, -7, -15, -32, 4,
			14, 8, 11, 10, -5, -6, -27, -23,
			18, 16, 14, 10, 0, 7, -21, -14,
			13, 9, 10, 1, -6, -6, -17, -32,
			14, 7, 6, -0, -16, -27, -35, -23,
			21, 26, 23, 8, 12, -3, -17, -34,
			17, 18, 16, 15, 4, 9, 15, 13 //
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameQueenTable = {
	{
		{
			3, 30, 48, 13, 2, 24, 153, 71,
			-31, -5, 14, -72, -30, 52, 25, -24,
			-43, -37, -3, -1, 50, 74, 125, 37,
			-23, -31, 29, -8, 4, 0, 1, 35,
			1, 3, -14, -2, -3, -21, -20, -28,
			-80, -18, -24, -25, -14, -20, -2, -43,
			-26, -38, 10, 17, 3, 2, -9, 25,
			-41, -42, -10, -6, -19, 5, -36, -6 //
		},
		{
			-69, -81, 22, -97, 34, -4, 45, -31,
			-28, -53, -29, -62, -73, 8, 13, 49,
			-34, -33, -44, 23, 16, 62, 96, 94,
			-18, -30, -19, -23, -14, 7, 25, 25,
			4, -26, -3, -6, -13, -3, 12, 14,
			-9, 12, -7, -0, 7, 1, 11, 22,
			-19, 13, 29, 27, 21, -6, 43, 47,
			34, 3, -1, 17, -1, -43, -17, 61 //
		},
		{
			-44, 14, -10, 53, 27, 67, 131, 37,
			-18, -35, -14, -35, -26, 28, 35, 87,
			-14, -35, -13, -14, 14, 79, 87, 64,
			-36, -27, -20, -12, -10, 4, 0, 19,
			-26, -27, -25, -28, -8, -10, 1, 10,
			-20, -20, -25, -21, -21, -13, 5, 2,
			-20, -17, -7, 0, -1, 13, 5, -9,
			-18, -19, -6, -2, -2, -26, -18, -32 //
		},
		{
			-82, 17, -21, -5, -3, -21, 22, 16,
			-18, 5, 16, 5, 7, -18, -25, 23,
			-54, -3, -8, -25, -19, -34, -3, -30,
			-5, -23, -25, 18, -10, -37, -12, 5,
			-23, -18, -13, -16, 3, 10, 4, 10,
			14, 35, -39, 12, 29, 64, 79, 68,
			-5, -13, -28, -23, -42, 31, 22, 71,
			62, 14, 13, 20, -37, 71, 20, -52 //
		},
		{
			-79, -85, 0, -6, -1, 21, -26, 8,
			-26, 9, 35, 26, 11, 11, 40, 48,
			-32, 4, 7, 20, -16, -0, -3, 16,
			18, -21, 13, 17, 19, 4, 12, 22,
			1, -8, -13, 2, 29, 33, 23, 54,
			-56, -30, -23, 36, 37, 55, 99, 109,
			-5, -73, -30, -55, -79, 28, 35, 102,
			-84, 20, -120, 59, 10, -237, 1, 12 //
		},
		{
			-31, -29, -130, -20, 12, 10, -18, -35,
			-49, -28, -4, 5, 13, 28, 37, 124,
			-50, -4, 43, -4, 38, -27, 47, 26,
			-32, 19, 18, 69, 38, 24, 52, 53,
			-27, 43, -16, 25, 23, 46, 34, 8,
			-15, -41, -35, 11, 73, 60, 65, 123,
			-85, -95, -81, -79, 6, -41, 10, 113,
			-104, -223, -131, 5, 78, 114, -110, 51 //
		},
		{
			16, 14, 16, 27, -28, -14, 23, 14,
			9, 29, 50, 42, 40, 36, 52, 15,
			6, 24, 19, 17, 16, 23, 24, 22,
			20, 4, 3, 9, -14, 7, 20, 26,
			-9, 3, -2, -2, -19, -0, 18, 34,
			2, -7, -10, 13, 10, 37, 62, 71,
			-32, -58, -64, -90, -80, 5, 38, 36,
			-96, -313, -28, -69, -135, -10, 88, 40 //
		},
		{
			-70, -36, -17, 4, -9, 32, 17, -43,
			-42, -17, -2, 15, -5, 35, -3, 44,
			-72, -12, 16, 24, 7, 38, 49, 24,
			-19, -35, -24, 34, 32, -3, 43, 20,
			-38, -25, -6, 11, 26, 30, 34, 63,
			-36, -20, -41, -38, 47, 105, 104, 97,
			-56, -95, -57, -58, -15, 26, 62, 78,
			-139, -39, -60, -56, 3, -90, 166, -8 //
		},
		{
			-17, -16, -14, -0, -1, -24, -33, -44,
			-9, -7, 0, -8, -2, 11, 28, 2,
			-20, -14, -14, -18, -23, -6, 17, 4,
			-22, -25, -22, -13, -9, -15, 5, 3,
			-30, -16, -23, -15, -12, -1, 8, 11,
			-12, -18, -8, 2, 30, 86, 75, 45,
			-34, -41, -32, -20, -8, 32, 16, 102,
			-32, -54, -34, -16, 28, 40, 138, 100 //
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameQueenTable = {
	{
		{
			-46, -35, -2, 21, 6, -2, -168, -58,
			48, -4, 27, 85, 64, -47, 3, 103,
			29, 51, 55, 63, 0, -76, -104, 19,
			46, 62, -3, 35, 80, 38, 30, -8,
			-28, 20, 46, 41, 17, 24, 18, 12,
			101, 33, 33, 26, 29, 18, -23, 85,
			-78, -14, -35, -10, -2, -19, -23, -90,
			-226, -95, -33, -38, 32, -57, -10, -68 //
		},
		{
			15, 17, 52, 108, 21, -14, -45, 13,
			-5, 43, 52, 117, 121, 15, -1, 32,
			25, 34, 63, -12, 36, 21, -64, -34,
			-16, 11, 34, 78, 52, 40, 29, 22,
			-19, 30, 24, 62, 48, 26, 6, 14,
			-29, -29, 18, 2, 2, 18, 12, 9,
			-78, -107, -89, -50, -35, 6, -116, -63,
			-126, -84, -54, -105, -40, -27, 25, -109 //
		},
		{
			32, -9, 43, -3, 39, 30, -28, 45,
			7, 26, 41, 69, 103, 52, 25, 9,
			-10, 20, 16, 26, 52, 31, 12, 29,
			-1, 7, 4, 9, 30, 39, 78, 35,
			-9, -2, 5, 19, 1, 8, 3, 26,
			-32, -13, 5, 0, -4, -2, -17, -32,
			-34, -20, -31, -32, -36, -86, -74, -62,
			-37, -40, -56, -27, -64, -61, -101, -51 //
		},
		{
			4, -58, 8, -4, -58, -48, -90, -70,
			-8, -11, -26, 11, -35, -13, -18, -80,
			9, 18, 35, 34, 36, 20, -14, 38,
			15, 35, 29, 8, 5, 40, 20, -8,
			22, 57, 34, 50, 42, -36, -2, 6,
			4, -42, 86, 33, -28, -42, -61, -33,
			-19, -6, 78, 48, 83, 6, -34, -91,
			-75, -77, 47, 81, -9, 9, 4, 41 //
		},
		{
			-31, -4, -99, -20, -38, -102, -70, -66,
			-9, -50, -53, -40, -30, -4, -96, -72,
			-2, -7, 5, -8, 36, -6, -10, -6,
			-45, 38, 1, 26, 8, -27, -9, -8,
			-14, -4, 47, 25, 5, -23, -9, 12,
			46, 74, 46, -6, 37, 11, -37, -13,
			-36, 59, 68, 104, 121, 30, -29, -7,
			-10, -29, 109, 35, 31, 160, 9, -9 //
		},
		{
			-80, -22, 52, -59, 11, -49, -0, -15,
			25, -19, -46, 18, 3, -55, -79, -117,
			67, -5, -30, -1, -4, 28, -102, -91,
			-1, -5, -6, -28, 5, 11, -50, -10,
			21, -6, 26, 30, 15, -10, -18, 53,
			-78, 15, 32, 10, 22, -6, -41, -25,
			39, 101, 76, 44, 44, 57, 13, 1,
			3, 140, 86, 12, 16, -67, 51, -2 //
		},
		{
			-71, -82, -107, -116, -6, -31, -46, -49,
			-75, -48, -102, -89, -85, -62, -85, -35,
			-32, -45, -26, -17, -17, -27, -42, -26,
			-30, -17, 1, 40, 55, 43, -19, -11,
			8, -26, 33, 25, 40, 70, -10, 9,
			-4, -20, 40, -11, 40, -3, 2, 1,
			1, 61, 53, 90, 82, 35, -5, 12,
			138, 275, 86, 115, 125, 32, -14, -22 //
		},
		{
			-40, -4, -36, -49, -28, -46, -71, 3,
			11, -18, -13, -27, -13, -63, -51, -50,
			44, -11, 1, -34, -10, -29, -69, 10,
			-43, 6, 14, -15, 0, 26, 10, 23,
			-10, -12, -10, 3, -1, 14, 22, 36,
			9, -32, 31, 39, -7, -45, -34, -26,
			-14, 21, 59, 77, 75, 40, -25, -9,
			56, 28, 66, 82, 54, 120, -60, 24 //
		},
		{
			-24, -31, -30, -26, -53, -53, -42, 17,
			-34, -32, -39, -16, -23, -56, -90, -68,
			-39, -19, -15, -11, 2, -7, -31, -11,
			-11, -4, -7, -2, 9, 22, 20, 25,
			3, -11, 5, 10, 29, 45, 39, 48,
			-19, -12, 3, 20, 16, -12, 25, 32,
			15, 20, 40, 39, 57, 42, 14, -14,
			36, 50, 63, 52, 43, 60, -39, -22 //
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> MiddleGameKingTable = {
	{
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			-260, -269, 0, 0, 0, 0, 0, 0,
			-289, -267, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, -398, -425, -421, 0, 0, 0,
			0, 0, -348, -441, -370, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, -791, -748, -755,
			0, 0, 0, 0, 0, -807, -729, -734 //
		},
		{
			804, 825, 0, 0, 0, 0, 0, 0,
			785, 803, 0, 0, 0, 0, 0, 0,
			727, 751, 0, 0, 0, 0, 0, 0,
			703, 721, 0, 0, 0, 0, 0, 0,
			721, 743, 0, 0, 0, 0, 0, 0,
			706, 765, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 456, 0, 0, 0, 0, 0,
			0, 0, 449, 0, 0, 0, 0, 0,
			0, 0, 376, 0, 0, 0, 0, 0,
			0, 0, 374, 0, 0, 0, 0, 0,
			0, 0, 391, 0, 0, 0, 0, 0,
			0, 0, 399, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, 393, 0, 0, 0, 0,
			0, 0, 0, 405, 0, 0, 0, 0,
			0, 0, 0, 352, 0, 0, 0, 0,
			0, 0, 0, 294, 0, 0, 0, 0,
			0, 0, 0, 315, 0, 0, 0, 0,
			0, 0, 0, 371, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, 342, 0, 0, 0,
			0, 0, 0, 0, 295, 0, 0, 0,
			0, 0, 0, 0, 248, 0, 0, 0,
			0, 0, 0, 0, 209, 0, 0, 0,
			0, 0, 0, 0, 226, 0, 0, 0,
			0, 0, 0, 0, 312, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, 0, 307, 0, 0,
			0, 0, 0, 0, 0, 324, 0, 0,
			0, 0, 0, 0, 0, 247, 0, 0,
			0, 0, 0, 0, 0, 239, 0, 0,
			0, 0, 0, 0, 0, 250, 0, 0,
			0, 0, 0, 0, 0, 289, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, 0, 0, 203, 197,
			0, 0, 0, 0, 0, 0, 180, 180,
			0, 0, 0, 0, 0, 0, 116, 90,
			0, 0, 0, 0, 0, 0, 63, 27,
			0, 0, 0, 0, 0, 0, 31, 15,
			0, 0, 0, 0, 0, 0, 51, 27,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
	}};

constexpr static std::array<std::array<int, 64>, PSQTBucketCount> EndGameKingTable = {
	{
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			-51, -5, 0, 0, 0, 0, 0, 0,
			-88, -56, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 14, 22, 23, 0, 0, 0,
			0, 0, -43, -19, -41, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 47, 22, -12,
			0, 0, 0, 0, 0, 2, -25, -70 //
		},
		{
			-108, -77, 0, 0, 0, 0, 0, 0,
			-59, -23, 0, 0, 0, 0, 0, 0,
			-40, 2, 0, 0, 0, 0, 0, 0,
			-38, 15, 0, 0, 0, 0, 0, 0,
			-44, 7, 0, 0, 0, 0, 0, 0,
			-38, -4, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, -47, 0, 0, 0, 0, 0,
			0, 0, -5, 0, 0, 0, 0, 0,
			0, 0, 40, 0, 0, 0, 0, 0,
			0, 0, 57, 0, 0, 0, 0, 0,
			0, 0, 56, 0, 0, 0, 0, 0,
			0, 0, 34, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, -32, 0, 0, 0, 0,
			0, 0, 0, 10, 0, 0, 0, 0,
			0, 0, 0, 58, 0, 0, 0, 0,
			0, 0, 0, 99, 0, 0, 0, 0,
			0, 0, 0, 96, 0, 0, 0, 0,
			0, 0, 0, 54, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, -38, 0, 0, 0,
			0, 0, 0, 0, 31, 0, 0, 0,
			0, 0, 0, 0, 72, 0, 0, 0,
			0, 0, 0, 0, 108, 0, 0, 0,
			0, 0, 0, 0, 108, 0, 0, 0,
			0, 0, 0, 0, 59, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, 0, -0, 0, 0,
			0, 0, 0, 0, 0, 37, 0, 0,
			0, 0, 0, 0, 0, 83, 0, 0,
			0, 0, 0, 0, 0, 101, 0, 0,
			0, 0, 0, 0, 0, 101, 0, 0,
			0, 0, 0, 0, 0, 78, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
		{
			0, 0, 0, 0, 0, 0, -29, -75,
			0, 0, 0, 0, 0, 0, 18, -20,
			0, 0, 0, 0, 0, 0, 47, 16,
			0, 0, 0, 0, 0, 0, 71, 29,
			0, 0, 0, 0, 0, 0, 80, 33,
			0, 0, 0, 0, 0, 0, 64, 28,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 //
		},
	}};

TunableSingle IsolatedPawnPenalty(-20, -14);
TunableSingle OpenFileRookBonus(45, 7);
TunableSingle SemiOpenFileRookBonus(16, 9);
TunableSingle QueenMobilityBonus(2, 8);
TunableSingle SemiOpenFileKingPenalty(-17, 14);
TunableSingle OpenFileKingPenalty(-80, 10);
TunableSingle KingShieldBonus(15, -12);
TunableSingle BishopPairBonus(31, 82);

TunableSingle PieceProtectedByPawnBonus(10, 11);
TunableSingle PieceAttackedByPawnPenalty(-45, -21);

TunableArray PassedPawnBonus(
	chess::PieceType::PAWN,
	std::vector<int>{0, 7, -2, -2, 22, 40, 146, 0},
	std::vector<int>{0, 13, 19, 45, 79, 166, 220, 0},
	1,
	1);

TunableArray VirtualKingMobilityBonus(
	chess::PieceType::QUEEN,
	std::vector<int>{0, 0, 0, 27, 45, 23, 24, 24, 20, 16, 13, 7, 5, -3, -13, -23, -30, -39, -44, -46, -32, -23, -21, -12, -16, 8, -38, -1},
	std::vector<int>{0, 0, 0, 26, 5, 24, 16, 6, 10, 8, 9, 14, 11, 14, 15, 17, 13, 10, 7, -1, -10, -19, -28, -38, -48, -68, -74, -92},
	0,
	0);

TunableArray KnightMobilityBonus(
	chess::PieceType::KNIGHT,
	std::vector<int>{0, 22, 29, 34, 36, 34, 32, 32, 33},
	std::vector<int>{0, -2, 11, 10, 15, 21, 22, 22, 17},
	0,
	0);

TunableArray BishopMobilityBonus(
	chess::PieceType::BISHOP,
	std::vector<int>{-268, 0, 23, 28, 42, 47, 61, 70, 77, 78, 83, 87, 87, 112, 0},
	std::vector<int>{-276, 0, -12, 26, 45, 59, 80, 89, 101, 108, 114, 114, 114, 112, 0},
	0,
	1);

TunableArray RookMobilityBonus(
	chess::PieceType::ROOK,
	std::vector<int>{0, 8, 13, 17, 16, 22, 23, 26, 26, 28, 32, 34, 34, 49, 46},
	std::vector<int>{0, 26, 29, 35, 44, 47, 52, 56, 67, 73, 75, 77, 81, 78, 75},
	0,
	0);