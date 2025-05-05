/* Mapulates bits on the bitboard 
 * IMPORTANT: Your CPU and environment MUST support <x86intrin.h> H/W instructions
 */

#ifndef _BITMALPULATION_H_
#define _BITMALPULATION_H_

#include "BitBoard.h"

/* Set the correspond square to bit '1' on the bitboard
 *
 * Para: b      -> The bitboard
 *       square -> The masked square e.g., A1, A2, ...
 * 
 * Return: An U64 number with corresponding masked bit
 */
U64 SetBit(U64 b, Square square);

/* Remove the '1' bit at a given square on the bitboard 
 *
 * Para: b      -> The bitboard
 *       square -> The square to pop e.g., A1, A2, ...
 * 
 * Return: A new bit board with the chosen bits poped
 */
U64 PopBit(U64 b, Square square);

/* Return the bit on a given square 
 *
 * Para: b  -> The bit board
 *       sq -> The square to evaluate 
 * 
 * Return: The bit at the given square
 */
U8 GetBit(U64 b, Square sq);

/* Count the number of '1s' in a given bitboard 
 *
 * Para: b   -> The bit board
 * 
 * Return: Number of '1s' in the bitboard
 */
U8 CountBits(U64 b);

/* Return the LSB Square that has been set to '1'
 *
 * Para: b   -> The bitboard
 * 
 * Return: The LSB square that has been set to '1'
 */
Square Get_LSMB(U64 b);

/* Return the MSB square that has been set to '1''
 *
 * Para: b   -> The bitboard
 * 
 * Return: The MSB square that has been set to '1'
 */
Square Get_MSMB(U64 b);

/* Move a single bit on a bit board from an old to a new square 
 *
 * Para: b     -> The bitboard
 *       oldSq -> The current square this bit is residing on
 *       newSq -> The square to move the bit to
 * 
 * Return: A new bitboard with the bit moved to a new square
 */
U64 MoveBit(U64 b, Square oldSq, Square newSq);

#endif