#include "MoveGen.h"

static const Square ENPASSEN_CAPTURE[2][8] =
{
    {A3, B3, C3, D3, E3, F3, G3, H3},
    {A6, B6, C6, D6, E6, F6, G8, H6}
};

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

U64 Gen_Magic_Sliders_Attacks(const Square pos, const U64 blockers, const PNum sliders) {
    U64 rev_blockers, index;

    if (sliders == ROOK) {
        rev_blockers = RELEVANT_ROOK_ATTACKS[pos] & blockers; // Create a relevant blockers map
        index = (rev_blockers * ROOK_MAGIC_NUMBERS[pos]) >> (64 - NUM_ROOK_RELEVANT_SQUARES[pos]);

        return Rook_Attacks[pos][index];
    }
    else {
        rev_blockers = RELEVANT_BISHOP_ATTACKS[pos] & blockers; // Create a relevant blockers map
        index = (rev_blockers * BISHOP_MAGIC_NUMBERS[pos]) >> (64 - NUM_BISHOP_RELEVANT_SQUARES[pos]);

        return Bishop_Attacks[pos][index];
    }
}

U64 Gen_Rook_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 eKpos, U64 *covered, U64 *epins) {
    U64 blockers = (ally | enemy); // Create a blockers map
    U64 firstBlockers = Find_Sliders_First_Blockers(pos, blockers, ROOK); // Find the first blockers
    U64 init_attacks = Gen_Magic_Sliders_Attacks(pos, blockers, ROOK);

    /* Include the blockers in the covered squares */
    if (covered != NULL) *covered |= (init_attacks | firstBlockers );

    if (covered != NULL && epins != NULL) // Calculate the enemy's positions that are pinned by this rook
        Gen_Sliders_XRay_Attacks(pos, enemy, ally, eKpos, firstBlockers & enemy, ROOK, epins);

    /* Exclude the allies, include the enemies in the attack map */
    return (init_attacks & (~ally)) | (firstBlockers & enemy);
}

