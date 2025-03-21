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

/* Defines the castling sequences */

#define BLACK_CASTLE_RIGHT 0x7000000000000000ULL
#define BLACK_CASTLE_LEFT  0x0600000000000000ULL
#define WHITE_CASTLE_RIGHT 0x0000000000000070ULL
#define WHITE_CASTLE_LEFT  0x0000000000000006ULL

#define B -1.0f // Black side
#define W 1.0f  // White side

/* Chess pieces absolute values as evaluated by AlphaZero in 2020 */

/* Pawn absolute value */
#define PAWN_ABS_VAL 1.0f;

/* Rook absolute value */
#define ROOK_ABS_VAL 5.63f;

/* Knight absolute value */
#define KNIGHT_ABS_VAL 3.05f;

/* Bishop absolute value */
#define BISHOP_ABS_VAL 3.33f;

/* Queen absolute value */
#define QUEEN_ABS_VAL 9.5f;

/* King absolute value */
#define KING_ABS_VAL 1e6f;

#define CASTLE_RIGHT 1;
#define CASTLE_LEFT -1;

/* ***************************** */

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

typedef struct Values {
    float rev_val;       // Relative value of the piece right now, as evaluated by a chess engine for example
    U8  numMoves;        // Number of available moves for this piece
    Square Position;        // Current position of the piece
    U64 attacks;         // Its available attacks  
} Values;

/* Since pawn has distince moves and attacks, need a different struct type to represent it. This is the largest type in all pieces struct type */
typedef struct Pawn {
    float abs_val; // Absolute value of this piece.
    Values values;
    U64 moves;
} Pawn;

/* Rook also needs to be defined distinctively since we need to know which castling side does it link to */
typedef struct Rook {
    float abs_val; // Absolute value of this piece
    Values values;
    U8 castling_side; // Which casling side does this rook link to. 0 for left, 1 for right
} Rook;

/* Other pieces. That includes knights, bishops, and queens. King does not need a struct type since there can only be 1 king */
typedef struct Piece {
    float abs_val;
    Values values;
} Piece;

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

    /* There would be a maximum of 10 rooks, knights and bishops, 9 queens, 8 pawns and 1 king */
    Pawn  wPawns[8];
    Rook wRooks[10];
    Piece wKnights[10];
    Piece wBishops[10];
    Piece wQueens[9];

    Pawn  bPawns[8];
    Rook bRooks[10];
    Piece bKnights[10];
    Piece bBishops[10];
    Piece bQueens[9];

    /* Since there is only 1 king, we don't need to redefine it as type 'Piece' here */

    /* All pieces regular possible attacks */
    U64 atk_wPawns;
    U64 atk_wRooks;
    U64 atk_wKnights;
    U64 atk_wBishops;
    U64 atk_wQueens;
    U64 atk_wKing;
    U64 atk_wAll;

    U64 atk_bPawns;
    U64 atk_bRooks;
    U64 atk_bKnights;
    U64 atk_bBishops;
    U64 atk_bQueens;
    U64 atk_bKing;
    U64 atk_bAll;

    /* All possible X-Ray attacks */
    U64 atk_wXray;
    U64 atk_bXray;

    /* Pawn EnPassen for both side */
    U64 EnPassen;

    /* Catleing left and right positions for the king. A value of 0 means it is not possible to castle on that side anymore. 
     * Index 0 -> Left castle, index 1 -> right castle.
     */
    U64 wCastle[2];
    U64 bCastle[2];

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