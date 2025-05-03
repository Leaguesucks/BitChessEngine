#include "MoveGen.h"

U64 Find_Sliders_First_Blockers(const Square pos, const U64 blockers, const PNum slider) {
    U64 firstBlockers = 0ULL;

    if (slider == ROOK) {
        U64 N_Blockers = Sliders_Rays[pos][NORTH] & blockers;
        U64 S_Blockers = Sliders_Rays[pos][SOUTH] & blockers;
        U64 W_Blockers = Sliders_Rays[pos][WEST]  & blockers;
        U64 E_Blockers = Sliders_Rays[pos][EAST]  & blockers;
    
        if (N_Blockers)
            firstBlockers = SetBit(firstBlockers, Get_LSMB(N_Blockers));
        if (S_Blockers)
            firstBlockers = SetBit(firstBlockers, Get_MSMB(S_Blockers));
        if (W_Blockers)
            firstBlockers = SetBit(firstBlockers, Get_LSMB(W_Blockers));
        if (E_Blockers)
            firstBlockers = SetBit(firstBlockers, Get_MSMB(E_Blockers));    
    }
    else if (slider == BISHOP) {
        U64 NE_Blockers = Sliders_Rays[pos][NE] & blockers;
        U64 NW_Blockers = Sliders_Rays[pos][NW] & blockers;
        U64 SE_Blockers = Sliders_Rays[pos][SE] & blockers;
        U64 SW_Blockers = Sliders_Rays[pos][SW] & blockers;

        if (NE_Blockers)
            firstBlockers = SetBit(firstBlockers, Get_LSMB(NE_Blockers));
        if (NW_Blockers)
            firstBlockers = SetBit(firstBlockers, Get_LSMB(NW_Blockers));
        if (SE_Blockers)
            firstBlockers = SetBit(firstBlockers, Get_MSMB(SE_Blockers));
        if (SW_Blockers)
            firstBlockers = SetBit(firstBlockers, Get_MSMB(SW_Blockers));
    }
    else // No need to account for the Queen or other pieces
        return 0ULL;

    return firstBlockers;
}

U64 Gen_Rook_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 eKpos, U64 *covered, U64 *epins) {
    U64 blockers = (ally | enemy); // Create a blockers map
    U64 rev_blockers = RELEVANT_ROOK_ATTACKS[pos] & blockers; // Create a relevant blockers map

    U64 firstBlockers = Find_Sliders_First_Blockers(pos, blockers, ROOK); // Find the first blockers

    U64 index = (rev_blockers * ROOK_MAGIC_NUMBERS[pos]) >> (64 - NUM_ROOK_RELEVANT_SQUARES[pos]);
    U64 init_attacks = Rook_Attacks[pos][index];

    /* Include the blockers in the covered squares */
    if (covered != NULL) *covered |= (init_attacks | firstBlockers );

    if (covered != NULL && epins != NULL) // Calculate the enemy's positions that are pinned by this rook
        Gen_Sliders_XRay_Attacks(pos, enemy, ally, eKpos, firstBlockers & enemy, ROOK, epins);

    /* Exclude the allies, include the enemies in the attack map */
    return (init_attacks & (~ally)) | (firstBlockers & enemy);
}

U64 Gen_Bishop_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 eKpos, U64 *covered, U64 *epins) {
    U64 blockers = (ally | enemy); // Create a blockers map
    U64 rev_blockers = RELEVANT_BISHOP_ATTACKS[pos] & blockers; // Create a relevant blockers map

    U64 firstBlockers  = Find_Sliders_First_Blockers(pos, blockers, BISHOP); // Find the first blockers

    U64 index = (rev_blockers * BISHOP_MAGIC_NUMBERS[pos]) >> (64 - NUM_BISHOP_RELEVANT_SQUARES[pos]);
    U64 init_attacks = Bishop_Attacks[pos][index];

    /* Include blockers in the covered squares */
    if (covered != NULL) *covered |= (init_attacks | firstBlockers);

    if (covered != NULL && epins != NULL) // Calculate the enemy's positions that are pinned by this bishop
        Gen_Sliders_XRay_Attacks(pos, enemy, ally, eKpos, firstBlockers & enemy, BISHOP, epins);

    /* Exclude the allies, include the enemies in the attack map */
    return (init_attacks & (~ally)) | (firstBlockers & enemy);
}

U64 Gen_Knight_Attacks(const Square pos, const U64 ally, U64 *covered) {
    U64 init_attacks = KNIGHT_ATTACKS[pos];
    *covered |= init_attacks; // A Knight's covered squares are all possible squares that the Knight can move to

    /* Exclude the allies and include the enemies in the attack map */
    return (init_attacks & (~ally));
}

