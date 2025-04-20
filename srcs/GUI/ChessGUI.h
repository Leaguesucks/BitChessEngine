#ifndef _CHESSGUI_H_
#define _CHESSGUI_H_

#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH  800

/* A Game struct that is used to store the window informations */
typedef struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
} Game;

/* Initialize an SDL window 
 *
 * Para: game -> The game window informations
 * 
 * Return: non-zero if success, 0 or negative numbers otherwise 
 */
int sdl_init(Game *game);

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
 *       wColor -> RGBA color of the white square
 *       bCOlor -> RGBA color of the black square
 */
void sdl_DrawChessBoard(Game *game, int dims, unsigned char *wColor, unsigned char *bColor);

#endif