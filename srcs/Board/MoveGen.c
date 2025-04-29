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

U64 Gen_Rook_Attacks(const Square pos, const U64 enemy, const U64 ally, U64 *covered) {
    U64 blockers = (ally | enemy); // Create a blockers map
    U64 rev_blockers = RELEVANT_ROOK_ATTACKS[pos] & blockers; // Create a relevant blockers map

    U64 firstBlockers = Find_Sliders_First_Blockers(pos, blockers, ROOK_ABS_VAL); // Find the first blockers

    U64 index = (rev_blockers * ROOK_MAGIC_NUMBERS[pos]) >> (64 - NUM_ROOK_RELEVANT_SQUARES[pos]);
    U64 init_attacks = Rook_Attacks[pos][index];

    /* Include the blockers in the covered squares */
    *covered |= (init_attacks | firstBlockers );

    /* Exclude the allies, include the enemies in the attack map */
    return (init_attacks & (~ally)) | (firstBlockers & enemy);
}

U64 Gen_Bishop_Attacks(const Square pos, const U64 enemy, const U64 ally, U64 *covered) {
    U64 blockers = (ally | enemy); // Create a blockers map
    U64 rev_blockers = RELEVANT_BISHOP_ATTACKS[pos] & blockers; // Create a relevant blockers map

    U64 firstBlockers  = Find_Sliders_First_Blockers(pos, blockers, BISHOP_ABS_VAL); // Find the first blockers

    U64 index = (rev_blockers * BISHOP_MAGIC_NUMBERS[pos]) >> (64 - NUM_BISHOP_RELEVANT_SQUARES[pos]);
    U64 init_attacks = Bishop_Attacks[pos][index];

    /* Include blockers in the covered squares */
    *covered |= (init_attacks | firstBlockers);

    /* Exclude the allies, include the enemies in the attack map */
    return (init_attacks & (~ally)) | (firstBlockers & enemy);
}

U64 Gen_Knight_Attacks(const Square pos, const U64 ally, U64 *covered) {
    U64 init_attacks = KNIGHT_ATTACKS[pos];
    *covered |= init_attacks; // A Knight's covered squares are all possible squares that the Knight can move to

    /* Exclude the allies and include the enemies in the attack map */
    return (init_attacks & (~ally));
}

U64 Gen_Pawn_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 enpassen, const float side, U64 *covered) {
    const U64 *attacks_table = (side == W) ? WHITE_PAWN_ATTACKS : BLACK_PAWN_ATTACKS;
    U64 init_attacks = attacks_table[pos]; // The attack tables has EnPassen attacks as well

    /* The pawns cannot EnPassen any other pieces that is not a pawn. Therefore, we would not say an EnPassen square is "covered" by the pawn */
    U64 enPassen_Rank = (side == W) ? 0x00000000ff000000ULL : 0x000000ff00000000ULL;
    /* Exclude the EnPassen attacks in a Pawn covered squares */
    *covered |= (init_attacks & (~enPassen_Rank));

    init_attacks &= ~ally; // Remove the allies from the attack map

    /* Include the enemies and the Enpassen enemies in the attack map */
    return init_attacks & (enemy | enpassen);
}

U64 Gen_Pawn_Moves(const Square pos, const U64 blockers, const float side) {
    U64 moves = 0ULL;
    int num_moves = 0;
    int rank, file, newRank;

    rank = (int) (pos / 8); // 0-7, top -> bottom
    file = (int) (pos % 8); // 0-7, left -> right

    if (side == W) {
        num_moves = (pos >= A2 && pos <= H2) ? 2 : 1; // First rank pawns can moves 2 squares

        for (int i = 1; i <= num_moves; i++) 
            if ((newRank = rank - i) >= 0) {
                moves = SetBit(moves, 1, (Square) (newRank * 8 + file));
                if (moves & blockers) break;
            }
    }
    else {
        num_moves = (pos >= A7 && pos <= H7) ? 2 : 1; // First rank pawns can moves 2 squares

        for (int i = 1; i <= num_moves; i++)
            if ((newRank = rank + i) <= 7) {
                moves = SetBit(moves, 1, (Square) (newRank * 8 + file));
                if (moves & blockers) break;
            }
    }

    /* Exclude all blockers in the movement map */
    return moves & (~blockers);
}

U64 Gen_King_Attacks(const Square pos, const U64 ally, const U64 ecovered, U64 *covered) {
    U64 init_attacks = KING_ATTACKS[pos];

    /* For King's covered squares are all its possible attacks */
    *covered |= init_attacks;

    /* Exclude the allies, include the enemy in the attack map. Also the King cannot moves to a square that is covered by the enemy */
    return init_attacks & (~ally) & (~ecovered);
}

