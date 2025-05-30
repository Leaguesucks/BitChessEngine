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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86intrin.h> // C/C++ (GCC, Clang)

#define REVERSE_ONE 0x8000000000000000ULL

/* Edges of the board, defined as the first and last rank/file */
#define EDGES 0xff818181818181ffULL

/* Defines the castling sequences squares
 * e.g., For white king side:
 *      K * * R
 */
#define BLACK_CASTLE_QUEEN_SEQUENCES 0xf800000000000000ULL
#define BLACK_CASTLE_KING_SEQUENCES  0x0f00000000000000ULL
#define WHITE_CASTLE_QUEEN_SEQUENCES 0x00000000000000f8ULL
#define WHITE_CASTLE_KING_SEQUENCES  0x000000000000000fULL

#define CASTLE_KING   0
#define CASTLE_QUEEN  1

#define WKING_CASTLE_SQUARE  G1
#define WQUEEN_CASTLE_SQUARE C1
#define BKING_CASTLE_SQUARE  G8
#define BQUEEN_CASTLE_SQUARE C8

#define MAX50COUNT 100;

#define BLACK 0
#define WHITE 1

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
    NORTH, NE, EAST, SE, SOUTH, SW, WEST, NW, NOT_A_DIRECTION
} Direction;

/* Chess pieces square identification */
typedef enum PNum {
    PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, 
    EMPTY
} PNum;

typedef struct BitBoard {
    
    /* Postions of all pieces of each type. 0 for Black, 1 for White */
    U64 positions[2][6];

    /* All positions of all pieces on each side. 0 for Black, 1 for White */
    U64 all_positions[2];

    /* Cuurent number of each pieces on the chess board. 0 - 5 for White, 6 - 11 for Black */
    U8 numPiece[2][6];

    /* Attack map on each square of the chess board. An "attack" means if an enemy is in the attack map, that enemy can be captured */
    U64 atk_on_each_square[64];

    /* Attack map of white and black pieces. 0 for Black, 1 for White */
    U64 all_attacks[2];

    /* Covered squares of white and black. 0 for Black, 1 for White
     * Here, a "covered" squares means a squares that can be attacked by the allied piece, even if that square is occupied by another piece.
     * => This is used to checked if it is valid for the King to moove to a square or capture a piece
     */
    U64 covered[2];

    /* All pawn & King moves map on each square of the chess board.
     * Here, a "move" is defined as a movement that cannot capture any piece, such as a pawn move or a King castling.
     */
    U64 moves_on_each_square[64];

    /* Pawn EnPassen attack squares for both side */
    U64 EnPassen;

    /* The square that the allied pawn will end up after an EnPassen attacks.
     *
     * Index 1 (BLACK-WHITE): Which side is the Pawn on
     * Index 2 (0-8): There are 8 pawns, each resides on a different file (There can only be a SINGLE pawn reside on each file of the EnPassen Rank)
     * 
     * Also, each pawn can only enpassen ONE enemy pawn at a time. Set to NOT_A_SQUARE when no EnPassen attacks can be founded
     */
    Square PostEnPassen[2][8];

    /* Castle right for both sides. From LSB to MSB, King -> Queen side for White -> Black 
     * e.g., 0b00001111 means both side has castle right to both King and Queen side
     *       0b00000111 means Black has lost its right to castle Queen side
     */
    U8 castle_right;

    /* The pin positions of each side. If a piece is pin then it cannot move. 0 for Black, 1 for White */
    U64 pins[2];

    /* Set to high if the King is being checked */
    U8 checked[2];

    /* The attack positions for each of the attacker (PAWN, ROOK, etc) to the King */
    U64 checkedAttackers[2][5];

    /* Who to move. 0 for black and 1 for white */
    U8 side2play;

    /* 50 moves rules counter. The game is a draw if it is 0. Reset back to 100 if there is a pawn
     * advance or capturing from both sides. Decrement starts from the next moves after the most
     * recent 'reset' action.
     */
    U8 _50Count;

    /* Full move counter since the begining of the game. Increment by 1 after black makes a move */
    U16 Full_MoveCount;
    
} BitBoard;

#endif