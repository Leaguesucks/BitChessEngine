/* Computed the legal moves of all pieces
 * 
 * IMPORTANT: All associated database needs to be initialized
 */

#ifndef _MOVEGEN_H_
#define _MOVEGEN_H_

#include "DataBase.h"

/* Generate 'free' moves for the rooks. 'Free' means we don't care if the piece is pin or not here
 * 
 * Para: pos   -> Current position of this rook
 *       enemy -> Enemy positions
 *       ally  -> Ally positions
 *              
 * Return: An attack map for the interested rook
 */
U64 Gen_Free_Rook_Attacks(Square pos, U64 enemy, U64 ally);

/* Generate attacks for all rooks and update them in the bitboard 
 *
 * Para: bb    -> The current bitboard
 *       side  -> The side to play (W or B)
 * 
 * Return: The total number of moves generated
 */
U8 Gen_Rook_Attacks(BitBoard *bb, float side);

#endif