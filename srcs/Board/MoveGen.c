#include "MoveGen.h"

U64 Gen_Sliding_Rook_Attacks(U64 allies, U64 enemies, Square pos) {
    U64 bb_noEdges = (allies | enemies) & (~EDGES); // Remove the pieces on the edges of a blockers map

    U64 index = (bb_noEdges * ROOK_MAGIC_NUMBERS[pos]) >> (64 - NUM_ROOK_RELEVANT_SQUARES[pos]);
    U64 init_attacks = Rook_Attacks[pos][index];

    /* Exclude allies in the attack map, include the enemies */
    return (init_attacks ^ allies) | enemies;
}