#include "BitManipulation.h"

U64 SetBit(U64 b, Square square) {
    return (b | (REVERSE_ONE >> square));
}

U8 GetBit(U64 b, Square sq) {
    return (b  & (REVERSE_ONE >> sq)) != 0;
}

U64 PopBit(U64 b, Square square) {
    return (b ^ (REVERSE_ONE >> square));
}

U8 CountBits(U64 b) {
    return (U8) __builtin_popcountll(b);
}

Square Get_LSMB(U64 b) {
    if (b == 0ULL)
        return NOT_A_SQUARE;

    return (Square) (H1 - __tzcnt_u64(b));
}

Square Get_MSMB(U64 b) {
    if (b == 0ULL)
        return NOT_A_SQUARE;

    return (Square) __lzcnt64(b);
}