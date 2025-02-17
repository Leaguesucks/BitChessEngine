/* Debug program for the bitboard */

#include "BitBoard.h"
#include "debug.h"

int main(void) {
    U64 b = 0ULL;
    b = SetBit(b, 1, A1);
    FILE *f = fopen("RookAttackDebug.txt", "w");
    PrintRookAttack(f);

    //PrintBoard(0x000202020202027CULL, stdout);

    return 0;
}