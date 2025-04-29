/* The Bit representation of the chess board 
 *
 * Here, the chess board is represented by a 64 bits unsigned int, the index
 * of each square is mapped from left to right, top to bottom
 * 
 * NOTE: The functions declared in this header file are EXTREMELY UNSAFE to trade off
 * for performance. Use these functions with through error checkings beforehand
 */

#ifndef _BITBOARD_H_
#define _BITBOARD_H_

#include <stdarg.h>
#include <stdio.h>

#define REVERSE_ONE 0x8000000000000000ULL

/* Edges of the board, defined as the first as last rank/file */
#define EDGES 0xff818181818181ffULL

/* Defines the castling sequences squares
 * e.g., For white king side:
 *      K * * R
 */
#define BLACK_CASTLE_QUEEN_SEQUENCES 0xf800000000000000ULL
#define BLACK_CASTLE_KING_SEQUENCES  0x0f00000000000000ULL
#define WHITE_CASTLE_QUEEN_SEQUENCES 0x00000000000000f8ULL
#define WHITE_CASTLE_KING_SEQUENCES  0x000000000000000fULL



/* Chess pieces absolute values as evaluated by AlphaZero in 2020 */

/* Pawn absolute value */
#define PAWN_ABS_VAL 1.0f

/* Rook absolute value */
#define ROOK_ABS_VAL 5.63f

/* Knight absolute value */
#define KNIGHT_ABS_VAL 3.05f

/* Bishop absolute value */
#define BISHOP_ABS_VAL 3.33f

/* Queen absolute value */
#define QUEEN_ABS_VAL 9.5f

/* King absolute value */
#define KING_ABS_VAL 1e6f

#define CASTLE_RIGHT 1;
#define CASTLE_LEFT -1;

/* ***************************** */

static const float B = (-1.0f); // Black side
static const float W = (1.0f);  // White side

typedef unsigned char   U8;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef unsigned long  U64;

typedef enum Square {
    A8, B8, C8, D8, E8, F8, G8, H8,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A1, B1, C1, D1, E1, F1, G1, H1,
    NOT_A_SQUARE
} Square;

typedef enum Direction {
    NORTH, NE, EAST, SE, SOUTH, SW, WEST, NW
} Direction;

typedef struct BitBoard {
    
    /* Postions of all pieces of each type */
    U64 pos_wPawns;
    U64 pos_wRooks;
    U64 pos_wKnights;
    U64 pos_wBishops;
    U64 pos_wQueens;
    U64 pos_wKing;
    U64 pos_wAll;

    U64 pos_bPawns;
    U64 pos_bRooks;
    U64 pos_bKnights;
    U64 pos_bBishops;
    U64 pos_bQueens;
    U64 pos_bKing;
    U64 pos_bAll;

    /* Cuurent number of each pieces on the chess board */
    U8 num_wPawns;
    U8 num_wRooks;
    U8 num_wKnights;
    U8 num_wBishops;
    U8 num_wQueens;

    U8 num_bPawns;
    U8 num_bRooks;
    U8 num_bKnights;
    U8 num_bBishops;
    U8 num_bQueens;

    /* Attack map on each square of the chess board */
    U64 atk_on_each_square[64];

    /* Attack map of white and black pieces */
    U64 atk_wAll;
    U64 atk_bAll;

    /* Covered squares of white and black.
     * Here, a "covered" squares means a squares that can be attacked by the allied piece, even if that square is occupied by another piece.
     * => This is used to checked if it is valid for the King to attack a square
     */
    U64 wCovered;
    U64 bCovered;

    /* All pawn & King moves map on each square of the chess board.
     * Here, a "move" is defined as a movement that cannot capture any piece, such as a pawn move or a King castling
     */
    U64 moves_on_each_square[64];

    /* All possible X-Ray attacks */
    U64 atk_wXray;
    U64 atk_bXray;

    /* Pawn EnPassen for both side */
    U64 EnPassen;

    /* Castle right for both sides. From LSB to MSB, King -> Queen side for White -> Black 
     * e.g., 0b00001111 means both side has castle right to both King and Queen side
     *       0b00000111 means Black has lost its right to castle Queen side
     */
    U8 castle_right;

    /* The pin positions of each side. If a piece is pin then it cannot move */
    U64 wPins;
    U64 bPins;

    /* Who to move. -1 for black and 1 for white */
    float side2play;

    /* 50 moves rules counter. The game is a draw if it is 0. Reset back to 100 if there is a pawn
     * advance or capturing from both sides. Decrement starts from the next moves after the most
     * recent 'reset' action.
     */
    U8 _50Count;

    /* Full move counter since the begining of the game. Increment by 1 after black makes a move */
    U16 Full_MoveCount;
    
} BitBoard;

#endif