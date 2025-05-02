#include "ChessGUI.h"

int sdl_init(Game *game, BitBoard *bb) {
    int sdims;

    /* Might have to adjust the path depending on based on your current directory */
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

    memset(game, 0, sizeof(Game));

    game->window   = SDL_CreateWindow("Chess Game", 0, 0, CHESS_DIM, CHESS_DIM, 0);
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    game->dims     = CHESS_DIM;
    sdims = game->dims / 8;

    if (game->window == NULL || game->renderer == NULL) {
        fprintf(stderr, "sdl_init ERROR: Cannot initialize window or renderer \'%s\'\n", SDL_GetError());
        return -1;
    }

    // game->ScreenSurface = SDL_GetWindowSurface(game->window);

    // if (game->ScreenSurface == NULL) {
    //     fprintf(stderr, "sdl_init ERROR: Cannot initialize the Screen Surface \'%s\'\n", SDL_GetError());
    //     return -1;
    // }

    for (int i = 0; i < NUMTEXTURE; i++) {
        game->GameTexture[i] = IMG_LoadTexture(game->renderer, TEXTURE_PATH[i]); // Load the image into a surface
        if (game->GameTexture[i] == NULL) {
            fprintf(stderr, "sdl_init ERROR: Cannot load image \'%s\': \'%s\'\n", TEXTURE_PATH[i], IMG_GetError());
            return -1;
        } 
    }

    if (bb != NULL)
        game->bb = bb;

    game->me.clicked_square = NOT_A_SQUARE;

    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++) {
            game->Tcoords[rank * 8 + file][X] = file * sdims;
            game->Tcoords[rank * 8 + file][Y] = rank * sdims;
        }
            

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
            else if (event.type == SDL_MOUSEBUTTONDOWN || 
                     event.type == SDL_MOUSEBUTTONUP   ||
                     event.type == SDL_MOUSEMOTION)
                sdl_Handle_MouseEvent(game, &(event.button));
            else
                sdl_Update_Window_Based_On_BitBoard(game, NOT_A_SQUARE);
        }

        sdl_DrawChessBoard(game); // Draw the chess board
        SDL_RenderPresent(game->renderer); // Update screen
    }
}

void sdl_DrawChessBoard(Game *game) {
    int sdims = game->dims / 8;

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

            SDL_Rect rect = {file * sdims, rank * sdims, sdims, sdims};
            SDL_RenderFillRect(game->renderer, &rect);

            TNum texture = EMPTY;
            if ((texture = game->PieceBoard[squareNum]) != EMPTY) { // Draw the piece
                SDL_Rect Trect = {game->Tcoords[squareNum][X], game->Tcoords[squareNum][Y], sdims, sdims};
                SDL_RenderCopy(game->renderer, game->GameTexture[texture], NULL, &Trect);
            }

            if (game->MaskBoard[squareNum]) { // Draw the mask square on top of the piece if there is one
                SDL_Rect MRect = {game->Tcoords[squareNum][X], game->Tcoords[squareNum][Y], sdims, sdims};
                SDL_RenderCopy(game->renderer, game->GameTexture[GDOT], NULL, &MRect);
            }
        }

    /* Draw the dragged square last (if there is any) */
    if (game->me.dragging && game->me.clicked_square != NOT_A_SQUARE) {
        if (game->PieceBoard[game->me.clicked_square] != EMPTY) { // Extra safeguard
            Square ClickedSquare = game->me.clicked_square;
            SDL_Rect Trect = {game->Tcoords[ClickedSquare][X],
                              game->Tcoords[ClickedSquare][Y],
                              sdims, sdims};
            SDL_RenderCopy(game->renderer, game->GameTexture[game->PieceBoard[ClickedSquare]], NULL, &Trect);
        }
    }
}

