/* A header files to print the BitBoard. The bit board is converted to 
 * its actual physical representations and printed to STDOUT via 
 * functions in this header files
 */

#ifndef _PHYSICAL_BOARD_H_
#define _PHYSICAL_BOARD_H_

#include <stdio.h>

#include "BitBoard.h"


const char *COL_ADDRESS = {' ','A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', ' '};
const char *ROW_ADDRESS = {'8', '7', '6', '5', '4', '3', '2', '1'};

/* Print the physical representation of the bit board on STDOUT
 * Para:
 *      board -> The bit board
 */
void PrintBoard(BitBoard *board) {
    for (int i = 0; i < 10; i++) { // Print each row
        for (int j = 0; j < 10; j++) { // Print each column
            if (i == 0 || i == 9) printf("%c ");
        }
    }
}

#endif