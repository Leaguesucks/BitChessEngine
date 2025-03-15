/* Debug program for the bitboard */

#include "debug.h"

int main(void) {
    U64 b = 0ULL;
    b = SetBit(b, 1, E4);
    //b = PopBit(b, 3, A1, A3, A5);

    //PrintBoard(b, stdout);
    for (int i = 0; i < 64; i++) {
        //PrintBoard(b, stdout);
        //PrintBoard(GetBlockedRookAttacks(b, i), stdout);
        //printf("%s\n", squares[i]);
        //getchar();

        //printf("%d %d\n", NUM_BISHOP_RELEVANT_SQUARES[i], NUM_ROOK_RELEVANT_SQUARES[i]);
    }

    return 0;
}