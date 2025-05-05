/* Debug program for the bitboard */

#include "debug.h"

int main(void) {
    srand(time(NULL));

    Init_Database();

    // while (1) {
    //     U64 b = random_U64_with_more_1sBit();
    //     PrintBoard(b, stdout);
    //     printf("\nLSMB 1 bits: %s", squares[Get_LSMB(b ,1)]);
    //     printf("\nLSMB 0 bits: %s\n", squares[Get_LSMB(b ,0)]);
    //     getchar();
    // }

    //FEN_Debug(stdout);
    //Debug();
    //printf("%ld\n", sizeof(BitBoard));

    // for (int i = 0; i < 64; i++) {
    //     PrintBoard(BLACK_PAWN_ATTACKS[i], stdout);
    //     getchar();
    // }

    //Print_Pawn_Attacks("resources/Pawn_Attacks.txt");

    //Debug_GUI();

    for (Square src = C6; src <= H3; src++)
        for (Square des = A8; des <= H1; des++) {
            system("clear");
            U64 map = Draw_BB_Line(src, des);
            printf("src = %s, des = %s\n\n", squares[src], squares[des]);
            PrintBoard(map, stdout);
            getchar();
        }
        // for (int i = 0; i < 8; i++) {
        //     system("clear");
        //     printf("Square = %s, direction = %s\n\n", squares[src], direction[i]);
        //     PrintBoard(Sliders_Rays[src][i], stdout);
        //     getchar();
        // }


    return 0;
}