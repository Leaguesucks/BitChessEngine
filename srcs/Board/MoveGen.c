#include "MoveGen.h"

U64 Gen_Free_Rook_Attacks(Square pos, U64 enemy, U64 ally) {
    U64 blockers = (ally | enemy); // Create a blockers map
    U64 rev_blockers = RELEVANT_ROOK_ATTACKS[pos] & blockers; // Create a relevant blockers map

    /* Find the postions of the first blockers */

    U64 N_Blockers = Sliders_Rays[pos][NORTH] & blockers;
    U64 S_Blockers = Sliders_Rays[pos][SOUTH] & blockers;
    U64 W_Blockers = Sliders_Rays[pos][WEST]  & blockers;
    U64 E_Blockers = Sliders_Rays[pos][EAST]  & blockers;

    U64 firstBlockers = 0ULL;
    if (N_Blockers)
        firstBlockers = SetBit(firstBlockers, 1, Get_LSMB(N_Blockers, 1));
    if (S_Blockers)
        firstBlockers = SetBit(firstBlockers, 1, Get_MSMB(S_Blockers, 1));
    if (W_Blockers)
        firstBlockers = SetBit(firstBlockers, 1, Get_LSMB(W_Blockers, 1));
    if (E_Blockers)
        firstBlockers = SetBit(firstBlockers, 1, Get_MSMB(E_Blockers, 1));

    U64 index = (rev_blockers * ROOK_MAGIC_NUMBERS[pos]) >> (64 - NUM_ROOK_RELEVANT_SQUARES[pos]);
    U64 init_attacks = Rook_Attacks[pos][index];

    /* Exclude the allies, include the enemies in the attack map  */
    return (init_attacks & (~ally)) | (firstBlockers & enemy);
}

U8 Gen_Rook_Attacks(BitBoard *bb, float side) {
    U64 pos_Allies, pos_Enemies, pos_Rooks;
    U8 numRooks;

    if (side == W) {
        pos_Allies  = bb->pos_wAll;
        pos_Enemies = bb->pos_bAll;
        pos_Rooks   = bb->pos_wRooks;
        numRooks    = bb->num_wRooks;
    }
    else{
        pos_Allies  = bb->pos_bAll;
        pos_Enemies = bb->pos_wAll;
        pos_Rooks   = bb->pos_bRooks;
        numRooks    = bb->num_bRooks;
    }

    U8 atk_counts = 0;
    for (U8 i = 0; i < numRooks; i++) {
        Square pos = Get_LSMB(pos_Rooks, 1);
        pos_Rooks = PopBit(pos_Rooks, 1, pos);
        U64 Free_Atk = Gen_Free_Rook_Attacks(pos, pos_Enemies, pos_Allies);
        bb->atk_on_each_square[pos] |= Free_Atk;
        atk_counts += CountBits(Free_Atk, 1);
    }

    return atk_counts;
}