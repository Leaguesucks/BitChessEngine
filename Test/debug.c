/* Debug program for the bitboard */

#include "debug.h"

int main(void) {
    srand(time(NULL));

    // while (1) {
    //     U64 b = random_U64_with_more_1sBit();
    //     PrintBoard(b, stdout);
    //     printf("\nLSMB 1 bits: %s", squares[Get_LSMB(b ,1)]);
    //     printf("\nLSMB 0 bits: %s\n", squares[Get_LSMB(b ,0)]);
    //     getchar();
    // }

    FEN_Debug(stdout);

    return 0;
}