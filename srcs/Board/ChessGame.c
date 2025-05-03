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

// U8 MovePiece(BitBoard *bb, const TNum Bpiece, const Square prev, const Square next) {
//     U64 *cPos;        // Pointer to current position of the piece to move
//     U64 *aPos;        // Pointer to the "all" position of the side of the piece to move
//     U64 cAtk;         // Current attack map of this piece
//     U64 cMove = 0ULL; // Current movement map (if any) of this piece

//     /* Pieces of opposite side are not supposed to move */
//     if (bb->side2play == W && Bpiece > WKING || bb->side2play == B && Bpiece <= WKING)
//         return 0;

//     if (bb->side2play == W) {
//         aPos = &(bb->pos_wAll);

//         if (Bpiece == WPAWN) {
//             cPos = &(bb->pos_wPawns);
//             cMove = bb->moves_on_each_square[prev];
//         }
//         else if (Bpiece == WROOK)
//             cPos = &(bb->pos_wRooks);
//         else if (Bpiece == WKNIGHT)
//             cPos = &(bb->pos_wKnights);
//         else if (Bpiece == WBISHOP)
//             cPos = &(bb->pos_wBishops);
//         else if (Bpiece == WQUEEN)
//             cPos = &(bb->pos_wQueens);
//         else if (Bpiece == WKING) {
//             cPos = &(bb->pos_wKing);
//             cMove = bb->moves_on_each_square[prev];
//         }
//         else
//             return 0;
//     }
//     else {
//         aPos = &(bb->pos_bAll);

//         if (Bpiece == BPAWN) {
//             cPos = &(bb->pos_bPawns);
//             cMove = bb->moves_on_each_square[prev];
//         }
//         else if (Bpiece == BROOK)
//             cPos = &(bb->pos_bRooks);
//         else if (Bpiece == BKNIGHT)
//             cPos = &(bb->pos_bKnights);
//         else if (Bpiece == BBISHOP)
//             cPos = &(bb->pos_bBishops);
//         else if (Bpiece == BQUEEN)
//             cPos = &(bb->pos_bQueens);
//         else if (Bpiece == BKING) {
//             cPos = &(bb->pos_bKing);
//             cMove = bb->moves_on_each_square[prev];
//         }
//         else
//             return 0;
//     }
    
//     if (!GetBit(*cPos, prev)) // The current position of this piece cannot be found
//         return 0;

//     cAtk = bb->atk_on_each_square[prev];

//     if (!GetBit(cAtk, next) && !GetBit(cMove, next)) // The moved square is not legal
//         return 0;

//     /* Move the piece to a new position */
//     *cPos = PopBit(*cPos, prev);
//     *cPos = SetBit(*cPos, next);

//     /* Update the new "all" position */
//     *aPos = PopBit(*cPos, prev);
//     *aPos = SetBit(*cPos, next);

//     /* Swap the turn to a new side */
//     bb->side2play *= -1;
// }