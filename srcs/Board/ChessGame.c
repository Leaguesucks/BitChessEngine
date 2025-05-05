#include "ChessGame.h"

static const char PCHAR[2][6] =
{
    {'p', 'r', 'n', 'b', 'q', 'k' }, // Black pieces
    {'P', 'R', 'N', 'B', 'Q', 'K'}   // White pieces
};

U8 FEN_Decode(BitBoard *bb, const char *fen) {
    char *fenDup = malloc(1000);

    if (fenDup == NULL) {
        fprintf(stderr, "Cannot allocate dynamic memory for FEN_Decode()\n");
        return 0;
    }

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
        
        PNum p = EMPTY;
        for (U8 side = 0; side < 2; side++) {
            for (PNum pNum = PAWN; pNum <= KING; pNum++)
                if (piece == PCHAR[side][pNum]) {
                    p = pNum;
                    bb->positions[side][pNum] = SetBit(bb->positions[side][pNum], square);
                    bb->numPiece[side][pNum]++;
                    break;
                }
            if (p != EMPTY)
                break;
        }

        if (p == EMPTY) {
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
        bb->side2play = WHITE;
    else if (strncmp(Side2Play, "b", 1) == 0)
        bb->side2play = BLACK;
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
        bb->EnPassen = SetBit(bb->EnPassen, square);
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

    /* Create the all position */
    for (PNum pNum = PAWN; pNum <= KING; pNum++) {
        bb->all_positions[WHITE] |= bb->positions[WHITE][pNum];
        bb->all_positions[BLACK] |= bb->positions[BLACK][pNum];
    }

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

U8 Handle_Promotion(BitBoard *bb, Square ProPos, PNum ProPiece) {
    U8 side = bb->side2play;

    if (ProPiece == PAWN || ProPiece == KING) // Obviously a pawn cannot be promoted to itself or his majesty
        return 0;

    /* Delete the promoted pawn */
    bb->positions[side][PAWN] = PopBit(bb->positions[side][PAWN], ProPos);
    bb->numPiece[side][PAWN]--;

    /* Transform the pawn to a new type */
    bb->positions[side][ProPiece] = SetBit(bb->positions[side][ProPiece], ProPos);
    bb->numPiece[side][ProPiece]++;

    return 1;
}

void Handle_Castling(BitBoard *bb, U8 cSide) {
    U8 side  = bb->side2play;
    
    Square kCastle;                                  // Castle square of the King
    Square rCastle;                                  // Castle square of the Rook
    Square rPos;                                     // Current position of the rook
    U8 SetLow;                                       // The number that is used to set the castle bit low
    
    if (cSide == CASTLE_KING) {
        if (side == WHITE) {
            SetLow  = 1;
            kCastle = WKING_CASTLE_SQUARE;
        }
        else {
            SetLow = 4;
            kCastle = BKING_CASTLE_SQUARE;
        }
        rCastle = kCastle - 1;
        rPos    = kCastle + 3;
    }
    else {
        if (side == WHITE) {
            SetLow = 2;
            kCastle = WQUEEN_CASTLE_SQUARE;
        }
        else {
            SetLow = 8;
            kCastle = BQUEEN_CASTLE_SQUARE;
        }
        rCastle = kCastle + 1;
        rPos    = kCastle - 4;
    }

    /* Castling by moving the King and the Rook */
    
    /* Since there is only ONE King, there is no need for popping and setting a single bit */
    bb->positions[side][KING] = SetBit(0ULL, kCastle);
    
    bb->positions[side][ROOK] = MoveBit(bb->positions[side][ROOK], rPos, rCastle);
    
    /* Set the corresponding castling bit low */
    bb->castle_right &= ~SetLow;
}

// void Capture(BitBoard *bb, PNum piece, Square capPos) {
//     U8 side = bb->side2play;
//     U8 eside = !side;

    
// }

// U8 MovePiece(BitBoard *bb, PNum piece, Square oldPos, Square newPos, PNum (*promote_handler) (void*)) {
//     U8 side = bb->side2play;
//     U64 curPos = bb->positions[side][piece];
// }