/* The Bit representation of the chess board*/

#ifndef _BITBOARD_H_
#define _BITBOARD_H_

#define BLACK 0
#define WHITE 1

#define RIGHT 0
#define LEFT  1

#define UP    0
#define DOWN  1

typedef unsigned char      U8 ;
typedef unsigned short int U16;
typedef unsigned       int U32;
typedef unsigned long  int U64;

typedef struct {
    U64 wPawns;
    U64 wKnights;
    U64 wBishops;
    U64 wRooks;
    U64 wQueens;
    U64 wKing;

    U64 bPawns;
    U64 bKnights;
    U64 bBishops;
    U64 bRooks;
    U64 bQueens;
    U64 bKing;
} BitBoard;


#endif