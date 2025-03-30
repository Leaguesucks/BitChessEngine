/* Control the flow of a chess game, from the begining or from a given position till the end (checkmate
 * of one side)
 * 
 * References:
 *      FEN Encode: https://www.chess.com/terms/fen-chess
 */

#ifndef _CHESSGAME_H_
#define _CHESSGAME_H_

#include <string.h>
#include "MoveGen.h"

/* Begining positons of a chess game in FEN */
#define BEGINING "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w QKqk - 0 0"

/* Setup the bit chessboard based on a given FEN.
 * NOTE: For the sake of simplicity, we have not checked for invalid FEN (yet).
 *       That being said, there are still very minor error checkings in this version
 *
 * Para: bb  -> The bit chessboard to setup
 *       fen -> The setup position encoded in FEN
 * 
 * Return: 1 if sucess, 0 otherwise
 */
U8 FEN_Decode(BitBoard *bb, const char *fen);

/* Init a chess game based on a given fen. Call this function everytime you want to start a new game
 *
 * Para: bb  -> The bit board to initialize
 *       fen -> The positions encoded in fen to initialized
 * 
 * Return: non-zero if success, 0 otherwise
 */
U8 Init_Game(BitBoard *bb, const char *fen);

#endif