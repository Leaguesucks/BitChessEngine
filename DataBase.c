#include "DataBase.h"

extern const U8 NUM_BISHOP_RELEVANT_SQUARES[64] =
{
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

extern const U8 NUM_ROOK_RELEVANT_SQUARES[64] =
{
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

extern const U64 Bishop_Magic_Numbers[64] =
{
    0x420c80100408202ULL,
    0x1063090051100ULL,
    0x32a0201061110104ULL,
    0x501025140082210ULL,
    0x820040120a801ULL,
    0x2014820084012100ULL,
    0x81008201012100ULL,
    0x2402208124020ULL,
    0x402080809104010ULL,
    0x42c0100111310000ULL,
    0x104002040440b0ULL,
    0x31020220250ULL,
    0x20020880c00ULL,
    0x8000d20222090000ULL,
    0x308c04202000ULL,
    0x2020282400042ULL,
    0x1044010673011201ULL,
    0x200484004a4092ULL,
    0x40010208a2000100ULL,
    0x28088902400401ULL,
    0x2000014200804800ULL,
    0xa803040044000840ULL,
    0x802010121020801ULL,
    0x100903050200800ULL,
    0x12600800a0044ULL,
    0x4044612400020080ULL,
    0x910004080831000ULL,
    0x2430008200002200ULL,
    0x100200800650104ULL,
    0x4002080040500ULL,
    0x42101105020200ULL,
    0x801480803401000ULL,
    0x490840840100ULL,
    0x2408002009000ULL,
    0x10004102081208ULL,
    0x100102500400cULL,
    0x8440040018410120ULL,
    0x1000410008160400ULL,
    0xa801200088083110ULL,
    0x10400808024400ULL,
    0x80020a088201ULL,
    0x601000261105000ULL,
    0x4063020a00610c00ULL,
    0x8809000820082000ULL,
    0x8000a20801010ULL,
    0x10000200204100ULL,
    0x281c082001120614ULL,
    0x8040011010024089ULL,
    0x208000828c012000ULL,
    0xc0401880831ULL,
    0x121110080020ULL,
    0x408040308064302ULL,
    0x285040400880000ULL,
    0x287801022011ULL,
    0x2c00200808a10240ULL,
    0x80110308210400ULL,
    0x20081080384048aULL,
    0x22021022088120ULL,
    0x660888a008080200ULL,
    0x1104008820000ULL,
    0x2007240022032000ULL,
    0x829080820040a00dULL,
    0x2004104401002088ULL,
    0xa12010940080a0c2ULL
};

extern const U64 Rook_Magic_Numbers[64] =
{
    0x800008044002102ULL,
    0x1000a1281204ULL,
    0x8531000208040001ULL,
    0x9001001042080005ULL,
    0x8a01000410002009ULL,
    0x400200040081101ULL,
    0x190080400421ULL,
    0x111004221908009ULL,
    0x800041002080ULL,
    0x20128900400ULL,
    0x44004002010040ULL,
    0x40008008080ULL,
    0x420008201200ULL,
    0x4012004080102200ULL,
    0x4c0200040100040ULL,
    0x400080002280ULL,
    0x24008100420004ULL,
    0x126010208440010ULL,
    0x100040002008080ULL,
    0xc1001008010004ULL,
    0x1020100008008080ULL,
    0x130004020010100ULL,
    0x1120005000c002ULL,
    0x8050802040008004ULL,
    0x411084402000091ULL,
    0x1018800100800200ULL,
    0x2000204008010410ULL,
    0x480005001100ULL,
    0xc430001080800800ULL,
    0x5c04108202002040ULL,
    0x408a054382002100ULL,
    0x7080004000402000ULL,
    0x4008054200032194ULL,
    0x4888084400011002ULL,
    0x100040080020080ULL,
    0x81001100080084ULL,
    0x2030080080100080ULL,
    0x8010080020040022ULL,
    0x4040200880400080ULL,
    0x8040400080008032ULL,
    0x2024020000b40541ULL,
    0x6004080400100ULL,
    0x809010002080400ULL,
    0xa14008004080080ULL,
    0x408010100201000ULL,
    0x7011460020118200ULL,
    0x8a0004000500025ULL,
    0x1180014000402000ULL,
    0x102000c02045181ULL,
    0x602a004802000104ULL,
    0x8402000410080200ULL,
    0x8800800800040080ULL,
    0x8001001000200902ULL,
    0x108808020001000ULL,
    0x2803002181400300ULL,
    0x8003800080400020ULL,
    0x4300004022008100ULL,
    0x300110009d40200ULL,
    0x180020001800400ULL,
    0x8280040002800800ULL,
    0x4100041000210008ULL,
    0x8180100080200108ULL,
    0x440001000402000ULL,
    0x80058055204000ULL
};

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

U64 Find_Relevant_Squares(U32 index, U64 rev_attacks) {
    U64 rev_squares = 0ULL;
    U8 NumBits = CountBits(rev_attacks, 1); // Number of relevant attacks

    for (U8 i = 0; i < NumBits; i++) {
        Square sq = Get_LSMB(rev_attacks, 1);
        rev_attacks = PopBit(rev_attacks, 1, sq);
        if ((index & (1 << i)) != 0) rev_squares = SetBit(rev_squares, 1, sq);
    }

    return rev_squares;
}
