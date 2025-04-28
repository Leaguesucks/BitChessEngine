#include "MoveGen.h"

U64 Find_Sliders_First_Blockers(const Square pos, const U64 blockers, const float slider) {
    U64 firstBlockers = 0ULL;

    if (slider == ROOK_ABS_VAL) {
        U64 N_Blockers = Sliders_Rays[pos][NORTH] & blockers;
        U64 S_Blockers = Sliders_Rays[pos][SOUTH] & blockers;
        U64 W_Blockers = Sliders_Rays[pos][WEST]  & blockers;
        U64 E_Blockers = Sliders_Rays[pos][EAST]  & blockers;
    
        if (N_Blockers)
            firstBlockers = SetBit(firstBlockers, 1, Get_LSMB(N_Blockers, 1));
        if (S_Blockers)
            firstBlockers = SetBit(firstBlockers, 1, Get_MSMB(S_Blockers, 1));
        if (W_Blockers)
            firstBlockers = SetBit(firstBlockers, 1, Get_LSMB(W_Blockers, 1));
        if (E_Blockers)
            firstBlockers = SetBit(firstBlockers, 1, Get_MSMB(E_Blockers, 1));    
    }
    else if (slider == BISHOP_ABS_VAL) {
        U64 NE_Blockers = Sliders_Rays[pos][NE] & blockers;
        U64 NW_Blockers = Sliders_Rays[pos][NW] & blockers;
        U64 SE_Blockers = Sliders_Rays[pos][SE] & blockers;
        U64 SW_Blockers = Sliders_Rays[pos][SW] & blockers;

        if (NE_Blockers)
            firstBlockers = SetBit(firstBlockers, 1, Get_LSMB(NE_Blockers, 1));
        if (NW_Blockers)
            firstBlockers = SetBit(firstBlockers, 1, Get_LSMB(NW_Blockers, 1));
        if (SE_Blockers)
            firstBlockers = SetBit(firstBlockers, 1, Get_MSMB(SE_Blockers, 1));
        if (SW_Blockers)
            firstBlockers = SetBit(firstBlockers, 1, Get_MSMB(SW_Blockers, 1));
    }
    else // No need to account for the Queen or other pieces
        return 0ULL;

    return firstBlockers;
}

U64 Gen_Rook_Attacks(const Square pos, const U64 enemy, const U64 ally) {
    U64 blockers = (ally | enemy); // Create a blockers map
    U64 rev_blockers = RELEVANT_ROOK_ATTACKS[pos] & blockers; // Create a relevant blockers map

    U64 firstBlockers = Find_Sliders_First_Blockers(pos, blockers, ROOK_ABS_VAL); // Find the first blockers

    U64 index = (rev_blockers * ROOK_MAGIC_NUMBERS[pos]) >> (64 - NUM_ROOK_RELEVANT_SQUARES[pos]);
    U64 init_attacks = Rook_Attacks[pos][index];

    /* Exclude the allies, include the enemies in the attack map */
    return (init_attacks & (~ally)) | (firstBlockers & enemy);
}

U64 Gen_Bishop_Attacks(const Square pos, const U64 enemy, const U64 ally) {
    U64 blockers = (ally | enemy); // Create a blockers map
    U64 rev_blockers = RELEVANT_BISHOP_ATTACKS[pos] & blockers; // Create a relevant blockers map

    U64 firstBlockers  = Find_Sliders_First_Blockers(pos, blockers, BISHOP_ABS_VAL); // Find the first blockers

    U64 index = (rev_blockers * BISHOP_MAGIC_NUMBERS[pos]) >> (64 - NUM_BISHOP_RELEVANT_SQUARES[pos]);
    U64 init_attacks = Bishop_Attacks[pos][index];

    /* Exclude the allies, include the enemies in the attack map */
    return (init_attacks & (~ally)) | (firstBlockers & enemy);
}

U64 Gen_Knight_Attacks(const Square pos, const U64 ally) {
    U64 init_attacks = KNIGHT_ATTACKS[pos];

    /* Exclude the allies and include the enemies in the attack map */
    return (init_attacks & (~ally));
}

U64 Gen_Pawn_Attacks(const Square pos, const U64 enemy, const U64 ally, const float side) {
    int rank = (int) (pos / 8); // 0-7, top -> bottom
    int file = (int) (pos % 8); // 0-7, left -> right

    U64 *attacks_table = (side == W) ? &WHITE_PAWN_ATTACKS : &BLACK_PAWN_ATTACKS;
    U64 init_attacks = attacks_table[pos];

    /* If a pawn is on its "EnPassen Rank" then we will include its EnPassen attacks */
    if (side == W)
        if (rank == 4) { // EnPassen Rank for white

        }

    /* Exclude the allies and include the enemies and the Enpassen enemies in the attack map */
    return (init_attacks & (~ally)) | enemy;
}

