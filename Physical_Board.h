/* A header files to print the BitBoard. The bit board is converted to 
 * its actual physical representations and printed to STDOUT via 
 * functions in this header files
 */

#ifndef _PHYSICAL_BOARD_H_
#define _PHYSICAL_BOARD_H_

#include <stdio.h>
#include <string.h>

#include "BitBoard.h"

/* For printing colored text (debugging or style purpose)
 *
 * USAGE: COLOR(color) "String" COLOR(RESET)              
 */
#define COLOR(code) "\033[" code "m"

#define BLK     "30"
#define RED     "31"
#define GREEN   "32"
#define YELLOW  "33"
#define BLUE    "34"
#define MAGENTA "35"
#define CYAN    "36"
#define WH      "37"
#define RESET   "0"
/* ****************************************************** */

/* Print the physical representation of the bit board on a STREAM
 * Para:
 *      b -> The bit board
 *      f -> The file or stream descriptor to print this chess board
 */
void PrintBoard(BitBoard *b, FILE* f) {
    const char *COL_ADDRESS_STR = "  A B C D E F G H \n\0";
    const U64 REVERSE_ONE = 0x8000000000000000;

    fprintf(f, COLOR(YELLOW)"%s"COLOR(RESET), COL_ADDRESS_STR);
    
    int rowCount = 0;
    char p;
    for (int i = 8; i >= 1; i--) { // Print each row
        fprintf(f, COLOR(YELLOW)"%d "COLOR(RESET), i);
        
        for (int j = 0; j < 8; j++) { // Print each column            
                 if (((b->wPawns   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'P';
            else if (((b->wRooks   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'R';
            else if (((b->wKnights << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'N';
            else if (((b->wBishops << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'B';
            else if (((b->wKing    << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'K';
            else if (((b->wQueen   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'Q';

            else if (((b->bPawns   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'p';
            else if (((b->bRooks   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'r';
            else if (((b->bKnights << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'n';
            else if (((b->bBishops << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'b';
            else if (((b->bKing    << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'k';
            else if (((b->bQueen   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'q';

            else                                                           p = '*';

            printf("%c ", p);
        }

        fprintf(f, COLOR(YELLOW)"%d \n"COLOR(RESET), i);
        rowCount++;
    }
    
    fprintf(f, COLOR(YELLOW)"%s"COLOR(RESET), COL_ADDRESS_STR);

}

#endif