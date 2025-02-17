/* Contains PreComputed Attacks Tables */

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "BitBoard.h"

#define REVERSE_ONE 0x8000000000000000ULL

/* Generate a pre-computed lookup table for the rook */
void RookAttackLookupTableGen(void);

#endif