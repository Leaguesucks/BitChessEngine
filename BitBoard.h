/* The Bit representation of the chess board 
 *
 * Here, the chess board is represented by a 64 bits unsigned int, the index
 * of each square is mapped from right to left, bottom to top
 * 
 * NOTE: The functions declared in this header file are EXTREMELY UNSAFE to trade off
 * for performance. Use these functions with through error checkings beforehand
 */

#ifndef _BITBOARD_H_
#define _BITBOARD_H_

#include <math.h>

// 0b 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
#define INIT_BLACK_PAWNS    0x00ff000000000000UL
// 0b 1000 0001 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
#define INIT_BLACK_ROOKS    0x8100000000000000UL
// 0b 0100 0010 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
#define INIT_BLACK_KNIGHTS  0x4200000000000000UL
// 0b 0010 0100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
#define INIT_BLACK_BISHOPS  0x2400000000000000UL
// 0b 0001 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
#define INIT_BLACK_QUEEN    0x1000000000000000UL
// 0b 0000 1000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
#define INIT_BLACK_KING     0x0800000000000000UL
// 0b 1111 1111 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
#define INIT_BLACK_ALL      0xffff000000000000UL

// 0b 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000
#define INIT_WHITE_PAWNS    0x000000000000ff00UL
// 0b 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1000 0001
#define INIT_WHITE_ROOKS    0x0000000000000081UL
// 0b 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0100 0010
#define INIT_WHITE_KNIGHTS  0x0000000000000042UL
// 0b 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0010 0100
#define INIT_WHITE_BISHOPS  0x0000000000000024UL
// 0b 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0001 0000
#define INIT_WHITE_QUEEN    0x0000000000000010UL
// 0b 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1000
#define INIT_WHITE_KING     0x0000000000000008UL
// 0b 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 1111 1111
#define INIT_WHITE_ALL      0x000000000000ffffUL

// 0b 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0010 0010
#define INIT_WHITE_CASTLE   0x0000000000000022UL
// 0b 0010 0010 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
#define INIT_BLACK_CASTLE   0x2200000000000000UL

/* Defines the edges of the board */
#define ROW_RIGHT_EDGE      0x0000000000000001UL
#define ROW_LEFT_EDGE       0x0000000000000080UL

/* Absolute weight of the pieces, can be used to identify the pieces' identity as well */
#define PAWN   1.0f
#define ROOK   5.0f
#define KNIGHT 3.0f
#define BISHOP 3.25f
#define QUEEN  9.0f
#define KING   INFINITY

typedef unsigned char      U8 ;
typedef unsigned short int U16;
typedef unsigned       int U32;
typedef unsigned long  int U64;

typedef struct {
    U64 wPawns;
    U64 wKnights;
    U64 wBishops;
    U64 wRooks;
    U64 wQueen;
    U64 wKing;

    U64 bPawns;
    U64 bKnights;
    U64 bBishops;
    U64 bRooks;
    U64 bQueen;
    U64 bKing;

    U64 enPassen;

    U64 wPins;
    U64 bPins;

    U64 wPawnsAttack;
    U64 wRooksAttack;
    U64 wKnightsAttack;
    U64 wBishopsAttack;
    U64 wQueenAttack;
    U64 wKingAttack;

    U64 bPawnsAttack;
    U64 bRooksAttack;
    U64 bKnightsAttack;
    U64 bBishopsAttack;
    U64 bQueenAttack;
    U64 bKingAttack;

    /* Pawns is the only pieces that move different that attack -> Need another variable to store their movement */
    U64 wPawnsMove;
    U64 bPawnsMove;

    U64 wCastle;
    U64 bCastle;

    U64 wAll; // White all pieces
    U64 bAll; // Black all pieces

    U8 wKingBeingChecked;
    U8 bKingBeingChecked;

} BitBoard;

typedef enum Direction {
    LEFT     ,
    RIGHT    ,
    UP       ,
    DOWN     ,
    UPLEFT   ,
    UPRIGHT  ,
    DOWNLEFT ,
    DOWNRIGHT,
} Direction;

typedef enum Side {
    BLACK,
    WHITE
} Side;

typedef enum Collison_Type {
    FRIEND_COLLISION,
    ENEMY_COLLISION ,
    NO_COLLISION
} Collision;

/* Initialize the bit board. Should be called first to start a new game 
 * Para:
 *      b -> The bit board
 */
void init_board(BitBoard* b) {
    b->wPawns            = INIT_WHITE_PAWNS   ;
    b->wRooks            = INIT_WHITE_ROOKS   ;
    b->wKnights          = INIT_WHITE_KNIGHTS ;
    b->wBishops          = INIT_WHITE_BISHOPS ;
    b->wQueen            = INIT_WHITE_QUEEN   ;
    b->wKing             = INIT_WHITE_KING    ;

    b->bPawns            = INIT_BLACK_PAWNS   ;
    b->bRooks            = INIT_BLACK_ROOKS   ;
    b->bKnights          = INIT_BLACK_KNIGHTS ;
    b->bBishops          = INIT_BLACK_BISHOPS ;
    b->bQueen            = INIT_BLACK_QUEEN   ;
    b->bKing             = INIT_BLACK_KING    ;

    b->wCastle           = INIT_WHITE_CASTLE  ;
    b->bCastle           = INIT_BLACK_CASTLE  ;

    b->wAll              = INIT_WHITE_ALL     ;
    b->bAll              = INIT_BLACK_ALL     ;

    b->enPassen          = 0                  ;

    b->wPins             = 0                  ;
    b->bPins             = 0                  ;

    b->wPawnsAttack      = 0                  ;
    b->wRooksAttack      = 0                  ;
    b->wKnightsAttack    = 0                  ;
    b->wBishopsAttack    = 0                  ;
    b->wQueenAttack      = 0                  ;
    b->wKingAttack       = 0                  ;

    b->bPawnsAttack      = 0                  ;
    b->bRooksAttack      = 0                  ;
    b->bKnightsAttack    = 0                  ;
    b->bBishopsAttack    = 0                  ;
    b->bQueenAttack      = 0                  ;
    b->bKingAttack       = 0                  ;

    b->wPawnsMove        = 0                  ;
    b->bPawnsMove        = 0                  ;

    b->wKingBeingChecked = 0                  ;
    b->bKingBeingChecked = 0                  ;
}

