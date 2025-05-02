/* Handlle initialization and displaying the Chess Board */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // For PNG image render
#include <stdio.h>
#include <string.h>

#include "../Board/ChessGame.h"

#define CHESS_DIM 800 // Width and height of the chess board
#define NUMTEXTURE  13  // Total number of textures

#define X 0
#define Y 1

/* Chess pieces textures identification */
typedef enum TNum {
    WPAWN, WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING,
    BPAWN, BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING,
    GDOT , EMPTY
} TNum;

/* A struct to handle the Nouse Event */
typedef struct MouseEvent {
    U8  dragging;                         // Equals 1 if a piece is being dragged. Otherwise 0
    U8 clicked;                           // If 0, then no piece is being clicked. If 1 a piece is being clicked and should be reset to 0 on the next click
    Square clicked_square;                // Which square that has been cliked on by. EMPTY_SQUARE if no or not a legal square is clicked on
    int return_coords[2];                 // Absolute coordinate of the locked piece to return after it is released
    int offsets[2];                       // Offset, which is used during drag to smooth out the animation

} MouseEvent;

/* A Game struct that is used to store the GUI informations */
typedef struct Game {
    SDL_Window *window;                   // The game Window
    SDL_Renderer *renderer;               // The game Renderer
    //SDL_Surface *ScreenSurface;         // The surface contained by the Window
    SDL_Texture *GameTexture[NUMTEXTURE]; // 13 diffrent surfaces, not including empty squares
    U8 PieceBoard[64];                    // Contain informations about what piece to display on the window. Each square is counted from left -> right, top -> bottom
    U8 MaskBoard[64];                     // Either > 0 for the square is being masked by a grey dot or 0.
    BitBoard *bb;                         // Pointer to the Bitboard representation of the positions
    
    int dims;                             // X and Y dimension of the chess board, which is a square

    int Tcoords[64][2];                   // The coordinate of each texture on the chessboard. 0 -> X, 1 -> Y

    MouseEvent me;                        // Handle the mouse event

    /* Either 0 or negative for: This bitboard is out-of-date and has not been updated on the GUI or
     *        Positive for     : This bitboard has been unpdated on the GUI successfully and should now be 1
     */
    //int GUI_Update;
} Game;

/* Initialize an SDL window 
 *
 * Para: game -> The game window informations
 *       bb   -> The bitboard that represent the positions (optional for now for debug)
 * 
 * Return: non-zero if success, 0 or negative numbers otherwise 
 */
int sdl_init(Game *game, BitBoard *bb);

/* Clean up the sdl windows 
 *
 * Para: game -> The game window informations
 */
void sdl_cleanup(Game *game);

/* The main "loop" of the GUI chess game. Handle rendering image on the screen, user interactions
 * and events e.g., quiting
 *
 * Para: game -> The game window informations
 */
void sdl_chessGame(Game *game);

/* Draw the chess board 
 *
 * Para: game   -> The game window informations
 */
void sdl_DrawChessBoard(Game *game);

/* Update the chess positions on the GUI according to the bitboard position 
 *
 * Para: game   -> The game window informations
 *       focus  -> The square that being "foucsed", meaning we will display the attacks and moves for that square
 */
void sdl_Update_Window_Based_On_BitBoard(Game *game, Square focus);

/* Handle the Mouse events 
 *
 * Para: game -> The game window information
 *       e    -> The mouse event that occured
 */
void sdl_Handle_MouseEvent(Game *game, SDL_MouseButtonEvent *e);

/* Handle cleaning up after unfocusing a square 
 *
 * Para: game -> The game window informations     
 */
void sdl_Clear_Focus(Game *game);

/* Handle clicking on s square is valid 
 *
 * Para: game   -> The game window informations
 *       square -> The clicked square to examine
 *       offsetX -> The mouse X-offset
 *       offsetY -> The mouse Y-offset
 */
void sdl_Focus_Square(Game *game, Square square, int offsetX, int offsetY);

#endif