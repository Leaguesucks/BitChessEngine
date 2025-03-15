/* Mapulates bits on the bitboard */

#ifndef _BITMALPULATION_H_
#define _BITMALPULATION_H_

#include "BitBoard.h"

/* Set the correspond bit to a square on the bitboard
 *
 * Para: b   -> The bitboard
 *       ns  -> Number of bit to set == length of s
 *       ... -> The masked square e.g., A1, A2, ...
 * 
 * Return: An U64 number with corresponding masked bit
 */
U64 SetBit(U64 b, int ns, ...);

/* Return and remove the bit at a given square on the bitboard 
 *
 * Para: b   -> The bitboard
 *       ns  -> Number of square bit to pop
 *       ... -> The squares to pop e.g., A1, A2, ...
 * 
 * Return: A new bit board with the chosen bits poped
 */
U64 PopBit(U64 b, int ns, ...);

/* Return the bit on a given square 
 *
 * Para: b  -> The bit board
 *       sq -> The square to evaluate 
 * 
 * Return: The bit at the given square
 */
U8 GetBit(U64 b, Square sq);

/* Count the number of interested bits (0 or 1) in a given bitboard 
 *
 * Para: b   -> The bit board
 *       Bit -> 0 or 1: The interested bit
 * 
 * Return: Number of interested bits in the bitboard
 */
U8 CountBits(U64 b, U8 Bit);

/* Return the LSB Square that has been set to Bit
 *
 * Para: b   -> The bitboard
 *       Bit -> The interested set bit
 * 
 * Return: The LSB square that has been set to 1
 */
Square Get_LSMB(U64 b, U8 Bit);

#endif