#include "ChessGUI.h"

int sdl_init(Game *game, BitBoard *bb) {

    /* Might have to adjust the path later on */
    const char *TEXTURE_PATH[NUMTEXTURE] = 
    {
        "resources/images/pieces/whitepawn.png",
        "resources/images/pieces/whiterook.png",
        "resources/images/pieces/whiteknight.png",
        "resources/images/pieces/whitebishop.png",
        "resources/images/pieces/whitequeen.png",
        "resources/images/pieces/whiteking.png",

        "resources/images/pieces/blackpawn.png",
        "resources/images/pieces/blackrook.png",
        "resources/images/pieces/blackknight.png",
        "resources/images/pieces/blackbishop.png",
        "resources/images/pieces/blackqueen.png",
        "resources/images/pieces/blackking.png",

        "resources/images/interfaces/greydot.png",
    };

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "sdl_init ERROR: Cannot initialize SDL\n");
        return -1;
    }

    game->window   = SDL_CreateWindow("Chess Game", 0, 0, CHESS_DIM, CHESS_DIM, 0);
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);

    if (game->window == NULL || game->renderer == NULL) {
        fprintf(stderr, "sdl_init ERROR: Cannot initialize window or renderer \'%s\'\n", SDL_GetError());
        return -1;
    }

    game->ScreenSurface = SDL_GetWindowSurface(game->window);

    if (game->ScreenSurface == NULL) {
        fprintf(stderr, "sdl_init ERROR: Cannot initialize the Screen Surface \'%s\'\n", SDL_GetError());
        return -1;
    }

    for (int i = 0; i < NUMTEXTURE; i++) {
        game->GameTexture[i] = IMG_LoadTexture(game->renderer, TEXTURE_PATH[i]); // Load the image into a surface
        if (game->GameTexture[i] == NULL) {
            fprintf(stderr, "sdl_init ERROR: Cannot load image \'%s\': \'%s\'\n", TEXTURE_PATH[i], IMG_GetError());
            return -1;
        } 
    }

    if (bb != NULL)
        game->bb = bb;

    return 1;
}

void sdl_cleanup(Game *game) {
    for (int i = 0; i < NUMTEXTURE; i++)
        SDL_DestroyTexture(game->GameTexture[i]);

    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

void sdl_chessGame(Game *game) {
    SDL_Event event;

    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                sdl_cleanup(game);
                return (void) 0;
            }
        }

        sdl_Update_Window_Based_On_BitBoard(game);

        sdl_DrawChessBoard(game, CHESS_DIM); // Draw the chess board
        SDL_RenderPresent(game->renderer); // Update screen
    }
}

void sdl_DrawChessBoard(Game *game, int dims) {
    int sDims = dims / 8;

    // Clear screen
    SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, 0xFF); // White background
    SDL_RenderClear(game->renderer);

    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++) {
            int squareNum = rank * 8 + file;
            if ((rank + file) % 2 == 0) // Draw a light square
                SDL_SetRenderDrawColor(game->renderer, 240, 217, 181, 255);
            else                        // Draw a dark square
                SDL_SetRenderDrawColor(game->renderer, 181, 136, 99, 255);

            SDL_Rect rect = {file * sDims, rank * sDims, sDims, sDims};
            SDL_RenderFillRect(game->renderer, &rect);

            TNum texture = EMPTY;
            if ((texture = game->WindowBoard[squareNum]) != EMPTY) // Draw the piece
                SDL_RenderCopy(game->renderer, game->GameTexture[texture], NULL, &rect);
        }
}

void sdl_Update_Window_Based_On_BitBoard(Game *game) {
    BitBoard *bb = game->bb;

    for (Square square = A8; square <= H1; square++)
        if (GetBit(bb->pos_wPawns, square))
            game->WindowBoard[square] = WPAWN;
        else if ((GetBit(bb->pos_wRooks, square)))
            game->WindowBoard[square] = WROOK;
        else if ((GetBit(bb->pos_wKnights, square)))
            game->WindowBoard[square] = WKNIGHT;
        else if ((GetBit(bb->pos_wBishops, square)))
            game->WindowBoard[square] = WBISHOP;
        else if ((GetBit(bb->pos_wQueens, square)))
            game->WindowBoard[square] = WQUEEN;
        else if ((GetBit(bb->pos_wKing, square)))
            game->WindowBoard[square] = WKING;

        else if (GetBit(bb->pos_bPawns, square))
            game->WindowBoard[square] = BPAWN;
        else if (GetBit(bb->pos_bRooks, square))
            game->WindowBoard[square] = BROOK;
        else if (GetBit(bb->pos_bKnights, square))
            game->WindowBoard[square] = BKNIGHT;
        else if (GetBit(bb->pos_bBishops, square))
            game->WindowBoard[square] = BBISHOP;
        else if (GetBit(bb->pos_bQueens, square))
            game->WindowBoard[square] = BQUEEN;
        else if (GetBit(bb->pos_bKing, square))
            game->WindowBoard[square] = BKING;

        else
            game->WindowBoard[square] = EMPTY;
}