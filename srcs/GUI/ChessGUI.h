#ifndef _CHESSGUI_H_
#define _CHESSGUI_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // For PNG image render
#include <stdio.h>
#include <string.h>

#include "../Board/ChessGame.h"

#define CHESS_DIM 800 // Width and height of the chess board
#define NUMTEXTURE  13  // Total number of textures

/* Chess pieces textures identification */
typedef enum TNum {
    WPAWN, WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING,
    BPAWN, BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING,
    GDOT , EMPTY
} TNum;

/* A Game struct that is used to store the GUI informations */
typedef struct Game {
    SDL_Window *window;                   // The game Window
    SDL_Renderer *renderer;               // The game Renderer
    SDL_Surface *ScreenSurface;           // The surface contained by the Window
    SDL_Texture *GameTexture[NUMTEXTURE]; // 13 diffrent surfaces, not including empty squares
    U8 WindowBoard[64];                   // Contain informations about what to display on the window. Each square is counted from left -> right, top -> bottom
    BitBoard *bb;                         // Pointer to the Bitboard representation of the positions
    
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
 *       dims   -> Width and height of the entire chess board, which is a square (width == height)
 *                 and preferably divisible by 8
 */
void sdl_DrawChessBoard(Game *game, int dims);

/* Update the chess positions on the GUI according to the bitboard position 
 *
 * Para: game -> The game window informations
 */
void sdl_Update_Window_Based_On_BitBoard(Game *game);

#endif