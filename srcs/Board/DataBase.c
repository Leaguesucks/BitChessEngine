#include "DataBase.h"

const U8 NUM_BISHOP_RELEVANT_SQUARES[64] =
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

const U8 NUM_ROOK_RELEVANT_SQUARES[64] =
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

const U64 RELEVANT_BISHOP_ATTACKS[64] =
{
    0x40201008040200ULL,
    0x20100804020000ULL,
    0x50080402000000ULL,
    0x28440200000000ULL,
    0x14224000000000ULL,
    0xa102040000000ULL,
    0x4081020400000ULL,
    0x2040810204000ULL,
    0x402010080400ULL,
    0x201008040200ULL,
    0x500804020000ULL,
    0x284402000000ULL,
    0x142240000000ULL,
    0xa1020400000ULL,
    0x40810204000ULL,
    0x20408102000ULL,
    0x40004020100800ULL,
    0x20002010080400ULL,
    0x50005008040200ULL,
    0x28002844020000ULL,
    0x14001422400000ULL,
    0xa000a10204000ULL,
    0x4000408102000ULL,
    0x2000204081000ULL,
    0x20400040201000ULL,
    0x10200020100800ULL,
    0x8500050080400ULL,
    0x44280028440200ULL,
    0x22140014224000ULL,
    0x100a000a102000ULL,
    0x8040004081000ULL,
    0x4020002040800ULL,
    0x10204000402000ULL,
    0x8102000201000ULL,
    0x4085000500800ULL,
    0x2442800284400ULL,
    0x40221400142200ULL,
    0x20100a000a1000ULL,
    0x10080400040800ULL,
    0x8040200020400ULL,
    0x8102040004000ULL,
    0x4081020002000ULL,
    0x2040850005000ULL,
    0x24428002800ULL,
    0x402214001400ULL,
    0x4020100a000a00ULL,
    0x20100804000400ULL,
    0x10080402000200ULL,
    0x4081020400000ULL,
    0x2040810200000ULL,
    0x20408500000ULL,
    0x244280000ULL,
    0x4022140000ULL,
    0x4020100a0000ULL,
    0x40201008040000ULL,
    0x20100804020000ULL,
    0x2040810204000ULL,
    0x20408102000ULL,
    0x204085000ULL,
    0x2442800ULL,
    0x40221400ULL,
    0x4020100a00ULL,
    0x402010080400ULL,
    0x40201008040200ULL
};

const U64 RELEVANT_ROOK_ATTACKS[64] =
{
    0x7e80808080808000ULL,
    0x3e40404040404000ULL,
    0x5e20202020202000ULL,
    0x6e10101010101000ULL,
    0x7608080808080800ULL,
    0x7a04040404040400ULL,
    0x7c02020202020200ULL,
    0x7e01010101010100ULL,
    0x7e808080808000ULL,
    0x3e404040404000ULL,
    0x5e202020202000ULL,
    0x6e101010101000ULL,
    0x76080808080800ULL,
    0x7a040404040400ULL,
    0x7c020202020200ULL,
    0x7e010101010100ULL,
    0x807e8080808000ULL,
    0x403e4040404000ULL,
    0x205e2020202000ULL,
    0x106e1010101000ULL,
    0x8760808080800ULL,
    0x47a0404040400ULL,
    0x27c0202020200ULL,
    0x17e0101010100ULL,
    0x80807e80808000ULL,
    0x40403e40404000ULL,
    0x20205e20202000ULL,
    0x10106e10101000ULL,
    0x8087608080800ULL,
    0x4047a04040400ULL,
    0x2027c02020200ULL,
    0x1017e01010100ULL,
    0x8080807e808000ULL,
    0x4040403e404000ULL,
    0x2020205e202000ULL,
    0x1010106e101000ULL,
    0x8080876080800ULL,
    0x404047a040400ULL,
    0x202027c020200ULL,
    0x101017e010100ULL,
    0x808080807e8000ULL,
    0x404040403e4000ULL,
    0x202020205e2000ULL,
    0x101010106e1000ULL,
    0x8080808760800ULL,
    0x40404047a0400ULL,
    0x20202027c0200ULL,
    0x10101017e0100ULL,
    0x80808080807e00ULL,
    0x40404040403e00ULL,
    0x20202020205e00ULL,
    0x10101010106e00ULL,
    0x8080808087600ULL,
    0x4040404047a00ULL,
    0x2020202027c00ULL,
    0x1010101017e00ULL,
    0x8080808080807eULL,
    0x4040404040403eULL,
    0x2020202020205eULL,
    0x1010101010106eULL,
    0x8080808080876ULL,
    0x404040404047aULL,
    0x202020202027cULL,
    0x101010101017eULL
};

