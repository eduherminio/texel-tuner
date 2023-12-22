#include "../external/chess.hpp"

using u64 = uint64_t;

// Shamelessly yoinked from 4ku

[[nodiscard]] static auto count(const u64 bb) {
    return chess::builtin::popcount(bb);
}

[[nodiscard]] static auto east(const u64 bb) {
    return (bb << 1) & ~0x0101010101010101ULL;
}

[[nodiscard]] static auto west(const u64 bb) {
    return (bb >> 1) & ~0x8080808080808080ULL;
}

[[nodiscard]] static u64 north(const u64 bb) {
    return bb << 8;
}

[[nodiscard]] static u64 south(const u64 bb) {
    return bb >> 8;
}

[[nodiscard]] static u64 nw(const u64 bb) {
    return north(west(bb));
}

[[nodiscard]] static u64 ne(const u64 bb) {
    return north(east(bb));
}

[[nodiscard]] static u64 sw(const u64 bb) {
    return south(west(bb));
}

[[nodiscard]] static u64 se(const u64 bb) {
    return south(east(bb));
}

[[nodiscard]] static u64 WhitePawnAttacks(const u64 bb) {
    return se(bb) | sw(bb);
}

[[nodiscard]] static u64 BlackPawnAttacks(const u64 bb) {
    return ne(bb) | nw(bb);
}