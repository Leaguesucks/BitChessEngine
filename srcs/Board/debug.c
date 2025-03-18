/* Debug program for the bitboard */

#include "debug.h"

int main(void) {
    U64 b = 0ULL;
    b = SetBit(b, 1, E4);
    //b = PopBit(b, 3, A1, A3, A5);

    //PrintBoard(b, stdout);
    //Print_King_Attacks("resources/King_Attacks.txt");
    
   for (Square s = A8; s <= H1; s++) {
        PrintBoard(King_Attacks[s], stdout);
        getchar();
   }

    return 0;
}