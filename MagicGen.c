#include "MagicGen.h"

U64 random_U64(void) {
    U64 u1, u2, u3, u4;
    u1 = (U64)(random()) & 0xFFFF; u2 = (U64)(random()) & 0xFFFF;
    u3 = (U64)(random()) & 0xFFFF; u4 = (U64)(random()) & 0xFFFF;
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

U64 random_U64_with_more_1sBit(void) {
    return random_uint64() & random_uint64() & random_uint64();
}