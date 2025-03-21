#include "ChessGame.h"

void Init_BB(BitBoard *bb) {
    memset(bb, 0, sizeof(BitBoard)); // Set Bitboard to 0

    for (U8 i = 0; i < 8; i++)
        bb->wPawns[i].abs_val = bb->bPawns[i].abs_val = PAWN_ABS_VAL;
    for (U8 i = 0; i < 9; i++)
        bb->wQueens[i].abs_val = bb->bQueens[i].abs_val = QUEEN_ABS_VAL;
    for (U8 i = 0; i < 10; i++) {
        bb->wRooks[i].abs_val = bb->bRooks[i].abs_val = ROOK_ABS_VAL;
        bb->wKnights[i].abs_val = bb->bKnights[i].abs_val = KNIGHT_ABS_VAL;
        bb->wBishops[i].abs_val = bb->bBishops[i].abs_val = BISHOP_ABS_VAL;
    }
}

U8 FEN_Decode(BitBoard *bb, const char *fen) {
    char *fenDup = malloc(1000);
    strncpy(fenDup, fen, strlen(fen));

    char *position = strtok(fenDup, " "); // The first field is the position filed
    if (position == NULL) {
        fprintf(stderr, "FEN cannot be emptied\n");
        return 0;    // Board cannot be emptied
    }

    U8 rank, file, ccount;
    rank = file = 0;

    for (ccount = 0; ccount < strlen(position); ccount++) {
        char piece, num_empties;

        if ((num_empties = (piece = position[ccount]) - '0') >= 1 && num_empties <= 8) { // Extract empties squares
            file += num_empties;
            continue;
        }

        if (piece == '/') { // Evaluate new rank
            file = 0;
            rank++;
            continue;
        }
        
        Square square = (Square) (rank*8 + file);
        
        if (piece >= 'a' && piece <= 'z') // This is a black piece
            bb->pos_bAll = SetBit(bb->pos_bAll, 1, square);
        else                              // This is a white piece
            bb->pos_wAll = SetBit(bb->pos_wAll, 1, square);

        if (piece == 'p') {
            U8 num = bb->num_bPawns++;
            bb->bPawns[num].values.Position = square;
            bb->pos_bPawns = SetBit(bb->pos_bPawns, 1, square);
        }
        else if (piece == 'r') {
            U8 num = bb->num_bRooks++;
            bb->bRooks[num].values.Position = square;
            bb->pos_bRooks = SetBit(bb->pos_bRooks, 1, square);
        }
        else if (piece == 'n') {
            U8 num = bb->num_bKnights++;
            bb->bKnights[num].values.Position = square;
            bb->pos_bKnights = SetBit(bb->pos_bKnights, 1, square);
        }
        else if (piece == 'b') {
            U8 num = bb->num_bBishops++;
            bb->bBishops[num].values.Position = square;
            bb->pos_bBishops = SetBit(bb->pos_bBishops, 1, square);
        }
        else if (piece == 'q') {
            U8 num = bb->num_bQueens++;
            bb->bQueens[num].values.Position = square;
            bb->pos_bQueens = SetBit(bb->pos_bQueens, 1, square);
        }
        else if (piece == 'k') {
            bb->pos_bKing = SetBit(bb->pos_bKing, 1, square);
        }
        else if (piece == 'P') {
            U8 num = bb->num_wPawns++;
            bb->wPawns[num].values.Position = square;
            bb->pos_wPawns = SetBit(bb->pos_wPawns, 1, square);
        }
        else if (piece == 'R') {
            U8 num = bb->num_wRooks++;
            bb->wRooks[num].values.Position = square;
            bb->pos_wRooks = SetBit(bb->pos_wRooks, 1, square);
        }
        else if (piece == 'N') {
            U8 num = bb->num_wKnights++;
            bb->wKnights[num].values.Position = square;
            bb->pos_wKnights = SetBit(bb->pos_wKnights, 1, square);
        }
        else if (piece == 'B') {
            U8 num = bb->num_wBishops++;
            bb->wBishops[num].values.Position = square;
            bb->pos_wBishops = SetBit(bb->pos_wBishops, 1, square);
        }
        else if (piece == 'Q') {
            U8 num = bb->num_wQueens++;
            bb->wQueens[num].values.Position = square;
            bb->pos_wQueens = SetBit(bb->pos_wQueens, 1, square);
        }
        else if (piece == 'K') {
            bb->pos_wKing = SetBit(bb->pos_wKing, 1, square);
        }
        else {
            fprintf(stderr, "\'%c\' (ASCII: %d) is not a valid piece\n", piece, piece);
            return 0;
        }

        file++;
    }

    char *Side2Play = strtok(NULL, " ");
    if (Side2Play == NULL) {
        fprintf(stderr, "Side to Play field cannot be emptied\n");
        return 0;
    }

    if (strncmp(Side2Play, "w", 1) == 0)
        bb->side2play = W;
    else if (strncmp(Side2Play, "b", 1) == 0)
        bb->side2play = B;
    else {
        fprintf(stderr, "\'%s\' is not a valid side to play\n", Side2Play);
        return 0;
    }

    char *Castling = strtok(NULL, " ");
    if (Castling == NULL) {
        fprintf(stderr, "Castling field cannot be emptied\n");
        return 0;
    }

    for (int i = 0; i < 4; i++) {
        char castle;
        if ((castle = Castling[i]) == '\0' || castle == '-') break;

        if (castle == 'q')
            bb->bCastle[0] = BLACK_CASTLE_LEFT;
        else if (castle == 'k')
            bb->bCastle[1] = BLACK_CASTLE_RIGHT;
        else if (castle == 'Q')
            bb->wCastle[0] = WHITE_CASTLE_LEFT;
        else if (castle == 'K')
            bb->wCastle[1] = WHITE_CASTLE_RIGHT;
        else {
            fprintf(stderr, "\'%c\' (ASCII: %d) is not a valid castling value\n", castle, castle);
            return 0;
        }
    }

    char *EnPassen = strtok(NULL, " ");
    if (EnPassen == NULL) {
        fprintf(stderr, "EnPassen Field cannot be NULL\n");
        return 0;
    }

    U8 ecount = 0;
    while (ecount < strlen(EnPassen) + 1) {
        if (EnPassen[0] == '-' || EnPassen[0] == '\0') break;

        char file = EnPassen[ecount++];
        char ref_sq;
        if (file >= 'a' && file <= 'h')
            ref_sq = 'a';
        else if (file >= 'A' && file <= 'H')
            ref_sq = 'A';
        else {
            fprintf(stderr, "\'%c\' (ASCII: %d) is not a valid file\n", file, file);
            return 0;
        }

        file -= ref_sq;

        char rank = EnPassen[ecount++];
        if (rank >= '3' && rank <= '4')
            rank -= '1';
        else {
            fprintf(stderr, "\'%c\' (ASCII: %d) is not a valid rank\n", rank, rank);
            return 0;
        }

        Square square = (Square) (rank*8 + file);
        bb->EnPassen = SetBit(bb->EnPassen, 1, square);
    }

    char *HalfMoveClk = strtok(NULL, " ");
    if (HalfMoveClk == NULL) {
        fprintf(stderr, "Half move clock field cannot be NULL\n");
        return 0;
    }

    char *endptr;
    long _50count = (U8) strtol(HalfMoveClk, &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "\'%s\' is not a valid half clock move\n", HalfMoveClk);
        return 0;
    }
    else if (_50count < 0 || _50count > 99) {
        fprintf(stderr, "Value of half clock move = %ld is invalid\n", _50count);
        return 0;
    }

    bb->_50Count = (U8) _50count;

    char *FullMove = strtok(NULL, " ");
    if (FullMove == NULL) {
        fprintf(stderr, "Number of full moves cannot be NULL\n");
        return 0;
    }

    char *endptr2;
    long full = strtol(FullMove, &endptr2, 10);

    if (*endptr2 != '\0' && *endptr2 != '\n') {
        fprintf(stderr, "\'%s\' is not a valid full moves\n", FullMove);
        return 0;
    }
    else if (full < 0) {
        fprintf(stderr, "Full move cannot be nagative\n");
        return 0;
    }

    bb->Full_MoveCount = (U16) full;

    free(fenDup);

    return 1;
}

U8 Init_Game(BitBoard *bb, const char *fen) {
    Init_BB(bb);
    return FEN_Decode(bb, fen);
}