U64 Gen_King_Moves(const Square pos, const U64 blockers, const U64 ecovered, const U8 KC, const U8 QC, const float side) {
    U64 Kcastling_sequence;
    U64 Qcastling_sequence;
    Square KMove;
    Square QMove;

    if (side == W) {
        Kcastling_sequence = WHITE_CASTLE_KING_SEQUENCES  & (~0x0101010101010101ULL);  // Remove the King edge
        Qcastling_sequence = WHITE_CASTLE_QUEEN_SEQUENCES & (~0x8080808080808080ULL);  // Remove the Queen edge
        KMove = G1;
        QMove = C1;
    }
    else {
        Kcastling_sequence = BLACK_CASTLE_KING_SEQUENCES  & (~0x0101010101010101ULL); // Remove the King edge
        Qcastling_sequence = BLACK_CASTLE_QUEEN_SEQUENCES & (~0x8080808080808080ULL); // Remove the Queen edge
        KMove = G8;
        QMove = C8;
    }

    U64 moves = 0ULL;
    U64 local_blockers = PopBit(blockers, 1, pos); // Remove the King's position from the blockers map

    if (KC)
        if ((Kcastling_sequence & (local_blockers | ecovered)) == 0) // There is no obstacle on the King side
            moves = SetBit(moves, 1, KMove);
    
    if (QC)
        if ((Qcastling_sequence & (local_blockers | ecovered)) == 0) // There is no obstacle on the Queen side
            moves = SetBit(moves, 1, QMove);

    return moves;
}

U16 Gen_All_Moves_Attacks(BitBoard *bb) {
    U64 pos_Allies, pos_Enemies;
    U64 pos_Pawns, pos_Rooks, pos_Knights, pos_Bishops, pos_Queens, pos_King;
    U64 eCovered;
    U64 *aCovered; // A pointer to the allies' covered map
    U8 numPawns, numRooks, numKnights, numBishops, numQueens;
    U8 K_Castle, Q_Castle; // Castling right

    const float side = bb->side2play;

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

        K_Castle    = bb->castle_right & 1;
        Q_Castle    = bb->castle_right & 2;

        eCovered    = bb->bCovered;
        aCovered    = &(bb->wCovered);
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

        K_Castle    = bb->castle_right & 4;
        Q_Castle    = bb->castle_right & 8;

        eCovered    = bb->wCovered;
        aCovered    = &(bb->bCovered);
    }

    U16 atk_counts = 0;
    Square pos = NOT_A_SQUARE;

    /* Find attacks for the Rooks */
    for (U8 i = 0; i < numRooks; i++) {
        pos = Get_LSMB(pos_Rooks, 1);
        pos_Rooks = PopBit(pos_Rooks, 1, pos);
        bb->atk_on_each_square[pos] = Gen_Rook_Attacks(pos, pos_Enemies, pos_Allies, aCovered);
        atk_counts += (U16) CountBits(bb->atk_on_each_square[pos], 1);
    }

    /* Find attacks for the Bishops */
    for (U8 i = 0; i < numBishops; i++) {
        pos = Get_LSMB(pos_Bishops, 1);
        pos_Bishops = PopBit(pos_Bishops, 1, pos);
        bb->atk_on_each_square[pos] = Gen_Bishop_Attacks(pos, pos_Enemies, pos_Allies, aCovered);
        atk_counts += (U16) CountBits(bb->atk_on_each_square[pos], 1);
    }

    /* Find attacks for the Queens */
    for (U8 i = 0; i < numQueens; i++) {
        pos = Get_LSMB(pos_Queens, 1);
        pos_Queens = PopBit(pos_Queens, 1, pos);
        bb->atk_on_each_square[pos] = (Gen_Rook_Attacks(pos, pos_Enemies, pos_Allies, aCovered) | Gen_Bishop_Attacks(pos, pos_Enemies, pos_Allies, aCovered));
        atk_counts += (U16) CountBits(bb->atk_on_each_square[pos], 1);
    }

    /* Find attacks for the Knights */
    for (U8 i = 0; i < numKnights; i++) {
        pos = Get_LSMB(pos_Knights, 1);
        pos_Knights = PopBit(pos_Knights, 1, pos);
        bb->atk_on_each_square[pos] = Gen_Knight_Attacks(pos, pos_Allies, aCovered);
        atk_counts += (U16) CountBits(bb->atk_on_each_square[pos], 1);
    }

    /* Find moves and attacks for the Pawns */
    for (U8 i = 0; i < numPawns; i++) {
        pos = Get_LSMB(pos_Pawns, 1);
        pos_Pawns = PopBit(pos_Pawns, 1, pos);
        bb->atk_on_each_square[pos]   = Gen_Pawn_Attacks(pos, pos_Enemies, pos_Allies, bb->EnPassen, side, aCovered);
        bb->moves_on_each_square[pos] = Gen_Pawn_Moves(pos, pos_Enemies | pos_Allies, side);
        atk_counts += (U16) CountBits(bb->atk_on_each_square[pos], 1);
        atk_counts += (U16) CountBits(bb->moves_on_each_square[pos], 1);
    }

    /* Find moves and attacks for the King */
    pos = Get_LSMB(pos_King, 1);
    bb->atk_on_each_square[pos]   = Gen_King_Attacks(pos, pos_Allies, eCovered, aCovered);
    bb->moves_on_each_square[pos] = Gen_King_Moves(pos, pos_Enemies | pos_Allies, eCovered, K_Castle, Q_Castle, side);
    atk_counts += (U16) CountBits(bb->atk_on_each_square[pos], 1);
    atk_counts += (U16) CountBits(bb->moves_on_each_square[pos], 1);

    return atk_counts;
}