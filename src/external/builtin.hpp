/*
Extracted 'as-is' from v0.5.13 of Disservin/chess-library by Eduardo Cáceres
More specifically, from this commit: https://github.com/Disservin/chess-library/tree/a4d2bf5cb882c8cdca0e36266b6bea10ae9c4b2e
*/

/*
MIT License

Copyright (c) 2023 disservin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Source: https://github.com/Disservin/chess-library
*/

/*
VERSION: 0.5.13
*/

#include "chess.hpp"

namespace chess
{
    using U64 = std::uint64_t;

/****************************************************************************\
 * Wrapper around compiler functions                                         *
\****************************************************************************/
    namespace builtin
    {
        /// @brief Get the least significant bit of a U64.
        /// @param b
        /// @return
        inline Square lsb(U64 b);

        /// @brief Get the most significant bit of a U64.
        /// @param b
        /// @return
        inline Square msb(U64 b);

#if defined(__GNUC__) // GCC, Clang, ICC
        inline Square lsb(U64 b)
        {
            assert(b);
            return Square(__builtin_ctzll(b));
        }

        inline Square msb(U64 b)
        {
            assert(b);
            return Square(63 ^ __builtin_clzll(b));
        }

#elif defined(_MSC_VER)

#ifdef _WIN64
#include <intrin.h>
        inline Square lsb(U64 b)
        {
            unsigned long idx;
            _BitScanForward64(&idx, b);
            return (Square)idx;
        }

        inline Square msb(U64 b)
        {
            unsigned long idx;
            _BitScanReverse64(&idx, b);
            return (Square)idx;
        }

#else

#error "MSVC 32-bit not supported."

#endif

#else

#error "Compiler not supported."

#endif

        /// @brief Count the number of set bits in a U64.
        /// @param mask
        /// @return
        inline int popcount(U64 mask)
        {
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)

            return (uint8_t)_mm_popcnt_u64(mask);

#else

            return __builtin_popcountll(mask);

#endif
        }
    } // namespace builtin
} // namespace chess