U64 Gen_Bishop_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 eKpos, U64 *covered, U64 *epins) {
    U64 blockers = (ally | enemy); // Create a blockers map
    U64 firstBlockers  = Find_Sliders_First_Blockers(pos, blockers, BISHOP); // Find the first blockers
    U64 init_attacks = Gen_Magic_Sliders_Attacks(pos, blockers, BISHOP);

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

U64 Gen_EnPassen_Attacks(const U64 initAtks, const U64 blockers, const U64 enpassen, const U8 side, U64 *covered, Square *postEnPass) {
    U64 PEnPass = initAtks & enpassen; // Potential enpassen attacks

    Square EnPassPos;                  // The potential EnPassen square
    Square EnPassCap;                  // The square that this pawn will move to after the EnPasse attack
    U64 enPassen_Rank_Map;             // A bit map of the EnPassen Rank
    U64 RawAttacks;                    // Raw attacks are pawn attacks without any EnPassen
    int eFile;                         // File of the potential enpassen attack

    if (side == WHITE)
        enPassen_Rank_Map = 0x00000000ff000000ULL;
    else
        enPassen_Rank_Map = 0x000000ff00000000ULL;

    RawAttacks = initAtks & (~enPassen_Rank_Map);

    /* Exclude the EnPassen attacks in a Pawn covered squares */
    *covered |= RawAttacks;

    U64 accepted_enpass = 0ULL;
    if (PEnPass) { // There can only be one EnPassen attack at a time
        EnPassPos = Get_LSMB(PEnPass);
        eFile     = EnPassPos % 8;
        EnPassCap = ENPASSEN_CAPTURE[side][eFile];
        if (!GetBit(blockers, EnPassCap)) { // If there is no blocker at the after capturing square
            accepted_enpass = SetBit(accepted_enpass, EnPassPos);
            *postEnPass = EnPassCap;
        }
        else
            *postEnPass = NOT_A_SQUARE;  
    }

    /* Return a new attack map with all accepted EnPassen attacks */
    return RawAttacks ^ accepted_enpass;
}

U64 Gen_Pawn_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 enpassen, const U8 side, U64 *covered, Square *postEnPass) {
    const U64 *attacks_table = (side == WHITE) ? WHITE_PAWN_ATTACKS : BLACK_PAWN_ATTACKS;
    U64 init_attacks = attacks_table[pos]; // The attack tables has EnPassen attacks as well

    init_attacks = Gen_EnPassen_Attacks(init_attacks, enemy | ally, enpassen, side, covered, postEnPass);

    /* Remove the allies and include the enemies in the attack map */
    return init_attacks & ~ally & enemy;
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

U64 Gen_King_Moves(const Square pos, const U64 blockers, const U64 ecovered, const U8 KC, const U8 QC, const U8 side) {
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

    U64 XRay_Attacks = new_attacks = 0ULL;
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

void Pre_Gen_Moves_Attacks(BitBoard *bb) {
    U8 side = bb->side2play;
    U8 eside = !side;

    bb->covered[side] = bb->all_attacks[side] = bb->pins[eside] = 0ULL;
    
    //memset(bb->atk_on_each_square, 0, 64 * sizeof(U64));
    //memset(bb->moves_on_each_square, 0, 64 * sizeof(U64)); // Halt for now for the sake of debugging
    memset(bb->checkedAttackers[eside], 0, 5 * sizeof(U64));

    bb->checked[eside] = 0;
}

// U64 Gen_Checked_Reponses(const U64 attacks, const U64 moves, const Square postEn, const PNum piece, const Square pos, const Square Kpos, const U64 *chSrcs, U64 *rMoves) {
//     U64 LchSrcs[5]; // The check sources from the enemy

//     memcpy(LchSrcs, chSrcs, 5 * sizeof(U64));


// }

void Set_Check(BitBoard *bb, const PNum attacker, const Square pos) {
    U8 side = bb->side2play;
    U8 eside = !side;

    if (attacker > QUEEN) // Extra safeguard: The King cannot checked each other
        return (void) 0;

    U64 attacks = bb->atk_on_each_square[pos];

    if (attacks & bb->positions[eside][KING]) { // If the enemy King positions is included in thid piece attack map
        if (!bb->checked[eside])
            bb->checked[eside] = 1;
        bb->checkedAttackers[eside][attacker] = SetBit(bb->checkedAttackers[eside][attacker], pos);
    }
}

U16 Set_And_Gen_Moves_Attacks(BitBoard *bb, const PNum piece, const Square pos) {
    U8 side = bb->side2play;
    U8 eside = !side;
    
    U8 K_Castle, Q_Castle; // Castling Rights
    U64 moves, attacks;

    if (piece == KING)
        if (side == WHITE) {
            K_Castle    = bb->castle_right & 1;
            Q_Castle    = bb->castle_right & 2;
        }
        else {
            K_Castle    = bb->castle_right & 4;
            Q_Castle    = bb->castle_right & 8;
        }
    else
        K_Castle = Q_Castle = 0;
    
    Square postEnpass = NOT_A_SQUARE; // Used to store the post EnPassen attack of a Pawn

    switch (piece) {
        case PAWN:
            moves = Gen_Pawn_Moves(pos, bb->all_positions[side] | bb->all_positions[eside], side);
            attacks = Gen_Pawn_Attacks(pos, bb->all_positions[eside], bb->all_positions[side], bb->EnPassen, side, &(bb->covered[side]), &postEnpass);
            break;

        case ROOK:
            attacks = Gen_Rook_Attacks(pos, bb->all_positions[eside], bb->all_positions[side], bb->positions[eside][KING], &(bb->covered[side]), &(bb->pins[eside]));
            moves = 0ULL;
            break;

        case KNIGHT:
            attacks = Gen_Knight_Attacks(pos, bb->all_positions[side], &(bb->covered[side]));
            moves = 0ULL;
            break;

        case BISHOP:
            attacks = Gen_Bishop_Attacks(pos, bb->all_positions[eside], bb->all_positions[side], bb->positions[eside][KING], &(bb->covered[side]), &(bb->pins[eside]));
            moves = 0ULL;
            break;

        case QUEEN:
            attacks = Gen_Rook_Attacks(pos, bb->all_positions[eside], bb->all_positions[side], bb->positions[eside][KING], &(bb->covered[side]), &(bb->pins[eside])) |
                      Gen_Bishop_Attacks(pos, bb->all_positions[eside], bb->all_positions[side], bb->positions[eside][KING], &(bb->covered[side]), &(bb->pins[eside]));
            moves = 0ULL;
            break;

        case KING:
            attacks = Gen_King_Attacks(pos, bb->all_positions[side], bb->covered[eside], &(bb->covered[side]));
            moves   = Gen_King_Moves(pos, bb->all_positions[eside] | bb->all_positions[side], bb->covered[eside], K_Castle, Q_Castle, side);
            break;

        case EMPTY:
            return 0;
    }

    if (!(SetBit(0ULL, pos) & bb->pins[side])) { // If this piece is not being pinned
        bb->atk_on_each_square[pos] = attacks;
        bb->moves_on_each_square[pos] = moves;
        Set_Check(bb, piece, pos);
    }
    else // This piece is being pinned. NOTE that even if a piece is pinned we stil calculate and keep their convered square
        bb->atk_on_each_square[pos] = bb->moves_on_each_square[pos] = attacks = moves = 0ULL;

    return (U16) CountBits(attacks) + (U16) CountBits(moves);
}

U16 Gen_All_Moves_Attacks(BitBoard *bb) {
    Pre_Gen_Moves_Attacks(bb);

    U64 positions[6]; // Positions of all allied pieces

    U16 atk_counts = 0;
    Square pos = NOT_A_SQUARE;

    memcpy(positions, bb->positions[bb->side2play], 6 * sizeof(U64));

    for (PNum p = PAWN; p <= KING; p++)
        while (positions[p] != 0ULL) {
            pos = Get_LSMB(positions[p]);
            positions[p] = PopBit(positions[p], pos);
            atk_counts += Set_And_Gen_Moves_Attacks(bb, p, pos);
        }

    return atk_counts;
}

U64 Draw_BB_Line(Square src, Square des) {
    if (src == NOT_A_SQUARE || des == NOT_A_SQUARE)
        return 0ULL;

    int srcRank, srcFile, desRank, desFile; // Rank, file of the source and destination
    int RankDiff, FileDiff;                 // Rank and file different from the source to the destination
    Direction direction;                    // Direction from source to destination
    
    U64 board, ray;
    board = ray = 0ULL;
    board = SetBit(board, src);
    board = SetBit(board, des);

    srcRank = src / 8;
    srcFile = src % 8;
    desRank = des / 8;
    desFile = des % 8;

    RankDiff = srcRank - desRank;
    FileDiff = srcFile - desFile;

    direction = NOT_A_DIRECTION;
    if (RankDiff == 0 && FileDiff < 0)
        direction = EAST;
    else if (RankDiff == 0 && FileDiff > 0)
        direction = WEST;
    else if (FileDiff == 0 && RankDiff < 0)
        direction = SOUTH;
    else if (FileDiff == 0 && RankDiff > 0)
        direction = NORTH;
    else if (RankDiff < 0 && RankDiff == FileDiff)
        direction = SE;
    else if (RankDiff < 0 && RankDiff == -FileDiff)
        direction = SW;
    else if (RankDiff > 0 && RankDiff == FileDiff)
        direction = NW;
    else if (RankDiff > 0 && RankDiff == -FileDiff)
        direction = NE;
    else
        return 0ULL;

    Direction start = (direction % 2) ? 1 : 0;

    for (Direction d = start; d < 8; d += 2) // Include pseudo blockers in all other directions
        if (d != direction)
            board |= Sliders_Rays[src][d];

    ray = (!start) ? Gen_Magic_Sliders_Attacks(src, board, ROOK) : Gen_Magic_Sliders_Attacks(src, board, BISHOP);

    /* Remove the destination */
    return ray & ~SetBit(0ULL, des);
}