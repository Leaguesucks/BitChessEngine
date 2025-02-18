/* This is where the magic begins
 * 
 * Used to find magic number for the sliders attack lookup table.
 * More information can be found at:
 * 
 *  https://www.chessprogramming.org/Magic_Bitboards
    https://www.chessprogramming.org/Best_Magics_so_far
    https://www.chessprogramming.org/Looking_for_Magics
    https://analog-hors.github.io/site/magic-bitboards/
 */

#ifndef _MAGICGEN_H_
#define _MAGICGEN_H_

#include <stdio.h>
#include <stdlib.h>

#include "BitBoard.h"
#include "BitManipulation.h"
#include "DataBase.h"

/* The code to generate raandom U64 nummbers with high number of 1 bits
 * is provided by Tord Romstad on:
 * https://www.chessprogramming.org/Looking_for_Magics
 */

/* Generate a random U64 number
 * 
 * Copyright: Tord Romstad 
 * https://www.chessprogramming.org/Looking_for_Magics
 *
 * Return: A random U64 number
 */
U64 random_U64(void);

/* Generate a random U64 number with high number of 1s bits 
 *
 * Copyright: Tord Romstad 
 * https://www.chessprogramming.org/Looking_for_Magics
 * 
 * Return: a random U64 number with high number of 1s bits
 */
U64 random_U64_with_more_1sBit(void);

#endif