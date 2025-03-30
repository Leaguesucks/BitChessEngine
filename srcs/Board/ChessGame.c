#include "ChessGame.h"

U8 FEN_Decode(BitBoard *bb, const char *fen) {
    char *fenDup = malloc(1000);
    strncpy(fenDup, fen, 1000);

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
            bb->num_bPawns++;
            bb->pos_bPawns = SetBit(bb->pos_bPawns, 1, square);
        }
        else if (piece == 'r') {
            bb->num_bRooks++;
            bb->pos_bRooks = SetBit(bb->pos_bRooks, 1, square);
        }
        else if (piece == 'n') {
            bb->num_bKnights++;
            bb->pos_bKnights = SetBit(bb->pos_bKnights, 1, square);
        }
        else if (piece == 'b') {
            bb->num_bBishops++;
            bb->pos_bBishops = SetBit(bb->pos_bBishops, 1, square);
        }
        else if (piece == 'q') {
            bb->num_bQueens++;
            bb->pos_bQueens = SetBit(bb->pos_bQueens, 1, square);
        }
        else if (piece == 'k') {
            bb->pos_bKing = SetBit(bb->pos_bKing, 1, square);
        }
        else if (piece == 'P') {
            bb->num_wPawns++;
            bb->pos_wPawns = SetBit(bb->pos_wPawns, 1, square);
        }
        else if (piece == 'R') {
            bb->num_wRooks++;
            bb->pos_wRooks = SetBit(bb->pos_wRooks, 1, square);
        }
        else if (piece == 'N') {
            bb->num_wKnights++;
            bb->pos_wKnights = SetBit(bb->pos_wKnights, 1, square);
        }
        else if (piece == 'B') {
            bb->num_wBishops++;
            bb->pos_wBishops = SetBit(bb->pos_wBishops, 1, square);
        }
        else if (piece == 'Q') {
            bb->num_wQueens++;
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
            bb->castle_right |= 0x8;
        else if (castle == 'k')
            bb->castle_right |= 0x4;
        else if (castle == 'Q')
            bb->castle_right |= 0x2;
        else if (castle == 'K')
            bb->castle_right |= 0x1;
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
    static U8 first = 0;
    U8 s1, s2;

    if (!first)
        s1 = Init_Database();
    memset(bb, 0, sizeof(BitBoard));
    s2 = FEN_Decode(bb, fen);    
    first = (s1 && s2) ? first + 1 : 0;

    return s1 && s2;
}