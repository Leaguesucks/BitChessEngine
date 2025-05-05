/* Computed the legal moves of all pieces
 * 
 * IMPORTANT: All associated database needs to be initialized
 */

#ifndef _MOVEGEN_H_
#define _MOVEGEN_H_

#include "DataBase.h"

/* Pawn EnPassen "moves"square after EnPassen attack another pawn. Here, everytime a pawn can be 
* EnPassen captured, its capture enemied pawn will moves behind or in front of that pawn,
* depending on which side the enpassened pawn is on
* 
* e.g., If a black pawn double moves to b5, then that black pawn can be enpassen captured
*       by the white pawns. If a white pawn captures that black pawn, then after the enpassen
*       capture, the white pawn will move to b6
* 
* There are 8 elements in this array for each side, each correspond with one pawn. In otherword,
* this array stores the square where the ally pawn will move to after enpassen capturing this 
* pawn
*/
static const Square ENPASSEN_CAPTURE[2][8];

/* Find the first blockers for the slider pieces 
 *
 * Para: pos      -> Current position of this slider piece
 *       blockers -> A blockers map for this piece, including its ally and enemy
 *       slider   -> Indetify the sliders of interest (rook, bishop, queen).
 * 
 * Return: The first blockers map for the slider piece
 */
U64 Find_Sliders_First_Blockers(const Square pos, const U64 blockers, const PNum slider);

/* Generate magic attacks for the Rook and the Bishop 
 *
 * Para: pos      -> The current position of the Rook and the Bishop
 *       blockers -> A combined blockers map of both the allies and the enemies
 *       slider   -> Identify if it is a Rook or a Bishop
 * 
 * Return: An attack map generated from the magic tables for the slider pieces
 */
U64 Gen_Magic_Sliders_Attacks(const Square pos, const U64 blockers, const PNum sliders);

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

/* Check if a Pawn EnPassen attack can happen
 * 
 * Para: initAtks   -> Initial (raw) attack map of this pawn
 *       blockers   -> The blockers map
 *       enpassen   -> The current EnPassen map
 *       side       -> The side this pawn is on
 *       covered    -> Pointer to return the covered square for this pawn
 *       postEnPass -> Pointer to return the square that the enemy Pawn will end up after an EnPassen attack this pawn
 *
 * Return: An attack map with EnPassen attacks included (if there is any)
 */
U64 Gen_EnPassen_Attacks(const U64 initAtks, const U64 blockers, const U64 enpassen, const U8 side, U64 *covered, Square *postEnPass);

/* Generate attacks for the pawns 
 *
 * Para: pos        -> Current position of this pawn
 *       enemy      -> Enemy positions
 *       ally       -> Ally positions
 *       enpassen   -> The current enpassen map
 *       side       -> Black or White pawn
 *       covered    -> Return the covered square for this pawn
 *       postEnPass -> Pointer to return the square that the enemy Pawn will end up after an EnPassen attack this pawn
 * 
 * Return: An attacks map for the interested pawn
 */
U64 Gen_Pawn_Attacks(const Square pos, const U64 enemy, const U64 ally, const U64 enpassen, const U8 side, U64 *covered, Square *postEnPass);

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
U64 Gen_King_Moves(const Square pos, const U64 blockers, const U64 ecovered, const U8 KC, const U8 QC, const U8 side);

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

/* Handle setting up the bitboard before computing its moves and attacks 
 *
 * Para: bb -> The bitboard
 */
void Pre_Gen_Moves_Attacks(BitBoard *bb);

/* Check if the enemy King is under check. If it is then update the checked status and
 * the checked attackers map accordingly. Call this function after an attack map for 
 * an attacker has been generated
 *
 * Para: bb       -> The bitboard
 *       attacker -> Which piece is the attacker
 *       pos      -> Current position of the attacker
 */
void Set_Check(BitBoard *bb, const PNum attacker, const Square pos);

/* Generate a check reponse for an allied piece if the King is currently under check 
 *
 * Para: attacks -> All calculated possible attacks of this piece (if there is any)
 *       moves   -> All calculated possible moves of this piece (if there is any)
 *       postEn  -> The square that if the piece is a Pawn, it will end up at that square after an EnPassen attack
 *       piece   -> What piece it is
 *       pos     -> Current position of this piece
 *       Kpos    -> Current position of the allied King
 *       chSrcs  -> An U64 array of size [5], which represent the source of check from the enemy piece to the King (Note that the 2 Kings cannot check each other)
 *       rMoves  -> U64 pointer to return the moves reponses of this piece (if there is any, really only the pawn needs this)
 * 
 * Return: All attacks and moves for this piece that is capable of blocking the checks
 */
U64 Gen_Checked_Reponses(const U64 attacks, const U64 moves, const Square postEn, const PNum piece, const Square pos, const Square Kpos, const U64 *chSrcs, U64 *rMoves);

/* Generate the moves and attacks of a non-King piece and update it on the Bitboard 
 * Para: bb    -> The bitboard
 *       piece -> Which piece to generate the attack
 *       pos   -> Current position of that piece
 * 
 * Return: The number of moves and attacks generated for that piece
 */
U16 Set_And_Gen_Moves_Attacks(BitBoard *bb, const PNum piece, const Square pos);

/* Generate moves and attacks for all pieces and update them in the bitboard 
 *
 * Para: bb -> The current bitboard
 * 
 * Return: The total number of moves and attackss generated
 */
U16 Gen_All_Moves_Attacks(BitBoard *bb);

/* A helper function. Draw a line from the source bit to the destination bit on a bitboard 
 *
 * Para: src -> The source
 *       des -> The destination
 * 
 * Return: A U64 bitboard with a ray drawn from the source to the destination, or 0 if no line can be drawn
 * 
 * NOTE: The source and destination will not be included in the line drawn
 */
U64 Draw_BB_Line(Square src, Square des);

#endif