const U64 BISHOP_MAGIC_NUMBERS[64] =
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

const U64 ROOK_MAGIC_NUMBERS[64] =
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

U64 Bishop_Attacks[64][512];
U64 Rook_Attacks[64][4096];

const U64 Knight_Attacks[64] =
{
    0x20400000000000ULL,
    0x10a00000000000ULL,
    0x88500000000000ULL,
    0x44280000000000ULL,
    0x22140000000000ULL,
    0x110a0000000000ULL,
    0x8050000000000ULL,
    0x4020000000000ULL,
    0x2000204000000000ULL,
    0x100010a000000000ULL,
    0x8800885000000000ULL,
    0x4400442800000000ULL,
    0x2200221400000000ULL,
    0x1100110a00000000ULL,
    0x800080500000000ULL,
    0x400040200000000ULL,
    0x4020002040000000ULL,
    0xa0100010a0000000ULL,
    0x5088008850000000ULL,
    0x2844004428000000ULL,
    0x1422002214000000ULL,
    0xa1100110a000000ULL,
    0x508000805000000ULL,
    0x204000402000000ULL,
    0x40200020400000ULL,
    0xa0100010a00000ULL,
    0x50880088500000ULL,
    0x28440044280000ULL,
    0x14220022140000ULL,
    0xa1100110a0000ULL,
    0x5080008050000ULL,
    0x2040004020000ULL,
    0x402000204000ULL,
    0xa0100010a000ULL,
    0x508800885000ULL,
    0x284400442800ULL,
    0x142200221400ULL,
    0xa1100110a00ULL,
    0x50800080500ULL,
    0x20400040200ULL,
    0x4020002040ULL,
    0xa0100010a0ULL,
    0x5088008850ULL,
    0x2844004428ULL,
    0x1422002214ULL,
    0xa1100110aULL,
    0x508000805ULL,
    0x204000402ULL,
    0x40200020ULL,
    0xa0100010ULL,
    0x50880088ULL,
    0x28440044ULL,
    0x14220022ULL,
    0xa110011ULL,
    0x5080008ULL,
    0x2040004ULL,
    0x402000ULL,
    0xa01000ULL,
    0x508800ULL,
    0x284400ULL,
    0x142200ULL,
    0xa1100ULL,
    0x50800ULL,
    0x20400ULL    
};

