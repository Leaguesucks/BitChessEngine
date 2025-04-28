#ifndef _DEBUG_H_
#define _DEBUG_H

#include "../srcs/Board/ChessGame.h"
#include "../srcs/GUI/ChessGUI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

const char *squares[65] = 
{
    "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8",
    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
    "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
    "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
    "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
    "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
    "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
    "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
    "NQ"
};

U64 random_U64(void) {
    U64 u1, u2, u3, u4;
    u1 = (U64)(random()) & 0xFFFF; u2 = (U64)(random()) & 0xFFFF;
    u3 = (U64)(random()) & 0xFFFF; u4 = (U64)(random()) & 0xFFFF;
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

U64 random_U64_with_more_1sBit(void) {
    return random_U64() & random_U64() & random_U64();
}

/* Print the bitboard (an U64 type) on f stream
 *
 * Para: f -> Where to print the board
 *       b -> The board, any U64 number
 */
void PrintBoard(U64 b, FILE *f) {
    fprintf(f, "   A B C D E F G H  \n\n");

    for (int row = 0; row < 8; row++) {
        fprintf(f, "%d  ", 8 - row);

        for (int col = 0; col < 8; col++) {
            if ((b << (col + 8*row)) & REVERSE_ONE) fprintf(f, COLOR(YELLOW)"1 "COLOR(RESET));
            else fprintf(f, "0 ");
        }

        fprintf(f, " %d \n", 8 - row);
    }

    fprintf(f, "\n   A B C D E F G H  \n\n");
}

/* An advance function to print THE bitboard, which contains information about the chess game
 * itself.
 *
 * Para: bb   -> The bitboard
 *       fs   -> The focus square. If a piece is on the focus square then we print it attacks map.
 *               If set to NSQ (Not a square) then no square is being focused.
 *       f    -> Where to print the result to
 */
void Print_BitBoard(const BitBoard bb, Square fs, FILE *f) {
    fprintf(f, "   A B C D E F G H  \n\n");
    int focus = 0;

    for (int rank = 0; rank < 8; rank++) {
        fprintf(f, "%d  ", 8 - rank);

        for (int file = 0; file < 8; file++) {
            Square square = (Square) (rank*8 + file);

            if (fs == NOT_A_SQUARE)
                focus = 0;
            else
                if (GetBit(bb.atk_on_each_square[fs], square) || GetBit(bb.pawn_moves_on_each_square[fs], square))
                    focus = 1;
                else
                    focus = 0;

            if (GetBit(bb.pos_bPawns, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"P "COLOR(RESET)) :
                                  fprintf(f, COLOR(GREEN)"P "COLOR(RESET));
            else if (GetBit(bb.pos_bRooks, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"R "COLOR(RESET)) :
                                  fprintf(f, COLOR(GREEN)"R "COLOR(RESET));
            else if (GetBit(bb.pos_bKnights, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"N "COLOR(RESET)) :
                                  fprintf(f, COLOR(GREEN)"N "COLOR(RESET));
            else if (GetBit(bb.pos_bBishops, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"B "COLOR(RESET)) :
                                  fprintf(f, COLOR(GREEN)"B "COLOR(RESET));
            else if (GetBit(bb.pos_bQueens, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"Q "COLOR(RESET)) :
                                  fprintf(f, COLOR(GREEN)"Q "COLOR(RESET));
            else if (GetBit(bb.pos_bKing, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"K "COLOR(RESET)) :
                                  fprintf(f, COLOR(GREEN)"K "COLOR(RESET));
            else if (GetBit(bb.pos_wPawns, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"P "COLOR(RESET)) :
                                  fprintf(f, COLOR(CYAN)"P "COLOR(RESET));
            else if (GetBit(bb.pos_wRooks, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"R "COLOR(RESET)) :
                                  fprintf(f, COLOR(CYAN)"R "COLOR(RESET));
            else if (GetBit(bb.pos_wKnights, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"N "COLOR(RESET)) :
                                  fprintf(f, COLOR(CYAN)"N "COLOR(RESET));
            else if (GetBit(bb.pos_wBishops, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"B "COLOR(RESET)) :
                                  fprintf(f, COLOR(CYAN)"B "COLOR(RESET));
            else if (GetBit(bb.pos_wQueens, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"Q"COLOR(RESET)) :
                                  fprintf(f, COLOR(CYAN)"Q "COLOR(RESET));
            else if (GetBit(bb.pos_wKing, square))
                (focus) ?  fprintf(f, COLOR(YELLOW)"K"COLOR(RESET)) :
                                  fprintf(f, COLOR(CYAN)"K "COLOR(RESET));
            else {
                if (fs == NOT_A_SQUARE)
                    fprintf(f, "0 ");
                else {
                    if (GetBit(bb.atk_on_each_square[fs], square) || GetBit(bb.pawn_moves_on_each_square[fs], square))
                        fprintf(f, COLOR(YELLOW)"1 "COLOR(RESET));
                    else
                        fprintf(f, "0 ");
                }
            }
        }
        fprintf(f, " %d \n", 8 - rank);
    }

    fprintf(f, "\n   A B C D E F G H  \n\n");

    fprintf(f, "Side to Play: ");
    if (bb.side2play == W)
        fprintf(f, COLOR(CYAN)"White\n"COLOR(RESET));
    else if (bb.side2play == B)
        fprintf(f, COLOR(GREEN)"Black\n"COLOR(RESET));
    else
        fprintf(f, COLOR(RED)"Error: %f\n"COLOR(RESET), bb.side2play);

    fprintf(f, "Castling Rights: ");

    if (bb.castle_right & 1)
        fprintf(f, COLOR(GREEN)"K "COLOR(RESET));
    if (bb.castle_right & 2)
        fprintf(f, COLOR(GREEN)"Q "COLOR(RESET));
    if (bb.castle_right & 4)
        fprintf(f, COLOR(CYAN)"k "COLOR(RESET));
    if (bb.castle_right & 8)
        fprintf(f, COLOR(CYAN)"q "COLOR(RESET));

    fprintf(f, "\nEnPassen Map:\n\n");
    PrintBoard(bb.EnPassen, f);

    fprintf(f, "Halfmove clock: %d\n", bb._50Count);
    fprintf(f, "Fullmove number: %d\n", bb.Full_MoveCount);
}

/* Print the bit board and its associated property given a fen positions 
 *
 * Para: fout -> Where to print the result to
 */
void FEN_Debug(FILE *fout) {
    BitBoard bb;
    char *fen = malloc(1000);

    while (1) {
        system("clear");
        memset(fen, '\0', 1000);
        printf("Enter FEN:\n");
        fgets(fen, 1000, stdin);
        Init_Game(&bb, fen);
        Print_BitBoard(bb, NOT_A_SQUARE, stdout);
        printf("\n*** Press any key to enter another FEN ***\n");
        getchar();
    }
    free(fen);
}

/* A more general debug functions. Modify at your own will to test for any function
 * 
 * This function automatically print the test result to stdout
 */
void Debug() {
    BitBoard bb;
    char *fs = malloc(1000);
    char *fen = malloc(1000);

    while (1) {
        system("clear");
        memset(fen, '\0', 1000);
        
        printf("Enter FEN: ");
        fgets(fen, 1000, stdin);
        fen[strlen(fen) - 1] = '\0'; // Get rid of the trailing newline char
        
        Init_Game(&bb, fen);
        Print_BitBoard(bb, NOT_A_SQUARE, stdout);

        while (1) {
            Square square;
            while (1) {
                memset(fs, '\0', 1000);
                printf("\nChoose a square: ");
                fgets(fs, 1000, stdin);
                fs[strlen(fs) - 1] = '\0'; // Get rid of the trailing newline char
                if (strlen(fs) != 2) {
                    fprintf(stderr, "Too many input! Please try again!\n\n");
                    continue;
                }

                int file = ( 'a' <= fs[0] && fs[0] <= 'h') ? fs[0] - 'a' : fs[0] - 'A';
                int rank = 7 - (fs[1] - '1');

                if (!(0 <= file && file <= 7) || !(0 <= rank && rank <= 7)) {
                    fprintf(stderr, "Invalid input! Please try again!\n\n");
                    continue;
                }

                square = (rank*8) + file;
                break;
            }

            /* *** ENTER WHAT YOU WANNA TEST HERE *** */
            Gen_All_Attacks(&bb, W);
            Gen_All_Attacks(&bb, B);


            /* ************************************* */
            
            Print_BitBoard(bb, square, stdout);
            printf("\n*** Enter \'n\' for a new fen! Otherwise enter any key for a new fs ***\n");
            if (getchar() == 'n')
                break;
            else {
                int c;
                while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer
                system("clear");
                Print_BitBoard(bb, NOT_A_SQUARE, stdout);
            }       
        }
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer
    }

    free(fs);
    free(fen);
}

/* Debug the GUI */
void Debug_GUI() {
    Game game;

    sdl_init(&game);
    sdl_chessGame(&game);
}

#endif