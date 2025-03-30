#include "BitManipulation.h"

U64 SetBit(U64 b, int ns, ...) {
    if (ns <= 0) return b;

    va_list args;
    va_start(args, ns);

    U64 lb = b;
    for (int i = 0; i < ns; i++) {
        Square square = va_arg(args, Square);
        lb |= (REVERSE_ONE >> square);
    }

    va_end(args);
    return lb;
}

U8 GetBit(U64 b, Square sq) {
    return (b  & (REVERSE_ONE >> sq)) != 0;
}

U64 PopBit(U64 b, int ns, ...) {
    if (ns <= 0) return b;
    
    va_list args;
    va_start(args, ns);

    U64 lb = b;
    for (int i = 0; i < ns; i++) {
        Square square = va_arg(args, Square);
        lb ^= (REVERSE_ONE >> square);
    }

    va_end(args);
    return lb;
}

U8 CountBits(U64 b, U8 Bit) {
    if (Bit)
        return (U8) __builtin_popcountll(b);
    else
        return (U8) (64 - __builtin_popcountll(b));
}

Square Get_LSMB(U64 b, U8 Bit) {
    if (b == 0ULL)
        return NOT_A_SQUARE;

    if (Bit)
        return (Square) (H1 - __tzcnt_u64(b));
    else
        return (Square) (H1 - __tzcnt_u64(~b));
}

Square Get_MSMB(U64 b, U8 Bit) {
    if (b == 0ULL)
        return NOT_A_SQUARE;

    if (Bit)
        return (Square) __lzcnt64(b);
    else
        return (Square) __lzcnt64(~b);
}