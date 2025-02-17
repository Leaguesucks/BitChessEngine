#ifndef _DEBUG_H_
#define _DEBUG_H

#include "BitBoard.h"

/* For printing colored text (debugging or style purpose)
 *
 * USAGE: COLOR(color) "String" COLOR(RESET)              
 */
#define COLOR(code) "\033[" code "m"

#define BLACK   "30"
#define RED     "31"
#define GREEN   "32"
#define YELLOW  "33"
#define BLUE    "34"
#define MAGENTA "35"
#define CYAN    "36"
#define WHITE   "37"
#define RESET   "0"
/* ****************************************************** */

/* Print the bitboard (an U64 type) on f stream
 *
 * Para: f -> Where to print the board
 *       b -> The board, any U64 number
 */
void PrintBoard(U64 b, FILE *f) {
    fprintf(f, "   A B C D F G H  \n\n");

    for (int row = 0; row < 8; row++) {
        fprintf(f, "%d  ", 8 - row);

        for (int col = 0; col < 8; col++) {
            if ((b << (col + 8*row)) & REVERSE_ONE) fprintf(f, "1 ");
            else fprintf(f, "0 ");
        }

        fprintf(f, " %d \n", 8 - row);
    }

    fprintf(f, "\n   A B C D F G H  \n\n");
}

// /* Print all possible rook distinct attacks to a file */
// void PrintRookAttack(FILE *f) {

//     U64 b = 0ULL;
//     U64 Lw, Le, Ln;
//     int index = 1;

//     fprintf(f, "Index 0:\n");
//     PrintBoard(b, f);

//     for (int i = 0; i < 64; i++) {
//         int curCol = i % 8;
//         int curRow = i / 8;
//         int local = 0;

//         fprintf(f, "Square %d:\n\n", i);

//         for (int w = curCol; w >= 0; w--) { // Move West
//             if (w != curCol) b = SetBit(b, 1, i - (curCol - w));
//             Lw = b;
//             for (int e = curCol; e < 8; e++) { // Move East
//                 if (e != curCol) b = SetBit(b, 1, i + (e - curCol));
//                 Le = b;
//                 for (int n = curRow; n >= 0; n--) { // Move North
//                     if (n != curRow) b = SetBit(b, 1, i - 8*(curRow - n));
//                     Ln = b;
//                     for (int s = curRow; s < 8; s++) { // Move South
//                         if (s != curRow)
//                             b = SetBit(b, 1, i + 8*(s - curRow));
//                         if (local != 0) {
//                             fprintf(f, "Index %d:\n", index);
//                             PrintBoard(b, f);
//                             index++;
//                         }
//                         if (s == 7) b = Ln;
//                         local++;
//                     }
//                     if (n == 0) b = Le;
//                 }
//                 if (e == 7) b = Lw;
//             }
//         }
        
//         b = 0;
//     }
// }

#endif