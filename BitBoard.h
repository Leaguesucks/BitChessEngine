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

typedef struct BitBoard {
    
    /* Postions */
    U64 wPawns;
    U64 wRooks;
    U64 wKnights;
    U64 wBishops;
    U64 wQueen;
    U64 wKing;

    U64 bPawns;
    U64 bRooks;
    U64 bKnights;
    U64 bBishops;
    U64 bQueen;
    U64 bKing;
    
} BitBoard;

#endif