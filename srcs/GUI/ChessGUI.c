#include "ChessGUI.h"

int sdl_init(Game *game) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "ERROR: Cannot initialize SDL\n");
        return -1;
    }

    game->window   = SDL_CreateWindow("Chess Game", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);

    return 1;
}

void sdl_cleanup(Game *game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

void sdl_chessGame(Game *game) {
    SDL_Event event;

    int Bdims = WINDOW_HEIGHT; // Dimension of the chess board

    unsigned char wColor[4] = {240, 217, 181, 255}; // RGBA color of the white square
    unsigned char bColor[4] = {181, 136, 99, 255}; // RGBA color of the dark square 

    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                sdl_cleanup(game);
                return (void) 0;
            }
        }

        sdl_DrawChessBoard(game, Bdims, wColor, bColor); // Draw the chess board
        SDL_RenderPresent(game->renderer); // Update screen
    }
}

void sdl_DrawChessBoard(Game *game, int dims, unsigned char *wColor, unsigned char *bColor) {
    int sDims = dims / 8;

    // Clear screen
    SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, 0xFF); // White background
    SDL_RenderClear(game->renderer);

    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++) {
            if ((rank + file) % 2 == 0) // Draw a light square
                SDL_SetRenderDrawColor(game->renderer, wColor[0], wColor[1], wColor[2], wColor[3]);
            else // Draw a dark square
                SDL_SetRenderDrawColor(game->renderer, bColor[0], bColor[1], bColor[2], bColor[3]);

            SDL_Rect rect = {rank * sDims, file * sDims, sDims, sDims};
            SDL_RenderFillRect(game->renderer, &rect);
        }
}