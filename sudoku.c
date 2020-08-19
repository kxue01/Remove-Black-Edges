/*
                sudoku.c

        This program determines if a sudoku puzzle
        (represented as a PGM file with magic number 2)
        is solved or not using the UArray2 data type

        Authors: Kenneth Xue (kxue01)
                Alyssa Rose (arose10)
*/
#include "uarray2.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <pnmrdr.h>

UArray2_T init_puzzle(FILE *file);
void not_solved(UArray2_T puzzle);
bool is_solved(UArray2_T puzzle);
void apply_solve(int col, int row, UArray2_T puzzle, void *p1, void *p2);
void UArray2_map_submap(UArray2_T puzzle, int this_col, void apply(int col,
        int row, UArray2_T puzzle, void *p1, void *p2), void *cl);

int main(int argc, char *argv[])
{
        if (argc > 2) {
                assert(0);
        }
        if (argc == 2) {
                FILE *fp = fopen(argv[1], "r");
                if (fp == NULL) {
                        assert(0);
                }
                else {
                        UArray2_T puzzle = init_puzzle(fp);
                        is_solved(puzzle);
                        UArray2_free(&puzzle);
                }
        }
        if (argc == 1) {
                FILE *fp = stdin;
                if (fp == NULL) {
                        assert(0);
                }
                else {
                        UArray2_T puzzle = init_puzzle(fp);
                        is_solved(puzzle);
                        UArray2_free(&puzzle);
                }
        }
        exit(0);
}


/*
Description: Creates the Uarray2_T that holds the data for Sudoku using
        the fp passed in to make a pnmrdr object, and reading in the
        pixels into the Uarray2_T to make the sudoku board.
Input: A file pointer to a pgm file
Output: A UArray2_T object that represents the sudoku
*/
UArray2_T init_puzzle(FILE *fp)
{
        Pnmrdr_T pgm = Pnmrdr_new(fp);
        Pnmrdr_mapdata map_data = Pnmrdr_data(pgm);
        float denom = map_data.denominator;
        UArray2_T puzzle = UArray2_new(9, 9, sizeof(int));
        if (map_data.type != 2 || denom != 9 || map_data.height != 9 ||
        map_data.width != 9) {
                Pnmrdr_free(&pgm);
                fclose(fp);
                not_solved(puzzle);
        }

        int this_pix;
        int *uarray_val;
        bool bad_pix = false;
        for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                        this_pix = Pnmrdr_get(pgm);
                        if (this_pix == 0 || this_pix > 9) {
                            bad_pix = true;
                        }
                        uarray_val = UArray2_at(puzzle, j, i);
                        *uarray_val = this_pix;
                }
        }

        fclose(fp);
        Pnmrdr_free(&pgm);
        if (bad_pix == true) {
            not_solved(puzzle);
        }
        return(puzzle);
}

/*
Description:Is called when a sudoku puzzle is not solved. frees the memory
        and exits with code 1.
Input: the Uarray2_t sudoku
Output: nothing
*/
void not_solved(UArray2_T puzzle)
{
        UArray2_free(&puzzle);
        exit(1);
}

/*
Description: Function used to determine if a sudoku is solved. Uses row major,
        cold major, and submap functions to loop through the puzzle,
        and uses our apply function to ensure that a sudoku is solved.
Input: the UArray2_T representing the Sudoku
Output: a bool for if it is a solved sudok
*/
bool is_solved(UArray2_T puzzle)
{
        int solver[10];
        for (int i = 0; i < 10; i++) {
                solver[i] = 0;
        }

        UArray2_map_row_major(puzzle, apply_solve, &solver);
        UArray2_map_col_major(puzzle, apply_solve, &solver);
        for (int i = 0; i < 9; i+=3) {
                UArray2_map_submap(puzzle, i, apply_solve, &solver);
        }
        return(1);
}


/*
Description:The apply function we pass into the row/col major functions. It
        creates an array of 10 ints. Ints 0-8 represent 1-9 on the sudoku board
        As we read in a number from the Uarray2_T, we will increment the
        corresponding index.
        int 9 represent the amount of numbers that we looped through. when
        int 9 reaches 9, that means we have looped through 9 numbers in the
        puzzle, we then check if indexes 0-8 are all 1. If not, that means
        there has been a repeat, and we call the function to exit with 1.
Input: int col and row which is not used. The UArray2_T representing the Sudoku
        p1 represents the element, and p2 is the closure which is our UArray
Output: none

*/
void apply_solve(int col, int row, UArray2_T puzzle, void *p1, void *p2)
{
        (void) col;
        (void) row;
        if (p2 == NULL || p1 == NULL) {
                not_solved(puzzle);
        }

        int *count = p2;
        int *this_pix = p1;
        count[*this_pix - 1] += 1;
        count[9] += 1;

        if (count[9] == 9) {
                for (int i = 0; i < 9; i++) {
                        if (count[i] != 1) {
                                not_solved(puzzle);
                        }
                }
                for (int i = 0; i < 10; i++) {
                        count[i] = 0;
                }
        }
}

/*
Description: Very simliar to row/col major. The function iterates through a
        3x9 area of our sudoku board, and does it one 3x3 square at once.
        It is called three times in our solve function to cover the whole
        9x9 board and checks each 3x3 square at the same time.
Input: UArray2_T representing the sudoku, int for the col to start on, so
        we can loop the function and start at different points.
        The apply function from above,
        and the closure is the array discussed in our apply function
Output: None
*/
void UArray2_map_submap(UArray2_T puzzle, int this_col,void apply(int col,
        int row, UArray2_T puzzle, void *p1, void *p2), void *cl)
{
        if (puzzle == NULL) {
                assert(0);
        }
        for (int i = 0; i < UArray2_height(puzzle); i++) {
                for (int j = this_col; j < this_col + 3; j++) {
                        void* thisElem = UArray2_at(puzzle, j, i);
                        if (thisElem == NULL) {
                                not_solved(puzzle);
                        }
                        apply(j, i, puzzle, thisElem, cl);
                }
        }
}