U64 Gen_Pawn_Moves(const Square pos, const U64 blockers, const float side) {
    U64 moves = 0ULL;
    U8 num_moves = 0;

    if (side == W) {
        num_moves = (pos >= A2 && pos <= H2) ? 2 : 1; // First rank pawns can moves 2 squarea

    }
}

U16 Gen_All_Attacks(BitBoard *bb, const float side) {
    U64 pos_Allies, pos_Enemies;
    U64 pos_Pawns, pos_Rooks, pos_Knights, pos_Bishops, pos_Queens, pos_King;
    U8 numPawns, numRooks, numKnights, numBishops, numQueens;
    U8 K_Castle, Q_Castle; // Castling right

    if (side == W) {
        pos_Allies  = bb->pos_wAll;
        pos_Enemies = bb->pos_bAll;

        pos_Pawns   = bb->pos_wPawns;
        pos_Rooks   = bb->pos_wRooks;
        pos_Knights = bb->pos_wKnights;
        pos_Bishops = bb->pos_wBishops;
        pos_Queens  = bb->pos_wQueens;
        pos_King    = bb->pos_wKing;

        numPawns    = bb->num_wPawns;
        numRooks    = bb->num_wRooks;
        numKnights  = bb->num_wKnights;
        numBishops  = bb->num_wBishops;
        numQueens   = bb->num_wQueens;

        K_Castle    = GetBit((U64) bb->castle_right, (Square) H1);
        Q_Castle    = GetBit((U64) bb->castle_right, (Square) G1);
    }
    else {
        pos_Allies  = bb->pos_bAll;
        pos_Enemies = bb->pos_wAll;

        pos_Pawns   = bb->pos_bPawns;
        pos_Rooks   = bb->pos_bRooks;
        pos_Knights = bb->pos_bKnights;
        pos_Bishops = bb->pos_bBishops;
        pos_Queens  = bb->pos_bQueens;
        pos_King    = bb->pos_bKing;

        numPawns    = bb->num_bPawns;
        numRooks    = bb->num_bRooks;
        numKnights  = bb->num_bKnights;
        numBishops  = bb->num_bBishops;
        numQueens   = bb->num_bQueens;

        K_Castle    = GetBit((U64) bb->castle_right, (Square) F1);
        Q_Castle    = GetBit((U64) bb->castle_right, (Square) E1);
    }

    U8 atk_counts = 0;
    U64 Free_Atk = 0ULL;
    Square pos = NOT_A_SQUARE;

    /* Find attacks for the Rooks */
    for (U8 i = 0; i < numRooks; i++) {
        pos = Get_LSMB(pos_Rooks, 1);
        pos_Rooks = PopBit(pos_Rooks, 1, pos);
        Free_Atk = Gen_Rook_Attacks(pos, pos_Enemies, pos_Allies);
        bb->atk_on_each_square[pos] = Free_Atk;
        atk_counts += (U16) CountBits(Free_Atk, 1);
    }

    /* Find attacks for the Bishops */
    for (U8 i = 0; i < numBishops; i++) {
        pos = Get_LSMB(pos_Bishops, 1);
        pos_Bishops = PopBit(pos_Bishops, 1, pos);
        Free_Atk = Gen_Bishop_Attacks(pos, pos_Enemies, pos_Allies);
        bb->atk_on_each_square[pos] = Free_Atk;
        atk_counts += (U16) CountBits(Free_Atk, 1);
    }

    /* Find attacks for the Queens */
    for (U8 i = 0; i < numQueens; i++) {
        pos = Get_LSMB(pos_Queens, 1);
        pos_Queens = PopBit(pos_Queens, 1, pos);
        Free_Atk = 0ULL;
        Free_Atk |= Gen_Rook_Attacks(pos, pos_Enemies, pos_Allies);
        Free_Atk |= Gen_Bishop_Attacks(pos, pos_Enemies, pos_Allies);
        bb->atk_on_each_square[pos] = Free_Atk;
        atk_counts += (U16) CountBits(Free_Atk, 1);
    }

    /* Find attacks for the Knights */
    for (U8 i = 0; i < numKnights; i++) {
        pos = Get_LSMB(pos_Knights, 1);
        pos_Knights = PopBit(pos_Knights, 1, pos);
        Free_Atk = Gen_Knight_Attacks(pos, pos_Allies);
        bb->atk_on_each_square[pos] = Free_Atk;
        atk_counts += (U16) CountBits(Free_Atk, 1);
    }

    return atk_counts;
}