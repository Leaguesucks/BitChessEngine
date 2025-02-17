#include "BitBoard.h"

U64 SetBit(U64 b, int ns, ...) {
    if (ns <= 0) return b;

    va_list args;
    va_start(args, ns);

    for (int i = 0; i < ns; i++) {
        Square s = va_arg(args, Square);
        b |= (REVERSE_ONE >> s);
    }
    
    va_end(args);
    return b;
}