U64 Gen_Pawn_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 enpassen, const U8 side, U64 *covered) {
    const U64 *attacks_table = (side == WHITE) ? WHITE_PAWN_ATTACKS : BLACK_PAWN_ATTACKS;
    U64 init_attacks = attacks_table[pos]; // The attack tables has EnPassen attacks as well

    /* The pawns cannot EnPassen any other pieces that is not a pawn. Therefore, we would not say an EnPassen square is "covered" by the pawn */
    U64 enPassen_Rank = (side == WHITE) ? 0x00000000ff000000ULL : 0x000000ff00000000ULL;
    /* Exclude the EnPassen attacks in a Pawn covered squares */
    *covered |= (init_attacks & (~enPassen_Rank));

    init_attacks &= ~ally; // Remove the allies from the attack map

    /* Include the enemies and the Enpassen enemies in the attack map */
    return init_attacks & enpassen & enemy;
}

U64 Gen_Pawn_Moves(const Square pos, const U64 blockers, const U8 side) {
    U64 moves = 0ULL;
    int num_moves = 0;
    int rank, file, newRank;

    rank = (int) (pos / 8); // 0-7, top -> bottom
    file = (int) (pos % 8); // 0-7, left -> right

    if (side == WHITE) {
        num_moves = (pos >= A2 && pos <= H2) ? 2 : 1; // First rank pawns can moves 2 squares

        for (int i = 1; i <= num_moves; i++) 
            if ((newRank = rank - i) >= 0) {
                moves = SetBit(moves, (Square) (newRank * 8 + file));
                if (moves & blockers) break;
            }
    }
    else {
        num_moves = (pos >= A7 && pos <= H7) ? 2 : 1; // First rank pawns can moves 2 squares

        for (int i = 1; i <= num_moves; i++)
            if ((newRank = rank + i) <= 7) {
                moves = SetBit(moves, (Square) (newRank * 8 + file));
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

    if (side == WHITE) {
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
    U64 local_blockers = PopBit(blockers, pos); // Remove the King's position from the blockers map

    if (KC)
        if ((Kcastling_sequence & (local_blockers | ecovered)) == 0) // There is no obstacle on the King side => Can castle King side
            moves = SetBit(moves, KMove);
    
    if (QC)
        if ((Qcastling_sequence & (local_blockers | ecovered)) == 0) // There is no obstacle on the Queen side => Can castle Queen side
            moves = SetBit(moves, QMove);

    return moves;
}

U64 Gen_Sliders_XRay_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 eKpos, const U64 eblockers, const PNum slider, U64 *epins) {
    U64 local_eBlockers = eblockers;
    U64 local_enemy = enemy;
    Square pos_eBlockers;  // Postion of the enemy blockers
    U64 new_attacks;

    U64 XRay_Attacks = 0ULL;
    while (local_eBlockers) {
        pos_eBlockers = Get_LSMB(local_eBlockers); // Get the enemy blockers position
        local_eBlockers = PopBit(local_eBlockers, pos_eBlockers);
        local_enemy = PopBit(local_enemy, pos_eBlockers); // Remove the enemy blockers from current positions

        if (slider == ROOK)
            new_attacks = Gen_Rook_Attacks(pos, local_enemy, ally, eKpos, NULL, NULL);
        else // Assume it is a bishops. We don't care if it is the Queen or Bishop or any other pieces since this function is used by sliders attacks generation functions anyway
            new_attacks = Gen_Bishop_Attacks(pos, local_enemy, ally, eKpos, NULL, NULL);

        if (new_attacks & eKpos) // If the enemy King is under attacks after an enemy blocker is removed, then that means that blocker is pinned
            *epins = SetBit(*epins, pos_eBlockers); 

        XRay_Attacks |= new_attacks;
    }

    return XRay_Attacks;
}

U16 Gen_All_Moves_Attacks(BitBoard *bb) {
    U64 pos_Allies, pos_Enemies;
    U64 positions[6];      // Allies' positions of each piece
    U64 pos_eKing;         // Position of the enemies' King
    U64 eCovered;          // Enemies covered positions
    U64 aPinned;           // Allies pinned positions
    U64 *aCovered;         // A pointer to the allies' covered map
    U64 *ePinned;          // A pointer to the enemies' pinned pieces
    U64 *aAtks;            // A pointer to the allies' all attacks map
    U8 K_Castle, Q_Castle; // Castling Rights

    U8 side = bb->side2play;
    U8 eside;

    if (side == WHITE) {
        eside = BLACK;
        K_Castle    = bb->castle_right & 1;
        Q_Castle    = bb->castle_right & 2;
    }
    else {
        eside = WHITE;
        K_Castle    = bb->castle_right & 4;
        Q_Castle    = bb->castle_right & 8;
    }

    for (PNum p = PAWN; p <= KING; p++)
        positions[p] = bb->positions[side][p];

    pos_Allies  = bb->all_positions[side];
    pos_Enemies = bb->all_positions[eside];
    pos_eKing   = bb->positions[eside][KING];
    eCovered    = bb->covered[eside];
    aPinned     = bb->pins[side];
    aCovered    = &(bb->covered[side]);
    ePinned     = &(bb->pins[eside]);
    aAtks       = &(bb->all_attacks[side]);

    U16 atk_counts = 0;
    Square pos = NOT_A_SQUARE;

    *aCovered = *ePinned = *aAtks = 0ULL;

    /* Find attacks for the Rooks */
    while (positions[ROOK] != 0ULL) {
        pos = Get_LSMB(positions[ROOK]);
        positions[ROOK] = PopBit(positions[ROOK], pos);
        if ((SetBit(0ULL, pos) & aPinned) == 0ULL) {
            bb->atk_on_each_square[pos] = Gen_Rook_Attacks(pos, pos_Enemies, pos_Allies, pos_eKing, aCovered, ePinned);
            atk_counts += (U16) CountBits(bb->atk_on_each_square[pos]);
        }
        else // This piece is being pinned
            bb->atk_on_each_square[pos] = 0ULL;

        *aAtks |= bb->atk_on_each_square[pos];
    }

    /* Find attacks for the Bishops */
    while (positions[BISHOP] != 0ULL) {
        pos = Get_LSMB(positions[BISHOP]);
        positions[BISHOP] = PopBit(positions[BISHOP], pos);
        if ((SetBit(0ULL, pos) & aPinned) == 0ULL) {
            bb->atk_on_each_square[pos] = Gen_Bishop_Attacks(pos, pos_Enemies, pos_Allies, pos_eKing, aCovered, ePinned);
            atk_counts += (U16) CountBits(bb->atk_on_each_square[pos]);
        }
        else // This piece is being pinned
            bb->atk_on_each_square[pos] = 0ULL;

        *aAtks |= bb->atk_on_each_square[pos];
    }

    /* Find attacks for the Queens */
    while (positions[QUEEN] != 0ULL) {
        pos = Get_LSMB(positions[QUEEN]);
        positions[QUEEN] = PopBit(positions[QUEEN], pos);
        if ((SetBit(0ULL, pos) & aPinned) == 0ULL) {
            bb->atk_on_each_square[pos] = (Gen_Rook_Attacks(pos, pos_Enemies, pos_Allies, pos_eKing, aCovered, ePinned) | 
                                           Gen_Bishop_Attacks(pos, pos_Enemies, pos_Allies, pos_eKing, aCovered, ePinned));
            atk_counts += (U16) CountBits(bb->atk_on_each_square[pos]);
        }
        else // This piece is being pinned
            bb->atk_on_each_square[pos] = 0ULL;

        *aAtks |= bb->atk_on_each_square[pos];
    }

    /* Find attacks for the Knights */
    while (positions[KNIGHT] != 0ULL) {
        pos = Get_LSMB(positions[KNIGHT]);
        positions[KNIGHT] = PopBit(positions[KNIGHT], pos);
        if ((SetBit(0ULL, pos) & aPinned) == 0ULL) {
            bb->atk_on_each_square[pos] = Gen_Knight_Attacks(pos, pos_Allies, aCovered);
            atk_counts += (U16) CountBits(bb->atk_on_each_square[pos]);
        }
        else // This piece is being pinned
            bb->atk_on_each_square[pos] = 0ULL;

        *aAtks |= bb->atk_on_each_square[pos];
    }

    /* Find moves and attacks for the Pawns */
    while (positions[PAWN] != 0ULL) {
        pos = Get_LSMB(positions[PAWN]);
        positions[PAWN] = PopBit(positions[PAWN], pos);
        if ((SetBit(0ULL, pos) & aPinned) == 0ULL) {
            bb->atk_on_each_square[pos]   = Gen_Pawn_Attacks(pos, pos_Enemies, pos_Allies, bb->EnPassen, side, aCovered);
            bb->moves_on_each_square[pos] = Gen_Pawn_Moves(pos, pos_Enemies | pos_Allies, side);
            atk_counts += (U16) CountBits(bb->atk_on_each_square[pos]);
            atk_counts += (U16) CountBits(bb->moves_on_each_square[pos]);
        }
        else { // This piece is being pinned
            bb->atk_on_each_square[pos] = 0ULL;
            bb->moves_on_each_square[pos] = 0ULL;
        }
         
        *aAtks |= bb->atk_on_each_square[pos];
    }

    /* Find moves and attacks for the King. Note that technically the King cannot be pinned */
    pos = Get_LSMB(positions[KING]);
    bb->atk_on_each_square[pos]   = Gen_King_Attacks(pos, pos_Allies, eCovered, aCovered);
    bb->moves_on_each_square[pos] = Gen_King_Moves(pos, pos_Enemies | pos_Allies, eCovered, K_Castle, Q_Castle, side);
    atk_counts += (U16) CountBits(bb->atk_on_each_square[pos]);
    atk_counts += (U16) CountBits(bb->moves_on_each_square[pos]);
    *aAtks |= bb->atk_on_each_square[pos];

    return atk_counts;
}