void sdl_Update_Window_Based_On_BitBoard(Game *game, Square focus) {
    BitBoard *bb = game->bb;

    for (Square square = A8; square <= H1; square++) {
        if (GetBit(bb->pos_wPawns, square))
            game->PieceBoard[square] = WPAWN;
        else if ((GetBit(bb->pos_wRooks, square)))
            game->PieceBoard[square] = WROOK;
        else if ((GetBit(bb->pos_wKnights, square)))
            game->PieceBoard[square] = WKNIGHT;
        else if ((GetBit(bb->pos_wBishops, square)))
            game->PieceBoard[square] = WBISHOP;
        else if ((GetBit(bb->pos_wQueens, square)))
            game->PieceBoard[square] = WQUEEN;
        else if ((GetBit(bb->pos_wKing, square)))
            game->PieceBoard[square] = WKING;

        else if (GetBit(bb->pos_bPawns, square))
            game->PieceBoard[square] = BPAWN;
        else if (GetBit(bb->pos_bRooks, square))
            game->PieceBoard[square] = BROOK;
        else if (GetBit(bb->pos_bKnights, square))
            game->PieceBoard[square] = BKNIGHT;
        else if (GetBit(bb->pos_bBishops, square))
            game->PieceBoard[square] = BBISHOP;
        else if (GetBit(bb->pos_bQueens, square))
            game->PieceBoard[square] = BQUEEN;
        else if (GetBit(bb->pos_bKing, square))
            game->PieceBoard[square] = BKING;

        else
            game->PieceBoard[square] = EMPTY;

        if (focus != NOT_A_SQUARE && (GetBit(bb->atk_on_each_square[focus], square) || GetBit(bb->moves_on_each_square[focus], square)))
            game->MaskBoard[square] = 1;
        else
            game->MaskBoard[square] = 0;
    }

    
}

void sdl_Handle_MouseEvent(Game *game, SDL_MouseButtonEvent *e) {
    int sdims = game->dims / 8;
    int x, y; // Current coordinate of the mouse
    int offset_x, offset_y;

    int rank, file; // The rank and the file of the clicked on square
    int square;     // Which square is clicked on

    x = e->x;
    y = e->y;
    file = x / sdims;
    rank = y / sdims;
    square = rank * 8 + file;

    if (e->button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN) {
        offset_x = x - (file * sdims);
        offset_y = y - (rank * sdims);

        if (game->me.clicked_square == NOT_A_SQUARE) { // If no piece is currently being clicked
            sdl_Focus_Square(game, (Square) square, offset_x, offset_y);
        }
        else{
            if (game->me.clicked_square == (Square) square) {// The same square is being clicked again
                game->me.clicked++;
                game->me.dragging = 1;
            }
            else {// Need to find a new square
                sdl_Clear_Focus(game);
                sdl_Focus_Square(game, (Square) square, offset_x, offset_y);
            }
        }
    }
    else if (e->button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONUP) {
        /* Released any piece if being drageed */
        /* Implement returning to a new position later. For now, return the piece to its original position */
        
        if (game->me.dragging) { // If the piece is being dragged then return to its original coordinate for now
            square = (int) game->me.clicked_square;
            game->Tcoords[square][X] = game->me.return_coords[X];
            game->Tcoords[square][Y] = game->me.return_coords[Y];

            game->me.dragging = 0;
        }

        if (game->me.clicked == 2) // Click the same piece twice will reset it
            sdl_Clear_Focus(game);
    }
    
    if (e->type == SDL_MOUSEMOTION) {
        if (game->me.dragging) { // A piece is being dragged
            game->Tcoords[game->me.clicked_square][X] = x - game->me.offsets[X];
            game->Tcoords[game->me.clicked_square][Y] = y - game->me.offsets[Y];
        }
    }
    
}

void sdl_Clear_Focus(Game *game) {
    game->me.dragging = 0;
    game->me.clicked = 0;
    game->me.clicked_square = NOT_A_SQUARE;
    game->me.offsets[X] = game->me.offsets[Y] = 0;
    game->me.return_coords[X] = game->me.return_coords[Y] = 0;
    sdl_Update_Window_Based_On_BitBoard(game, NOT_A_SQUARE);
}

void sdl_Focus_Square(Game *game, Square square, int offsetX, int offsetY) {
    if (square < 0 || square > 63) { // No square has been clicked on
        sdl_Clear_Focus(game);
        return (void) 0;
    }
    if (game->PieceBoard[square] == EMPTY || game->PieceBoard[square] == GDOT) { // There is no pieces on that square
        sdl_Clear_Focus(game);
        return (void) 0;
    }
    
    /* Else a piece has been successfully clicked on */
    /* Implement if we are supposed to be clicking on that piece later on */
    
    game->me.clicked_square = (Square) square;
    game->me.return_coords[X] = game->Tcoords[square][X];
    game->me.return_coords[Y] = game->Tcoords[square][Y];
    game->me.offsets[X] = offsetX;
    game->me.offsets[Y] = offsetY;
    
    game->me.dragging = 1;
    game->me.clicked++;

    sdl_Update_Window_Based_On_BitBoard(game, square); // Display the moves and attacks on the window
}