const U64 King_Attacks[64] =
{
    0x40c0000000000000ULL,
    0xa0e0000000000000ULL,
    0x5070000000000000ULL,
    0x2838000000000000ULL,
    0x141c000000000000ULL,
    0xa0e000000000000ULL,
    0x507000000000000ULL,
    0x203000000000000ULL,
    0xc040c00000000000ULL,
    0xe0a0e00000000000ULL,
    0x7050700000000000ULL,
    0x3828380000000000ULL,
    0x1c141c0000000000ULL,
    0xe0a0e0000000000ULL,
    0x705070000000000ULL,
    0x302030000000000ULL,
    0xc040c000000000ULL,
    0xe0a0e000000000ULL,
    0x70507000000000ULL,
    0x38283800000000ULL,
    0x1c141c00000000ULL,
    0xe0a0e00000000ULL,
    0x7050700000000ULL,
    0x3020300000000ULL,
    0xc040c0000000ULL,
    0xe0a0e0000000ULL,
    0x705070000000ULL,
    0x382838000000ULL,
    0x1c141c000000ULL,
    0xe0a0e000000ULL,
    0x70507000000ULL,
    0x30203000000ULL,
    0xc040c00000ULL,
    0xe0a0e00000ULL,
    0x7050700000ULL,
    0x3828380000ULL,
    0x1c141c0000ULL,
    0xe0a0e0000ULL,
    0x705070000ULL,
    0x302030000ULL,
    0xc040c000ULL,
    0xe0a0e000ULL,
    0x70507000ULL,
    0x38283800ULL,
    0x1c141c00ULL,
    0xe0a0e00ULL,
    0x7050700ULL,
    0x3020300ULL,
    0xc040c0ULL,
    0xe0a0e0ULL,
    0x705070ULL,
    0x382838ULL,
    0x1c141cULL,
    0xe0a0eULL,
    0x70507ULL,
    0x30203ULL,
    0xc040ULL,
    0xe0a0ULL,
    0x7050ULL,
    0x3828ULL,
    0x1c14ULL,
    0xe0aULL,
    0x705ULL,
    0x302ULL    
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

void Print_Relevant_Rook_Bishop_Attacks(char* fname) {
    FILE *f = (fname == NULL) ? stdout : fopen(fname, "w");
    if (f == NULL) {
        fprintf(stderr, "Error: Cannot open file \'%s\'\n", fname);
        exit(EXIT_FAILURE);
    }

    fprintf(f, "Relevant attacks for the bishops:\n\n");
    for (Square square = A8; square <= H1; square++)
        fprintf(f, "0x%lxULL,\n", GetRelevantFreeBishopAttacks(square));


    fprintf(f, "\nRelevant attacks for the rooks:\n\n");
    for (Square square = A8; square <= H1; square++)
        fprintf(f, "0x%lxULL,\n", GetRelevantFreeRookAttacks(square));

    fprintf(f, "\nRelevant rooks and bishops attacks generated successfully\n");

    if (f != stdout) fclose(f);
}

U8 Init_Rook_Bishop_Lookup_Table(void) {
    memset(Bishop_Attacks, 0, 512 * 64 * sizeof(U64));
    memset(Rook_Attacks, 0, 4096 * 64 * sizeof(U64));
    U8 success = 1;

    for (Square square = A8; square <= H1; square++) {
        U32 Num_Bishop_Relevant_Squares_Variations = 1 << NUM_BISHOP_RELEVANT_SQUARES[square];
        U32 Num_Rook_Relevant_Squares_Variations = 1 << NUM_ROOK_RELEVANT_SQUARES[square];

        // Load up the bishop lookup table
        for (U32 i = 0; i < Num_Bishop_Relevant_Squares_Variations; i++) {
            U64 Relevant_Squares = Find_Relevant_Squares(i, RELEVANT_BISHOP_ATTACKS[square]);
            U64 LookUp_Index = (Relevant_Squares * BISHOP_MAGIC_NUMBERS[square]) >> (64 - NUM_BISHOP_RELEVANT_SQUARES[square]);
            
            if (Bishop_Attacks[square][LookUp_Index] == 0ULL)
                Bishop_Attacks[square][LookUp_Index] = GetBlockedBishopAttacks(Relevant_Squares, square);
            else { // There is a collision
                fprintf(stderr, "ERROR: Bishop Magic Number for square %d = 0x%lx failed!\n", square, BISHOP_MAGIC_NUMBERS[square]);
                if (success != 0) {
                    success = 0;
                    fprintf(stderr, "Var Num %d: Bishop_Attacks[%d][%ld] = %ld\n", i, square, LookUp_Index, Bishop_Attacks[square][LookUp_Index]);
                    return success;
                }
            }
        }

        // Load up the rook lookup table
        for (U32 i = 0; i < Num_Rook_Relevant_Squares_Variations; i++) {
            U64 Relevant_Squares = Find_Relevant_Squares(i, RELEVANT_ROOK_ATTACKS[square]);
            U64 LookUp_Index = (Relevant_Squares * ROOK_MAGIC_NUMBERS[square]) >> (64 - NUM_ROOK_RELEVANT_SQUARES[square]);
            
            if (Rook_Attacks[square][LookUp_Index] == 0ULL)
                Rook_Attacks[square][LookUp_Index] = GetBlockedRookAttacks(Relevant_Squares, square);
            else { // There is a collision
                fprintf(stderr, "ERROR: ROOK Magic Number for square %d = 0x%lx failed!\n", square, ROOK_MAGIC_NUMBERS[square]);
                if (success != 0) {
                    success = 0;
                    fprintf(stderr, "Var Num %d: ROOK_Attacks[%d][%ld] = %ld\n", i, square, LookUp_Index, Rook_Attacks[square][LookUp_Index]);
                    return success;
                }
            }
        }
    }

    return success;
}

void Print_Knight_Attacks(char *fname) {
    FILE *f = (fname == NULL) ? stdout : fopen(fname, "w");
    if (f == NULL) {
        fprintf(stderr, "Error: Cannot open file \'%s\'\n", fname);
        exit(EXIT_FAILURE);
    }

    for (Square square = A8; square <= H1; square++) {
        int rank = ((int) square / 8) + 1; // Current rank of this square
        int file = ((int) square % 8) + 1; // Current file of this square
        U64 attacks = 0ULL;

        int newRank, newFile;
        if ((newRank = rank - 1) >= 1 && (newFile = file - 2) >= 1) // Short North Long West
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank - 2) >= 1 && (newFile = file - 1) >= 1) // Long North Short West
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank - 2) >= 1 && (newFile = file + 1) <= 8) // Long North Short East
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank - 1) >= 1 && (newFile = file + 2) <= 8) // Short North Long East
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 1) <= 8 && (newFile = file + 2) <= 8) // Short South Long East
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 2) <= 8 && (newFile = file + 1) <= 8) // Long South Short East
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 2) <= 8 && (newFile = file - 1) >= 1) // Long South Short West
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 1) <= 8 && (newFile = file - 2) >= 1) // Short South Long West
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        
        fprintf(f, "0x%lxULL,\n", attacks);
    }

    fclose(f);
}

