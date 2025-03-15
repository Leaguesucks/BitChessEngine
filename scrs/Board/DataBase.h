/* Contains PreComputed Attacks Tables and helper functions
 * How the lookup tables work with magic numbers to help calculating the slider pieces move can be
 * found here:
 *
 * COPYRIGHT:  
 *      https://www.chessprogramming.org/Magic_Bitboards
 *      https://analog-hors.github.io/site/magic-bitboards/
 */

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "BitBoard.h"
#include "BitManipulation.h"

/* Number of relevant squares for each square the bishop resides in */
extern const U8 NUM_BISHOP_RELEVANT_SQUARES[64];

/* Number of relevant squares for each square the rook resides in */
extern const U8 NUM_ROOK_RELEVANT_SQUARES[64];

/* Magic number that to generate an index to access the rook attacks lookup table. This is 
 * created by MagicGen.c
 */
extern const U64 Rook_Magic_Numbers[64];

/* Magic number that to generate an index to access the bishop attacks lookup table. This is 
 * created by MagicGen.c
 */
extern const U64 Bishop_Magic_Numbers[64];

/* All possible attacks of each square of the bishop */
extern U64 Bishop_Attacks[64][512];

/* All possible attacks of each square of the rook */
extern U64 Rook_Attacks[64][4096];

/* Generate all relevant possible bishop attacks on a given square. The 'relevant' means we generate
 * all bishop attacks on an empty board, except the edges
 *
 * Para: sq -> The square which the bishop resides on
 * 
 * Return: All relevant possible bishop attacks on that square
 *
 *  NOTE: We also do not masked the current square which the bishop resides on
 */
U64 GetRelevantFreeBishopAttacks(Square sq);

/* Generate all relevant possible rook attacks on a given square. The 'relevant' means we generate
 * all rook attacks on an empty board, except the edges.
 *
 * Para: sq -> The square which the rook resides on
 * 
 * Return: All relevant possible rook attacks on that square
 * 
 * NOTE: We also do not masked the current square which the rook resides on
 */
U64 GetRelevantFreeRookAttacks(Square sq);

/* Generate all possible moves for a bishop on a given square on a bitboard, or in other word this
 * time we account for blockers. NOTE that different from the 'relevant' attacks, this time we
 * account for those attacks that reach the edges. The current square on which the bishop reside
 * will not be included, however
 * 
 * Para: b -> The bit board that contains blockers
 *       sq -> The current square on which the bishop reside on
 * 
 * Return: A bishop attacks map
 * 
 * NOTE: Both allied and enemy pieces are considered blockers here
 */
U64 GetBlockedBishopAttacks(U64 b, Square sq);

/* Generate all possible moves for a rook on a given square on a bitboard, or in other word this
 * time we account for blockers. NOTE that different from the 'relevant' attacks, this time we
 * account for those attacks that reach the edges. The current square on which the rook reside
 * will not be included, however
 * 
 * Para: b -> The bit board that contains blockers
 *       sq -> The current square on which the rook reside on
 * 
 * Return: A rook attacks map
 * 
 * NOTE: Both allied and enemy pieces are considered blockers here
 */
U64 GetBlockedRookAttacks(U64 b, Square sq);

/* Find the relevant occupied squares for rook and bishop
 *
 * Para: index -> From 0 - max 2^12 - 1 = 4095: Represents max 4096
 *                different relevant occupied squares of a rook 
 *                and 2^6 = 64 for a bishop for a given rook/bishop
 *                relevant attacks map
 *       rev_attacks -> Relevant attacks map of the rook/bishop
 * 
 * Return: The relevant squares of a relevant attacks map given an index
 */
U64 Find_Relevant_Squares(U32 index, U64 rev_attacks);

#endif