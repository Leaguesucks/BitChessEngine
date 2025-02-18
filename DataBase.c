#include "DataBase.h"

U64 GetRelevantFreeBishopAttacks(Square sq) {
    U64 attacks = 0ULL;

    int curRk = (int) (sq / 8); // Curren rank (0-7, top to bottom)
    int curFl = (int) (sq % 8); // Current file (0-7, left to right)

    for (int rank = curRk - 1, file = curFl - 1; rank >= 1 && file >= 1; rank--, file--) attacks = SetBit(attacks, 1, (Square) (file + 8*rank)); // SW
    for (int rank = curRk - 1, file = curFl + 1; rank >= 1 && file <= 6; rank--, file++) attacks = SetBit(attacks, 1, (Square) (file + 8*rank)); // SE
    for (int rank = curRk + 1, file = curFl - 1; rank <= 6 && file >= 1; rank++, file--) attacks = SetBit(attacks, 1, (Square) (file + 8*rank)); // NW
    for (int rank = curRk + 1, file = curFl + 1; rank <= 6 && file <= 6; rank++, file++) attacks = SetBit(attacks, 1, (Square) (file + 8*rank)); // NE

    return attacks;
}

U64 GetRelevantFreeRookAttacks(Square sq) {
    U64 attacks = 0ULL;

    int curRk = (int) (sq / 8); // Current rank (0-7, top to bottom)
    int curFl = (int) (sq % 8); // Current file (0-7, left to right)

    for (int file = curFl - 1, decrement = 1; file >= 1; file--, decrement++) attacks = SetBit(attacks, 1, (Square) (sq - decrement)); // Left
    for (int file = curFl + 1, increment = 1; file <= 6; file++, increment++) attacks = SetBit(attacks, 1, (Square) (sq + increment)); // Right
    for (int rank = curRk - 1, decrement = 1; rank >= 1; rank--, decrement++) attacks = SetBit(attacks, 1, (Square) (sq - 8*decrement)); // Down
    for (int rank = curRk + 1, increment = 1; rank <= 6; rank++, increment++) attacks = SetBit(attacks, 1, (Square) (sq + 8*increment)); // Up

    return attacks;
}

U64 GetBlockedBishopAttacks(U64 b, Square sq) {
    U64 attacks = 0ULL;

    int curRk = (int) (sq / 8); // Curren rank (0-7, top to bottom)
    int curFl = (int) (sq % 8); // Current file (0-7, left to right)

    for (int rank = curRk - 1, file = curFl - 1; rank >= 0 && file >= 0; rank--, file--) { // SW
        if (GetBit(b, (Square) (file + 8*rank)) != 0) break; // Blocker encountered
        attacks = SetBit(attacks, 1, (Square) (file + 8*rank)); 
    }
    for (int rank = curRk - 1, file = curFl + 1; rank >= 0 && file <= 7; rank--, file++) { // SE
        if (GetBit(b, (Square) (file + 8*rank)) != 0) break; // Blocker encountered
        attacks = SetBit(attacks, 1, (Square) (file + 8*rank));
    }
    for (int rank = curRk + 1, file = curFl - 1; rank <= 7 && file >= 0; rank++, file--) { // NW
        if (GetBit(b, (Square) (file + 8*rank)) != 0) break; // Blocker encountered
        attacks = SetBit(attacks, 1, (Square) (file + 8*rank));
    }
    for (int rank = curRk + 1, file = curFl + 1; rank <= 7 && file <= 7; rank++, file++) { // NE
        if (GetBit(b, (Square) (file + 8*rank)) != 0) break; // Blocker encountered
        attacks = SetBit(attacks, 1, (Square) (file + 8*rank));
    }

    return attacks;
}

U64 GetBlockedRookAttacks(U64 b, Square sq) {
    U64 attacks = 0ULL;

    int curRk = (int) (sq / 8); // Current rank (0-7, top to bottom)
    int curFl = (int) (sq % 8); // Current file (0-7, left to right)

    for (int file = curFl - 1, decrement = 1; file >= 0; file--, decrement++) {
        if (GetBit(b, (Square) (sq - decrement)) != 0) break; // Blocker encountered
        attacks = SetBit(attacks, 1, (Square) (sq - decrement)); 
    }
    for (int file = curFl + 1, increment = 1; file <= 7; file++, increment++) { // Right
        if (GetBit(b, (Square) (sq + increment)) != 0) break; // Blocker encountered
        attacks = SetBit(attacks, 1, (Square) (sq + increment)); 
    }
    for (int rank = curRk - 1, decrement = 1; rank >= 0; rank--, decrement++) { // Down
        if (GetBit(b, (Square) (sq - 8*decrement)) != 0) break; // Blocker encountered
        attacks = SetBit(attacks, 1, (Square) (sq - 8*decrement)); 
    }
    for (int rank = curRk + 1, increment = 1; rank <= 7; rank++, increment++) { // Up
        if (GetBit(b, (Square) (sq + 8*increment)) != 0) break; // Blocker encountered
        attacks = SetBit(attacks, 1, (Square) (sq + 8*increment)); 
    }

    return attacks;
}

U64 Find_Relevant_Squares(U16 index, U64 rev_attacks) {
    U64 rev_squares = 0ULL;
    U8 NumBits = CountBits(rev_attacks, 1); // Number of relevant attacks

    for (U8 i = 0; i < NumBits; i++) {
        Square sq = Get_LSMB(rev_attacks, 1);
        rev_attacks = PopBit(rev_attacks, 1, sq);
        if ((index & (1 << i)) != 0) rev_squares != (REVERSE_ONE >> sq);
    }

    return rev_squares;
}