void Print_King_Attacks(char *fname) {
    FILE *f = (fname == NULL) ? stdout : fopen(fname, "w");
    if (f == NULL) {
        fprintf(stderr, "Error: Cannot open file \'%s\'\n", fname);
        exit(EXIT_FAILURE);
    }

    for (Square square = A8; square <= H1; square++) {
        int rank = ((int) square / 8) + 1; // Current rank of this square
        int file = ((int) square % 8) + 1; // Current file of this square
        U64 attacks = 0ULL;

        int newFile, newRank;
        
        if ((newRank = rank - 1) >= 1 && (newFile = file - 1) >= 1) // NW
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank - 1) >= 1) // N
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (file - 1));
        if ((newRank = rank - 1) >= 1 && (newFile = file + 1) <= 8) // NE
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newFile = file + 1) <= 8) // E
            attacks = SetBit(attacks, 1, (rank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 1) <= 8 && (newFile = file + 1) <= 8) // SE
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile) - 1);
        if ((newRank = rank + 1) <= 8) // S
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (file - 1));
        if ((newRank = rank + 1) <= 8 && (newFile = file - 1) >= 1) // SW
            attacks = SetBit(attacks, 1, (newRank - 1) * 8 + (newFile - 1));
        if ((newFile = file - 1) >= 1) // W
            attacks = SetBit(attacks, 1, (rank - 1) * 8 + (newFile - 1));

        fprintf(f, "0x%lxULL,\n", attacks);
    }

    fclose(f);
}