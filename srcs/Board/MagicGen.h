/* This is where the magic begins
 * 
 * Used to find magic number for the sliders attack lookup table.
 * More information can be found at:
 * 
 * COPYRIGHT:
 *  https://www.chessprogramming.org/Magic_Bitboards
    https://www.chessprogramming.org/Best_Magics_so_far
    https://www.chessprogramming.org/Looking_for_Magics
    https://analog-hors.github.io/site/magic-bitboards/
    https://github.com/maksimKorzh/chess_programming.git
 */

#ifndef _MAGICGEN_H_
#define _MAGICGEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BitBoard.h"
#include "BitManipulation.h"
#include "DataBase.h"

/* Generate a random U64 number
 *
 * Return: A random U64 number
 */
U64 random_U64(void);

/* Generate a random U64 number with high number of 1s bits 
 *
 * Return: A random U64 number with high number of 1s bits
 */
U64 random_U64_with_more_1sBit(void);

/* Generate the magic number for EACH rook or the bishop residing squares
 *
 * Para: sq -> Which square to generate the magic number for
 *       p  -> Which piece to generate a magic number for: BISHOP or ROOK
 *
 * Return: The magic number if successful, 0 otherwise
 */
U64 Find_Magic_Number(Square sq, PNum mp);

/* Generate magic numbers for both the bishops and the rooks and print the results
 * 
 * Para: f -> The output file to print the result to
 */
void Generate_And_Print_Magic_Numbers(FILE *f);


#endif