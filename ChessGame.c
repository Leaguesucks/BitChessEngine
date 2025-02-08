#include "ChessGame.h"

int main(int argv, char *argc[]) {
    init_board(&board);
    Side2Play = WHITE;
    in = stdin;
    out = stdout;

    PrintBoard(&board, NULL);

    while (1) {
        GetRequest();
    }

    return 0;
}