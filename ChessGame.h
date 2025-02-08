/* A chess game on STDIN/OUT that used to test the chess engine and 
 * the bit board representation
 */

#ifndef _CHESSGAME_H_
#define _CHESSGAME_H_


#include <stdio.h>
#include <string.h>

#include "BitBoard.h"

/* For printing colored text (debugging or style purpose)
 *
 * USAGE: COLOR(color) "String" COLOR(RESET)              
 */
#define COLOR(code) "\033[" code "m"

#define BLK     "30"
#define RED     "31"
#define GREEN   "32"
#define YELLOW  "33"
#define BLUE    "34"
#define MAGENTA "35"
#define CYAN    "36"
#define WH      "37"
#define RESET   "0"
/* ****************************************************** */

#define LEFT_ARROW  "^[[D"
#define RIGHT_ARROW "^[[C"
#define UP_ARROW    "^[[A"
#define DOWN_ARROW  "^[[B"

typedef struct {
    int row;
    int col;
    char rowstr;
    char colstr;
    U64 pos;
    U64 mov; // Set independently of other attributes
} LockSquare;

static Side Side2Play;
static BitBoard board;

/* For debug purpose */
static FILE *in;
static FILE *out;

/* Set the value of a lock 
 *
 * Para:
 *      l    -> The square lock to set
 *      pos  -> Position of the lock square in bit encode
 *      row  -> The locked row
 *      col  -> The locked column
 *      rank -> Rank of the locked square in str ('1'-'8')
 *      iter -> Column of the locked square in str ('A' - 'H')
 * 
 *      If no lock exists, then set pos = row = col = 0, rank = iter = 'N'
 *      This function does not set attribute mov
 */
void setLock(LockSquare *l, U64 pos, int r, int c, char rank, char iter) {
    l->row = r;
    l->col = c;
    l->rowstr = rank;
    l->colstr = iter;
    l->pos = pos;
}

/* Used when a selected locked square has been identified (we know what piece it is) */

/* Display all possible attack and move of a selected piece 
 *
 * Para:
 *      l -> The locked square
 *      s -> The side to play
 * 
 * Return
 *      A U64 val represent all possible move and attack of the chosen pieces
 *      Return 0 if there is an error or this piece has no valid move
 */
U64 GetAtkMov(LockSquare *l, Side s) {
    switch (s) {
        case WHITE:
            

        default:
            return 0;
    }
}

/* Print the physical representation of the bit board on a STREAM
 * Para:
 *      b -> The bit board
 *      lockSquare -> If there is a square that is locked, this is the coordinate of the square
 * 
 *      if no piece are locked int, specify lockSquare NULL
 */
void PrintBoard(BitBoard *b, LockSquare *s) {
    const char *COL_ADDRESS_STR = "  A B C D E F G H \n";
    const U64 REVERSE_ONE = 0x8000000000000000;

    fprintf(out, COLOR(YELLOW)"%s"COLOR(RESET), COL_ADDRESS_STR);
    
    int rowCount = 0;
    char p;
    for (int i = 8; i >= 1; i--) { // Print each row
        fprintf(out, COLOR(YELLOW)"%d "COLOR(RESET), i);
        
        for (int j = 0; j < 8; j++) { // Print each column            
                 if (((b->wPawns   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'P';
            else if (((b->wRooks   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'R';
            else if (((b->wKnights << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'N';
            else if (((b->wBishops << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'B';
            else if (((b->wKing    << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'K';
            else if (((b->wQueen   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'Q';

            else if (((b->bPawns   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'p';
            else if (((b->bRooks   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'r';
            else if (((b->bKnights << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'n';
            else if (((b->bBishops << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'b';
            else if (((b->bKing    << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'k';
            else if (((b->bQueen   << (j+rowCount*8)) & REVERSE_ONE) != 0) p = 'q';

            else                                                           p = '*';

            if (s == NULL) {
                fprintf(out, "%c ", p);
                continue;
            }

            if (s->row == i && s->col == j+1) {
                fprintf(out, COLOR(YELLOW)"%c "COLOR(RESET), p);
            }
            else {
                fprintf(out, "%c ", p);
            }
        }

        fprintf(out, COLOR(YELLOW)"%d \n"COLOR(RESET), i);
        rowCount++;
    }
    
    fprintf(out, COLOR(YELLOW)"%s"COLOR(RESET), COL_ADDRESS_STR);
    
}

/* Process the user request 
 *
 * Para:
 *      request -> The request
 *      respond -> The string to return the reponse into
 */
void ProcessRequest(char *request, char *respond) {
    const char *LIST = "./help: print out all list of command\n"
                       "./setUsername [username]: Set the username of a player\n"
                       "./lock [s]: Lock to a square s e.g., ./lock k7\n"
                       "./unlock: Unlock a piece if locked in"
                       "./move [s]: Move the locked in squared to a new square\n"
                       "./send [msg]: Send message to your opponent\n"
                       "./print: Print the chessboard again\n"
                       "./new: Start a new game\n"
                       "./end: End the current game\n"
                       "./exit: Exit the program\n";

    static LockSquare lock;

    char command[50];
    sscanf(request, "./%s", command);

    if (!strncmp(command, "help", sizeof("help"))) {
        strncpy(respond, LIST, 450);
        return;
    }
    else if (!strncmp(command, "lock", sizeof("lock"))) {
        char *s;
        strtok(request, " ");
        if ((s = strtok(NULL, " ")) == NULL) {
            sprintf(respond, "Please choose a square to lock in\n");
            return;
        }
        if (s[0] < 'A' || s[0] > 'H' || s[1] <'1' || s[1] > '8') {
            sprintf(respond, "Plese provide a valid address e.g., A4");
            return;
        }

        /* *** For now allowed locked in to both side pieces *** */

        setLock(&lock, 0, s[1] - ('1'-1), s[0] - ('A'-1), s[1], s[0]); // Initial set

        U64 pos = 128UL;
        pos = MoveTo(pos, UP, lock.row-1);
        pos = MoveTo(pos, RIGHT, lock.col-1);
        setLock(&lock, pos, s[1] - ('1'-1), s[0] - ('A'-1), s[1], s[0]); // Final set

        Collision col;
        if ((col = CheckCollision(&board, pos, Side2Play)) == NO_COLLISION || col == ENEMY_COLLISION) {
            sprintf(respond, "No piece selected\n");
            setLock(&lock, 0, 0, 0, 'N', 'N');
            return;
        }

        sprintf(respond, "Lock on \'%c%c\'\n", s[0], s[1]);
        PrintBoard(&board, &lock);
    }
    else if (!strncmp(command, "unlock", sizeof("unlock"))) {
        if (lock.col == 0 || lock.row == 0 || lock.rowstr == 'N' || lock.colstr == 'N' || lock.pos == 0) {
            sprintf(respond, "No piece selected\n");
            return;
        }
        sprintf(respond, "Unlock \'%c%c\'\n", lock.colstr, lock.rowstr);
        setLock(&lock, 0UL, 0, 0, 'N', 'N');
        PrintBoard(&board, NULL);
    }
}

/* Get user request from a FILE stream
 *
 */
void GetRequest(void) {
    char request[100], respond[1000];
    memset(request, '\0', 100);
    memset(respond, '\0', 1000);
    fgets(request, 100, in);
    ProcessRequest(request, respond);
    fprintf(out, "\n%s\n", respond);
}

#endif