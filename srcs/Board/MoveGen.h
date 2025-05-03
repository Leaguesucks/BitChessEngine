/* Computed the legal moves of all pieces
 * 
 * IMPORTANT: All associated database needs to be initialized
 */

#ifndef _MOVEGEN_H_
#define _MOVEGEN_H_

#include "DataBase.h"

/* Find the first blockers for the slider pieces 
 *
 * Para: pos      -> Current position of this slider piece
 *       blockers -> A blockers map for this piece, including its ally and enemy
 *       slider   -> Indetify the sliders of interest (rook, bishop, queen).
 * 
 * Return: The first blockers map for the slider piece
 */
U64 Find_Sliders_First_Blockers(const Square pos, const U64 blockers, const PNum slider);

/* Generate attacks for the rooks
 * 
 * Para: pos     -> Current position of this rook
 *       enemy   -> Enemy positions
 *       ally    -> Ally positions
 *       eKpos   -> Enemy King position
 *       covered -> Pointer to return the covered square for this piece. Set to NULL in a recursive relation
 *       epins   -> Pointer to return the enemy pinned positions. Set to NULL in a recursive relation
 *              
 * Return: An attack map for the interested rook
 */
U64 Gen_Rook_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 eKpos, U64 *covered, U64 *epins);

/* Generate attacks for the bishops 
 *
 * Para: pos     -> Current position of this bishop
 *       enemy   -> Enemy positions
 *       ally    -> Ally positions
 *       eKpos   -> Enemy King position
 *       covered -> Return the covered square for this piece. Set to NULL in a recursive relation
 *       epins   -> Pointer to return the enemy pinned positions. Set to NULL in a recursive relation
 * 
 * Return: An attack map for the interested bishop
 */
U64 Gen_Bishop_Attacks(const Square pos, const U64 enemy, const U64 eKpos, const U64 ally, U64 *covered, U64 *epins);

/* Generate attacks for the knights 
 *
 * Para: pos     -> Current position of this knight
 *       ally    -> Ally positions
 *       covered -> Return the covered square for this piece
 * 
 * Return: An attacks map for the interested knight
 */
U64 Gen_Knight_Attacks(const Square pos, const U64 ally, U64 *covered);

/* Generate attacks for the pawns 
 *
 * Para: pos      -> Current position of this pawn
 *       enemy    -> Enemy positions
 *       ally     -> Ally positions
 *       enpassen -> The current enpassen map
 *       side     -> Black or White pawn
 *       covered  -> Return the covered square for this piece
 * 
 * Return: An attacks map for the interested pawn
 */
U64 Gen_Pawn_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 enpassen, const U8 side, U64 *covered);

/* Generate moves for the pawns 
 *
 * Para: pos      -> Current position of this pawn
 *       blockers -> Blockers map, which is a combination of both the allies and enemies
 *       side     -> Black or White pawn
 * 
 * Return: A all possible movement map for the interested pawn
 * 
 * NOTE: For this function, there is no lookup tables. The pawn moves are calculated dynamically
 */
U64 Gen_Pawn_Moves(const Square pos, const U64 blockers, const U8 side);

/* Generate attacks for the King
 *
 * Para: pos        -> Current position of the King
 *       ally       -> The ally positions
 *       ecovered   -> The enemy's covered map
 *       covered    -> Return the covered square for this piece
 * 
 * Return: A all possible attack map for the King
 */
U64 Gen_King_Attacks(const Square pos, const U64 ally, const U64 ecovered, U64 *covered);

/* Generate moves for the King aka check if the King can castle 
 *
 * Para: pos      -> Current position of the King
 *       blockers -> The blockers map, which is a combination of the allies and enemies positions
 *       ecovered -> The enemy's covered map
 *       KC       -> King castling status
 *       QC       -> Quen castling status
 *       side     -> The side to play
 * 
 * Return: A all possible movement map for the King
 */
U64 Gen_King_Moves(const Square pos, const U64 blockers, const U64 ecovered, const U8 KC, const U8 QC, const float side);

/* Calculate the X-Ray attacks of the sliders pieces 
 *
 * The X-Ray attacks are computed by removing the enemy blockers and re-calculate the new sliders' attacks
 * 
 * Para: pos       -> Current position of the slider piece
 *       enemy     -> Current enemy positions
 *       ally      -> Current ally positions
 *       eKpos     -> Current position of the enemy King
 *       eblockers -> An enemy blocker map
 *       slider    -> Which slider is it (rook, bishop, queen)
 *       epins     -> Pointer to return the enemy pinned positions
 *      
 * Return: An X-Ray attack map for the slider piece 
 */
U64 Gen_Sliders_XRay_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 eKpos, const U64 eblockers, const PNum slider, U64 *epins);

/* Generate moves and attacks for all pieces and update them in the bitboard 
 *
 * Para: bb -> The current bitboard
 * 
 * Return: The total number of moves generated
 */
U16 Gen_All_Moves_Attacks(BitBoard *bb);

#endif