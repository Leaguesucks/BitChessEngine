/* Debug program for the bitboard */

#include "debug.h"

int main(void) {
    U64 b = 0ULL;
    b = SetBit(b, 1, E4);
    //b = PopBit(b, 3, A1, A3, A5);

    //PrintBoard(b, stdout);
    //Print_King_Attacks("resources/King_Attacks.txt");
    
    for (Square s = 0; s < 64; s++) {
        PrintBoard(WHITE_PAWN_ATTACKS[s], stdout);
        getchar();
   }

   printf("\nBlack Pawns\n\n");

   for (Square s = 0; s < 64; s++) {
        PrintBoard(BLACK_PAWN_ATTACKS[s], stdout);
        getchar();
   }

    //Print_Pawn_Attacks("resources/Pawn_Attacks.txt");

    return 0;
}