/* Find the row number based on a position
 * This function is UNSAFE
 *
 * Para: 
 *      The current position
 * 
 * Return: 
 *      The row num (index from 0-7)
 */
int FindRow(U64 pos) {
    int row   = 0;
    U64 rowEnd   = 128UL;

    int division;
    while ((division = (pos>>(row*8))/rowEnd > 0) && division != 1) row++;

    return row;
}

/* Map one position to another by moving it either left/right or up/down 
 * Para:
 *      pos -> Current position
 *      dir -> Direction to move to
 *      sq  -> How many squares to move in that direction
 * 
 * Direction:
 *      LEFT, RIGHT, UP, DOWN, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT
 * 
 * Return:
 *      New position, 0 if cannot move in that direction
 */
U64 MoveTo(U64 pos, Direction dir, U32 sq) {
    int row = FindRow(pos);
    U64 newpos;
    
    const U64 RightEdge = ROW_RIGHT_EDGE;
    const U64 LeftEdge = ROW_LEFT_EDGE;

    switch (dir) {
        case LEFT:
            newpos = pos<<sq;
            if (newpos > LeftEdge<<row) return 0; // Hit the left edge
            return newpos;

        case RIGHT:
            newpos = pos>>sq;
            if (newpos < RightEdge<<row) return 0; // Hit the right edge
            return newpos;

        case UP:
            newpos = pos<<(8*sq);
            return newpos;

        case DOWN:
            newpos = pos>>(8*sq);
            return newpos;

        case UPLEFT:
            newpos = pos<<sq;
            if (newpos > LeftEdge<<row) return 0; // Hit the left edge
            newpos <<= (8*sq);
            return newpos;

        case UPRIGHT:
            newpos = pos>>sq;
            if (newpos < RightEdge<<row) return 0; // Hit the right edge
            newpos <<= (8*sq);
            return newpos;

        case DOWNLEFT:
            newpos = pos<<sq;
            if (newpos > ROW_LEFT_EDGE<<row) return 0; // Hit the left edge
            newpos >>= (8*sq);
            return newpos;

        case DOWNRIGHT:
            newpos = pos>>sq;
            if (newpos < ROW_RIGHT_EDGE<<row) return 0; // Hit the right edge
            newpos >>= (8*sq);
            return newpos;

        default:
            return 0;
    }
}

/* Check if a position collides with an allied or an enemy 
 *
 * Para:
 *      b -> The bit board
 *      p -> Position of interest
 *      s -> The side to play: BLACK or WHITE
 * 
 * Return:
 *      Collision type: FRIEND_COLLISION, ENEMY_COLLISION or NO_COLLISION
 */
Collision CheckCollision(BitBoard *b, U64 p, Side s) {
    switch (s) {
        case WHITE:
            if (p & b->wAll) { return FRIEND_COLLISION; }
            if (p & b->bAll) { return ENEMY_COLLISION ; }
                               return NO_COLLISION    ;

        case BLACK:
            if (p & b->bAll) { return FRIEND_COLLISION; }
            if (p & b->wAll) { return ENEMY_COLLISION ; }
                               return NO_COLLISION    ;

        default:
            return NO_COLLISION;
    }
}

/* Calculate all possible move for the pawns 
 *
 * Para:
 *      b -> The bit board
 *      p -> Current position of the pawn
 *      s -> The side to play: BLACK or WHITE
 * 
 * Return:
 *      It will store all possible pawn move back to the bit board
 */
void PawnMove(BitBoard *b, U64 p, Side s) {
    U64 newpos = 0;
    switch (s) {
        case WHITE:
            if ((newpos = MoveTo(p, UP, 1)) == 0) return;
            if (CheckCollision(b, newpos, s) != NO_COLLISION) return;
            b->wPawnsMove ^= newpos;
            if (!(b->wPawns & INIT_WHITE_PAWNS)) return;
            if ((newpos = MoveTo(p, UP, 2)) == 0) return;
            if (CheckCollision(b, newpos, s) != NO_COLLISION) return;
            b->wPawnsMove ^= newpos;
            return;

        case BLACK:
            if ((newpos = MoveTo(p, DOWN, 1)) == 0) return;
            if (CheckCollision(b, newpos, s) != NO_COLLISION) return;
            b->bPawnsMove ^= newpos;
            if (!(b->bPawns & INIT_BLACK_PAWNS)) return;
            if ((newpos = MoveTo(p, DOWN, 2)) == 0) return;
            if (CheckCollision(b, newpos, s) != NO_COLLISION) return;
            b->bPawnsMove ^= newpos;
            return;
    }
}

#endif