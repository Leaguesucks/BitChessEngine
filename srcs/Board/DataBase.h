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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "BitBoard.h"
#include "BitManipulation.h"

/* Number of relevant squares for each square the bishop resides in */
extern const U8 NUM_BISHOP_RELEVANT_SQUARES[64];

/* Number of relevant squares for each square the rook resides in */
extern const U8 NUM_ROOK_RELEVANT_SQUARES[64];

/* Magic number that to generate an index to access the rook attacks lookup table. This is 
 * created by MagicGen.c
 */
extern const U64 ROOK_MAGIC_NUMBERS[64];

/* Magic number that to generate an index to access the bishop attacks lookup table. This is 
 * created by MagicGen.c
 */
extern const U64 BISHOP_MAGIC_NUMBERS[64];

/* Relevant bishop attacks */
extern const U64 RELEVANT_BISHOP_ATTACKS[64];

/* Relevant rook attaacks */
extern const U64 RELEVANT_ROOK_ATTACKS[64];

/* All possible attacks of each square of the bishop */
extern U64 Bishop_Attacks[64][512];

/* All possible attacks of each square of the rook */
extern U64 Rook_Attacks[64][4096];

/* All possible attacks for each square of the knights */
extern const U64 Knight_Attacks[64];

/* All possible attacks for each square of the king. This means no castling here */
extern const U64 King_Attacks[64];

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
 *                and 2^9 = 512 for a bishop for a given rook/bishop
 *                relevant attacks map
 *       rev_attacks -> Relevant attacks map of the rook/bishop
 * 
 * Return: The relevant squares variation of a relevant attacks map given an index
 * 
 * e.g., if there is a rooks on H1, then its relevant occupied squares would be:
 * 
 * 
 *         A B C D E F G H  
 *
 *      8  0 0 0 0 0 0 0 0  8 
 *      7  0 0 0 0 0 0 0 1  7 
 *      6  0 0 0 0 0 0 0 1  6 
 *      5  0 0 0 0 0 0 0 1  5 
 *      4  0 0 0 0 0 0 0 1  4 
 *      3  0 0 0 0 0 0 0 1  3 
 *      2  0 0 0 0 0 0 0 1  2 
 *      1  0 1 1 1 1 1 1 0  1 
 *
 *         A B C D E F G H   
 * 
 * There are 12 '1' bits so the number of different variations would be 2^12 = 4096 variations.
 * The index would be 0-4095. One of the variations might look like this:
 * 
 * 
 *         A B C D E F G H  
 *
 *      8  0 0 0 0 0 0 0 0  8 
 *      7  0 0 0 0 0 0 0 1  7 
 *      6  0 0 0 0 0 0 0 1  6 
 *      5  0 0 0 0 0 0 0 1  5 
 *      4  0 0 0 0 0 0 0 0  4 
 *      3  0 0 0 0 0 0 0 1  3 
 *      2  0 0 0 0 0 0 0 1  2 
 *      1  0 1 1 0 0 1 1 0  1 
 *
 *         A B C D E F G H   
 * 
 * OR this:
 * 
 *         A B C D E F G H  
 *
 *      8  0 0 0 0 0 0 0 0  8 
 *      7  0 0 0 0 0 0 0 1  7 
 *      6  0 0 0 0 0 0 0 1  6 
 *      5  0 0 0 0 0 0 0 1  5 
 *      4  0 0 0 0 0 0 0 1  4 
 *      3  0 0 0 0 0 0 0 1  3 
 *      2  0 0 0 0 0 0 0 1  2 
 *      1  0 0 0 0 0 0 0 0  1 
 *
 *         A B C D E F G H   
 */
U64 Find_Relevant_Squares(U32 index, U64 rev_attacks);

/* Get the relevant free rooks and bishops attacks and print the result to fout.
 * Used this function ONCE to find the U64 number of corresponds to the relevant
 * rook or bishop attacks on each square of the chess board.
 * 
 * In other word, this function is used to load up Releant_Rook/Bishop_Attacks[64]
 * 
 * Para: fname -> File name to output the result to. If set to NULL then print to STDOUT
 */
void Print_Relevant_Rook_Bishop_Attacks(char* fname);

/* After all rooks and bishops magic numbers has been found, load up their lookup table.
 * Should run this function everytime a new game process is started. The function will
 * also check if the magic numbers works (i.e., it should almost be a perfect hash function
 * that lead to no collision)
 *
 * Return: 1 if success, 0 otherwise
 */
U8 Init_Rook_Bishop_Lookup_Table(void);

/* Calculate all possible knight attacks for each square on the chess board and print
 * the result to fname, which is an output file.
 * Use this function to load up Knight_Attacks
 *
 * Para: fname -> Name of the output file, print to stdout if set to NULL
 */
void Print_Knight_Attacks(char *fname);

/* Calculate all possible king attacks, exclude castling, for each square on the chess
 * board and print the result to fname, which is an output file.
 * Use this function to load up King_Attacks
 *
 * Para: fname -> Name of the output file, print to stdout if set to NULL
 */
void Print_King_Attacks(char *fname);

#endif