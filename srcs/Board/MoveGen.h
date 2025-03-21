/* Computed the legal moves of all pieces */

#ifndef _MOVEGEN_H_
#define _MOVEGEN_H_

#include "DataBase.h"

/* Generate 'sliding' moves for the rooks. 'Sliding' means we don't care if the piece is pin or not here
 * 
 * Para: alies   -> The alies map of the interested rook, including the interested rook itself
 *       enemies -> The enemies map of the interested rook
 *       pos     -> Current position of the interested rook
 * 
 * Return: An attack map for the interested rook
 */
U64 Gen_Sliding_Rook_Attacks(U64 allies, U64 enemies, Square pos);

#endif