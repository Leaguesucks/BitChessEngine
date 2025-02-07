/* A chess game on STDIN/OUT that used to test the chess engine and 
 * the bit board representation
 */

#include "Physical_Board.h"

BitBoard board;

int main(int argv, char *argc[]) {
    init_board(&board);

    PrintBoard(&board, stdout);

    char *request = malloc(100*sizeof(char));

   while (1) {

   }

    return 0;
}