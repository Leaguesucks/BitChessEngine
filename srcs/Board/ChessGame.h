/* Control the flow of a chess game, from the begining or from a given position till the end (checkmate
 * of one side)
 * 
 * References:
 *      FEN Encode: https://www.chess.com/terms/fen-chess
 */

#ifndef _CHESSGAME_H_
#define _CHESSGAME_H_

#include "MoveGen.h"

/* Begining positons of a chess game in FEN */
#define BEGINING "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w QKqk - 0 0"

/* All pieces identifier on the square as string in FEN notation. Used mainly as a convenient helper */
static const char PCHAR[2][6];

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

/* Handle when a Pawn wants promotion. Promotion detection is donw outside of this function
 *
 * Para: bb       -> The bit board
 *       ProPos   -> Promote position, which is the square of the last rank that this pawn reaches
 *       ProPiece -> What piece to promote into 
 * 
 * Return: nonzero and update the bitboard accordingly if suceed, zero otherwise
 */
U8 Handle_Promotion(BitBoard *bb, Square ProPos, PNum ProPiece);

/* Handle when a King wants to castle. Castling detection is done outside of this function 
 *
 * Para: bb    -> The bit board
 *       cSide -> The castling side
 *       
 * Note: The check for the possibility of castling is done outside of this function, so no return
 *       for verification is needed
 */
void Handle_Castling(BitBoard *bb, U8 cSide);

/* Handle capturing a piece. The condition to capture should have already been done outside of this
 * function. If an enemy piece can be captured, then simply eliminate the that enemy piece. Note that
 * this function only check if an enemy piece has been captured. It doesn't move the attack piece or 
 * does any checking or further action
 *
 * Para: bb     -> The bit board
 *       piece  -> What attacking piece it is
 *       capPos -> The capturing position 
 */
void Capture(BitBoard *bb, PNum piece, Square capPos);

/* Handle moving a piece from one position to another on the chessboard. This function need to call
 * an outside defined function to handle the pawn promotion
 *
 * Para: bb              -> The bit board
 *       piece           -> What piece to move
 *       oldPos          -> Current position of this piece
 *       newPos          -> The new position to move this piece to
 *       promote_handler -> A promotion handler the promotion event to the user. Whatever the inputs
 *                          this function may take, the return should be what kind of piece the user
 *                          wants to promote their pawn into
 * 
 * Return: nonzero if suceed, zero otherwise
 */
U8 MovePiece(BitBoard *bb, PNum piece, Square oldPos, Square newPos, PNum (*promote_handler) (void*));

#endif