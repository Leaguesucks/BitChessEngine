#include "MagicGen.h"

U64 random_U64(void) {
    U64 u1, u2, u3, u4;
    u1 = (U64)(random()) & 0xFFFF; u2 = (U64)(random()) & 0xFFFF;
    u3 = (U64)(random()) & 0xFFFF; u4 = (U64)(random()) & 0xFFFF;
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

U64 random_U64_with_more_1sBit(void) {
    return random_U64() & random_U64() & random_U64();
}

U64 Find_Magic_Number(Square sq, magic_piece mp) {
    U64 rev_squares[4096], attacks[4096], used_attacks[4096];
    U64 relevant_attacks;
    U32 relevant_attack_num;

    if (mp == BISHOP) {
        relevant_attacks = GetRelevantFreeBishopAttacks(sq);
        relevant_attack_num = NUM_BISHOP_RELEVANT_SQUARES[sq];
    }
    else {
        relevant_attacks = GetRelevantFreeRookAttacks(sq);
        relevant_attack_num = NUM_ROOK_RELEVANT_SQUARES[sq];
    }

    // For n relevant attacks, there 2^n different possible relevant attacks configurations
    U32 possible_relevant_attacks_config_num = 1 << relevant_attack_num;

    for (U32 i = 0; i < possible_relevant_attacks_config_num; i++) {
        rev_squares[i] = Find_Relevant_Squares(i, relevant_attacks);

        // Initiate all possible attack configurations given this square
        if (mp == BISHOP)
            attacks[i] = GetBlockedBishopAttacks(rev_squares[i], sq);
        else
            attacks[i] = GetBlockedRookAttacks(rev_squares[i], sq);
    }

    /* Generate and test for a magic number */
    for (U32 j = 0; j < 1000000000; j++) {
        U64 magic_number = random_U64_with_more_1sBit(); // Generate a random magic number

        // Skip inefficient magic numbers
        if (CountBits((relevant_attacks * magic_number) & 0xFF00000000000000) < 6)
            continue;

        memset(used_attacks, 0ULL, 4096 * sizeof(U64));

        U8 fail = 0;
        
        /* Test for the magic number */
        for (U32 k = 0; k < possible_relevant_attacks_config_num; k++) {
            U64 magic_index = (rev_squares[k] * magic_number) >> (64 - relevant_attack_num);

            if (used_attacks[magic_index] == 0ULL) // No collision
                used_attacks[magic_index] = attacks[k]; // This index has been used
            else { // Collision happens, this magic number doesnt work
                fail = 1;
                break;
            }
        }

        if (fail == 0) return magic_number; // Magic number success if there is no collision
    }

    // After 100000000 unsucessful attemps, return failure
    return 0ULL;
}

void Generate_And_Print_Magic_Numbers(FILE *f) {
    fprintf(f, "Magic number for bishops:\n\n");

    for (U32 attempt = 0; attempt < 100; attempt++) { // Try for 100 times
        Square i;
        for (i = A8; i <= H1; i++) {
            U64 magic_number = Find_Magic_Number(i, BISHOP);
            if (magic_number == 0ULL) {
                fprintf(f, "\nAttempt %d failed, retrying...\n", attempt);
                break;
            }
            fprintf(f, "0x%lxULL,\n", magic_number);
        }
        if (i >= 64) {
            fprintf(f, "\nMagic bishop success!\n\n");
            break;
        }
    }

    fprintf(f, "Magic number for rooks:\n\n");

    for (U32 attempt = 0; attempt < 100; attempt++) { // Try for 100 times
        Square i;
        for (i = A8; i <= H1; i++) {
            U64 magic_number = Find_Magic_Number(i, ROOK);
            if (magic_number == 0ULL) {
                fprintf(f, "\nAttempt %d failed, retrying...\n", attempt);
                break;
            }
            fprintf(f, "0x%lxULL,\n", magic_number);
        }
        if (i >= H1) {
            fprintf(f, "\nMagic rook success!\n\n");
            break;
        }
    }
}

/* Run this program to obtain the magic number */
int main(int argc, char* argv[]) {
    FILE *out = fopen("resources/Magic_Numbers.txt", "w");
    if (out == NULL) {
        perror("Cannot open out file for writing\n");
        exit(EXIT_FAILURE);
    }

    Generate_And_Print_Magic_Numbers(out);

    return 0;
}