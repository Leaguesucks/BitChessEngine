#include "BitManipulation.h"

U64 SetBit(U64 b, int ns, ...) {
    if (ns <= 0) return b;

    va_list args;
    va_start(args, ns);

    for (int i = 0; i < ns; i++) {
        Square square = va_arg(args, Square);
        b |= (REVERSE_ONE >> square);
    }

    va_end(args);
    return b;
}

U8 GetBit(U64 b, Square sq) {
    return (b  & (REVERSE_ONE >> sq)) != 0;
}

U64 PopBit(U64 b, int ns, ...) {
    if (ns <= 0) return b;
    
    va_list args;
    va_start(args, ns);

    for (int i = 0; i < ns; i++) {
        Square square = va_arg(args, Square);
        b ^= (REVERSE_ONE >> square);
    }

    va_end(args);
    return b;
}

U8 CountBits(U64 b, U8 Bit) {
    if (Bit != 1 && Bit != 0) return 0;

    U8 count = 0;
    for (int i = 0; i < 64; i++)
        if (GetBit(b, (Square) i) == Bit) count++;

    return count;
}

Square Get_LSMB(U64 b, U8 Bit) {
    for (Square i = H1; i >= A8; i--)
        if (GetBit(b, i) == Bit) return i;
    return NOT_A_SQUARE;
}